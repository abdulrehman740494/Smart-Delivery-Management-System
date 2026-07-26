#pragma once
#include "Global.h"



// djb2-style string hash, reduced into MAX_HASH_SIZE buckets.
unsigned int hashFunction(const std::string& key);

void hashInsert(const std::string& trackingCode, int packageId);
bool hashRemove(const std::string& trackingCode);

// Returns the package ID for a tracking code, or -1 if not found.
int hashSearch(const std::string& trackingCode);

// For the "Tracking System" screen's stats view: how many entries are
// chained in a given bucket.
int hashBucketLength(unsigned int bucketIndex);

// ---- TUI screen ---------------------------------------------------------
