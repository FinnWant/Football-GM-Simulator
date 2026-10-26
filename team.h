#ifndef TEAM_H
#define TEAM_H
#include "playerH.h"
#include <memory>
#include <string>
#include <vector>
using namespace std;

// How many players a full roster carries at each position
const vector<pair<Position, int>> ROSTER_TEMPLATE = {
    {Position::QB, 3}, {Position::RB, 4}, {Position::WR, 6},
    {Position::TE, 3}, {Position::OL, 9}, {Position::DL, 8},
    {Position::LB, 7}, {Position::DB, 10}, {Position::K, 1},
    {Position::P, 1}};

class team {
  string team_name;
  string coach_name;
  string city;
  vector<shared_ptr<player>> players;
  int wins{0};
  int losses{0};
  int ties{0};
  int points_for{0};
  int points_against{0};

public:
  team(string t_name, string c_name, string t_city,
       vector<shared_ptr<player>> p_list);
  string get_team_name() const;
  string get_coach_name() const;
  string get_city() const;
  const vector<shared_ptr<player>> &get_players() const;
  void set_team_name(const string &t_name);
  void set_coach_name(const string &c_name);
  void set_city(const string &t_city);
  void set_players(const vector<shared_ptr<player>> &p_list);
  void update_record(bool win, bool tie);
  void add_points(int scored, int allowed);
  // Replaces the season totals, e.g. when loading a save
  void set_record(int w, int l, int t, int pf, int pa);
  // Clears the season totals for a new season
  void reset_record();
  int get_wins() const;
  int get_losses() const;
  int get_ties() const;
  int get_points_for() const;
  int get_points_against() const;
  // Ties count as half a win, as in the NFL
  double get_win_pct() const;
  // "10-7", or "9-7-1" once the team has a tie
  string get_record_string() const;
};

#endif // TEAM_H