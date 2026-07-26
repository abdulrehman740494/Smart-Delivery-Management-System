#pragma once
#include "Global.h"
#include <vector>



const int MAX_EDGES_PER_LOCATION = 10;

// ---- Locations (vertices) ------------------------------------------------
int findLocationIndexById(int id);
int addLocation(const std::string& name);
bool deleteLocationById(int id);

// ---- Roads (edges) --------------------------------------------------------
// Adds an undirected road of the given weight (distance) between two
// locations. Returns false if either location doesn't exist or if one of
// the two adjacency lists is already full.
bool addRoad(int fromId, int toId, double weight);

// ---- Shortest path (Dijkstra, array-based — no STL priority_queue) --------
// Fills `pathOut` with the sequence of location IDs from fromId to toId
// (inclusive) and returns the total distance, or returns -1.0 and leaves
// pathOut empty if no path exists.
double shortestPath(int fromId, int toId, std::vector<int>& pathOut);

// A single road, expressed by location ID (not array index) — used only
// for reading the graph out for display (e.g. drawing it on a canvas).
struct RoadView {
    int fromLocationId;
    int toLocationId;
    double weight;
};

// Returns every road currently in the graph, each one listed ONCE
// (undirected roads are stored twice internally — once per endpoint —
// this collapses that back down for display purposes).
std::vector<RoadView> getAllRoads();
