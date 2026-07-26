#pragma once
#include "Global.h"



// ---- Core heap operations ------------------------------------------------
bool heapPush(int packageId);      // false if the heap is full or the package doesn't exist
bool heapIsEmpty();
int heapPeek();                     // returns the next package ID to dispatch, or -1 if empty
int heapPop();                      // removes and returns the next package ID to dispatch, or -1 if empty
int heapSize();

int heapSnapshot(int outIds[], int maxCount);

// ---- TUI screen ---------------------------------------------------------
