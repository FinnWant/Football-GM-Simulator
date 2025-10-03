#pragma once
#include "playerH.h"
#include "team.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

using namespace std;

struct game_state {
  int score[2];
  string Teams[2];
  int quarter;
  int time_remaining; // in seconds
  int down;
  int yards_to_go;
  int ball_position; // yard line (0..100)
  bool possession;   // 0 for home, 1 for away

  game_state()
      : score{0}, quarter{1}, time_remaining{900}, down{1}, yards_to_go{10},
        ball_position{25}, possession{0} {};

  void play_game(team home, team away) {
    // initialize state
    score[0] = 0;
    score[1] = 0;
    quarter = 1;
    time_remaining = 900; // 15 minutes per quarter
    down = 1;
    yards_to_go = 10;
    ball_position = 25; // start at 25 yard line (home perspective)
    possession = 0;     // home team starts with the ball

    pair<int, int> recordHome = home.get_record();
    pair<int, int> recordAway = away.get_record();

    // helper lambdas to apply gains/losses relative to possession
    auto apply_gain = [&](int yards) {
      // offensive gain reduces yards_to_go and moves ball towards opponent
      if (possession == 0) {
        ball_position += yards;
      } else {
        ball_position -= yards;
      }
      yards_to_go -= yards;
    };
    auto apply_loss = [&](int yards) {
      // loss increases yards_to_go and moves ball towards own endzone
      if (possession == 0) {
        ball_position -= yards;
      } else {
        ball_position += yards;
      }
      yards_to_go += yards;
    };

    // generate starting lineups (use shared_ptrs to avoid slicing)
    vector<shared_ptr<player>> home_offense;
    vector<shared_ptr<player>> home_defense;
    vector<shared_ptr<player>> away_offense;
    vector<shared_ptr<player>> away_defense;

    for (auto sp : home.get_players()) {
      if (!sp)
        continue;
      string pos = sp->get_position();
      if (pos == "QB" || pos == "RB" || pos == "WR" || pos == "TE" ||
          pos == "OL" || pos == "P") {
        home_offense.push_back(sp);
      } else {
        home_defense.push_back(sp);
      }
    }
    for (auto sp : away.get_players()) {
      if (!sp)
        continue;
      string pos = sp->get_position();
      if (pos == "QB" || pos == "RB" || pos == "WR" || pos == "TE" ||
          pos == "OL" || pos == "P") {
        away_offense.push_back(sp);
      } else {
        away_defense.push_back(sp);
      }
    }

    // sort players by overall rating
    auto cmp = [](const shared_ptr<player> &a, const shared_ptr<player> &b) {
      int av = a ? a->get_overall_rating() : 0;
      int bv = b ? b->get_overall_rating() : 0;
      return av > bv;
    };
    sort(home_offense.begin(), home_offense.end(), cmp);
    sort(away_offense.begin(), away_offense.end(), cmp);
    sort(home_defense.begin(), home_defense.end(), cmp);
    sort(away_defense.begin(), away_defense.end(), cmp);

    cout << "The " << recordHome.first << "-" << recordHome.second << " "
         << home.get_team_name() << " are playing the " << recordAway.first
         << "-" << recordAway.second << " " << away.get_team_name() << "!"
         << endl;

    do {
      // basic status print
      cout << "Quarter: " << quarter << " " << (time_remaining / 60) << "m "
           << (time_remaining % 60) << "s" << endl;
      cout << "Score: " << home.get_team_name() << " " << score[0] << " - "
           << away.get_team_name() << " " << score[1] << endl;

      // print yard line from the possessing team's perspective (0..100)
      int display_pos =
          (possession == 0) ? ball_position : (100 - ball_position);
      cout << (possession == 0 ? home.get_team_name() : away.get_team_name())
           << " ball at the " << display_pos << " yard line.\n";
      cout << "Down: " << down << ", Yards to go: " << yards_to_go << endl;

      if (down < 4) {
        cout << "It's " << down << " down. They have " << yards_to_go
             << " yards to go for the first." << endl;

        // Decide pass vs run
        bool do_pass = (yards_to_go > 6);
        if (do_pass) {
          cout << "They decide to pass the ball." << endl;

          // choose offense/defense collections based on possession
          auto &off = (possession == 0) ? home_offense : away_offense;
          auto &def = (possession == 0) ? away_defense : home_defense;

          auto qb_it =
              find_if(off.begin(), off.end(), [](const shared_ptr<player> &p) {
                return p && p->get_position() == "QB";
              });
          auto wr_it =
              find_if(off.begin(), off.end(), [](const shared_ptr<player> &p) {
                return p && p->get_position() == "WR";
              });
          auto db_it =
              find_if(def.begin(), def.end(), [](const shared_ptr<player> &p) {
                return p && p->get_position() == "DB";
              });
          auto ol_it =
              find_if(off.begin(), off.end(), [](const shared_ptr<player> &p) {
                return p && p->get_position() == "OL";
              });
          auto dl_it =
              find_if(def.begin(), def.end(), [](const shared_ptr<player> &p) {
                return p && p->get_position() == "DL";
              });

          shared_ptr<player> qb_sp = (qb_it != off.end()) ? *qb_it : nullptr;
          shared_ptr<player> wr_sp = (wr_it != off.end()) ? *wr_it : nullptr;
          shared_ptr<player> db_sp = (db_it != def.end()) ? *db_it : nullptr;
          shared_ptr<player> ol_sp = (ol_it != off.end()) ? *ol_it : nullptr;
          shared_ptr<player> dl_sp = (dl_it != def.end()) ? *dl_it : nullptr;

          quarterback *qb =
              qb_sp ? dynamic_cast<quarterback *>(qb_sp.get()) : nullptr;
          wide_receiver *wr =
              wr_sp ? dynamic_cast<wide_receiver *>(wr_sp.get()) : nullptr;
          defensive_back *db =
              db_sp ? dynamic_cast<defensive_back *>(db_sp.get()) : nullptr;
          lineman *ol = ol_sp ? dynamic_cast<lineman *>(ol_sp.get()) : nullptr;
          defensive_lineman *dl =
              dl_sp ? dynamic_cast<defensive_lineman *>(dl_sp.get()) : nullptr;

          double ol_blocking = ol ? ol->get_blocking() : 50;
          double qb_speed = qb ? qb->get_speed() : 50;
          double dl_pass_rushing = dl ? dl->get_pass_rushing() : 50;
          double qb_throw = qb ? qb->get_throw_power() : 50;
          double wr_catch = wr ? wr->get_catching() : 50;
          double db_cover = db ? db->get_coverage() : 50;

          double pass_rush_margin = (ol_blocking * 0.6 + qb_speed * 0.4) -
                                    (dl_pass_rushing + (rand() % 20));
          double skill_influence =
              qb_throw * 0.25 + wr_catch * 0.35 - db_cover * 0.4;
          double completion_score = pass_rush_margin + (skill_influence / 10.0);

          int roll = rand() % 100;
          if (completion_score <= -10.0) {
            cout << "The defensive line dominates the offensive line!" << endl;
            if (roll < 60) {
              // sack
              down++;
              time_remaining -= 10;
              cout << "The quarterback is sacked!" << endl;
              yards_to_go += 4; // lost yardage
              apply_loss(4);
            } else if (roll < 90) {
              if (rand() % 2) {
                cout << "The pass is incomplete due to pressure!" << endl;
                down++;
                time_remaining -= 7;
              } else {
                int gain = rand() % 5 + 1; // short gain
                cout << "The pass is completed for a short gain of " << gain
                     << " yards!" << endl;
                apply_gain(gain);
                down++;
                time_remaining -= 10;
              }
            } else {
              int gain = rand() % 10 + 1; // moderate gain
              cout << "The pass is contested but completed for a gain of "
                   << gain << " yards!" << endl;
              apply_gain(gain);
              down++;
              time_remaining -= 12;
            }
          } else if (completion_score <= 0.0) {
            if (roll < 25) {
              down++;
              time_remaining -= 10;
              cout << "The quarterback is sacked!" << endl;
              yards_to_go += 4;
              apply_loss(4);
            } else if (roll < 75) {
              if (rand() % 2) {
                cout << "The pass is incomplete due to pressure!" << endl;
                down++;
                time_remaining -= 7;
              } else {
                int gain = rand() % 5 + 1;
                cout << "The pass is completed for a short gain of " << gain
                     << " yards!" << endl;
                apply_gain(gain);
                down++;
                time_remaining -= 10;
              }
            } else {
              int gain = rand() % 10 + 1;
              cout << "The pass is contested but completed for a gain of "
                   << gain << " yards!" << endl;
              apply_gain(gain);
              down++;
              time_remaining -= 12;
            }
          } else if (completion_score <= 8.0) {
            if (roll < 10) {
              down++;
              time_remaining -= 10;
              cout << "The quarterback is sacked!" << endl;
              yards_to_go += 4;
              apply_loss(4);
            } else if (roll < 75) {
              int gain = rand() % 10 + 1;
              cout << "The pass is contested but completed for a gain of "
                   << gain << " yards!" << endl;
              apply_gain(gain);
              down++;
              time_remaining -= 12;
            } else {
              int gain = rand() % 20 + 5;
              cout << "The pass is completed for a gain of " << gain
                   << " yards!" << endl;
              apply_gain(gain);
              down++;
              time_remaining -= 15;
            }
          } else {
            if (roll < 85) {
              int gain = rand() % 20 + 5;
              cout << "The pass is completed for a gain of " << gain
                   << " yards!" << endl;
              apply_gain(gain);
              down++;
              time_remaining -= 15;
            } else {
              int gain = rand() % 50 + 20;
              cout << "The pass is a big play for a gain of " << gain
                   << " yards!" << endl;
              apply_gain(gain);
              down++;
              time_remaining -= 20;
            }
          }
        } else {
          // run play
          cout << "They decide to run the ball." << endl;
          auto &off = (possession == 0) ? home_offense : away_offense;
          auto &def = (possession == 0) ? away_defense : home_defense;

          auto rb_it =
              find_if(off.begin(), off.end(), [](const shared_ptr<player> &p) {
                return p && p->get_position() == "RB";
              });
          auto lb_it =
              find_if(def.begin(), def.end(), [](const shared_ptr<player> &p) {
                return p && p->get_position() == "LB";
              });
          auto dl_it =
              find_if(def.begin(), def.end(), [](const shared_ptr<player> &p) {
                return p && p->get_position() == "DL";
              });

          shared_ptr<player> lb_sp = (lb_it != def.end()) ? *lb_it : nullptr;
          shared_ptr<player> dl_sp = (dl_it != def.end()) ? *dl_it : nullptr;
          shared_ptr<player> rb_sp = (rb_it != off.end()) ? *rb_it : nullptr;

          linebacker *lb =
              lb_sp ? dynamic_cast<linebacker *>(lb_sp.get()) : nullptr;
          defensive_lineman *dl =
              dl_sp ? dynamic_cast<defensive_lineman *>(dl_sp.get()) : nullptr;
          runningback *rb =
              rb_sp ? dynamic_cast<runningback *>(rb_sp.get()) : nullptr;

          int rb_runpower = rb ? ((rb->get_carrying() + rb->get_elusiveness() +
                                   rb->get_speed()) %
                                  20)
                               : 10;
          int defense_stop = ((lb ? lb->get_tackling() : 50) +
                              (dl ? dl->get_tackling() : 50)) +
                             (rand() % 40);

          if (rb_runpower > defense_stop) {
            int gain = rand() % 10 + 3; // typical run gain
            cout << "The run is successful for a gain of " << gain << " yards!"
                 << endl;
            apply_gain(gain);
            down++;
            time_remaining -= 10;
          } else {
            int loss = rand() % 3 + 1; // typical loss on stopped run
            cout << "The run is stopped for a loss of " << loss << " yards!"
                 << endl;
            apply_loss(loss);
            down++;
            time_remaining -= 8;
          }
        }
      }

      if (down == 4) {
        // check if in field goal range (say 45 yards or closer)
        if ((possession == 0 && ball_position >= 55) ||
            (possession == 1 && ball_position <= 45)) {
          cout << "They attempt a field goal." << endl;
          int kick_power = 70 + (rand() % 31);    // 70 to 100
          int kick_accuracy = 60 + (rand() % 41); // 60 to 100
          int distance =
              possession == 0 ? (100 - ball_position) + 17 : ball_position + 17;
          int kick_difficulty = distance / 2 + (rand() % 30);
          if (kick_power + kick_accuracy > kick_difficulty) {
            cout << "The field goal is good!" << endl;
            if (possession == 0)
              score[0] += 3;
            else
              score[1] += 3;
            // kickoff to other team
            possession = !possession;
            ball_position = 25;
            down = 1;
            yards_to_go = 10;
            time_remaining -= 15;
          } else {
            cout << "The field goal is no good!" << endl;
            // turnover on downs: receiving team takes ball at spot
            possession = !possession;
            // keep ball_position as the spot (no flip)
            down = 1;
            yards_to_go = 10;
            time_remaining -= 15;
          }
        } else {
          // punt
          cout << "They decide to punt the ball." << endl;

          // try to find a punter on the kicking team
          auto &kicking = (possession == 0) ? home_offense : away_offense;
          int base_power = 65;
          int punt_accuracy = 70;
          for (auto &p : kicking) {
            if (p && p->get_position() == "P") {
              // punter found
              // assume punter class has get_punt_power and get_punt_accuracy
              punter *pt = dynamic_cast<punter *>(p.get());
              if (pt) {
                base_power = (int)pt->get_punt_power();
                punt_accuracy = (int)pt->get_punt_accuracy();
              }
              break;
            }
          }

          // map base_power to yards roughly, add some randomness and accuracy
          int punt_distance = int(base_power * 0.55) + (rand() % 11 - 5) +
                              ((punt_accuracy - 50) / 5);
          punt_distance = max(15, min(65, punt_distance));
          cout << "The punt travels " << punt_distance << " yards." << endl;

          if (possession == 0) {
            int landing_pos = ball_position + punt_distance;
            if (landing_pos >= 100) {
              cout
                  << "Touchback! Receiving team will start at the 20 yard line."
                  << endl;
              possession = 1;     // away receives
              ball_position = 80; // receiving team's 20 (home perspective)
            } else {
              possession = 1; // away receives
              ball_position = landing_pos;
            }
          } else {
            int landing_pos = ball_position - punt_distance;
            if (landing_pos <= 0) {
              cout
                  << "Touchback! Receiving team will start at the 20 yard line."
                  << endl;
              possession = 0;     // home receives
              ball_position = 20; // home perspective: 20
            } else {
              possession = 0; // home receives
              ball_position = landing_pos;
            }
          }
          // receiving team gets a first down
          down = 1;
          yards_to_go = 10;
          time_remaining -= 10;
        }
      }

      // Post-play checks
      // Touchdown depends on which team has the ball and direction of play
      bool scored = false;
      if (possession == 0 && ball_position >= 100) {
        cout << "Touchdown by " << home.get_team_name() << "!" << endl;
        score[0] += 7;
        scored = true;
      } else if (possession == 1 && ball_position <= 0) {
        cout << "Touchdown by " << away.get_team_name() << "!" << endl;
        score[1] += 7;
        scored = true;
      }
      if (scored) {
        // reset after touchdown
        possession = !possession;
        ball_position = 25;
        down = 1;
        yards_to_go = 10;
        continue; // next kickoff/play
      }

      // Safety: offense was tackled in their own endzone
      if ((possession == 0 && ball_position <= 0) ||
          (possession == 1 && ball_position >= 100)) {
        cout << "Safety! "
             << (possession == 0 ? away.get_team_name() : home.get_team_name())
             << " score 2 points." << endl;
        if (possession == 0)
          score[1] += 2;
        else
          score[0] += 2;
        possession = !possession;
        ball_position = 25;
        down = 1;
        yards_to_go = 10;
        continue;
      }

      // First down achieved
      if (yards_to_go <= 0) {
        cout << "First down!" << endl;
        down = 1;
        int remaining_to_goal =
            (possession == 0) ? (100 - ball_position) : ball_position;
        yards_to_go = min(10, max(1, remaining_to_goal));
      }

      // Turnover on downs
      if (down > 4) {
        cout << "Turnover on downs! Possession changes." << endl;
        possession = !possession;
        ball_position = 100 - ball_position; // flip perspective
        down = 1;
        yards_to_go = 10;
      }

    } while (quarter <= 4 && time_remaining > 0);

    cout << "Final Score: " << home.get_team_name() << " " << score[0] << " - "
         << away.get_team_name() << " " << score[1] << endl;
    if (score[0] > score[1]) {
      cout << home.get_team_name() << " win!" << endl;
    } else if (score[1] > score[0]) {
      cout << away.get_team_name() << " win!" << endl;
    } else {
      cout << "The game ends in a tie!" << endl;
    }

    return;
  }
};