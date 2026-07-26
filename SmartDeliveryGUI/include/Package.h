#pragma once
#include "Global.h"



// Builds a tracking code like "PKG1001" from a package ID.
std::string generateTrackingCode(int id);

// Linear search for the array index holding package `id`. Returns -1 if
// not found or not currently in use.
int findPackageIndexById(int id);

// Creates a new package, filling the first free (inUse == false) slot if
// one exists, otherwise appending. Returns the new package's ID, or -1 if
// the packages[] array is completely full.
int addPackage(const std::string& senderName, const std::string& receiverName,
                int originLocationId, int destLocationId,
                double weightKg, int priority);

// Marks the package as no longer in use (logical delete — the slot can be
// reused by a future addPackage call). Returns false if the ID was not found.
bool deletePackageById(int id);

// Changes the status field of an existing package. Returns false if the
// ID was not found.
bool updatePackageStatus(int id, const std::string& newStatus);

// ---- TUI screen (called from main.cpp) -----------------------------------
