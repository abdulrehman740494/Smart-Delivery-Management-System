#include "Driver.h"
#include "UndoStack.h"
#include <iostream>
#include <ctime>
 

int findDriverIndexById(int id) {
    for (int i = 0; i < driverCount; ++i) {
        if (drivers[i].inUse && drivers[i].id == id) return i;
    }
    return -1;
}

int addDriver(const std::string& name) {
    int slot = -1;
    for (int i = 0; i < driverCount; ++i) {
        if (!drivers[i].inUse) { slot = i; break; }
    }
    if (slot == -1) {
        if (driverCount >= MAX_DRIVERS) return -1; // array full
        slot = driverCount;
        ++driverCount;
    }

    int newId = nextDriverId++;
    drivers[slot].id = newId;
    drivers[slot].name = name;
    drivers[slot].deliveriesCompleted = 0;
    drivers[slot].rating = 5.0; // new drivers start at a perfect rating
    drivers[slot].onDuty = false;
    drivers[slot].inUse = true;
    drivers[slot].historyHead = nullptr;
    drivers[slot].historyTail = nullptr;
    drivers[slot].historyCount = 0;

    return newId;
}

void clearDriverHistory(Driver& driver) {
    HistoryNode* current = driver.historyHead;
    while (current != nullptr) {
        HistoryNode* toDelete = current;
        current = current->next;
        delete toDelete;
    }
    driver.historyHead = nullptr;
    driver.historyTail = nullptr;
    driver.historyCount = 0;
}

bool deleteDriverById(int id) {
    int idx = findDriverIndexById(id);
    if (idx == -1) return false;
    pushDriverDeleteUndo(drivers[idx]);
    drivers[idx].inUse = false;
    return true;
}

bool toggleDriverDuty(int id) {
    int idx = findDriverIndexById(id);
    if (idx == -1) return false;
    drivers[idx].onDuty = !drivers[idx].onDuty;
    return true;
}


// Delivery history (doubly linked list)

bool addHistoryEntry(int driverId, int packageId, const std::string& timestamp) {
    int idx = findDriverIndexById(driverId);
    if (idx == -1) return false;

    HistoryNode* node = new HistoryNode();
    node->packageId = packageId;
    node->timestamp = timestamp;
    node->prev = nullptr;
    node->next = nullptr;

    Driver& d = drivers[idx];
    if (d.historyTail == nullptr) {
        // First entry — head and tail both point to it.
        d.historyHead = node;
        d.historyTail = node;
    } else {
        // Append at the tail: link both directions.
        node->prev = d.historyTail;
        d.historyTail->next = node;
        d.historyTail = node;
    }
    ++d.historyCount;
    ++d.deliveriesCompleted;
    return true;
}

std::string currentTimestamp() {
    std::time_t t = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", std::localtime(&t));
    return std::string(buf);
}

