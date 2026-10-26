#include "persistence.h"
#include "playerH.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Every file starts with a header naming its kind and format version, e.g.
// "FGM_TEAM 2". Team files from before version 2 have no header.
static const string TEAM_HEADER = "FGM_TEAM";
static const int TEAM_VERSION = 2;
static const string LEAGUE_HEADER = "FGM_LEAGUE";
static const int LEAGUE_VERSION = 1;

// More players or games than any real roster or season, so a corrupt count
// can't allocate a huge vector
static const int MAX_SAVED_PLAYERS = 200;
static const int MAX_SAVED_GAMES = 1000;

static void write_line(ostream &out, const string &s) { out << s << '\n'; }

// Helper to read a line and return false on EOF. Drops the '\r' left by
// Windows line endings so "QB\r" still reads as "QB".
static bool read_line(istream &in, string &out) {
  if (!std::getline(in, out))
    return false;
  if (!out.empty() && out.back() == '\r')
    out.pop_back();
  return true;
}

// Reads exactly values.size() integers from line; false if any are missing
// or the line has anything else on it
static bool parse_ints(const string &line, vector<int> &values) {
  std::istringstream iss(line);
  for (int &value : values) {
    if (!(iss >> value))
      return false;
  }
  string extra;
  return !(iss >> extra);
}

// Version number from a "KIND N" header line: 0 if the line isn't a header
// of that kind at all, -1 if it is but the version is unreadable
static int header_version(const string &line, const string &kind) {
  if (line.compare(0, kind.size() + 1, kind + " ") != 0)
    return 0;
  vector<int> version(1);
  if (!parse_ints(line.substr(kind.size() + 1), version) || version[0] < 1)
    return -1;
  return version[0];
}

// name, coach, city, [W L T PF PA], player count, then 4 lines per player
static void write_team(ostream &out, const team &t, bool with_record) {
  write_line(out, t.get_team_name());
  write_line(out, t.get_coach_name());
  write_line(out, t.get_city());
  if (with_record) {
    out << t.get_wins() << ' ' << t.get_losses() << ' ' << t.get_ties() << ' '
        << t.get_points_for() << ' ' << t.get_points_against() << '\n';
  }

  const auto &players = t.get_players();
  out << players.size() << '\n';

  for (const auto &sp : players) {
    if (!sp) {
      // write placeholder for empty slot
      write_line(out, "EMPTY");
      continue;
    }
    write_line(out, sp->get_position_name());
    write_line(out, sp->get_name());
    out << sp->get_number() << ' ' << sp->get_overall_rating() << ' '
        << sp->get_age() << '\n';

    // position-specific ratings in attribute_names() order
    vector<int> attrs = sp->get_attributes();
    for (size_t i = 0; i < attrs.size(); ++i)
      out << (i ? " " : "") << attrs[i];
    out << '\n';
  }
}

static bool read_team(istream &in, team &out_team, bool with_record) {
  string team_name, coach_name, city;
  if (!read_line(in, team_name))
    return false;
  if (!read_line(in, coach_name))
    return false;
  if (!read_line(in, city))
    return false;

  string line;
  // wins, losses, ties, points for, points against
  vector<int> record(5, 0);
  if (with_record) {
    if (!read_line(in, line) || !parse_ints(line, record))
      return false;
    for (int value : record) {
      if (value < 0)
        return false;
    }
  }

  if (!read_line(in, line))
    return false;
  vector<int> count_field(1);
  if (!parse_ints(line, count_field) || count_field[0] < 0 ||
      count_field[0] > MAX_SAVED_PLAYERS)
    return false;
  int count = count_field[0];

  vector<shared_ptr<player>> roster;
  roster.reserve(count);

  for (int i = 0; i < count; ++i) {
    string pos_name;
    if (!read_line(in, pos_name))
      return false;
    if (pos_name == "EMPTY") {
      roster.push_back(nullptr);
      continue;
    }
    Position pos;
    if (!position_from_string(pos_name, pos))
      return false;
    string name;
    if (!read_line(in, name))
      return false;
    // number, overall, age
    vector<int> bio(3);
    if (!read_line(in, line) || !parse_ints(line, bio))
      return false;

    // read subtype line
    vector<int> attrs(attribute_names(pos).size());
    if (!read_line(in, line) || !parse_ints(line, attrs))
      return false;
    roster.push_back(make_player(pos, bio[1], bio[0], bio[2], name, attrs));
  }

  // construct output team
  out_team = team(team_name, coach_name, city, roster);
  out_team.set_record(record[0], record[1], record[2], record[3], record[4]);
  return true;
}

bool save_team(const team &t, const string &path) {
  ofstream out(path);
  if (!out)
    return false;
  out << TEAM_HEADER << ' ' << TEAM_VERSION << '\n';
  write_team(out, t, true);
  out.close();
  // catches a full disk or other write failure
  return !out.fail();
}

bool load_team(const string &path, team &out_team) {
  ifstream in(path);
  if (!in)
    return false;

  string first;
  if (!read_line(in, first))
    return false;
  int version = header_version(first, TEAM_HEADER);
  if (version == 0) {
    // version 1 files have no header or record: the first line is the name
    in.clear();
    in.seekg(0);
    return read_team(in, out_team, false);
  }
  if (version != TEAM_VERSION)
    return false;
  return read_team(in, out_team, true);
}

// header, "season my_team week", team count, each team with its record,
// game count, then one "week home away played overtime home_score
// away_score" line per scheduled game
bool save_league(const league &lg, const string &path) {
  ofstream out(path);
  if (!out)
    return false;
  out << LEAGUE_HEADER << ' ' << LEAGUE_VERSION << '\n';
  out << lg.get_season() << ' ' << lg.get_my_team() << ' ' << lg.get_week()
      << '\n';
  out << lg.get_teams().size() << '\n';
  for (const team &t : lg.get_teams())
    write_team(out, t, true);
  out << lg.get_schedule().size() << '\n';
  for (const scheduled_game &g : lg.get_schedule()) {
    out << g.week << ' ' << g.home << ' ' << g.away << ' ' << g.played << ' '
        << g.overtime << ' ' << g.home_score << ' ' << g.away_score << '\n';
  }
  out.close();
  return !out.fail();
}

bool load_league(const string &path, league &out_league) {
  ifstream in(path);
  if (!in)
    return false;

  string line;
  if (!read_line(in, line) ||
      header_version(line, LEAGUE_HEADER) != LEAGUE_VERSION)
    return false;

  vector<int> state(3); // season, my team, next week
  if (!read_line(in, line) || !parse_ints(line, state) || state[0] < 1 ||
      state[1] < -1 || state[1] >= LEAGUE_TEAMS || state[2] < 1 ||
      state[2] > SEASON_WEEKS + 1)
    return false;

  // the schedule and divisions assume exactly this many teams
  vector<int> count(1);
  if (!read_line(in, line) || !parse_ints(line, count) ||
      count[0] != LEAGUE_TEAMS)
    return false;

  vector<team> teams;
  for (int i = 0; i < count[0]; ++i) {
    team t("", "", "", {});
    if (!read_team(in, t, true))
      return false;
    teams.push_back(t);
  }

  if (!read_line(in, line) || !parse_ints(line, count) || count[0] < 0 ||
      count[0] > MAX_SAVED_GAMES)
    return false;
  vector<scheduled_game> games;
  for (int i = 0; i < count[0]; ++i) {
    vector<int> f(7);
    if (!read_line(in, line) || !parse_ints(line, f))
      return false;
    bool valid = f[0] >= 1 && f[0] <= SEASON_WEEKS && f[1] >= 0 &&
                 f[1] < LEAGUE_TEAMS && f[2] >= 0 && f[2] < LEAGUE_TEAMS &&
                 f[1] != f[2] && (f[3] == 0 || f[3] == 1) &&
                 (f[4] == 0 || f[4] == 1) && f[5] >= 0 && f[6] >= 0;
    if (!valid)
      return false;
    games.push_back({f[0], f[1], f[2], f[3] == 1, f[4] == 1, f[5], f[6]});
  }

  out_league = league(teams, state[0], state[1], games, state[2]);
  return true;
}
