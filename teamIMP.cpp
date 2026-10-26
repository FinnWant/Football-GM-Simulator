#include "playerH.h"
#include "team.h"
#include <memory>
#include <string>
#include <vector>
using namespace std;

team::team(string t_name, string c_name, string t_city,
           vector<shared_ptr<player>> p_list)
    : team_name(move(t_name)), coach_name(move(c_name)), city(move(t_city)),
      players(move(p_list)) {}
string team::get_team_name() const { return team_name; }
string team::get_coach_name() const { return coach_name; }
string team::get_city() const { return city; }
const vector<shared_ptr<player>> &team::get_players() const { return players; }
void team::set_team_name(const string &t_name) { team_name = t_name; }
void team::set_coach_name(const string &c_name) { coach_name = c_name; }
void team::set_city(const string &t_city) { city = t_city; }
void team::set_players(const vector<shared_ptr<player>> &p_list) {
  players = p_list;
}
void team::update_record(bool win, bool tie) {
  if (win) {
    wins++;
  } else if (tie) {
    ties++;
  } else {
    losses++;
  }
}
void team::add_points(int scored, int allowed) {
  points_for += scored;
  points_against += allowed;
}
void team::set_record(int w, int l, int t, int pf, int pa) {
  wins = w;
  losses = l;
  ties = t;
  points_for = pf;
  points_against = pa;
}
void team::reset_record() { set_record(0, 0, 0, 0, 0); }
int team::get_wins() const { return wins; }
int team::get_losses() const { return losses; }
int team::get_ties() const { return ties; }
int team::get_points_for() const { return points_for; }
int team::get_points_against() const { return points_against; }
double team::get_win_pct() const {
  int games = wins + losses + ties;
  return games ? (wins + 0.5 * ties) / games : 0.0;
}
string team::get_record_string() const {
  string rec = to_string(wins) + "-" + to_string(losses);
  if (ties > 0)
    rec += "-" + to_string(ties);
  return rec;
}
