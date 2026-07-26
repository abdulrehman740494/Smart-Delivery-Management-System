#include "HashTable.h"
#include "Package.h"
#include <iostream>



struct HashNode {
    std::string key;   // tracking code
    int packageId;
    HashNode* next;
};

static HashNode* buckets[MAX_HASH_SIZE]; // zero-initialized (all nullptr) — static storage duration

unsigned int hashFunction(const std::string& key) {
    // djb2 — a simple, well-known string hash that spreads keys well.
    unsigned int hash = 5381;
    for (char c : key) hash = ((hash << 5) + hash) + (unsigned char)c; // hash * 33 + c
    return hash % MAX_HASH_SIZE;
}

void hashInsert(const std::string& trackingCode, int packageId) {
    unsigned int idx = hashFunction(trackingCode);

    // If this key already exists (shouldn't normally happen since
    // tracking codes are unique), update it instead of adding a duplicate.
    for (HashNode* node = buckets[idx]; node != nullptr; node = node->next) {
        if (node->key == trackingCode) { node->packageId = packageId; return; }
    }

    HashNode* node = new HashNode();
    node->key = trackingCode;
    node->packageId = packageId;
    node->next = buckets[idx];   // insert at the head of the chain
    buckets[idx] = node;
}

bool hashRemove(const std::string& trackingCode) {
    unsigned int idx = hashFunction(trackingCode);
    HashNode* current = buckets[idx];
    HashNode* prev = nullptr;

    while (current != nullptr) {
        if (current->key == trackingCode) {
            if (prev == nullptr) buckets[idx] = current->next;
            else prev->next = current->next;
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

int hashSearch(const std::string& trackingCode) {
    unsigned int idx = hashFunction(trackingCode);
    for (HashNode* node = buckets[idx]; node != nullptr; node = node->next) {
        if (node->key == trackingCode) return node->packageId;
    }
    return -1;
}

int hashBucketLength(unsigned int bucketIndex) {
    int count = 0;
    for (HashNode* node = buckets[bucketIndex]; node != nullptr; node = node->next) ++count;
    return count;
}

