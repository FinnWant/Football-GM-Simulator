#include "persistence.h"
#include "playerH.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

static void write_line(ofstream &out, const string &s) { out << s << '\n'; }

bool save_team(const team &t, const string &path) {
  ofstream out(path);
  if (!out)
    return false;

  write_line(out, t.get_team_name());
  write_line(out, t.get_coach_name());
  write_line(out, t.get_city());

  auto players = t.get_players();
  out << players.size() << '\n';

  for (const auto &sp : players) {
    if (!sp) {
      // write placeholder for empty slot
      write_line(out, "EMPTY");
      continue;
    }
    string pos = sp->get_position();
    write_line(out, pos);
    write_line(out, sp->get_name());
    out << sp->get_number() << ' ' << sp->get_overall_rating() << ' '
        << sp->get_age() << '\n';

    // write subtype-specific attributes in a fixed order per position
    if (pos == "QB") {
      quarterback *q = dynamic_cast<quarterback *>(sp.get());
      out << q->get_throw_power() << ' ' << q->get_throw_accuracy() << ' '
          << q->get_speed() << ' ' << q->get_passing_vision() << '\n';
    } else if (pos == "RB") {
      runningback *r = dynamic_cast<runningback *>(sp.get());
      out << r->get_speed() << ' ' << r->get_carrying() << ' '
          << r->get_elusiveness() << ' ' << r->get_blocking() << '\n';
    } else if (pos == "WR") {
      wide_receiver *w = dynamic_cast<wide_receiver *>(sp.get());
      out << w->get_speed() << ' ' << w->get_catching() << ' '
          << w->get_route_running() << '\n';
    } else if (pos == "TE") {
      tight_end *te = dynamic_cast<tight_end *>(sp.get());
      out << te->get_speed() << ' ' << te->get_catching() << ' '
          << te->get_blocking() << '\n';
    } else if (pos == "OL") {
      lineman *ln = dynamic_cast<lineman *>(sp.get());
      out << ln->get_strength() << ' ' << ln->get_blocking() << ' '
          << ln->get_tackling() << '\n';
    } else if (pos == "DL") {
      defensive_lineman *dl = dynamic_cast<defensive_lineman *>(sp.get());
      out << dl->get_strength() << ' ' << dl->get_tackling() << ' '
          << dl->get_pass_rushing() << '\n';
    } else if (pos == "LB") {
      linebacker *lb = dynamic_cast<linebacker *>(sp.get());
      out << lb->get_speed() << ' ' << lb->get_tackling() << ' '
          << lb->get_coverage() << '\n';
    } else if (pos == "DB") {
      defensive_back *db = dynamic_cast<defensive_back *>(sp.get());
      out << db->get_speed() << ' ' << db->get_coverage() << ' '
          << db->get_tackling() << '\n';
    } else if (pos == "K") {
      kicker *k = dynamic_cast<kicker *>(sp.get());
      out << k->get_kick_power() << ' ' << k->get_kick_accuracy() << '\n';
    } else if (pos == "P") {
      punter *p = dynamic_cast<punter *>(sp.get());
      out << p->get_punt_power() << ' ' << p->get_punt_accuracy() << '\n';
    } else {
      // base player or unknown: write nothing extra
      out << '\n';
    }
  }

  out.close();
  return true;
}

// Helper to read a line and return false on EOF
static bool read_line(ifstream &in, string &out) {
  if (!std::getline(in, out))
    return false;
  return true;
}

bool load_team(const string &path, team &out_team) {
  ifstream in(path);
  if (!in)
    return false;

  string team_name, coach_name, city;
  if (!read_line(in, team_name))
    return false;
  if (!read_line(in, coach_name))
    return false;
  if (!read_line(in, city))
    return false;

  string line;
  if (!read_line(in, line))
    return false;
  size_t count = stoi(line);

  vector<shared_ptr<player>> roster;
  roster.reserve(count);

  for (size_t i = 0; i < count; ++i) {
    string pos;
    if (!read_line(in, pos))
      return false;
    if (pos == "EMPTY") {
      roster.push_back(nullptr);
      continue;
    }
    string name;
    if (!read_line(in, name))
      return false;
    int number = 0, overall = 50, age = 22;
    if (!read_line(in, line))
      return false;
    {
      std::istringstream iss(line);
      iss >> number >> overall >> age;
    }

    // read subtype line
    if (!read_line(in, line))
      return false;
    std::istringstream iss(line);

    if (pos == "QB") {
      int tp, ta, spd, pv;
      iss >> tp >> ta >> spd >> pv;
      roster.push_back(make_shared<quarterback>(pos, overall, number, age, name,
                                                tp, ta, spd, pv));
    } else if (pos == "RB") {
      int s, c, e, b;
      iss >> s >> c >> e >> b;
      roster.push_back(make_shared<runningback>(pos, overall, number, age, name,
                                                s, c, e, b));
    } else if (pos == "WR") {
      int s, c, rr;
      iss >> s >> c >> rr;
      roster.push_back(make_shared<wide_receiver>(pos, overall, number, age,
                                                  name, s, c, rr));
    } else if (pos == "TE") {
      int s, c, bl;
      iss >> s >> c >> bl;
      roster.push_back(
          make_shared<tight_end>(pos, overall, number, age, name, s, c, bl));
    } else if (pos == "OL") {
      int str, bl, tkl;
      iss >> str >> bl >> tkl;
      roster.push_back(
          make_shared<lineman>(pos, overall, number, age, name, str, bl, tkl));
    } else if (pos == "DL") {
      int str, tkl, pr;
      iss >> str >> tkl >> pr;
      roster.push_back(make_shared<defensive_lineman>(pos, overall, number, age,
                                                      name, str, tkl, pr));
    } else if (pos == "LB") {
      int s, tkl, cvr;
      iss >> s >> tkl >> cvr;
      roster.push_back(make_shared<linebacker>(pos, overall, number, age, name,
                                               s, tkl, cvr));
    } else if (pos == "DB") {
      int s, cvr, tkl;
      iss >> s >> cvr >> tkl;
      roster.push_back(make_shared<defensive_back>(pos, overall, number, age,
                                                   name, s, cvr, tkl));
    } else if (pos == "K") {
      int kp, ka;
      iss >> kp >> ka;
      roster.push_back(
          make_shared<kicker>(pos, overall, number, age, name, kp, ka));
    } else if (pos == "P") {
      int pp, pa;
      iss >> pp >> pa;
      roster.push_back(
          make_shared<punter>(pos, overall, number, age, name, pp, pa));
    } else {
      // fallback: base player
      roster.push_back(make_shared<player>(pos, overall, number, age, name));
    }
  }

  // construct output team
  out_team = team(team_name, coach_name, city, roster);
  return true;
}
