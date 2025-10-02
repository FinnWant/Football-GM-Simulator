#ifndef TEAM_H
#define TEAM_H
#include <string>
#include <vector>
#include "playerH.h"
using namespace std;

class team{
    vector<player> players;
    string team_name;
    string coach_name;
    int wins;
    int losses;
    int ties;
    pair<int, int> record;
public:
    team(string t_name, string c_name, vector<player> p_list);
    string get_team_name() const;
    string get_coach_name() const;
    vector<player> get_players() const;
    void set_team_name(const string& t_name);
    void set_coach_name(const string& c_name);
    void set_players(const vector<player>& p_list);
    pair<int, int> get_record() const;
    void update_record(bool win, bool tie);
    int get_wins() const;
    int get_losses() const;
    int get_ties() const;
};

#endif // TEAM_H