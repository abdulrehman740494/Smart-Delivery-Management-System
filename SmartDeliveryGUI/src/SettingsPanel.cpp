#include "Panels.h"
#include "Global.h"
#include "UndoStack.h"
#include <imgui.h>

void renderSettingsPanel() {
    ImGui::TextWrapped("Smart Delivery Management System — a DSA semester project. "
                        "Every module is backed by a manually implemented data structure; "
                        "this GUI is a new front end over the exact same logic as the console version.");

    ImGui::Spacing();
    ImGui::Text("Live counts");
    ImGui::Separator();
    ImGui::Text("Packages tracked:        %d", packageCount);
    ImGui::Text("Drivers on file:         %d", driverCount);
    ImGui::Text("Locations mapped:        %d", locationCount);
    ImGui::Text("Undo actions available:  %s", canUndo() ? "Yes" : "No");

    ImGui::Spacing();
    ImGui::Text("Data structures used");
    ImGui::Separator();
    ImGui::BulletText("Graph (adjacency list) + Dijkstra — Delivery Routes");
    ImGui::BulletText("Binary Search Tree — Warehouse Inventory");
    ImGui::BulletText("Min-Heap (array-based) — Priority Dispatch");
    ImGui::BulletText("Hash Table (separate chaining) — Tracking System");
    ImGui::BulletText("Doubly Linked List — Driver delivery history");
    ImGui::BulletText("Circular Queue — Incoming pickup requests");
    ImGui::BulletText("Stack (LIFO) — Undo last action");
    ImGui::BulletText("Quicksort & Mergesort — Driver leaderboard");
    ImGui::BulletText("Binary Search (leftmost match) — Package lookup by priority");
}
