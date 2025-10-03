#include "playerH.h"
#include "team.h"
#include <memory>
#include <string>
#include <vector>
using namespace std;

team::team(string t_name, string c_name, vector<shared_ptr<player>> p_list)
    : team_name(t_name), coach_name(c_name), players(p_list), wins(0),
      losses(0), ties(0), record{0, 0} {}
string team::get_team_name() const { return team_name; }
string team::get_coach_name() const { return coach_name; }
vector<shared_ptr<player>> team::get_players() const { return players; }
void team::set_team_name(const string &t_name) { team_name = t_name; }
void team::set_coach_name(const string &c_name) { coach_name = c_name; }
void team::set_players(const vector<shared_ptr<player>> &p_list) {
  players = p_list;
}
pair<int, int> team::get_record() const { return record; }
void team::update_record(bool win, bool tie) {
  if (win) {
    wins++;
  } else if (tie) {
    ties++;
  } else {
    losses++;
  }
  record = make_pair(wins, losses);
}
int team::get_wins() const { return wins; }
int team::get_losses() const { return losses; }
int team::get_ties() const { return ties; }
