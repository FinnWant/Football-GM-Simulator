#ifndef TEAM_H
#define TEAM_H
#include "playerH.h"
#include <memory>
#include <string>
#include <vector>
using namespace std;

class team {
  vector<shared_ptr<player>> players;
  string team_name;
  string coach_name;
  int wins{0};
  int losses{0};
  int ties{0};
  pair<int, int> record{0, 0};

public:
  team(string t_name, string c_name, vector<shared_ptr<player>> p_list);
  string get_team_name() const;
  string get_coach_name() const;
  vector<shared_ptr<player>> get_players() const;
  void set_team_name(const string &t_name);
  void set_coach_name(const string &c_name);
  void set_players(const vector<shared_ptr<player>> &p_list);
  pair<int, int> get_record() const;
  void update_record(bool win, bool tie);
  int get_wins() const;
  int get_losses() const;
  int get_ties() const;
};

#endif // TEAM_H