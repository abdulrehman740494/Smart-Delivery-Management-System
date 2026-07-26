#include "UndoStack.h"
#include "HashTable.h"
#include <iostream>



enum class UndoType { None, PackageDelete, DriverDelete };

struct UndoAction {
    UndoType type;
    Package pkgSnapshot;
    Driver driverSnapshot;
};

static UndoAction undoStack[MAX_STACK_SIZE];
static int undoTop = -1; // -1 means the stack is empty

bool canUndo() { return undoTop >= 0; }

static void pushGeneric(const UndoAction& action) {
    if (undoTop + 1 >= MAX_STACK_SIZE) return; // stack full — silently non-undoable, see header note
    ++undoTop;
    undoStack[undoTop] = action;
}

void pushPackageDeleteUndo(const Package& deletedPackage) {
    UndoAction action;
    action.type = UndoType::PackageDelete;
    action.pkgSnapshot = deletedPackage;
    pushGeneric(action);
}

void pushDriverDeleteUndo(const Driver& deletedDriver) {
    UndoAction action;
    action.type = UndoType::DriverDelete;
    action.driverSnapshot = deletedDriver; // copies historyHead/historyTail pointer VALUES, not the nodes
    pushGeneric(action);
}

static bool restorePackageSlot(const Package& snapshot) {
    int slot = -1;
    for (int i = 0; i < packageCount; ++i) if (!packages[i].inUse) { slot = i; break; }
    if (slot == -1) {
        if (packageCount >= MAX_PACKAGES) return false;
        slot = packageCount;
        ++packageCount;
    }
    packages[slot] = snapshot;
    packages[slot].inUse = true;
    hashInsert(packages[slot].trackingCode, packages[slot].id);
    return true;
}

static bool restoreDriverSlot(const Driver& snapshot) {
    int slot = -1;
    for (int i = 0; i < driverCount; ++i) if (!drivers[i].inUse) { slot = i; break; }
    if (slot == -1) {
        if (driverCount >= MAX_DRIVERS) return false;
        slot = driverCount;
        ++driverCount;
    }
    drivers[slot] = snapshot; // brings historyHead/historyTail/historyCount back exactly as they were
    drivers[slot].inUse = true;
    return true;
}

std::string performUndo() {
    if (undoTop < 0) return "";

    UndoAction action = undoStack[undoTop];
    --undoTop;

    if (action.type == UndoType::PackageDelete) {
        if (restorePackageSlot(action.pkgSnapshot))
            return "Restored package " + action.pkgSnapshot.trackingCode;
        return "Could not restore package (storage full).";
    }
    if (action.type == UndoType::DriverDelete) {
        if (restoreDriverSlot(action.driverSnapshot))
            return "Restored driver " + action.driverSnapshot.name;
        return "Could not restore driver (storage full).";
    }
    return "";
}

