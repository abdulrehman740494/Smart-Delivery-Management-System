#pragma once
#include <string>

const int MAX_PACKAGES   = 200;
const int MAX_DRIVERS    = 50;
const int MAX_LOCATIONS  = 30;
const int MAX_HASH_SIZE  = 211;   // prime bucket count for the tracking-ID hash table
const int MAX_QUEUE_SIZE = 50;    // circular queue capacity for incoming requests
const int MAX_STACK_SIZE = 100;   // undo stack capacity

// Core data shapes=

// A single package moving through the system.
struct Package {
    int id;                    // unique package ID (also the BST / hash key)
    std::string trackingCode;  // human-facing tracking code, e.g. "PKG10023"
    std::string senderName;
    std::string receiverName;
    int originLocationId;
    int destLocationId;
    double weightKg;
    int priority;               // 1 = highest priority, 5 = lowest (used by the min-heap)
    std::string status;         // "Pending" | "In Transit" | "Delivered" | "Cancelled"
    bool inUse;                 // false = this slot in the packages[] array is free
};

// One entry in a driver's delivery history (doubly linked list node).
struct HistoryNode {
    int packageId;
    std::string timestamp;
    HistoryNode* prev;
    HistoryNode* next;
};

struct Driver {
    int id;
    std::string name;
    int deliveriesCompleted;
    double rating;              // 0.0 - 5.0, used for ranking (sorting demo)
    bool onDuty;
    bool inUse;
    HistoryNode* historyHead;   // oldest delivery
    HistoryNode* historyTail;   // most recent delivery
    int historyCount;
};

struct InventoryItem {
    int itemId;
    std::string itemName;
    int quantity;
};

// A warehouse / delivery location — also a vertex in the road-network graph.
struct Location {
    int id;
    std::string name;
    bool inUse;
};

struct PickupRequest {
    int requestId;
    std::string customerName;
    std::string pickupDetails;
};

// Global state (defined in Global.cpp)
extern Package packages[MAX_PACKAGES];
extern int packageCount;

extern Driver drivers[MAX_DRIVERS];
extern int driverCount;

extern Location locations[MAX_LOCATIONS];
extern int locationCount;

// Running counters used to generate new IDs / tracking codes.
extern int nextPackageId;
extern int nextDriverId;
extern int nextLocationId;
