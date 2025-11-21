#include "persistence.h"
#include "playerH.h"
#include "simulation.h"
#include "team.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>
using namespace std;

// NFL team data structure
struct NFLTeamInfo {
  string name;
  string city;
  string coach;
};

// NFL Teams Database
vector<NFLTeamInfo> nflTeams = {{"Bills", "Buffalo", "Sean McDermott"},
                                {"Dolphins", "Miami", "Mike McDaniel"},
                                {"Patriots", "New England", "Bill Belichick"},
                                {"Jets", "New York", "Robert Saleh"},
                                {"Ravens", "Baltimore", "John Harbaugh"},
                                {"Bengals", "Cincinnati", "Zac Taylor"},
                                {"Browns", "Cleveland", "Kevin Stefanski"},
                                {"Steelers", "Pittsburgh", "Mike Tomlin"},
                                {"Texans", "Houston", "DeMeco Ryans"},
                                {"Colts", "Indianapolis", "Shane Steichen"},
                                {"Jaguars", "Jacksonville", "Doug Pederson"},
                                {"Titans", "Tennessee", "Mike Vrabel"},
                                {"Broncos", "Denver", "Sean Payton"},
                                {"Chiefs", "Kansas City", "Andy Reid"},
                                {"Raiders", "Las Vegas", "Josh McDaniels"},
                                {"Chargers", "Los Angeles", "Brandon Staley"},
                                {"Cowboys", "Dallas", "Mike McCarthy"},
                                {"Giants", "New York", "Brian Daboll"},
                                {"Eagles", "Philadelphia", "Nick Sirianni"},
                                {"Commanders", "Washington", "Ron Rivera"},
                                {"Bears", "Chicago", "Matt Eberflus"},
                                {"Lions", "Detroit", "Dan Campbell"},
                                {"Packers", "Green Bay", "Matt LaFleur"},
                                {"Vikings", "Minnesota", "Kevin O'Connell"},
                                {"Falcons", "Atlanta", "Arthur Smith"},
                                {"Panthers", "Carolina", "Frank Reich"},
                                {"Saints", "New Orleans", "Dennis Allen"},
                                {"Buccaneers", "Tampa Bay", "Todd Bowles"},
                                {"Cardinals", "Arizona", "Jonathan Gannon"},
                                {"Rams", "Los Angeles", "Sean McVay"},
                                {"49ers", "San Francisco", "Kyle Shanahan"},
                                {"Seahawks", "Seattle", "Pete Carroll"}};

// Random number generator setup
random_device rd;
mt19937 gen(rd());

// Helper function to generate random number in range [min, max]
int randInt(int min, int max) {
  uniform_int_distribution<> dis(min, max);
  return dis(gen);
}

// Lists of names for random generation
vector<string> firstNames = {
    "John",      "Michael",     "David",   "James",  "Robert", "William",
    "Thomas",    "Christopher", "Daniel",  "Joseph", "Steven", "Brian",
    "Kevin",     "Jason",       "Justin",  "Ryan",   "Eric",   "Brandon",
    "Alexander", "Zachary",     "Nathan",  "Austin", "Hunter", "Dylan",
    "Blake",     "Logan",       "Evan",    "Sean",   "Tyler",  "Landon",
    "Parker",    "Matthew",     "Andrew",  "Mark",   "Paul",   "Kenneth",
    "George",    "Scott",       "Charles", "Anthony"};
vector<string> lastNames = {
    "Smith",    "Johnson", "Williams",  "Brown",      "Jones",     "Garcia",
    "Miller",   "Davis",   "Rodriguez", "Martinez",   "Hernandez", "Lopez",
    "Gonzalez", "Wilson",  "Anderson",  "Clark",      "Lee",       "Walker",
    "Hall",     "Allen",   "Young",     "King",       "Wright",    "Scott",
    "Green",    "Adams",   "Baker",     "Nelson",     "Carter",    "Mitchell",
    "Perez",    "Roberts", "Turner",    "Phillips",   "Campbell",  "Parker",
    "Evans",    "Edwards", "Collins",   "Stewart",    "Sanchez",   "Morris",
    "Rogers",   "Reed",    "Cook",      "Morgan",     "Bell",      "Murphy",
    "Bailey",   "Rivera",  "Cooper",    "Richardson", "Cox",       "Howard",
    "Ward",     "Torres",  "Peterson",  "Gray",       "Ramirez",   "James",
    "Brooks"};
vector<string> teamNames = {
    "Eagles",    "Lions",     "Bears",      "Vikings",    "Falcons",
    "Panthers",  "Ravens",    "Bengals",    "Browns",     "Steelers",
    "Texans",    "Colts",     "Jaguars",    "Titans",     "Broncos",
    "Sharks",    "Tigers",    "Armada",     "Sentinels",  "Cyclones",
    "Warriors",  "Avalanche", "Comets",     "Rockets",    "Fusion",
    "Guardians", "Marauders", "Dragons",    "Phoenix",    "Hornets",
    "Spartans",  "Knights",   "Outlaws",    "Renegades",  "Stallions",
    "Riders",    "Legends",   "Pioneers",   "Mustangs",   "Trailblazers",
    "Thunder",   "Blizzard",  "Hurricanes", "Miners",     "Raptors",
    "Beacons",   "Savages",   "Phantoms",   "Barracudas", "Outriders"};
vector<string> cities = {
    "New York",       "Los Angeles",      "Chicago",       "Houston",
    "Phoenix",        "Philadelphia",     "San Antonio",   "San Diego",
    "Dallas",         "Austin",           "Jacksonville",  "Fort Worth",
    "Columbus",       "Charlotte",        "San Francisco", "Indianapolis",
    "Seattle",        "Denver",           "Boston",        "El Paso",
    "Detroit",        "Nashville",        "Portland",      "Memphis",
    "Oklahoma City",  "Las Vegas",        "Louisville",    "Baltimore",
    "Milwaukee",      "Albuquerque",      "Tucson",        "Fresno",
    "Sacramento",     "Kansas City",      "Mesa",          "Atlanta",
    "Omaha",          "Colorado Springs", "Raleigh",       "Miami",
    "Virginia Beach", "Oakland",          "Minneapolis",   "Tulsa",
    "Arlington",      "New Orleans"};
vector<string> coachFirstNames = {
    "Andy",  "Bill",  "Mike",   "John",   "Pete",   "Sean",  "Kyle",  "Matt",
    "Bruce", "Dan",   "Jim",    "Ron",    "Frank",  "Todd",  "Doug",  "Rick",
    "Eli",   "Greg",  "Nick",   "Ryan",   "Hunter", "Shane", "Corey", "Sam",
    "Alex",  "Chris", "Pat",    "Ed",     "Tom",    "Gary",  "Mark",  "Steve",
    "Vince", "Leo",   "Walter", "Martin", "Oscar",  "Theo",  "Drew",  "Jordan"};

// Generate a random name
string generateName() {
  string first = firstNames[randInt(0, firstNames.size() - 1)];
  string last = lastNames[randInt(0, lastNames.size() - 1)];
  return first + " " + last;
}

// Generate a random city name
string generateCity() {
  string city = cities[randInt(0, cities.size() - 1)];
  return city;
}

// Generate a random team name
string generateTeamName() {
  string name = teamNames[randInt(0, teamNames.size() - 1)];
  return name;
}

// Generate a random coach name
string generateCoachName() {
  string first = coachFirstNames[randInt(0, coachFirstNames.size() - 1)];
  string last = lastNames[randInt(0, lastNames.size() - 1)];
  return first + " " + last;
}

// Function to clear input buffer and handle invalid input
void clearInputBuffer() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    cout << "Invalid input. Please enter a number between " << min << " and "
         << max << endl;
    clearInputBuffer();
  }
}

// Generate a random player for the given position
shared_ptr<player> generateRandPlayer(string position) {
  string name = generateName();
  int number = randInt(1, 99);
  int age = randInt(21, 35);
  int overall = randInt(50, 99);

  if (position == "QB") {
    return make_shared<quarterback>(position, overall, number, age, name,
                                    randInt(50, 99), randInt(50, 99),
                                    randInt(50, 99), randInt(50, 99));
  } else if (position == "RB") {
    return make_shared<runningback>(position, overall, number, age, name,
                                    randInt(50, 99), randInt(50, 99),
                                    randInt(50, 99), randInt(50, 99));
  } else if (position == "WR") {
    return make_shared<wide_receiver>(position, overall, number, age, name,
                                      randInt(50, 99), randInt(50, 99),
                                      randInt(50, 99));
  } else if (position == "TE") {
    return make_shared<tight_end>(position, overall, number, age, name,
                                  randInt(50, 99), randInt(50, 99),
                                  randInt(50, 99));
  } else if (position == "OL") {
    return make_shared<lineman>(position, overall, number, age, name,
                                randInt(50, 99), randInt(50, 99),
                                randInt(50, 99));
  } else if (position == "DL") {
    return make_shared<defensive_lineman>(position, overall, number, age, name,
                                          randInt(50, 99), randInt(50, 99),
                                          randInt(50, 99));
  } else if (position == "LB") {
    return make_shared<linebacker>(position, overall, number, age, name,
                                   randInt(50, 99), randInt(50, 99),
                                   randInt(50, 99));
  } else if (position == "DB") {
    return make_shared<defensive_back>(position, overall, number, age, name,
                                       randInt(50, 99), randInt(50, 99),
                                       randInt(50, 99));
  } else if (position == "K") {
    return make_shared<kicker>(position, overall, number, age, name,
                               randInt(50, 99), randInt(50, 99));
  } else if (position == "P") {
    return make_shared<punter>(position, overall, number, age, name,
                               randInt(50, 99), randInt(50, 99));
  }
  // Default case - should never reach here if position is valid
  return make_shared<player>(position, overall, number, age, name);
}

// Create a player with user input
shared_ptr<player> createPlayer() {
  string name, position;
  int number, age, overall;

  cout << "\nCreate New Player\n";
  cout << "Enter player name: ";
  cin.ignore();
  getline(cin, name);

  cout << "\nAvailable positions:\n";
  cout << "QB - Quarterback\nRB - Running Back\nWR - Wide Receiver\n";
  cout << "TE - Tight End\nOL - Offensive Lineman\nDL - Defensive Lineman\n";
  cout << "LB - Linebacker\nDB - Defensive Back\nK - Kicker\nP - Punter\n";

  do {
    cout << "Enter position (QB/RB/WR/TE/OL/DL/LB/DB/K/P): ";
    cin >> position;
    transform(position.begin(), position.end(), position.begin(), ::toupper);
  } while (position != "QB" && position != "RB" && position != "WR" &&
           position != "TE" && position != "OL" && position != "DL" &&
           position != "LB" && position != "DB" && position != "K" &&
           position != "P");

  number = getIntInput(1, 99, "Enter jersey number (1-99): ");
  age = getIntInput(21, 35, "Enter age (21-35): ");
  overall = getIntInput(50, 99, "Enter overall rating (50-99): ");

  if (position == "QB") {
    int tp = getIntInput(50, 99, "Enter throw power (50-99): ");
    int ta = getIntInput(50, 99, "Enter throw accuracy (50-99): ");
    int spd = getIntInput(50, 99, "Enter speed (50-99): ");
    int pv = getIntInput(50, 99, "Enter passing vision (50-99): ");
    return make_shared<quarterback>(position, overall, number, age, name, tp,
                                    ta, spd, pv);
  } else if (position == "RB") {
    int spd = getIntInput(50, 99, "Enter speed (50-99): ");
    int car = getIntInput(50, 99, "Enter carrying (50-99): ");
    int elu = getIntInput(50, 99, "Enter elusiveness (50-99): ");
    int blk = getIntInput(50, 99, "Enter blocking (50-99): ");
    return make_shared<runningback>(position, overall, number, age, name, spd,
                                    car, elu, blk);
  } else if (position == "WR") {
    int spd = getIntInput(50, 99, "Enter speed (50-99): ");
    int ctch = getIntInput(50, 99, "Enter catching (50-99): ");
    int rr = getIntInput(50, 99, "Enter route running (50-99): ");
    return make_shared<wide_receiver>(position, overall, number, age, name, spd,
                                      ctch, rr);
  } else if (position == "TE") {
    int spd = getIntInput(50, 99, "Enter speed (50-99): ");
    int ctch = getIntInput(50, 99, "Enter catching (50-99): ");
    int blk = getIntInput(50, 99, "Enter blocking (50-99): ");
    return make_shared<tight_end>(position, overall, number, age, name, spd,
                                  ctch, blk);
  } else if (position == "OL" || position == "DL") {
    int str = getIntInput(50, 99, "Enter strength (50-99): ");
    int blk = getIntInput(50, 99, "Enter blocking (50-99): ");
    int tkl = getIntInput(50, 99, "Enter tackling (50-99): ");
    if (position == "OL") {
      return make_shared<lineman>(position, overall, number, age, name, str,
                                  blk, tkl);
    } else {
      return make_shared<defensive_lineman>(position, overall, number, age,
                                            name, str, tkl, blk);
    }
  } else if (position == "LB") {
    int spd = getIntInput(50, 99, "Enter speed (50-99): ");
    int tkl = getIntInput(50, 99, "Enter tackling (50-99): ");
    int cvr = getIntInput(50, 99, "Enter coverage (50-99): ");
    return make_shared<linebacker>(position, overall, number, age, name, spd,
                                   tkl, cvr);
  } else if (position == "DB") {
    int spd = getIntInput(50, 99, "Enter speed (50-99): ");
    int cvr = getIntInput(50, 99, "Enter coverage (50-99): ");
    int tkl = getIntInput(50, 99, "Enter tackling (50-99): ");
    return make_shared<defensive_back>(position, overall, number, age, name,
                                       spd, cvr, tkl);
  } else if (position == "K") {
    int kp = getIntInput(50, 99, "Enter kick power (50-99): ");
    int ka = getIntInput(50, 99, "Enter kick accuracy (50-99): ");
    return make_shared<kicker>(position, overall, number, age, name, kp, ka);
  } else if (position == "P") {
    int pp = getIntInput(50, 99, "Enter punt power (50-99): ");
    int pa = getIntInput(50, 99, "Enter punt accuracy (50-99): ");
    return make_shared<punter>(position, overall, number, age, name, pp, pa);
  }

  // Default case - should never reach here if position is valid
  return make_shared<player>(position, overall, number, age, name);
}

// Generate a random team with a full roster
team generateRandTeam() {
  string teamName = generateTeamName();
  string coachName = generateCoachName();
  string city = generateCity();
  vector<shared_ptr<player>> roster;

  // Create players for each position based on roster requirements
  for (int i = 0; i < 3; i++)
    roster.push_back(generateRandPlayer("QB"));
  for (int i = 0; i < 4; i++)
    roster.push_back(generateRandPlayer("RB"));
  for (int i = 0; i < 6; i++)
    roster.push_back(generateRandPlayer("WR"));
  for (int i = 0; i < 3; i++)
    roster.push_back(generateRandPlayer("TE"));
  for (int i = 0; i < 9; i++)
    roster.push_back(generateRandPlayer("OL"));
  for (int i = 0; i < 8; i++)
    roster.push_back(generateRandPlayer("DL"));
  for (int i = 0; i < 7; i++)
    roster.push_back(generateRandPlayer("LB"));
  for (int i = 0; i < 10; i++)
    roster.push_back(generateRandPlayer("DB"));
  roster.push_back(generateRandPlayer("K"));
  roster.push_back(generateRandPlayer("P"));

  return team(teamName, coachName, city, roster);
}

// Create a team with user input
// Generate all NFL teams
vector<team> generateNFLTeams() {
  vector<team> nflTeams_generated;

  for (const auto &teamInfo : nflTeams) {
    string fullName = teamInfo.name;
    string city = teamInfo.city;
    vector<shared_ptr<player>> roster;

    // Generate roster using proper NFL position counts
    for (int i = 0; i < 3; i++)
      roster.push_back(generateRandPlayer("QB"));
    for (int i = 0; i < 4; i++)
      roster.push_back(generateRandPlayer("RB"));
    for (int i = 0; i < 6; i++)
      roster.push_back(generateRandPlayer("WR"));
    for (int i = 0; i < 3; i++)
      roster.push_back(generateRandPlayer("TE"));
    for (int i = 0; i < 9; i++)
      roster.push_back(generateRandPlayer("OL"));
    for (int i = 0; i < 8; i++)
      roster.push_back(generateRandPlayer("DL"));
    for (int i = 0; i < 7; i++)
      roster.push_back(generateRandPlayer("LB"));
    for (int i = 0; i < 10; i++)
      roster.push_back(generateRandPlayer("DB"));
    roster.push_back(generateRandPlayer("K"));
    roster.push_back(generateRandPlayer("P"));

    team newTeam(fullName, teamInfo.coach, city, roster);
    nflTeams_generated.push_back(newTeam);
  }

  return nflTeams_generated;
}

team createTeam() {
  string teamName, coachName, city;
  char choice;
  vector<shared_ptr<player>> roster;

  cout << "\nCreate New Team\n";
  cout << "Enter team name: ";
  cin.ignore();
  getline(cin, teamName);

  cout << "Enter head coach name: ";
  getline(cin, coachName);

  cout << "Enter City Name: ";
  getline(cin, city);

  cout << "\nDo you want to:\n";
  cout << "1. Create team player by player\n";
  cout << "2. Generate random roster\n";
  cout << "Enter choice (1 or 2): ";
  cin >> choice;

  if (choice == '1') {
    cout << "\nYou will need to create:\n";
    cout << "3 Quarterbacks\n4 Running Backs\n6 Wide Receivers\n";
    cout << "3 Tight Ends\n9 Offensive Linemen\n8 Defensive Linemen\n";
    cout << "7 Linebackers\n10 Defensive Backs\n1 Kicker\n1 Punter\n\n";

    // Create each position group
    cout << "\nQuarterbacks (3):\n";
    for (int i = 0; i < 3; i++) {
      cout << "\nQuarterback " << i + 1 << ":\n";
      roster.push_back(createPlayer());
    }

    cout << "\nRunning Backs (4):\n";
    for (int i = 0; i < 4; i++) {
      cout << "\nRunning Back " << i + 1 << ":\n";
      roster.push_back(createPlayer());
    }

    cout << "\nWide Receivers (6):\n";
    for (int i = 0; i < 6; i++) {
      cout << "\nWide Receiver " << i + 1 << ":\n";
      roster.push_back(createPlayer());
    }

    cout << "\nTight Ends (3):\n";
    for (int i = 0; i < 3; i++) {
      cout << "\nTight End " << i + 1 << ":\n";
      roster.push_back(createPlayer());
    }

    cout << "\nOffensive Linemen (9):\n";
    for (int i = 0; i < 9; i++) {
      cout << "\nOffensive Lineman " << i + 1 << ":\n";
      roster.push_back(createPlayer());
    }

    cout << "\nDefensive Linemen (8):\n";
    for (int i = 0; i < 8; i++) {
      cout << "\nDefensive Lineman " << i + 1 << ":\n";
      roster.push_back(createPlayer());
    }

    cout << "\nLinebackers (7):\n";
    for (int i = 0; i < 7; i++) {
      cout << "\nLinebacker " << i + 1 << ":\n";
      roster.push_back(createPlayer());
    }

    cout << "\nDefensive Backs (10):\n";
    for (int i = 0; i < 10; i++) {
      cout << "\nDefensive Back " << i + 1 << ":\n";
      roster.push_back(createPlayer());
    }

    cout << "\nKicker:\n";
    roster.push_back(createPlayer());

    cout << "\nPunter:\n";
    roster.push_back(createPlayer());

  } else {
    // Generate random roster
    for (int i = 0; i < 3; i++)
      roster.push_back(generateRandPlayer("QB"));
    for (int i = 0; i < 4; i++)
      roster.push_back(generateRandPlayer("RB"));
    for (int i = 0; i < 6; i++)
      roster.push_back(generateRandPlayer("WR"));
    for (int i = 0; i < 3; i++)
      roster.push_back(generateRandPlayer("TE"));
    for (int i = 0; i < 9; i++)
      roster.push_back(generateRandPlayer("OL"));
    for (int i = 0; i < 8; i++)
      roster.push_back(generateRandPlayer("DL"));
    for (int i = 0; i < 7; i++)
      roster.push_back(generateRandPlayer("LB"));
    for (int i = 0; i < 10; i++)
      roster.push_back(generateRandPlayer("DB"));
    roster.push_back(generateRandPlayer("K"));
    roster.push_back(generateRandPlayer("P"));
  }

  return team(teamName, coachName, city, roster);
}

// Display a team's roster
void displayTeam(const team &t) {
  cout << "\nTeam: " << t.get_team_name() << "\n";
  cout << "Head Coach: " << t.get_coach_name() << "\n";
  cout << "Roster (" << t.get_players().size() << " players):\n";

  // Group players by position for organized display
  map<string, vector<const player *>> positionGroups;
  for (const auto &sp : t.get_players()) {
    if (!sp)
      continue;
    positionGroups[sp->get_position()].push_back(sp.get());
  }

  // Display each position group
  vector<string> positionOrder = {"QB", "RB", "WR", "TE", "OL",
                                  "DL", "LB", "DB", "K",  "P"};
  for (const auto &pos : positionOrder) {
    if (positionGroups.count(pos)) {
      cout << "\n" << pos << ":\n";
      for (const auto *p : positionGroups[pos]) {
        cout << "  " << p->get_name() << " #" << p->get_number()
             << " (OVR: " << p->get_overall_rating() << ")\n";
      }
    }
  }
  cout << "\n";
}

int main() {
  char choice;
  vector<team> nflTeams;

  do {
    cout << "\nFootball Team Manager\n";
    cout << "1. Load NFL Teams\n";
    cout << "2. View NFL Team\n";
    cout << "3. Create Custom Team\n";
    cout << "4. Generate Random Team\n";
    cout << "5. Play a Game\n";
    cout << "6. Exit\n";
    cout << "7. Save a Team to File\n";
    cout << "8. Load a Team from File\n";
    cout << "Enter choice (1-8): ";
    cin >> choice;

    switch (choice) {
    case '1': {
      cout << "Generating NFL teams...\n";
      nflTeams = generateNFLTeams();
      cout << "All 32 NFL teams have been generated!\n";
      break;
    }
    case '2': {
      if (nflTeams.empty()) {
        cout << "Please load NFL teams first (option 1).\n";
        break;
      }

      cout << "\nAvailable NFL Teams:\n";
      for (size_t i = 0; i < nflTeams.size(); ++i) {
        cout << i + 1 << ". " << nflTeams[i].get_team_name() << "\n";
      }

      int teamChoice;
      cout << "\nEnter team number (1-32): ";
      cin >> teamChoice;

      if (teamChoice >= 1 && teamChoice <= 32) {
        displayTeam(nflTeams[teamChoice - 1]);
      } else {
        cout << "Invalid team number.\n";
      }
      break;
    }
    case '3': {
      team customTeam = createTeam();
      cout << "\nCustom team created:\n";
      displayTeam(customTeam);
      break;
    }
    case '4': {
      team randomTeam = generateRandTeam();
      cout << "\nRandom team generated:\n";
      displayTeam(randomTeam);
      break;
    }
    case '5': {
      cout << "Would you like to: \n1.) Create 2 teams from scratch \n2.) Load "
              "an NFL team and play against the computer\n3.) Generate 2 "
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
        // ensure NFL teams are loaded
        if (nflTeams.empty()) {
          cout << "NFL teams are not loaded. Generating NFL teams now...\n";
          nflTeams = generateNFLTeams();
        }

        cout << "\nAvailable NFL Teams:\n";
        for (size_t i = 0; i < nflTeams.size(); ++i) {
          cout << i + 1 << ". " << nflTeams[i].get_team_name() << "\n";
        }
        int teamChoice =
            getIntInput(1, (int)nflTeams.size(), "Enter team number (1-32): ");
        // player will use the selected NFL team as home
        home = nflTeams[teamChoice - 1];
        cout << "Generating a random opponent...\n";
        away = generateRandTeam();
      } else if (gameChoice == 3) {
        // default: two random teams
        home = generateRandTeam();
        away = generateRandTeam();
      } else {
        // load from file and play a generated opponent
        cout << "Enter filename to load team from: ";
        string path;
        cin >> path;
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
    case '6':
      cout << "Goodbye!\n";
      break;
    case '7': {
      cout << "Save a team to file.\n";
      cout << "1) Save an NFL team (must be loaded)\n";
      cout << "2) Create a new custom team to save\n";
      cout << "3) Generate a random team to save\n";
      int schoice = getIntInput(1, 3, "Enter choice (1-3): ");
      team tsave("", "", "", {});
      if (schoice == 1) {
        if (nflTeams.empty()) {
          cout << "NFL teams are not loaded. Generating now...\n";
          nflTeams = generateNFLTeams();
        }
        cout << "Select NFL team to save:\n";
        for (size_t i = 0; i < nflTeams.size(); ++i)
          cout << i + 1 << ". " << nflTeams[i].get_team_name() << "\n";
        int idx = getIntInput(1, (int)nflTeams.size(), "Enter team number: ");
        tsave = nflTeams[idx - 1];
      } else if (schoice == 2) {
        tsave = createTeam();
      } else {
        tsave = generateRandTeam();
      }
      string path;
      cout << "Enter filename to save to: ";
      cin >> path;
      if (save_team(tsave, path))
        cout << "Team saved to " << path << "\n";
      else
        cout << "Failed to save team to " << path << "\n";
      break;
    }
    case '8': {
      cout << "Load a team from file.\n";
      string path;
      cout << "Enter filename to load from: ";
      cin >> path;
      team loaded("", "", "", {});
      if (load_team(path, loaded)) {
        cout << "Team loaded from " << path << ":\n";
        displayTeam(loaded);
      } else {
        cout << "Failed to load team from " << path << "\n";
      }
      break;
    }
    default:
      cout << "Invalid choice. Please try again.\n";
    }
  } while (choice != '6');

  return 0;
}