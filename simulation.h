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
        cout << "The " << home.get_team_name() << " are playing the " << away.get_team_name() << "!" << endl;
        cout << "Kickoff!" << endl;
        do{
            cout << "Quarter: " << quarter << ", Time Remaining: " << time_remaining/60 << "m " << time_remaining%60 << "s" << endl;
            cout << "Score: " << home.get_team_name() << " " << score[0] << " - " << away.get_team_name() << " " << score[1] << endl;
            cout << (possession == 0 ? home.get_team_name() : away.get_team_name()) << " have the ball at the " << ball_position << " yard line." << endl;
            cout << "Down: " << down << ", Yards to go: " << yards_to_go << endl;
            //check down and distance
            // if down == 4, punt or field goal attempt if in range
            // else attempt to run or pass based on distance
            // update game state based on result
            if(down < 4){
                cout << "It's " << down << " down. They have " << yards_to_go << " yards to go for the first." << endl;
                if(yards_to_go > 6){
                    cout << "They decide to pass the ball." << endl;
                    if(possession == 1){
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
                        int pass_rush = ol.get_blocking() + qb.get_speed() > dl.get_pass_rushing() + rand() % 20;
                        //if successful, check is qb escapes
                        if(pass_rush > 50){
                        }
                        //if escapes, check if pass is completed
                        //if completed, check yards after catch
                        //else incomplete

                        
                    }
                }
            }

           

        }while(quarter <= 4 && time_remaining > 0);

    };
};