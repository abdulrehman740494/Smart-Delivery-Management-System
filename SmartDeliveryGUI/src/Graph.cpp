#include "Graph.h"
#include <iostream>
#include <algorithm>


struct RoadEdge {
    int toIndex;   // index into locations[] / adjacency[]
    double weight; // road distance
};

static RoadEdge adjacency[MAX_LOCATIONS][MAX_EDGES_PER_LOCATION];
static int edgeCount[MAX_LOCATIONS]; // zero-initialized automatically (static storage duration)

const double INF = 1e9;


// Locations


int findLocationIndexById(int id) {
    for (int i = 0; i < locationCount; ++i) {
        if (locations[i].inUse && locations[i].id == id) return i;
    }
    return -1;
}

int addLocation(const std::string& name) {
    int slot = -1;
    for (int i = 0; i < locationCount; ++i) {
        if (!locations[i].inUse) { slot = i; break; }
    }
    if (slot == -1) {
        if (locationCount >= MAX_LOCATIONS) return -1;
        slot = locationCount;
        ++locationCount;
    }

    int newId = nextLocationId++;
    locations[slot].id = newId;
    locations[slot].name = name;
    locations[slot].inUse = true;
    edgeCount[slot] = 0; // clear any stale edges left over from a previous occupant of this slot

    return newId;
}

bool deleteLocationById(int id) {
    int idx = findLocationIndexById(id);
    if (idx == -1) return false;
    locations[idx].inUse = false;
    edgeCount[idx] = 0;
   
    return true;
}

// Roads (edges)


bool addRoad(int fromId, int toId, double weight) {
    int fromIdx = findLocationIndexById(fromId);
    int toIdx = findLocationIndexById(toId);
    if (fromIdx == -1 || toIdx == -1) return false;
    if (edgeCount[fromIdx] >= MAX_EDGES_PER_LOCATION || edgeCount[toIdx] >= MAX_EDGES_PER_LOCATION) return false;

    adjacency[fromIdx][edgeCount[fromIdx]++] = { toIdx, weight };
    adjacency[toIdx][edgeCount[toIdx]++] = { fromIdx, weight }; // undirected road
    return true;
}


// Dijkstra's shortest path (array-based, O(V^2) — fine for MAX_LOCATIONS = 30)


double shortestPath(int fromId, int toId, std::vector<int>& pathOut) {
    pathOut.clear();
    int startIdx = findLocationIndexById(fromId);
    int endIdx = findLocationIndexById(toId);
    if (startIdx == -1 || endIdx == -1) return -1.0;

    double dist[MAX_LOCATIONS];
    bool visited[MAX_LOCATIONS];
    int prevIndex[MAX_LOCATIONS];
    for (int i = 0; i < MAX_LOCATIONS; ++i) { dist[i] = INF; visited[i] = false; prevIndex[i] = -1; }
    dist[startIdx] = 0.0;

    for (int iteration = 0; iteration < locationCount; ++iteration) {
        // Find the closest unvisited, in-use vertex (manual linear scan —
        // this is the step an STL priority_queue would normally speed up).
        int u = -1;
        double best = INF;
        for (int i = 0; i < locationCount; ++i) {
            if (locations[i].inUse && !visited[i] && dist[i] < best) { best = dist[i]; u = i; }
        }
        if (u == -1) break; // remaining vertices are unreachable
        visited[u] = true;
        if (u == endIdx) break; // shortest path to the destination is finalized

        for (int e = 0; e < edgeCount[u]; ++e) {
            int v = adjacency[u][e].toIndex;
            if (!locations[v].inUse || visited[v]) continue;
            double newDist = dist[u] + adjacency[u][e].weight;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                prevIndex[v] = u;
            }
        }
    }

    if (dist[endIdx] >= INF) return -1.0; // no path found

    // Reconstruct the path by walking prevIndex backwards from the end.
    std::vector<int> reversed;
    for (int at = endIdx; at != -1; at = prevIndex[at]) reversed.push_back(locations[at].id);
    std::reverse(reversed.begin(), reversed.end());
    pathOut = reversed;

    return dist[endIdx];
}

std::vector<RoadView> getAllRoads() {
    std::vector<RoadView> roads;
    for (int u = 0; u < locationCount; ++u) {
        if (!locations[u].inUse) continue;
        for (int e = 0; e < edgeCount[u]; ++e) {
            int v = adjacency[u][e].toIndex;
            if (!locations[v].inUse) continue;
            if (u < v) roads.push_back({ locations[u].id, locations[v].id, adjacency[u][e].weight });
        }
    }
    return roads;
}

