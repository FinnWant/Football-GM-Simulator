# Football-GM-Simulator  
*A C++ command-line simulation of managing a football (American) team as a General Manager*

## 📌 Project Overview  
Football-GM-Simulator is a lightweight, console-based simulation engine written in C++. The goal is to give you a sandbox to manage a football franchise: draft players, build a roster, handle seasons, and simulate outcomes. It’s designed with extensibility in mind—ideal for learning, hobby-projects, or as a base for more detailed graphical versions in the future.

## 🧩 Key Features  
- Fully in C++ (single major executable) — no external dependencies beyond the standard library  
- Basic object-oriented design: Player, Team, Simulator modules (see `playerH.h` / `playerIMP.cpp`, `team.h` / `teamIMP.cpp`, `simulation.h` / `simulation.cpp`)  
- Persistence: Saves and loads state via the `persistence` module (`persistence.h` / `persistence.cpp`)  
- Simulations of seasons, games, and progress of players over time  
- Modular structure to allow you to plug in your own logic (for example: player growth, injury modelling, play-calling)  
- Ideal platform for extending: UI front-end, database integration, web API, ML-driven draft logic, etc.

## 📂 Repository Structure  
/.vscode ← configuration, optional
.gitignore
main.cpp ← program entry point
persistence.h/.cpp ← save/load functionality
playerH.h ← player class definitions
playerIMP.cpp ← player class implementations
team.h ← team class definitions
teamIMP.cpp ← team class implementations
simulation.h/.cpp ← game simulation (lineups, play-by-play)
generation.h/.cpp ← random players, rosters, teams and the NFL franchise list
league.h/.cpp ← the 32-team league: season, standings, playing weeks
schedule.h/.cpp ← divisions and the 17-week schedule generator
rng.h ← shared random number generator
tests/ ← unit tests and the balance check
Makefile ← build script


## 🚀 Getting Started  
### Prerequisites  
- A C++17 compiler (g++ 8 or later, or MSYS2 UCRT64 on Windows)  
- make (`mingw32-make` on MSYS2)  
- Standard library only — no additional libraries required

### Build Instructions  
```
git clone https://github.com/FinnWant/Football-GM-Simulator.git
cd Football-GM-Simulator
make              # mingw32-make on MSYS2
./FootballGMSim
```

### Testing
```
make test       # unit tests, then the balance check
make balance    # balance report only
```
The balance check plays 2,000 seeded games between random teams and fails if
league averages (points, completion %, sacks, yards per carry, ...) drift out
of NFL-like ranges. Run it after changing anything in `simulation.cpp`.
`./tests/balance 10000 7` plays 10,000 games with seed 7.

Without make, compile every .cpp file in the top folder directly:
```
g++ -std=c++17 *.cpp -o FootballGMSim
```

## Usage

Once built, run FootballGMSim. You’ll be able to:

- Run a league of all 32 NFL teams through a 17-week season: each team plays
  its division rivals home and away plus 11 other opponents
- Pick your team, play week by week (or watch your team's game
  play-by-play), check division standings and your schedule and results
- Sim to the end of the season and start the next one with a new schedule
- Save and load the whole league mid-season
- Create, generate, save and load individual teams, and play exhibition
  games that don't count toward the standings
- Draft or sign players (coming soon)

## 📈 Why This Project?

This simulator provides a fun, educational tool in several ways:
Learning C++ architecture: Demonstrates how to structure classes, split headers/implementations, manage persistence, and simulate events
Algorithmic design & simulation: Game-logic, season progression, team management all offer algorithmic challenge
Extensibility for hobbyists: You could build a GUI on top, introduce a web interface, integrate machine learning for drafting, or build a mobile version
Passion for sports & coding: Combines your love for football (your favourite team being the Pittsburgh Steelers!) and software development into a cohesive project.

## 🤝 Contributing

Contributions are very welcome! Here’s how to help:
Fork the repository
Create a feature branch (git checkout -b feature/YourFeature)
Commit your changes with clear messages
Push to your fork and open a Pull Request
Ensure your code compiles and adheres to the existing coding style
Write tests if you introduce new logic
Describe your feature/enhancement in the PR description

## 📝 License

#### This project is open-source under the MIT License — feel free to modify, share, and build upon it.

## 📬 Contact & Feedback
Created by FinnWant. If you have ideas, bug reports, or want to collaborate:
Open an issue on GitHub
Or drop me a message on GitHub

Enjoy building and managing your football empire! 🏈
