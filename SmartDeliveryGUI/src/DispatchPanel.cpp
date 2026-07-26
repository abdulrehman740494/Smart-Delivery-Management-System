#include "Panels.h"
#include "MinHeap.h"
#include "Package.h"
#include <imgui.h>


void renderDispatchPanel() {
    static int packageId = 0;
    static std::string lastMessage;

    if (ImGui::CollapsingHeader("Add Package to Dispatch Queue", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputInt("Package ID", &packageId);
        if (ImGui::Button("Add to Queue")) {
            if (findPackageIndexById(packageId) == -1) lastMessage = "No package with that ID.";
            else lastMessage = heapPush(packageId) ? "Added to dispatch queue." : "Queue is full.";
        }
        if (!lastMessage.empty()) {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.5f, 0.9f, 0.5f, 1.0f), "%s", lastMessage.c_str());
        }
    }

    ImGui::Spacing();
    if (ImGui::Button("Dispatch Next (highest priority)", ImVec2(260, 36))) {
        if (heapIsEmpty()) {
            lastMessage = "Dispatch queue is empty.";
        } else {
            int id = heapPop();
            updatePackageStatus(id, "In Transit");
            int idx = findPackageIndexById(id);
            lastMessage = "Dispatched " + (idx != -1 ? packages[idx].trackingCode : std::to_string(id)) + " (now In Transit)";
        }
    }

    ImGui::Spacing();
    ImGui::Text("Dispatch Queue (heap order — position 0 is next out)");
    ImGui::Spacing();

    static int snapshot[MAX_PACKAGES];
    int n = heapSnapshot(snapshot, MAX_PACKAGES);

    if (ImGui::BeginTable("heapTable", 4,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
            ImVec2(0, 300))) {
        ImGui::TableSetupColumn("Heap Position");
        ImGui::TableSetupColumn("Package ID");
        ImGui::TableSetupColumn("Tracking");
        ImGui::TableSetupColumn("Priority");
        ImGui::TableHeadersRow();

        for (int i = 0; i < n; ++i) {
            int idx = findPackageIndexById(snapshot[i]);
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%d%s", i, i == 0 ? " (next)" : "");
            ImGui::TableNextColumn(); ImGui::Text("%d", snapshot[i]);
            ImGui::TableNextColumn(); ImGui::Text("%s", idx != -1 ? packages[idx].trackingCode.c_str() : "?");
            ImGui::TableNextColumn(); ImGui::Text("%s", idx != -1 ? std::to_string(packages[idx].priority).c_str() : "?");
        }
        ImGui::EndTable();
    }
    if (n == 0) ImGui::TextDisabled("Dispatch queue is empty.");
    else ImGui::TextDisabled("Note: only position 0 is guaranteed to be the true minimum — the rest of the array follows the heap property, not a full sort.");
}
