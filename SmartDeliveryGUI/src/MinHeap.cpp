#include "MinHeap.h"
#include "Package.h"
#include <iostream>



static int heapArray[MAX_PACKAGES]; // stores package IDs
static int heapCount = 0;

static double priorityOf(int packageId) {
    int idx = findPackageIndexById(packageId);
    return idx == -1 ? 999.0 : packages[idx].priority; // missing packages sink to the back
}

static void swapHeap(int i, int j) {
    int temp = heapArray[i];
    heapArray[i] = heapArray[j];
    heapArray[j] = temp;
}

static void siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (priorityOf(heapArray[parent]) <= priorityOf(heapArray[i])) break;
        swapHeap(i, parent);
        i = parent;
    }
}

static void siftDown(int i) {
    while (true) {
        int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
        if (left < heapCount && priorityOf(heapArray[left]) < priorityOf(heapArray[smallest])) smallest = left;
        if (right < heapCount && priorityOf(heapArray[right]) < priorityOf(heapArray[smallest])) smallest = right;
        if (smallest == i) break;
        swapHeap(i, smallest);
        i = smallest;
    }
}

// Core operations


bool heapPush(int packageId) {
    if (heapCount >= MAX_PACKAGES) return false;
    if (findPackageIndexById(packageId) == -1) return false; // package must exist

    heapArray[heapCount] = packageId;
    siftUp(heapCount);
    ++heapCount;
    return true;
}

bool heapIsEmpty() { return heapCount == 0; }
int heapSize() { return heapCount; }

int heapPeek() {
    return heapCount == 0 ? -1 : heapArray[0];
}

int heapPop() {
    if (heapCount == 0) return -1;
    int top = heapArray[0];
    heapArray[0] = heapArray[heapCount - 1];
    --heapCount;
    if (heapCount > 0) siftDown(0);
    return top;
}

int heapSnapshot(int outIds[], int maxCount) {
    int n = (heapCount < maxCount) ? heapCount : maxCount;
    for (int i = 0; i < n; ++i) outIds[i] = heapArray[i];
    return n;
}

