#include "Panels.h"
#include "HashTable.h"
#include "Package.h"
#include <imgui.h>
#include <cstring>
#include <algorithm>


void renderTrackingPanel() {
    static char code[32] = "";
    static int foundPackageId = -1;
    static bool searched = false;

    if (ImGui::CollapsingHeader("Track a Package", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Tracking code (e.g. PKG1001)", code, sizeof(code));
        if (ImGui::Button("Track")) {
            foundPackageId = hashSearch(code);
            searched = true;
        }

        if (searched) {
            if (foundPackageId == -1) {
                ImGui::TextColored(ImVec4(0.9f, 0.4f, 0.4f, 1.0f), "No package found with that tracking code.");
            } else {
                int idx = findPackageIndexById(foundPackageId);
                if (idx == -1) {
                    ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.3f, 1.0f), "Found in hash table, but the package record is gone.");
                } else {
                    ImGui::Spacing();
                    ImGui::Text("Tracking:  %s", packages[idx].trackingCode.c_str());
                    ImGui::Text("Sender:    %s", packages[idx].senderName.c_str());
                    ImGui::Text("Receiver:  %s", packages[idx].receiverName.c_str());
                    ImGui::Text("Weight:    %.1f kg", packages[idx].weightKg);
                    ImGui::Text("Priority:  %d", packages[idx].priority);
                    ImGui::TextColored(ImVec4(0.5f, 0.9f, 0.5f, 1.0f), "Status:    %s", packages[idx].status.c_str());
                }
            }
        }
    }

    ImGui::Spacing();
    ImGui::Text("Hash Table Bucket Occupancy (%d buckets)", MAX_HASH_SIZE);
    ImGui::TextDisabled("Each square is one bucket; brighter/taller means a longer collision chain.");
    ImGui::Spacing();

    ImVec2 canvasSize(ImGui::GetContentRegionAvail().x, 120);
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImGui::InvisibleButton("bucketCanvas", canvasSize);
    ImDrawList* draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(28, 28, 34, 255));

    float cellWidth = canvasSize.x / (float)MAX_HASH_SIZE;
    int maxChain = 1;
    for (int i = 0; i < MAX_HASH_SIZE; ++i) maxChain = std::max(maxChain, hashBucketLength(i));

    for (int i = 0; i < MAX_HASH_SIZE; ++i) {
        int len = hashBucketLength(i);
        float x0 = canvasPos.x + i * cellWidth;
        float heightFrac = len == 0 ? 0.05f : (float)len / (float)maxChain;
        float barHeight = canvasSize.y * heightFrac;
        ImU32 color = (len == 0) ? IM_COL32(50, 50, 58, 255)
                    : (len == 1) ? IM_COL32(90, 150, 220, 255)
                    : IM_COL32(220, 130, 70, 255); // orange = a real collision chain
        draw->AddRectFilled(ImVec2(x0, canvasPos.y + canvasSize.y - barHeight),
                              ImVec2(x0 + std::max(cellWidth - 1.0f, 1.0f), canvasPos.y + canvasSize.y),
                              color);
    }
}
