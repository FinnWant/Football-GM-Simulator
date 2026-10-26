#pragma once
#include "playerH.h"
#include "team.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// Returns the positions a team needs to take the field but has nobody at.
vector<Position> missing_positions(const team &t);

// One team's players sorted best-first, for picking starters.
class lineup {
  const team *t;
  vector<shared_ptr<player>> players;

public:
  explicit lineup(const team &tm);
  const team &get_team() const;
  // Highest rated player at pos, or nullptr if the roster has none
  shared_ptr<player> starter(Position pos) const;
  // The n highest rated players at pos (fewer if the roster is short)
  vector<shared_ptr<player>> starters(Position pos, size_t n) const;
  // Highest rated offensive player, for the pregame show
  shared_ptr<player> star() const;
  // The starter as its position subclass, or nullptr
  template <class T> T *starter_as(Position pos) const {
    return dynamic_cast<T *>(starter(pos).get());
  }
  // The n best at pos as their position subclass
  template <class T> vector<T *> starters_as(Position pos, size_t n) const {
    vector<T *> out;
    for (const auto &p : starters(pos, n)) {
      if (T *typed = dynamic_cast<T *>(p.get()))
        out.push_back(typed);
    }
    return out;
  }
};

// One team's totals for a game
struct team_stats {
  int pass_attempts{0};
  int completions{0};
  int pass_yards{0};
  int interceptions{0};
  int times_sacked{0};
  int rushes{0};
  int rush_yards{0};
  int fumbles_lost{0};
  int touchdowns{0};
  int field_goals{0};
  int field_goal_attempts{0};
  int punts{0};
};

struct game_result {
  bool played{false}; // false if a team could not field a lineup
  bool overtime{false};
  int score[2]{0, 0};
  team_stats stats[2];
};

// Adds a finished game to both teams' records and points for/against
void apply_result(team &home, team &away, const game_result &r);

class game_state {
public:
  game_result play_game(const team &home, const team &away);
  // Where play-by-play goes. Pass a stream with no buffer, such as
  // ostream quiet(nullptr), to simulate silently.
  void set_output(ostream &os) { log = &os; }

private:
  ostream *log = &cout;
  int score[2]{0, 0};
  team_stats stats[2];
  int quarter{1};
  bool in_overtime{false};
  int time_remaining{900}; // in seconds
  int down{1};
  int yards_to_go{10};
  int ball_position{25}; // yard line (0..100) measured from the home team's goal
  bool possession{false}; // 0 for home, 1 for away
  // the two lineups, indexed like possession; only valid during play_game
  const lineup *sides[2]{nullptr, nullptr};

  const lineup &offense() const { return *sides[possession]; }
  const lineup &defense() const { return *sides[!possession]; }
  string team_name(bool side) const;

  void apply_gain(int yards);
  void apply_loss(int yards);
  int yards_to_goal() const;
  int running_clock() const;
  void set_first_down();
  void kickoff_to(bool receiving);
  void turnover();

  void play_snap();
  void play_overtime();
  void print_status() const;
  void print_box_score() const;
  bool call_pass() const;
  // Each returns true if the defense took the ball away
  bool run_pass_play();
  bool run_run_play();
  void attempt_field_goal();
  void punt();
  void check_play_result();
  void advance_clock();
};
