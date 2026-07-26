#pragma once
#include "Global.h"



// ---- Driver records --------------------------------------------------
int findDriverIndexById(int id);
int addDriver(const std::string& name);
bool deleteDriverById(int id);      // also frees the driver's history list
bool toggleDriverDuty(int id);

// ---- Delivery history (doubly linked list) -----------------------------
// Appends a new history entry at the tail (most recent delivery).
bool addHistoryEntry(int driverId, int packageId, const std::string& timestamp);
std::string currentTimestamp(); // "YYYY-MM-DD HH:MM" for the current moment, used when logging a delivery

// Frees every HistoryNode belonging to a driver and resets head/tail/count.
// Called automatically by deleteDriverById, but exposed here in case a
// caller needs to clear history without deleting the driver.
void clearDriverHistory(Driver& driver);

// ---- TUI screen ---------------------------------------------------------
