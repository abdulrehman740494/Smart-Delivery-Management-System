#pragma once
#include "Global.h"


bool queueIsFull();
bool queueIsEmpty();
int queueSize();

bool enqueueRequest(const std::string& customerName, const std::string& pickupDetails);

// Removes and returns the oldest request via `out`. Returns false if the
// queue was empty.
bool dequeueRequest(PickupRequest& out);

bool queuePeekAt(int offsetFromFront, PickupRequest& out);

// ---- TUI screen ---------------------------------------------------------
