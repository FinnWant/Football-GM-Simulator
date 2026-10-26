#include "generation.h"
#include "league.h"
#include "persistence.h"
#include "playerH.h"
#include "simulation.h"
#include "team.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>
using namespace std;

// Function to clear input buffer and handle invalid input
void clearInputBuffer() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Input has closed (EOF/Ctrl+D/Ctrl+Z), so no prompt can ever be answered
void exitIfInputClosed() {
  if (cin.eof()) {
    cout << "\nInput closed. Goodbye!\n";
    exit(0);
  }
}

// Get valid integer input in range
int getIntInput(int min, int max, const string &prompt) {
  int value;
  while (true) {
    cout << prompt;
    if (cin >> value && value >= min && value <= max) {
      clearInputBuffer();
      return value;
    }
    exitIfInputClosed();
    cout << "Invalid input. Please enter a number between " << min << " and "
         << max << endl;
    clearInputBuffer();
  }
}

// Create a player at the given position with user input
shared_ptr<player> createPlayer(Position position) {
  string name;

  cout << "Enter player name: ";
  getline(cin >> ws, name);

  int number = getIntInput(1, 99, "Enter jersey number (1-99): ");
  int age = getIntInput(21, 35, "Enter age (21-35): ");
  int overall = getIntInput(50, 99, "Enter overall rating (50-99): ");

  vector<int> attrs;
  for (const string &attr : attribute_names(position))
    attrs.push_back(getIntInput(50, 99, "Enter " + attr + " (50-99): "));
  return make_player(position, overall, number, age, name, attrs);
}

// Create a team with user input
team createTeam() {
  string teamName, coachName, city;
  vector<shared_ptr<player>> roster;

  cout << "\nCreate New Team\n";
  // cin >> ws skips any leftover newline without eating typed characters
  cout << "Enter team name: ";
  getline(cin >> ws, teamName);

  cout << "Enter head coach name: ";
  getline(cin >> ws, coachName);

  cout << "Enter City Name: ";
  getline(cin >> ws, city);

  cout << "\nDo you want to:\n";
  cout << "1. Create team player by player\n";
  cout << "2. Generate random roster\n";
  int choice = getIntInput(1, 2, "Enter choice (1 or 2): ");

  if (choice == 1) {
    cout << "\nYou will need to create:\n";
    for (const auto &[position, count] : ROSTER_TEMPLATE)
      cout << count << " x " << position_full_name(position) << "\n";

    for (const auto &[position, count] : ROSTER_TEMPLATE) {
      for (int i = 0; i < count; i++) {
        cout << "\n" << position_full_name(position) << " " << i + 1 << " of "
             << count << ":\n";
        roster.push_back(createPlayer(position));
      }
    }
  } else {
    roster = generateRoster();
  }

  return team(teamName, coachName, city, roster);
}

// Display a team's roster
void displayTeam(const team &t) {
  cout << "\nTeam: " << t.get_team_name() << "\n";
  cout << "Head Coach: " << t.get_coach_name() << "\n";
  cout << "Roster (" << t.get_players().size() << " players):\n";

  // Group players by position for organized display
  map<Position, vector<const player *>> positionGroups;
  for (const auto &sp : t.get_players()) {
    if (!sp)
      continue;
    positionGroups[sp->get_position()].push_back(sp.get());
  }

  // Display each position group
  for (Position pos : ALL_POSITIONS) {
    if (positionGroups.count(pos)) {
      cout << "\n" << position_to_string(pos) << ":\n";
      for (const auto *p : positionGroups[pos]) {
        cout << "  " << p->get_name() << " #" << p->get_number()
             << " (OVR: " << p->get_overall_rating() << ")\n";
      }
    }
  }
  cout << "\n";
}

// Read a whole line, so filenames and names may contain spaces
string getLineInput(const string &prompt) {
  string value;
  cout << prompt;
  getline(cin >> ws, value);
  exitIfInputClosed();
  return value;
}

// Make sure a league exists, creating the 32 NFL teams if needed
void ensureLeague(league &lg) {
  if (lg.empty()) {
    cout << "No league loaded. Generating the 32 NFL teams...\n";
    lg = league::create_nfl();
  }
}

// List the league's teams with records and return the chosen index
int pickLeagueTeam(const league &lg, const string &prompt) {
  const vector<team> &teams = lg.get_teams();
  cout << "\n";
  for (size_t i = 0; i < teams.size(); ++i) {
    cout << i + 1 << ". " << teams[i].get_city() << " "
         << teams[i].get_team_name() << " (" << teams[i].get_record_string()
         << ")" << ((int)i == lg.get_my_team() ? "  <- your team" : "")
         << "\n";
  }
  return getIntInput(1, (int)teams.size(),
                     prompt + " (1-" + to_string(teams.size()) + "): ") -
         1;
}

// "Buffalo Bills"
string fullName(const team &t) { return t.get_city() + " " + t.get_team_name(); }

void printWeekResults(const league &lg, int week,
                      const vector<scheduled_game> &games) {
  const vector<team> &teams = lg.get_teams();
  cout << "\nWeek " << week << " results:\n";
  for (const scheduled_game &g : games) {
    bool mine = g.home == lg.get_my_team() || g.away == lg.get_my_team();
    cout << (mine ? " * " : "   ") << teams[g.away].get_team_name() << " "
         << g.away_score << " @ " << teams[g.home].get_team_name() << " "
         << g.home_score << (g.overtime ? " (OT)" : "") << "\n";
  }
}

void printStandings(const league &lg) {
  const vector<team> &teams = lg.get_teams();
  cout << "\nSeason " << lg.get_season() << " standings";
  if (lg.season_over())
    cout << " (final)";
  else if (lg.get_week() == 1)
    cout << " (preseason)";
  else
    cout << " after week " << lg.get_week() - 1;
  cout << "\n";
  for (int d = 0; d < NUM_DIVISIONS; d++) {
    printf("\n%-24s %3s %3s %3s %5s %5s %5s\n", division_name(d).c_str(), "W",
           "L", "T", "PCT", "PF", "PA");
    for (int idx : lg.division_standings(d)) {
      const team &t = teams[idx];
      char pct[8];
      snprintf(pct, sizeof pct, "%.3f", t.get_win_pct());
      // NFL style: .750, but 1.000 for an unbeaten team
      const char *shown = pct[0] == '0' ? pct + 1 : pct;
      printf("%-22s%s %3d %3d %3d %5s %5d %5d\n", fullName(t).c_str(),
             idx == lg.get_my_team() ? " *" : "  ", t.get_wins(),
             t.get_losses(), t.get_ties(), shown, t.get_points_for(),
             t.get_points_against());
    }
  }
  fflush(stdout);
}

void printTeamSchedule(const league &lg, int teamIndex) {
  const vector<team> &teams = lg.get_teams();
  cout << "\n" << fullName(teams[teamIndex]) << " - season "
       << lg.get_season() << " (" << teams[teamIndex].get_record_string()
       << ")\n";
  for (int week = 1; week <= SEASON_WEEKS; week++) {
    for (const scheduled_game &g : lg.get_schedule()) {
      if (g.week != week || (g.home != teamIndex && g.away != teamIndex))
        continue;
      bool home = g.home == teamIndex;
      const team &opp = teams[home ? g.away : g.home];
      printf("Week %2d  %s %-14s", week, home ? "vs" : " @",
             opp.get_team_name().c_str());
      if (g.played) {
        int us = home ? g.home_score : g.away_score;
        int them = home ? g.away_score : g.home_score;
        printf("  %c %d-%d%s", us > them ? 'W' : us < them ? 'L' : 'T', us,
               them, g.overtime ? " (OT)" : "");
      }
      printf("\n");
    }
  }
  fflush(stdout);
}

// Playing a week needs a season still in progress
bool checkSeasonNotOver(const league &lg) {
  if (lg.season_over()) {
    cout << "The regular season is over. Start the next season (option 6)"
            " to keep playing.\n";
    return false;
  }
  return true;
}

void announceSeasonEnd(const league &lg) {
  const vector<team> &teams = lg.get_teams();
  cout << "\nThe season " << lg.get_season()
       << " regular season is complete! Division winners:\n";
  for (int d = 0; d < NUM_DIVISIONS; d++) {
    const team &winner = teams[lg.division_standings(d)[0]];
    cout << "  " << division_name(d) << ": " << fullName(winner) << " ("
         << winner.get_record_string() << ")\n";
  }
}

void leagueMenu(league &lg) {
  ensureLeague(lg);
  while (true) {
    cout << "\nLeague - Season " << lg.get_season();
    if (lg.season_over())
      cout << ", regular season complete";
    else
      cout << ", week " << lg.get_week() << " of " << SEASON_WEEKS;
    if (lg.get_my_team() >= 0)
      cout << ", your team: " << fullName(lg.get_teams()[lg.get_my_team()])
           << " (" << lg.get_teams()[lg.get_my_team()].get_record_string()
           << ")";
    cout << "\n1. Play next week\n";
    cout << "2. Play next week and watch your team's game\n";
    cout << "3. Sim to the end of the regular season\n";
    cout << "4. Standings\n";
    cout << "5. Team schedule & results\n";
    cout << "6. Start next season\n";
    cout << "7. Choose your team\n";
    cout << "8. View a team's roster\n";
    cout << "9. Start a new league\n";
    cout << "10. Save league\n";
    cout << "11. Load league\n";
    cout << "12. Back to main menu\n";
    int choice = getIntInput(1, 12, "Enter choice (1-12): ");

    switch (choice) {
    case 1:
    case 2: {
      if (!checkSeasonNotOver(lg))
        break;
      if (choice == 2 && lg.get_my_team() < 0)
        lg.set_my_team(pickLeagueTeam(lg, "Choose your team first"));
      int week = lg.get_week();
      vector<scheduled_game> games = lg.play_week(choice == 2 ? &cout : nullptr);
      printWeekResults(lg, week, games);
      if (lg.season_over())
        announceSeasonEnd(lg);
      break;
    }
    case 3:
      if (!checkSeasonNotOver(lg))
        break;
      while (!lg.season_over())
        lg.play_week();
      announceSeasonEnd(lg);
      break;
    case 4:
      printStandings(lg);
      break;
    case 5: {
      int teamIndex = lg.get_my_team();
      if (teamIndex < 0 ||
          getIntInput(1, 2, "1. Your team  2. Another team: ") == 2)
        teamIndex = pickLeagueTeam(lg, "Enter team number");
      printTeamSchedule(lg, teamIndex);
      break;
    }
    case 6:
      if (!lg.season_over() &&
          getIntInput(1, 2, "The season isn't finished. Skip the rest and "
                            "start next season? (1 = yes, 2 = no): ") != 1)
        break;
      lg.start_next_season();
      cout << "Season " << lg.get_season()
           << " begins! Records are reset and a new schedule is out.\n";
      break;
    case 7:
      lg.set_my_team(pickLeagueTeam(lg, "Choose your team"));
      cout << "You are now the GM of the "
           << fullName(lg.get_teams()[lg.get_my_team()]) << "!\n";
      break;
    case 8:
      displayTeam(lg.get_teams()[pickLeagueTeam(lg, "Enter team number")]);
      break;
    case 9:
      if (getIntInput(1, 2, "This replaces the current league. Continue? "
                            "(1 = yes, 2 = no): ") != 1)
        break;
      lg = league::create_nfl();
      cout << "New league created with all 32 NFL teams.\n";
      break;
    case 10: {
      string path = getLineInput("Enter filename to save the league to: ");
      if (save_league(lg, path))
        cout << "League saved to " << path << "\n";
      else
        cout << "Failed to save league to " << path << "\n";
      break;
    }
    case 11: {
      string path = getLineInput("Enter filename to load the league from: ");
      if (load_league(path, lg))
        cout << "Loaded season " << lg.get_season() << " league from " << path
             << "\n";
      else
        cout << "Failed to load a league from " << path << "\n";
      break;
    }
    case 12:
      return;
    }
  }
}

int main() {
  int choice;
  league lg;

  do {
    cout << "\nFootball Team Manager\n";
    cout << "1. League & Season\n";
    cout << "2. View a League Team\n";
    cout << "3. Create Custom Team\n";
    cout << "4. Generate Random Team\n";
    cout << "5. Play an Exhibition Game\n";
    cout << "6. Exit\n";
    cout << "7. Save a Team to File\n";
    cout << "8. Load a Team from File\n";
    choice = getIntInput(1, 8, "Enter choice (1-8): ");

    switch (choice) {
    case 1:
      leagueMenu(lg);
      break;
    case 2:
      ensureLeague(lg);
      displayTeam(lg.get_teams()[pickLeagueTeam(lg, "Enter team number")]);
      break;
    case 3: {
      team customTeam = createTeam();
      cout << "\nCustom team created:\n";
      displayTeam(customTeam);
      break;
    }
    case 4: {
      team randomTeam = generateRandTeam();
      cout << "\nRandom team generated:\n";
      displayTeam(randomTeam);
      break;
    }
    case 5: {
      // exhibition games never change league records
      cout << "Would you like to: \n1.) Create 2 teams from scratch \n2.) Pick "
              "a league team and play against the computer\n3.) Generate 2 "
              "random teams and have them play\n";
      cout << "4.) Load a saved team from file and play against a generated "
              "opponent\n";
      int gameChoice = getIntInput(1, 4, "Enter choice (1-4): ");

      team home = generateRandTeam();
      team away = generateRandTeam();
      game_state game;

      if (gameChoice == 1) {
        cout << "\nCreate Home Team:\n";
        home = createTeam();
        cout << "\nCreate Away Team:\n";
        away = createTeam();
      } else if (gameChoice == 2) {
        ensureLeague(lg);
        // player will use the selected league team as home
        home = lg.get_teams()[pickLeagueTeam(lg, "Enter team number")];
        cout << "Generating a random opponent...\n";
        away = generateRandTeam();
      } else if (gameChoice == 3) {
        // default: two random teams
        home = generateRandTeam();
        away = generateRandTeam();
      } else {
        // load from file and play a generated opponent
        string path = getLineInput("Enter filename to load team from: ");
        team loaded("", "", "", {});
        if (load_team(path, loaded)) {
          cout << "Loaded team: " << loaded.get_team_name() << "\n";
          home = loaded;
          away = generateRandTeam();
        } else {
          cout << "Failed to load team from " << path
               << ". Using two random teams instead.\n";
          home = generateRandTeam();
          away = generateRandTeam();
        }
      }

      cout << "\nHome team: " << home.get_team_name() << "\n";
      cout << "Away team: " << away.get_team_name() << "\n";
      game.play_game(home, away);
      break;
    }
    case 6:
      cout << "Goodbye!\n";
      break;
    case 7: {
      cout << "Save a team to file.\n";
      cout << "1) Save a league team\n";
      cout << "2) Create a new custom team to save\n";
      cout << "3) Generate a random team to save\n";
      int schoice = getIntInput(1, 3, "Enter choice (1-3): ");
      team tsave("", "", "", {});
      if (schoice == 1) {
        ensureLeague(lg);
        tsave = lg.get_teams()[pickLeagueTeam(lg, "Select team to save")];
      } else if (schoice == 2) {
        tsave = createTeam();
      } else {
        tsave = generateRandTeam();
      }
      string path = getLineInput("Enter filename to save to: ");
      if (save_team(tsave, path))
        cout << "Team saved to " << path << "\n";
      else
        cout << "Failed to save team to " << path << "\n";
      break;
    }
    case 8: {
      cout << "Load a team from file.\n";
      string path = getLineInput("Enter filename to load from: ");
      team loaded("", "", "", {});
      if (load_team(path, loaded)) {
        cout << "Team loaded from " << path << " ("
             << loaded.get_record_string() << "):\n";
        displayTeam(loaded);
      } else {
        cout << "Failed to load team from " << path << "\n";
      }
      break;
    }
    }
  } while (choice != 6);

  return 0;
}
