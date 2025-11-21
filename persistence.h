#pragma once
#include "team.h"
#include <string>

// Save a team to a simple line-based file. Returns true on success.
bool save_team(const team &t, const std::string &path);

// Load a team from a file produced by save_team. On success returns true and
// sets out_team.
bool load_team(const std::string &path, team &out_team);
