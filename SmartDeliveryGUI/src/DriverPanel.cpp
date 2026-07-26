#include "Panels.h"
#include "Driver.h"
#include "Package.h"
#include <imgui.h>
#include <cstring>
#include <cmath>

static void drawStar(ImDrawList* draw, ImVec2 center, float outerRadius, ImU32 color) {
    float innerRadius = outerRadius * 0.45f;
    ImVec2 pts[10];
    for (int k = 0; k < 10; ++k) {
        float angle = -1.57079633f + k * 0.62831853f; // start pointing up, step 36 degrees
        float r = (k % 2 == 0) ? outerRadius : innerRadius;
        pts[k] = ImVec2(center.x + r * std::cos(angle), center.y + r * std::sin(angle));
    }
    draw->AddConvexPolyFilled(pts, 10, color);
}

static void drawRatingStars(double rating) {
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw = ImGui::GetWindowDrawList();
    int filled = (int)(rating + 0.5); // round to nearest whole star
    const float starSize = 7.0f;
    const float spacing = 16.0f;
    for (int s = 0; s < 5; ++s) {
        ImVec2 c(p.x + 9 + s * spacing, p.y + 9);
        ImU32 col = (s < filled) ? IM_COL32(240, 153, 123, 255) : IM_COL32(70, 70, 76, 255);
        drawStar(draw, c, starSize, col);
    }
    ImGui::Dummy(ImVec2(5 * spacing, 18));
}

static void drawDutyBadge(bool onDuty) {
    const char* label = onDuty ? "On duty" : "Off duty";
    ImVec4 bg = onDuty ? ImVec4(0.06f, 0.30f, 0.24f, 1.0f) : ImVec4(0.20f, 0.20f, 0.19f, 1.0f);
    ImVec4 fg = onDuty ? ImVec4(0.62f, 0.88f, 0.79f, 1.0f) : ImVec4(0.70f, 0.70f, 0.66f, 1.0f);

    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 padding(8, 3);
    ImVec2 pillSize(textSize.x + padding.x * 2, textSize.y + padding.y * 2);
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(p, ImVec2(p.x + pillSize.x, p.y + pillSize.y),
                          ImGui::ColorConvertFloat4ToU32(bg), pillSize.y * 0.5f);
    draw->AddText(ImVec2(p.x + padding.x, p.y + padding.y - 1), ImGui::ColorConvertFloat4ToU32(fg), label);
    ImGui::Dummy(pillSize);
}


void renderDriverPanel() {
    static char name[64] = "";
    static std::string lastMessage;

    static int selectedDriverId = -1;
    static HistoryNode* historyCursor = nullptr;
    static int historyPosition = 0;

    static int logDriverId = 0;
    static int logPackageId = 0;

    if (ImGui::CollapsingHeader("Add New Driver", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Driver name", name, sizeof(name));
        if (ImGui::Button("Add Driver", ImVec2(140, 32))) {
            if (strlen(name) == 0) {
                lastMessage = "Driver name is required.";
            } else {
                int newId = addDriver(name);
                lastMessage = (newId == -1) ? "Driver storage is full!" : ("Added driver #" + std::to_string(newId));
                name[0] = '\0';
            }
        }
        if (!lastMessage.empty()) {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.5f, 0.9f, 0.5f, 1.0f), "%s", lastMessage.c_str());
        }
    }

    if (ImGui::CollapsingHeader("Log a Completed Delivery")) {
        ImGui::InputInt("Driver ID##log", &logDriverId);
        ImGui::InputInt("Package ID delivered", &logPackageId);
        if (ImGui::Button("Log Delivery")) {
            if (findDriverIndexById(logDriverId) == -1) lastMessage = "No driver with that ID.";
            else { addHistoryEntry(logDriverId, logPackageId, currentTimestamp()); lastMessage = "Delivery logged."; }
        }
    }

    ImGui::Spacing();
    ImGui::Text("All Drivers");
    ImGui::Spacing();

    if (ImGui::BeginTable("driversTable", 6,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
            ImVec2(0, 300))) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Deliveries");
        ImGui::TableSetupColumn("Rating");
        ImGui::TableSetupColumn("On Duty");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        for (int i = 0; i < driverCount; ++i) {
            if (!drivers[i].inUse) continue;
            ImGui::PushID(drivers[i].id);
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%d", drivers[i].id);
            ImGui::TableNextColumn(); ImGui::Text("%s", drivers[i].name.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%d", drivers[i].deliveriesCompleted);
            ImGui::TableNextColumn(); drawRatingStars(drivers[i].rating);
            ImGui::TableNextColumn(); drawDutyBadge(drivers[i].onDuty);

            ImGui::TableNextColumn();
            if (ImGui::SmallButton("Toggle Duty")) toggleDriverDuty(drivers[i].id);
            ImGui::SameLine();
            if (ImGui::SmallButton("History")) {
                selectedDriverId = drivers[i].id;
                historyCursor = drivers[i].historyHead;
                historyPosition = 1;
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("Delete")) {
                deleteDriverById(drivers[i].id);
                if (selectedDriverId == drivers[i].id) { selectedDriverId = -1; historyCursor = nullptr; }
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    // ---- History viewer (doubly linked list walk) --------------------
    if (selectedDriverId != -1) {
        int idx = findDriverIndexById(selectedDriverId);
        ImGui::Spacing();
        ImGui::Separator();
        if (idx == -1) {
            ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.3f, 1.0f), "Selected driver was deleted.");
            selectedDriverId = -1;
        } else {
            ImGui::Text("Delivery history: %s", drivers[idx].name.c_str());
            if (drivers[idx].historyHead == nullptr) {
                ImGui::TextDisabled("No delivery history yet.");
            } else if (historyCursor != nullptr) {
                ImGui::Text("Entry %d of %d", historyPosition, drivers[idx].historyCount);
                ImGui::Text("Package ID: %d", historyCursor->packageId);
                ImGui::Text("Delivered:  %s", historyCursor->timestamp.c_str());

                ImGui::BeginDisabled(historyCursor->prev == nullptr);
                if (ImGui::Button("< Older")) { historyCursor = historyCursor->prev; --historyPosition; }
                ImGui::EndDisabled();
                ImGui::SameLine();
                ImGui::BeginDisabled(historyCursor->next == nullptr);
                if (ImGui::Button("Newer >")) { historyCursor = historyCursor->next; ++historyPosition; }
                ImGui::EndDisabled();
            }
        }
    }
}
