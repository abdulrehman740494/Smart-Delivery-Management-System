#include "Panels.h"
#include "SortSearch.h"
#include <imgui.h>



void renderReportsPanel() {
    static int algoChoice = 0; // 0 = quicksort, 1 = mergesort
    static Driver leaderboard[MAX_DRIVERS];
    static int leaderboardCount = 0;

    if (ImGui::CollapsingHeader("Driver Leaderboard", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::RadioButton("Quicksort", &algoChoice, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Mergesort", &algoChoice, 1);

        if (ImGui::Button("Rank Drivers")) {
            leaderboardCount = 0;
            for (int i = 0; i < driverCount; ++i) if (drivers[i].inUse) leaderboard[leaderboardCount++] = drivers[i];
            if (leaderboardCount > 0) {
                if (algoChoice == 0) quicksortDriversByRating(leaderboard, 0, leaderboardCount - 1);
                else mergesortDriversByRating(leaderboard, 0, leaderboardCount - 1);
            }
        }

        if (leaderboardCount > 0 && ImGui::BeginTable("leaderboardTable", 4,
                ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg, ImVec2(0, 220))) {
            ImGui::TableSetupColumn("Rank");
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Deliveries");
            ImGui::TableSetupColumn("Rating");
            ImGui::TableHeadersRow();
            for (int i = 0; i < leaderboardCount; ++i) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("%d", i + 1);
                ImGui::TableNextColumn(); ImGui::Text("%s", leaderboard[i].name.c_str());
                ImGui::TableNextColumn(); ImGui::Text("%d", leaderboard[i].deliveriesCompleted);
                ImGui::TableNextColumn(); ImGui::Text("%.1f", leaderboard[i].rating);
            }
            ImGui::EndTable();
        } else if (leaderboardCount == 0) {
            ImGui::TextDisabled("Click \"Rank Drivers\" to build the leaderboard.");
        }
    }

    ImGui::Spacing();
    static int targetPriority = 3;
    static Package sorted[MAX_PACKAGES];
    static int sortedCount = -1; // -1 = not searched yet

    if (ImGui::CollapsingHeader("Search Packages by Priority (Binary Search)", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderInt("Priority to search for", &targetPriority, 1, 5);
        if (ImGui::Button("Search")) {
            sortedCount = 0;
            for (int i = 0; i < packageCount; ++i) if (packages[i].inUse) sorted[sortedCount++] = packages[i];
            if (sortedCount > 0) quicksortPackagesByPriority(sorted, 0, sortedCount - 1);
        }

        if (sortedCount >= 0) {
            int firstIdx = (sortedCount > 0) ? binarySearchLeftmostPriority(sorted, sortedCount, targetPriority) : -1;
            if (firstIdx == -1) {
                ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.3f, 1.0f), "No packages found with that priority.");
            } else if (ImGui::BeginTable("searchTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg, ImVec2(0, 180))) {
                ImGui::TableSetupColumn("Tracking");
                ImGui::TableSetupColumn("Receiver");
                ImGui::TableSetupColumn("Status");
                ImGui::TableHeadersRow();
                for (int i = firstIdx; i < sortedCount && sorted[i].priority == targetPriority; ++i) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn(); ImGui::Text("%s", sorted[i].trackingCode.c_str());
                    ImGui::TableNextColumn(); ImGui::Text("%s", sorted[i].receiverName.c_str());
                    ImGui::TableNextColumn(); ImGui::Text("%s", sorted[i].status.c_str());
                }
                ImGui::EndTable();
            }
        }
    }
}
