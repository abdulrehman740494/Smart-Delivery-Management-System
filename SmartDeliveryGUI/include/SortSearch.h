#pragma once
#include "Global.h"

// ---- Sorting (drivers, by rating, descending — best driver first) --------
void quicksortDriversByRating(Driver arr[], int low, int high);
void mergesortDriversByRating(Driver arr[], int low, int high);

// ---- Sorting (packages, by priority, ascending — required before binary search) --
void quicksortPackagesByPriority(Package arr[], int low, int high);

// ---- Binary search (leftmost match, supports duplicate priorities) --------
// `arr` must already be sorted ascending by priority. Returns the index
// of the FIRST (leftmost) package with the given priority, or -1.
int binarySearchLeftmostPriority(Package arr[], int n, int targetPriority);

// ---- TUI screen ---------------------------------------------------------
