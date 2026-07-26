#pragma once
#include "Global.h"
#include <string>

bool canUndo();

void pushPackageDeleteUndo(const Package& deletedPackage);
void pushDriverDeleteUndo(const Driver& deletedDriver);

// Pops the most recent undoable action and reverses it. Returns a
// human-readable description of what was restored, or an empty string if
// there was nothing to undo.
std::string performUndo();

// ---- TUI screen ---------------------------------------------------------
