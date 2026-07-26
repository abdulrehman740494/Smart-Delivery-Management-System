#include "Panels.h"
#include "RequestQueue.h"
#include "Package.h"
#include <imgui.h>
#include <cstring>



void renderRequestPanel() {
    static char customerName[64] = "";
    static char details[128] = "";
    static std::string lastMessage;

    static bool hasPendingConversion = false;
    static PickupRequest pendingConversion;
    static char convReceiver[64] = "";
    static int convOrigin = 1, convDest = 1;
    static float convWeight = 1.0f;
    static int convPriority = 3;

    if (ImGui::CollapsingHeader("Submit Pickup Request", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Customer name", customerName, sizeof(customerName));
        ImGui::InputText("Pickup details", details, sizeof(details));
        if (ImGui::Button("Submit Request")) {
            if (strlen(customerName) == 0) lastMessage = "Customer name is required.";
            else {
                lastMessage = enqueueRequest(customerName, details) ? "Request submitted." : "Queue is full.";
                customerName[0] = '\0';
                details[0] = '\0';
            }
        }
        if (!lastMessage.empty()) {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.5f, 0.9f, 0.5f, 1.0f), "%s", lastMessage.c_str());
        }
    }

    ImGui::Spacing();
    if (ImGui::Button("Process Next Request (dequeue)", ImVec2(240, 32))) {
        PickupRequest req;
        if (dequeueRequest(req)) {
            pendingConversion = req;
            hasPendingConversion = true;
            convReceiver[0] = '\0';
        } else {
            lastMessage = "No pending requests.";
        }
    }

    ImGui::Spacing();
    ImGui::Text("Queue (FIFO order — request 0 is next to process)");
    ImGui::Spacing();

    if (ImGui::BeginTable("queueTable", 3,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
            ImVec2(0, 220))) {
        ImGui::TableSetupColumn("Request ID");
        ImGui::TableSetupColumn("Customer");
        ImGui::TableSetupColumn("Details");
        ImGui::TableHeadersRow();

        PickupRequest r;
        for (int i = 0; i < queueSize(); ++i) {
            if (!queuePeekAt(i, r)) break;
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%d", r.requestId);
            ImGui::TableNextColumn(); ImGui::Text("%s", r.customerName.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%s", r.pickupDetails.c_str());
        }
        ImGui::EndTable();
    }
    if (queueSize() == 0) ImGui::TextDisabled("No pending pickup requests.");

    // ---- Inline conversion form for the request just dequeued ------------
    if (hasPendingConversion) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Create package for request #%d (%s)", pendingConversion.requestId, pendingConversion.customerName.c_str());
        ImGui::InputText("Receiver name", convReceiver, sizeof(convReceiver));
        ImGui::InputInt("Origin location ID##conv", &convOrigin);
        ImGui::InputInt("Destination location ID##conv", &convDest);
        ImGui::SliderFloat("Weight (kg)##conv", &convWeight, 0.1f, 100.0f, "%.1f");
        ImGui::SliderInt("Priority##conv", &convPriority, 1, 5);

        if (ImGui::Button("Create Package")) {
            int newId = addPackage(pendingConversion.customerName, convReceiver, convOrigin, convDest, (double)convWeight, convPriority);
            lastMessage = (newId == -1) ? "Package storage full!" : ("Created " + generateTrackingCode(newId));
            hasPendingConversion = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Dismiss (no package)")) {
            hasPendingConversion = false;
        }
    }
}
