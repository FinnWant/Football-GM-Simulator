#pragma once
#include "team.h"
#include "playerH.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <array>

using namespace std;    

struct game_state{
    int score[2];
    string Teams[2];
    int quarter;
    int time_remaining; // in seconds
    int down;
    int yards_to_go;
    int ball_position; // yard line
    bool possession; // 0 for home, 1 for away

    game_state() : score{0}, quarter{1}, time_remaining{900}, down{1}, yards_to_go{10}, ball_position{25}, possession{0} {};

    void play_game(team home, team away){
        score[0] = 0;
        score[1] = 0;
        quarter = 1;
        time_remaining = 900; // 15 minutes per quarter
        down = 1;
        yards_to_go = 10;
        ball_position = 25; // start at 25 yard line
        possession = 0; // home team starts with the ball
        pair<int,int> recordHome = home.get_record();
        pair<int,int> recordAway = away.get_record();
        //generate starting lineups
        vector<player> home_offense;
        vector<player> home_defense;
        vector<player> away_offense;
        vector<player> away_defense;
        for(auto p : home.get_players()){
            if(p.get_position() == "QB" || p.get_position() == "RB" || p.get_position() == "WR" || p.get_position() == "TE" || p.get_position() == "OL"){
                home_offense.push_back(p);
            }else{
                home_defense.push_back(p);
            }
        }
        for(auto p : away.get_players()){
            if(p.get_position() == "QB" || p.get_position() == "RB" || p.get_position() == "WR" || p.get_position() == "TE" || p.get_position() == "OL"){
                away_offense.push_back(p);
            }else{
                away_defense.push_back(p);
            }
        }
        //sort players by overall rating
        sort(home_offense.begin(), home_offense.end(), [](const player& a, const player& b){
            return a.get_overall_rating() > b.get_overall_rating();
        });
        sort(home_defense.begin(), home_defense.end(), [](const player& a, const player& b){
            return a.get_overall_rating() > b.get_overall_rating();
        });
        sort(away_offense.begin(), away_offense.end(), [](const player& a, const player& b){
            return a.get_overall_rating() > b.get_overall_rating();
        });
        sort(away_defense.begin(), away_defense.end(), [](const player& a, const player& b){
            return a.get_overall_rating() > b.get_overall_rating();
        });
        cout << "The " << recordHome.first << "-" << recordHome.second<< " " << home.get_team_name() << " are playing the " <<recordAway.first<< "-" 
             << recordAway.second << " " << away.get_team_name() << "!" << endl;
        cout << "Kickoff!" << endl;
        do{
            //check down and distance
            // if down == 4, punt or field goal attempt if in range
            // else attempt to run or pass based on distance
            // update game state based on result
            cout << "Quarter: " << quarter << ", Time Remaining: " << time_remaining/60 << "m " << time_remaining%60 << "s" << endl;
            cout << "Score: " << home.get_team_name() << " " << score[0] << " - " << away.get_team_name() << " " << score[1] << endl;
            cout << (possession == 0 ? home.get_team_name() : away.get_team_name()) << " have the ball at the " << ball_position << " yard line." << endl;
            cout << "Down: " << down << ", Yards to go: " << yards_to_go << endl;
            if(down < 4){
                cout << "It's " << down << " down. They have " << yards_to_go << " yards to go for the first." << endl;
                if(yards_to_go > 6){
                    cout << "They decide to pass the ball." << endl;
                    if(possession == 1){
                        // away playing offense (possession == 1 means away has the ball)
                        // get QB, best WR, best DB, rand OL, and rand DL
                        auto qb_it = find_if(away_offense.begin(), away_offense.end(), [](const player& p){
                            return p.get_position() == "QB";
                        });
                        auto wr_it = find_if(away_offense.begin(), away_offense.end(), [](const player& p){
                            return p.get_position() == "WR";
                        });
                        auto db_it = find_if(home_defense.begin(), home_defense.end(), [](const player& p){
                            return p.get_position() == "DB";
                        });
                        auto ol_it = find_if(away_offense.begin(), away_offense.end(), [](const player& p){
                            return p.get_position() == "OL";
                        });
                        auto dl_it = find_if(home_defense.begin(), home_defense.end(), [](const player& p){
                            return p.get_position() == "DL";
                        });
                        quarterback qb = dynamic_cast<quarterback&>(*qb_it);
                        wide_receiver wr = dynamic_cast<wide_receiver&>(*wr_it);
                        defensive_back db = dynamic_cast<defensive_back&>(*db_it);
                        lineman ol = dynamic_cast<lineman&>(*ol_it);
                        defensive_lineman dl = dynamic_cast<defensive_lineman&>(*dl_it);
                        //check pass rush success
                        double pass_rush_margin = (ol.get_blocking() * 0.6 + qb.get_speed() * 0.4)
                                                 - (dl.get_pass_rushing() + (rand() % 20));

                        // incorporate QB throw power, WR catching, and DB coverage
                        double qb_throw = qb.get_throw_power();
                        double wr_catch = wr.get_catching();
                        double db_cover = db.get_coverage();

                        // combine into a single completion score. The skill influence is scaled
                        // down so the pass_rush_margin (which can vary widely) remains important.
                        double skill_influence = qb_throw * 0.25 + wr_catch * 0.35 - db_cover * 0.4; // roughly -40..60
                        double completion_score = pass_rush_margin + (skill_influence / 10.0);

                        // roll for stochastic outcome
                        int roll = rand() % 100;

                        if (completion_score <= -10.0) {
                            // defensive domination
                            cout << "The defensive line dominates the offensive line!" << endl;
                            if (roll < 60) { 
                                //sack 
                                down++;
                                time_remaining -= 10;
                                cout << "The quarterback is sacked!" << endl;
                                yards_to_go += 4; // lost yardage
                                ball_position -= 4;
                            }
                            else if (roll < 90) {
                                // hurried throw -> incomplete or short gain
                                if (rand() % 2) {
                                    cout << "The pass is incomplete due to pressure!" << endl;
                                    down++;
                                    time_remaining -= 7;
                                } else {
                                    int gain = rand() % 5 + 1; // short gain
                                    cout << "The pass is completed for a short gain of " << gain << " yards!" << endl;
                                    ball_position += gain;
                                    yards_to_go -= gain;
                                    down++;
                                    time_remaining -= 10;
                                }
                            }
                            else {
                                // contested completion
                                int gain = rand() % 10 + 1; // moderate gain
                                cout << "The pass is contested but completed for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 12;
                             }
                        } else if (completion_score <= 0.0) {
                            // pressure
                            if (roll < 25) {
                                //sack 
                                down++;
                                time_remaining -= 10;
                                cout << "The quarterback is sacked!" << endl;
                                yards_to_go += 4; // lost yardage
                                ball_position -= 4;
                            }
                            else if (roll < 75) {
                                // hurried throw -> incomplete or short gain
                                if (rand() % 2) {
                                    cout << "The pass is incomplete due to pressure!" << endl;
                                    down++;
                                    time_remaining -= 7;
                                } else {
                                    int gain = rand() % 5 + 1; // short gain
                                    cout << "The pass is completed for a short gain of " << gain << " yards!" << endl;
                                    ball_position += gain;
                                    yards_to_go -= gain;
                                    down++;
                                    time_remaining -= 10;
                                }
                            }
                            else { 
                                // contested completion
                                int gain = rand() % 10 + 1; // moderate gain
                                cout << "The pass is contested but completed for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 12;
                            }
                        } else if (completion_score <= 8.0) {
                            // slightly pressured but QB gets throw off
                            if (roll < 10) {
                                //sack 
                                down++;
                                time_remaining -= 10;
                                cout << "The quarterback is sacked!" << endl;
                                yards_to_go += 4; // lost yardage
                                ball_position -= 4;
                             }
                            else if (roll < 75) {
                                // contested completion
                                int gain = rand() % 10 + 1; // moderate gain
                                cout << "The pass is contested but completed for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 12;
                            }
                            else {
                                // clean pocket
                                int gain = rand() % 20 + 5; // good gain
                                cout << "The pass is completed for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 15;
                            }
                        } else {
                            // clean pocket
                            if (roll < 85) {
                                // clean pocket
                                int gain = rand() % 20 + 5; // good gain
                                cout << "The pass is completed for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 15;
                            } else {
                                // big play
                                int gain = rand() % 50 + 20; // big gain
                                cout << "The pass is a big play for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 20;
                            }
                        }
                    }
                    else{
                        //home playing offense
                        //get QB, best WR, best DB, rand OL, and rand DL
                        auto qb_it = find_if(home_offense.begin(), home_offense.end(), [](const player& p){
                            return p.get_position() == "QB";
                        });
                        auto wr_it = find_if(home_offense.begin(), home_offense.end(), [](const player& p){
                            return p.get_position() == "WR";
                        });
                        auto db_it = find_if(away_defense.begin(), away_defense.end(), [](const player& p){
                            return p.get_position() == "DB";
                        });
                        auto ol_it = find_if(home_offense.begin(), home_offense.end(), [](const player& p){
                            return p.get_position() == "OL";
                        });
                        auto dl_it = find_if(away_defense.begin(), away_defense.end(), [](const player& p){
                            return p.get_position() == "DL";
                        });
                        quarterback qb = dynamic_cast<quarterback&>(*qb_it);
                        wide_receiver wr = dynamic_cast<wide_receiver&>(*wr_it);
                        defensive_back db = dynamic_cast<defensive_back&>(*db_it);
                        lineman ol = dynamic_cast<lineman&>(*ol_it);
                        defensive_lineman dl = dynamic_cast<defensive_lineman&>(*dl_it);
                        //check pass rush success
                        double pass_rush_margin = (ol.get_blocking() * 0.6 + qb.get_speed() * 0.4)
                                                 - (dl.get_pass_rushing() + (rand() % 20));

                        // incorporate QB throw power, WR catching, and DB coverage
                        double qb_throw = qb.get_throw_power();
                        double wr_catch = wr.get_catching();
                        double db_cover = db.get_coverage();

                        double skill_influence = qb_throw * 0.25 + wr_catch * 0.35 - db_cover * 0.4;
                        double completion_score = pass_rush_margin + (skill_influence / 10.0);

                        // roll for stochastic outcome
                        int roll = rand() % 100;

                        if (completion_score <= -10.0) {
                            // defensive domination
                            cout << "The defensive line dominates the offensive line!" << endl;
                            if (roll < 60) { 
                                //sack 
                                down++;
                                time_remaining -= 10;
                                cout << "The quarterback is sacked!" << endl;
                                yards_to_go += 4; // lost yardage
                                ball_position -= 4;
                            }
                            else if (roll < 90) {
                                // hurried throw -> incomplete or short gain
                                if (rand() % 2) {
                                    cout << "The pass is incomplete due to pressure!" << endl;
                                    down++;
                                    time_remaining -= 7;
                                } else {
                                    int gain = rand() % 5 + 1; // short gain
                                    cout << "The pass is completed for a short gain of " << gain << " yards!" << endl;
                                    ball_position += gain;
                                    yards_to_go -= gain;
                                    down++;
                                    time_remaining -= 10;
                                }
                            }
                            else {
                                // contested completion
                                int gain = rand() % 10 + 1; // moderate gain
                                cout << "The pass is contested but completed for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 12;
                             }
                        } else if (completion_score <= 0.0) {
                            // pressure
                            if (roll < 25) {
                                //sack 
                                down++;
                                time_remaining -= 10;
                                cout << "The quarterback is sacked!" << endl;
                                yards_to_go += 4; // lost yardage
                                ball_position -= 4;
                            }
                            else if (roll < 75) {
                                // hurried throw -> incomplete or short gain
                                if (rand() % 2) {
                                    cout << "The pass is incomplete due to pressure!" << endl;
                                    down++;
                                    time_remaining -= 7;
                                } else {
                                    int gain = rand() % 5 + 1; // short gain
                                    cout << "The pass is completed for a short gain of " << gain << " yards!" << endl;
                                    ball_position += gain;
                                    yards_to_go -= gain;
                                    down++;
                                    time_remaining -= 10;
                                }
                            }
                            else { 
                                // contested completion
                                int gain = rand() % 10 + 1; // moderate gain
                                cout << "The pass is contested but completed for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 12;
                            }
                        } else if (completion_score <= 8.0) {
                            // slightly pressured but QB gets throw off
                            if (roll < 10) {
                                //sack 
                                down++;
                                time_remaining -= 10;
                                cout << "The quarterback is sacked!" << endl;
                                yards_to_go += 4; // lost yardage
                                ball_position -= 4;
                             }
                            else if (roll < 75) {
                                // contested completion
                                int gain = rand() % 10 + 1; // moderate gain
                                cout << "The pass is contested but completed for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 12;
                            }
                            else {
                                // clean pocket
                                int gain = rand() % 20 + 5; // good gain
                                cout << "The pass is completed for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 15;
                            }
                        } else {
                            // clean pocket
                            if (roll < 85) {
                                // clean pocket
                                int gain = rand() % 20 + 5; // good gain
                                cout << "The pass is completed for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 15;
                            } else {
                                // big play
                                int gain = rand() % 50 + 20; // big gain
                                cout << "The pass is a big play for a gain of " << gain << " yards!" << endl;
                                ball_position += gain;
                                yards_to_go -= gain;
                                down++;
                                time_remaining -= 20;
                            }
                        }
                    }
                }
                else{
                    //run play
                    cout << "They decide to run the ball." << endl;
                    if(possession == 1){
                        //away playing offense
                        //get best RB from offense
                        auto rb_it = find_if(away_offense.begin(), away_offense.end(), [](const player& p){
                            return p.get_position() == "RB";
                        });
                        //get best linebacker and lineman from defense
                        auto lb_it = find_if(home_defense.begin(), home_defense.end(), [](const player& p){
                            return p.get_position() == "LB";
                        });
                        auto dl_it = find_if(home_defense.begin(), home_defense.end(), [](const player& p){
                            return p.get_position() == "DL";
                        });
                        linebacker lb = dynamic_cast<linebacker&>(*lb_it);
                        defensive_lineman dl = dynamic_cast<defensive_lineman&>(*dl_it);
                        runningback rb = dynamic_cast<runningback&>(*rb_it);
                        int rb_runpower = rb.get_carrying() + rb.get_elusiveness() + rb.get_speed() % 20;
                        int defense_stop = (lb.get_tackling() + dl.get_tackling()) + (rand() % 40);
                        if(rb_runpower > defense_stop){
                            int gain = rand() % 10 + 3; // typical run gain
                            cout << "The run is successful for a gain of " << gain << " yards!" << endl;
                            ball_position += gain;
                            yards_to_go -= gain;
                            down++;
                            time_remaining -= 10;
                        }else{
                            int loss = rand() % 3 + 1; // typical loss on stopped run
                            cout << "The run is stopped for a loss of " << loss << " yards!" << endl;
                            ball_position -= loss;
                            yards_to_go += loss;
                            down++;
                            time_remaining -= 8;
                        }
                    }else{
                        //away playing offense
                        //get best RB from offense
                        auto rb_it = find_if(home_offense.begin(), home_offense.end(), [](const player& p){
                            return p.get_position() == "RB";
                        });
                        //get best linebacker and lineman from defense
                        auto lb_it = find_if(away_defense.begin(), home_defense.end(), [](const player& p){
                            return p.get_position() == "LB";
                        });
                        auto dl_it = find_if(away_defense.begin(), home_defense.end(), [](const player& p){
                            return p.get_position() == "DL";
                        });
                        linebacker lb = dynamic_cast<linebacker&>(*lb_it);
                        defensive_lineman dl = dynamic_cast<defensive_lineman&>(*dl_it);
                        runningback rb = dynamic_cast<runningback&>(*rb_it);
                        int rb_runpower = rb.get_carrying() + rb.get_elusiveness() + rb.get_speed() % 20;
                        int defense_stop = (lb.get_tackling() + dl.get_tackling()) + (rand() % 40);
                        if(rb_runpower > defense_stop){
                            int gain = rand() % 10 + 3; // typical run gain
                            cout << "The run is successful for a gain of " << gain << " yards!" << endl;
                            ball_position += gain;
                            yards_to_go -= gain;
                            down++;
                            time_remaining -= 10;
                        }else{
                            int loss = rand() % 3 + 1; // typical loss on stopped run
                            cout << "The run is stopped for a loss of " << loss << " yards!" << endl;
                            ball_position -= loss;
                            yards_to_go += loss;
                            down++;
                            time_remaining -= 8;
                        }
                    }
                }            
            }

            // Post-play checks: first downs, touchdowns, safeties, and turnovers on downs
            // touchdown (reached or crossed opponent's goal line at 100)
            if(ball_position >= 100){
                cout << "Touchdown by " << (possession == 0 ? home.get_team_name() : away.get_team_name()) << "!" << endl;
                if(possession == 0) score[0] += 7; else score[1] += 7;
                // reset for kickoff to other team
                possession = !possession;
                ball_position = 25;
                down = 1;
                yards_to_go = 10;
            }
            // safety (ball backed into own endzone)
            else if(ball_position <= 0){
                cout << "Safety! " << (possession == 0 ? away.get_team_name() : home.get_team_name()) << " score 2 points." << endl;
                if(possession == 0) score[1] += 2; else score[0] += 2;
                possession = !possession;
                ball_position = 25;
                down = 1;
                yards_to_go = 10;
            }
            // first down achieved
            else if(yards_to_go <= 0){
                cout << "First down!" << endl;
                down = 1;
                int remaining_to_goal = 100 - ball_position;
                // new distance to go is either 10 or whatever remains to the goal
                yards_to_go = min(10, max(1, remaining_to_goal));
            }
            // turnover on downs
            else if(down > 4){
                cout << "Turnover on downs! Possession changes." << endl;
                possession = !possession;
                // flip the field perspective
                ball_position = 100 - ball_position;
                down = 1;
                yards_to_go = 10;
            }

        }while(quarter <= 4 && time_remaining > 0);

        cout << "Final Score: " << home.get_team_name() << " " << score[0] << " - " << away.get_team_name() << " " << score[1] << endl;
        if(score[0] > score[1]){
            cout << home.get_team_name() << " win!" << endl;
        }else if(score[1] > score[0]){
            cout << away.get_team_name() << " win!" << endl;
        }else{
            cout << "The game ends in a tie!" << endl;
        }

        return;
    }
};