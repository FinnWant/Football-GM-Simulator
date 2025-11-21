# Football-GM-Simulator  
*A C++ command-line simulation of managing a football (American) team as a General Manager*

## 📌 Project Overview  
Football-GM-Simulator is a lightweight, console-based simulation engine written in C++. The goal is to give you a sandbox to manage a football franchise: draft players, build a roster, handle seasons, and simulate outcomes. It’s designed with extensibility in mind—ideal for learning, hobby-projects, or as a base for more detailed graphical versions in the future.

## 🧩 Key Features  
- Fully in C++ (single major executable) — no external dependencies beyond the standard library  
- Basic object-oriented design: Player, Team, Simulator modules (see `playerH.h` / `playerIMP.cpp`, `team.h` / `teamIMP.cpp`, `simulation.h`)  
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
simulation.h ← simulation logic (running seasons, games)
(simulator/) ← (optional) extra resources, data sets, future UI components


## 🚀 Getting Started  
### Prerequisites  
- A modern C++ compiler (supports C++11 or later)  
- (Optional) CMake or your build system of choice  
- Standard library only — no additional libraries required

### Build Instructions (example)  
git clone https://github.com/FinnWant/Football-GM-Simulator.git  
cd Football-GM-Simulator  
# Using g++ as example:
g++ -std=c++17 main.cpp persistence.cpp playerIMP.cpp teamIMP.cpp -o FootballGMSim  
./FootballGMSim


Alternatively, if you choose to add a CMakeLists.txt, you can build with:
mkdir build && cd build  
cmake ..  
make  
./FootballGMSim

## Usage

Once built, run FootballGMSim. You’ll be able to:

Create or load a franchise (team)
Draft or sign players (coming soon)
Run one or more seasons of play (coming soon)
View results, player stats, standings (comming soon)
Save your progress and pick up later
(Future UI enhancements or CLI menus may expand this basic workflow.)

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
