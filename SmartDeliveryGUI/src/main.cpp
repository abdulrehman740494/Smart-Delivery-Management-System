#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "Panels.h"



static const char* panelLabel(ActivePanel p) {
    switch (p) {
        case ActivePanel::Packages:  return "Package Management";
        case ActivePanel::Drivers:   return "Driver Management";
        case ActivePanel::Warehouse: return "Warehouse Inventory (BST)";
        case ActivePanel::Routes:    return "Delivery Routes (Graph)";
        case ActivePanel::Dispatch:  return "Priority Dispatch (Min-Heap)";
        case ActivePanel::Requests:  return "Incoming Requests (Queue)";
        case ActivePanel::Tracking:  return "Tracking System (Hash Table)";
        case ActivePanel::Reports:   return "Reports (Sorting / Searching)";
        case ActivePanel::Undo:      return "Undo Last Action (Stack)";
        case ActivePanel::Settings:  return "Settings / About";
    }
    return "";
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Smart Delivery Management System");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window)) {
        return 1; // ImGui/SFML failed to initialize (e.g. no graphics context available)
    }
    ImGui::StyleColorsDark();

    // Slightly roomier spacing than the ImGui default so forms/tables
    // don't feel cramped on a 1280-wide window.
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(12, 12);
    style.ItemSpacing = ImVec2(8, 8);
    style.FrameRounding = 4.0f;

    ActivePanel active = ActivePanel::Packages;

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)window.getSize().x, (float)window.getSize().y));
        ImGui::Begin("SmartDeliveryRoot", nullptr,
                      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

        // ---- Sidebar --------------------------------------------------
        ImGui::BeginChild("Sidebar", ImVec2(240, 0), true);
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "SMART DELIVERY");
        ImGui::TextDisabled("Management System");
        ImGui::Separator();

        struct MenuEntry { ActivePanel panel; const char* label; };
        static const MenuEntry entries[] = {
            { ActivePanel::Packages,  "Packages" },
            { ActivePanel::Drivers,   "Drivers" },
            { ActivePanel::Warehouse, "Warehouse (BST)" },
            { ActivePanel::Routes,    "Delivery Routes (Graph)" },
            { ActivePanel::Dispatch,  "Priority Dispatch (Heap)" },
            { ActivePanel::Requests,  "Incoming Requests (Queue)" },
            { ActivePanel::Tracking,  "Tracking (Hash Table)" },
            { ActivePanel::Reports,   "Reports (Sort / Search)" },
            { ActivePanel::Undo,      "Undo (Stack)" },
            { ActivePanel::Settings,  "Settings" },
        };
        for (const auto& entry : entries) {
            bool selected = (active == entry.panel);
            if (ImGui::Selectable(entry.label, selected, 0, ImVec2(0, 28))) active = entry.panel;
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // ---- Content area -----------------------------------------------
        ImGui::BeginChild("Content", ImVec2(0, 0), false);
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%s", panelLabel(active));
        ImGui::Separator();
        ImGui::Spacing();

        switch (active) {
            case ActivePanel::Packages:  renderPackagePanel(); break;
            case ActivePanel::Drivers:   renderDriverPanel(); break;
            case ActivePanel::Warehouse: renderWarehousePanel(); break;
            case ActivePanel::Routes:    renderGraphPanel(); break;
            case ActivePanel::Dispatch:  renderDispatchPanel(); break;
            case ActivePanel::Requests:  renderRequestPanel(); break;
            case ActivePanel::Tracking:  renderTrackingPanel(); break;
            case ActivePanel::Reports:   renderReportsPanel(); break;
            case ActivePanel::Undo:      renderUndoPanel(); break;
            case ActivePanel::Settings:  renderSettingsPanel(); break;
        }
        ImGui::EndChild();

        ImGui::End();

        window.clear(sf::Color(18, 18, 22));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
