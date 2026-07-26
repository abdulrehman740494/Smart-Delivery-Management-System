#include "Package.h"
#include "HashTable.h"
#include "UndoStack.h"
#include <iostream>

// Core operations


std::string generateTrackingCode(int id) {
    return "PKG" + std::to_string(id);
}

int findPackageIndexById(int id) {
    for (int i = 0; i < packageCount; ++i) {
        if (packages[i].inUse && packages[i].id == id) return i;
    }
    return -1;
}

int addPackage(const std::string& senderName, const std::string& receiverName,
                int originLocationId, int destLocationId,
                double weightKg, int priority) {
    // First, try to reuse a slot freed by an earlier deletion.
    int slot = -1;
    for (int i = 0; i < packageCount; ++i) {
        if (!packages[i].inUse) { slot = i; break; }
    }
    // No free slot found — append a brand new one if there is room.
    if (slot == -1) {
        if (packageCount >= MAX_PACKAGES) return -1; // array full
        slot = packageCount;
        ++packageCount;
    }

    int newId = nextPackageId++;
    packages[slot].id = newId;
    packages[slot].trackingCode = generateTrackingCode(newId);
    packages[slot].senderName = senderName;
    packages[slot].receiverName = receiverName;
    packages[slot].originLocationId = originLocationId;
    packages[slot].destLocationId = destLocationId;
    packages[slot].weightKg = weightKg;
    packages[slot].priority = priority;
    packages[slot].status = "Pending";
    packages[slot].inUse = true;

    hashInsert(packages[slot].trackingCode, newId); // keep the tracking-code index in sync

    return newId;
}

bool deletePackageById(int id) {
    int idx = findPackageIndexById(id);
    if (idx == -1) return false;
    pushPackageDeleteUndo(packages[idx]); // save a full copy before we touch anything
    hashRemove(packages[idx].trackingCode);
    packages[idx].inUse = false;
    return true;
}

bool updatePackageStatus(int id, const std::string& newStatus) {
    int idx = findPackageIndexById(id);
    if (idx == -1) return false;
    packages[idx].status = newStatus;
    return true;
}

