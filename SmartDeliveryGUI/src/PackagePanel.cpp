#include "Panels.h"
#include "Package.h"
#include <imgui.h>
#include <cstring>

static ImVec4 priorityColor(int priority) {
    switch (priority) {
        case 1: return ImVec4(0.90f, 0.30f, 0.30f, 1.0f);
        case 2: return ImVec4(0.95f, 0.55f, 0.25f, 1.0f);
        case 3: return ImVec4(0.90f, 0.80f, 0.25f, 1.0f);
        case 4: return ImVec4(0.55f, 0.80f, 0.35f, 1.0f);
        default: return ImVec4(0.35f, 0.75f, 0.45f, 1.0f);
    }
}

static ImVec4 statusColor(const std::string& status) {
    if (status == "Pending") return ImVec4(0.60f, 0.60f, 0.65f, 1.0f);
    if (status == "In Transit") return ImVec4(0.40f, 0.60f, 0.90f, 1.0f);
    if (status == "Delivered") return ImVec4(0.35f, 0.75f, 0.45f, 1.0f);
    return ImVec4(0.85f, 0.35f, 0.35f, 1.0f); // Cancelled
}

void renderPackagePanel() {
    static char sender[64] = "";
    static char receiver[64] = "";
    static int originId = 1;
    static int destId = 1;
    static float weight = 1.0f;
    static int priority = 3;
    static std::string lastMessage;

    if (ImGui::CollapsingHeader("Add New Package", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Sender name", sender, sizeof(sender));
        ImGui::InputText("Receiver name", receiver, sizeof(receiver));
        ImGui::InputInt("Origin location ID", &originId);
        ImGui::InputInt("Destination location ID", &destId);
        ImGui::SliderFloat("Weight (kg)", &weight, 0.1f, 100.0f, "%.1f");
        ImGui::SliderInt("Priority (1 = highest)", &priority, 1, 5);

        if (ImGui::Button("Add Package", ImVec2(140, 32))) {
            if (strlen(sender) == 0 || strlen(receiver) == 0) {
                lastMessage = "Sender and receiver names are required.";
            } else {
                int newId = addPackage(sender, receiver, originId, destId, (double)weight, priority);
                if (newId == -1) {
                    lastMessage = "Package storage is full!";
                } else {
                    lastMessage = "Added " + generateTrackingCode(newId);
                    sender[0] = '\0';
                    receiver[0] = '\0';
                }
            }
        }
        if (!lastMessage.empty()) {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.5f, 0.9f, 0.5f, 1.0f), "%s", lastMessage.c_str());
        }
    }

    ImGui::Spacing();
    ImGui::Text("All Packages");
    ImGui::Spacing();

    static const char* statusOptions[] = { "Pending", "In Transit", "Delivered", "Cancelled" };

    if (ImGui::BeginTable("packagesTable", 7,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
            ImVec2(0, 400))) {
        ImGui::TableSetupColumn("Tracking");
        ImGui::TableSetupColumn("Sender");
        ImGui::TableSetupColumn("Receiver");
        ImGui::TableSetupColumn("Weight");
        ImGui::TableSetupColumn("Priority");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        for (int i = 0; i < packageCount; ++i) {
            if (!packages[i].inUse) continue;
            ImGui::PushID(packages[i].id);
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%s", packages[i].trackingCode.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%s", packages[i].senderName.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%s", packages[i].receiverName.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%.1f kg", packages[i].weightKg);

            ImGui::TableNextColumn();
            {
                ImVec4 pc = priorityColor(packages[i].priority);
                ImVec2 dotPos = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddCircleFilled(
                    ImVec2(dotPos.x + 7, dotPos.y + 9), 6.0f, ImGui::ColorConvertFloat4ToU32(pc));
                ImGui::Dummy(ImVec2(16, 0));
                ImGui::SameLine();
                ImGui::Text("P%d", packages[i].priority);
            }

            ImGui::TableNextColumn();
            int statusIdx = 0;
            for (int s = 0; s < 4; ++s) if (packages[i].status == statusOptions[s]) statusIdx = s;
            {
                ImVec4 sc = statusColor(packages[i].status);
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(sc.x * 0.30f, sc.y * 0.30f, sc.z * 0.30f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, sc);
                ImGui::SetNextItemWidth(120);
                if (ImGui::Combo("##status", &statusIdx, statusOptions, 4)) {
                    updatePackageStatus(packages[i].id, statusOptions[statusIdx]);
                }
                ImGui::PopStyleColor(2);
            }

            ImGui::TableNextColumn();
            if (ImGui::SmallButton("Delete")) {
                deletePackageById(packages[i].id);
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}
