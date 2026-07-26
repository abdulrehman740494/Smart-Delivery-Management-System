#include "Panels.h"
#include "Graph.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>



void renderGraphPanel()
{
    static char locationName[64] = "";
    static int fromId = 1, toId = 1;
    static double roadWeight = 1.0;
    static std::string lastMessage;

    static int pathFromId = 1, pathToId = 1;
    static std::vector<int> lastPath;
    static double lastDistance = -1.0;
    static bool pathSearched = false;

    if (ImGui::CollapsingHeader("Add Location", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputText("Location name", locationName, sizeof(locationName));
        if (ImGui::Button("Add Location##btn"))
        {
            if (strlen(locationName) == 0)
                lastMessage = "Name is required.";
            else
            {
                int newId = addLocation(locationName);
                lastMessage = (newId == -1) ? "Location storage full!" : ("Added location #" + std::to_string(newId));
                locationName[0] = '\0';
            }
        }
    }

    if (ImGui::CollapsingHeader("Add Road", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputInt("From location ID", &fromId);
        ImGui::InputInt("To location ID", &toId);
        ImGui::InputDouble("Distance (km)", &roadWeight, 1.0, 5.0, "%.1f");
        if (ImGui::Button("Add Road##btn"))
        {
            lastMessage = addRoad(fromId, toId, roadWeight) ? "Road added." : "Could not add road — check IDs.";
        }
    }

    if (!lastMessage.empty())
        ImGui::TextColored(ImVec4(0.5f, 0.9f, 0.5f, 1.0f), "%s", lastMessage.c_str());

    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Find Shortest Path", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputInt("From##path", &pathFromId);
        ImGui::InputInt("To##path", &pathToId);
        if (ImGui::Button("Find Shortest Path##btn", ImVec2(180, 32)))
        {
            lastDistance = shortestPath(pathFromId, pathToId, lastPath);
            pathSearched = true;
        }
        if (pathSearched)
        {
            if (lastDistance >= 0)
                ImGui::TextColored(ImVec4(0.5f, 0.9f, 0.5f, 1.0f), "Total distance: %.1f km", lastDistance);
            else
                ImGui::TextColored(ImVec4(0.9f, 0.4f, 0.4f, 1.0f), "No path exists between those locations.");
        }
    }

    ImGui::Spacing();
    ImGui::Text("Route map (locations arranged in a circle; roads shown as lines)");

    float remainingHeight = ImGui::GetContentRegionAvail().y;
    ImVec2 canvasSize(ImGui::GetContentRegionAvail().x, std::max(remainingHeight, 260.0f));
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
;
    static bool mapLoadAttempted = false;
    static bool mapLoaded = false;
    if (!mapLoadAttempted)
    {
        mapLoaded = mapTexture.loadFromFile("assets/map_background.png");
        mapLoadAttempted = true;
    }

    if (mapLoaded)
    {
        ImGui::Image(mapTexture, sf::Vector2f(canvasSize.x, canvasSize.y));
        ImGui::SetCursorScreenPos(canvasPos); // draw InvisibleButton over the same rect
    }

    ImGui::InvisibleButton("graphCanvas", canvasSize);
    ImDrawList *draw = ImGui::GetWindowDrawList();
    if (!mapLoaded)
    {
        draw->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(28, 28, 34, 255));
    }
    draw->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(10, 11, 14, 90));

    // Lay out every in-use location evenly around a circle.
    struct NodePos
    {
        int id;
        std::string name;
        ImVec2 pos;
    };
    std::vector<NodePos> nodePositions;
    int activeCount = 0;
    for (int i = 0; i < locationCount; ++i)
        if (locations[i].inUse)
            ++activeCount;

    float cx = canvasPos.x + canvasSize.x / 2.0f;
    float cy = canvasPos.y + canvasSize.y / 2.0f;
    float radius = std::min(canvasSize.x, canvasSize.y) / 2.0f - 40.0f;

    int placed = 0;
    for (int i = 0; i < locationCount; ++i)
    {
        if (!locations[i].inUse)
            continue;
        float angle = (activeCount > 0) ? (2.0f * 3.14159265f * placed / activeCount) : 0.0f;
        ImVec2 p(cx + radius * std::cos(angle), cy + radius * std::sin(angle));
        nodePositions.push_back({locations[i].id, locations[i].name, p});
        ++placed;
    }

    auto findPos = [&](int locId) -> ImVec2 *
    {
        for (auto &n : nodePositions)
            if (n.id == locId)
                return &n.pos;
        return nullptr;
    };

    // Draw every road as a plain line first.
    for (const auto &road : getAllRoads())
    {
        ImVec2 *a = findPos(road.fromLocationId);
        ImVec2 *b = findPos(road.toLocationId);
        if (!a || !b)
            continue;
        draw->AddLine(*a, *b, IM_COL32(190, 195, 205, 235), 2.5f);
        ImVec2 mid((a->x + b->x) / 2.0f, (a->y + b->y) / 2.0f);
        char label[16];
        snprintf(label, sizeof(label), "%.0f", road.weight);
        draw->AddText(ImVec2(mid.x + 1, mid.y + 1), IM_COL32(0, 0, 0, 210), label);
        draw->AddText(mid, IM_COL32(255, 220, 130, 255), label);
    }

    // Redraw the last shortest-path route on top, highlighted.
    for (size_t i = 0; i + 1 < lastPath.size(); ++i)
    {
        ImVec2 *a = findPos(lastPath[i]);
        ImVec2 *b = findPos(lastPath[i + 1]);
        if (!a || !b)
            continue;
        draw->AddLine(*a, *b, IM_COL32(90, 200, 130, 255), 4.0f);
    }

    for (const auto &n : nodePositions)
    {
        bool onPath = std::find(lastPath.begin(), lastPath.end(), n.id) != lastPath.end();
        ImU32 fill = onPath ? IM_COL32(90, 200, 130, 255) : IM_COL32(90, 150, 220, 255);
        draw->AddCircle(n.pos, 20.5f, IM_COL32(255, 255, 255, 200), 0, 2.5f);
        draw->AddCircleFilled(n.pos, 18.0f, fill);
        char idLabel[8];
        snprintf(idLabel, sizeof(idLabel), "%d", n.id);
        draw->AddText(ImVec2(n.pos.x - 6, n.pos.y - 7), IM_COL32(15, 15, 20, 255), idLabel);
        draw->AddText(ImVec2(n.pos.x - 19, n.pos.y + 23), IM_COL32(0, 0, 0, 200), n.name.c_str());
        draw->AddText(ImVec2(n.pos.x - 20, n.pos.y + 22), IM_COL32(255, 255, 255, 255), n.name.c_str());
    }

    if (nodePositions.empty())
    {
        draw->AddText(ImVec2(cx - 80, cy), IM_COL32(140, 140, 150, 255), "Add locations to see the map here.");
    }
}