#include <string>
#include <vector>
#include "team.h"
#include "playerH.h"
using namespace std;


team::team(string t_name, string c_name, vector<player> p_list)
    : team_name(t_name), coach_name(c_name), players(p_list) {}
string team::get_team_name() const { return team_name; }
string team::get_coach_name() const { return coach_name; }
vector<player> team::get_players() const { return players; }
void team::set_team_name(const string& t_name) { team_name = t_name; }
void team::set_coach_name(const string& c_name) { coach_name = c_name; }
void team::set_players(const vector<player>& p_list) { players = p_list; }
