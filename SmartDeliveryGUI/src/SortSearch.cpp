#include "SortSearch.h"
#include <iostream>

// Quicksort — drivers by rating, DESCENDING (best driver first)

static void swapDrivers(Driver& a, Driver& b) { Driver temp = a; a = b; b = temp; }

void quicksortDriversByRating(Driver arr[], int low, int high) {
    if (low >= high) return;
    double pivot = arr[high].rating;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j].rating > pivot) { // ">" gives descending order
            ++i;
            swapDrivers(arr[i], arr[j]);
        }
    }
    swapDrivers(arr[i + 1], arr[high]);
    quicksortDriversByRating(arr, low, i);
    quicksortDriversByRating(arr, i + 2, high);
}

// Mergesort — drivers by rating, DESCENDING (alternative algorithm, same result)

static void mergeDrivers(Driver arr[], int low, int mid, int high) {
    int leftSize = mid - low + 1;
    int rightSize = high - mid;
    Driver* leftArr = new Driver[leftSize];
    Driver* rightArr = new Driver[rightSize];

    for (int i = 0; i < leftSize; ++i) leftArr[i] = arr[low + i];
    for (int i = 0; i < rightSize; ++i) rightArr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = low;
    while (i < leftSize && j < rightSize) {
        if (leftArr[i].rating >= rightArr[j].rating) arr[k++] = leftArr[i++];
        else arr[k++] = rightArr[j++];
    }
    while (i < leftSize) arr[k++] = leftArr[i++];
    while (j < rightSize) arr[k++] = rightArr[j++];

    delete[] leftArr;
    delete[] rightArr;
}

void mergesortDriversByRating(Driver arr[], int low, int high) {
    if (low >= high) return;
    int mid = low + (high - low) / 2;
    mergesortDriversByRating(arr, low, mid);
    mergesortDriversByRating(arr, mid + 1, high);
    mergeDrivers(arr, low, mid, high);
}

// Quicksort — packages by priority, ASCENDING (required before binary search)

static void swapPackages(Package& a, Package& b) { Package temp = a; a = b; b = temp; }

void quicksortPackagesByPriority(Package arr[], int low, int high) {
    if (low >= high) return;
    int pivot = arr[high].priority;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j].priority < pivot) {
            ++i;
            swapPackages(arr[i], arr[j]);
        }
    }
    swapPackages(arr[i + 1], arr[high]);
    quicksortPackagesByPriority(arr, low, i);
    quicksortPackagesByPriority(arr, i + 2, high);
}

// Binary search — leftmost match (handles duplicate priorities correctly)

int binarySearchLeftmostPriority(Package arr[], int n, int targetPriority) {
    int lo = 0, hi = n - 1, result = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid].priority == targetPriority) {
            result = mid;   // record this match, then keep searching LEFT for an earlier one
            hi = mid - 1;
        } else if (arr[mid].priority < targetPriority) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return result;
}

