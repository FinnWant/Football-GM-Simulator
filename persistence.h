#pragma once
#include "league.h"
#include "team.h"
#include <string>

// Save a team to a simple line-based file. Returns true on success.
bool save_team(const team &t, const std::string &path);

// Load a team from a file produced by save_team. On success returns true and
// sets out_team. Files from before records were saved load with a 0-0 record.
bool load_team(const std::string &path, team &out_team);

// Save every team in the league, with records, plus the season state.
bool save_league(const league &lg, const std::string &path);

// Load a league written by save_league. On success returns true and sets
// out_league; on failure out_league is unchanged.
bool load_league(const std::string &path, league &out_league);
