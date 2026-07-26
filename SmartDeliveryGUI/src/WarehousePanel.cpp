#include "Panels.h"
#include "BST.h"
#include <imgui.h>
#include <cstring>
#include <vector>


void renderWarehousePanel() {
    static char itemName[64] = "";
    static int quantity = 1;
    static std::string lastMessage;

    static int searchId = 0;
    static std::string searchResult;

    if (ImGui::CollapsingHeader("Add / Restock Item", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Item name", itemName, sizeof(itemName));
        ImGui::InputInt("Quantity", &quantity);
        if (ImGui::Button("Add to Inventory", ImVec2(160, 32))) {
            if (strlen(itemName) == 0) {
                lastMessage = "Item name is required.";
            } else {
                InventoryItem item;
                item.itemId = nextInventoryItemId();
                item.itemName = itemName;
                item.quantity = quantity;
                inventoryRoot = bstInsert(inventoryRoot, item);
                lastMessage = "Added item #" + std::to_string(item.itemId);
                itemName[0] = '\0';
            }
        }
        if (!lastMessage.empty()) {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.5f, 0.9f, 0.5f, 1.0f), "%s", lastMessage.c_str());
        }
    }

    if (ImGui::CollapsingHeader("Search by Item ID")) {
        ImGui::InputInt("Item ID##search", &searchId);
        if (ImGui::Button("Search")) {
            BSTNode* found = bstSearch(inventoryRoot, searchId);
            searchResult = (found == nullptr) ? "Not found." : (found->data.itemName + " - qty " + std::to_string(found->data.quantity));
        }
        if (!searchResult.empty()) {
            ImGui::SameLine();
            ImGui::Text("%s", searchResult.c_str());
        }
    }

    ImGui::Spacing();
    ImGui::Text("Inventory (sorted by Item ID — BST in-order walk)");
    ImGui::Spacing();

    std::vector<InventoryItem> items;
    bstInOrderCollect(inventoryRoot, items);

    if (ImGui::BeginTable("inventoryTable", 4,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
            ImVec2(0, 350))) {
        ImGui::TableSetupColumn("Item ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Quantity");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        for (const auto& item : items) {
            ImGui::PushID(item.itemId);
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%d", item.itemId);
            ImGui::TableNextColumn(); ImGui::Text("%s", item.itemName.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%d", item.quantity);
            ImGui::TableNextColumn();
            if (ImGui::SmallButton("Remove")) {
                inventoryRoot = bstDelete(inventoryRoot, item.itemId);
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    if (items.empty()) ImGui::TextDisabled("No items in inventory yet.");
}
