#include <iostream>
#include <vector>        // DSA Concept: Vector (dynamic array)
#include <queue>         // DSA Concept: Queue (FIFO structure for alerts)
#include <limits.h>      // Provides INT_MAX for shortest path initialization
#include <string>        // String handling
#include <unordered_map> // DSA Concept: Hash Map (fast lookup for barangay names)
#include <algorithm>     // DSA Concept: Sorting (used for evacuation ranking)

using namespace std;

// ================= GRAPH CLASS =================
class Graph {
private:
    int V;   // Number of vertices (barangays)
    vector< vector< pair<int, int> > > adj; // DSA Concept: Adjacency List (Graph representation)
    vector<bool> flooded; // DSA Concept: Boolean Vector (tracks flood status)

public:
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
        flooded.resize(V, false);
    }

    // Add undirected edge between two barangays with weight (distance)
    void addEdge(int u, int v, int w) {
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
    }

    // Mark a barangay as flooded
    void markFlooded(int node) {
        flooded[node] = true;
    }

    // Reset all barangays to SAFE
    void resetFlood() {
        for (int i = 0; i < flooded.size(); i++)
            flooded[i] = false;
    }

    // Display flood status of all barangays
    void displayFloodStatus(const vector<string>& names) {
        cout << "\n--- FLOOD STATUS ---\n";
        for (int i = 0; i < V; i++) {
            cout << names[i] << ": " 
                 << (flooded[i] ? "FLOODED" : "SAFE") << endl;
        }
    }

    // DSA Concept: Dijkstra’s Algorithm (Shortest Path)
    int shortestDistance(int start, int end) {
        vector<int> dist(V, INT_MAX); // Distance array
        // DSA Concept: Priority Queue (min-heap for shortest path)
        priority_queue< pair<int,int>, vector< pair<int,int> >, greater< pair<int,int> > > pq;

        dist[start] = 0;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            pair<int,int> top = pq.top();
            pq.pop();

            int d = top.first;
            int u = top.second;

            if (u == end) return d;

            // Relaxation step
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i].first;
                int w = adj[u][i].second;

                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        return INT_MAX; // No path found
    }
};

// ================= EVACUATION CENTER STRUCT =================
// DSA Concept: Struct (custom data type for grouping related info)
struct EvacCenter {
    int node;
    int capacity;
    int currentPeople;
    string name;
};

// ================= INPUT VALIDATION FUNCTION =================
int readInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail()) { // Robustness: error handling
            cin.clear();
            cin.ignore(99999, '\n');
            cout << "Invalid input. Enter a number.\n";
        } else {
            cin.ignore(99999, '\n');
            return value;
        }
    }
}

// ================= MAIN FUNCTION =================
int main() {

    // DSA Concept: Vector (stores barangay names)
    vector<string> names = {
        "Barangay A","Barangay B","Barangay C","Barangay D",
        "Barangay E","Barangay F","Barangay G","Barangay H"
    };

    // DSA Concept: Hash Map (maps barangay name → node index)
    unordered_map<string, int> nameToNode;
    for (int i = 0; i < names.size(); i++) {
        nameToNode[names[i]] = i;
    }

    // Graph with 8 barangays
    Graph g(8);

    // Add roads (edges) between barangays
    g.addEdge(0, 1, 4);
    g.addEdge(1, 2, 3);
    g.addEdge(2, 3, 2);
    g.addEdge(1, 4, 6);
    g.addEdge(4, 5, 5);
    g.addEdge(5, 6, 1);
    g.addEdge(6, 7, 7);

    // DSA Concept: Vector of Structs (stores evacuation centers)
    vector<EvacCenter> evacCenters;

    evacCenters.push_back({3, 2, 0, "Evacuation Center 1 (Barangay D)"});
    evacCenters.push_back({5, 3, 0, "Evacuation Center 2 (Barangay F)"});
    evacCenters.push_back({7, 4, 0, "Evacuation Center 3 (Barangay H)"});

    // DSA Concept: Queue (manages emergency alerts FIFO)
    queue<string> alertQueue;

    int choice = 0;

    // ================= MENU LOOP =================
    while (true) {
        cout << "\n===== FLOOD MONITORING SYSTEM =====\n";
        cout << "1. Mark flooded area\n";
        cout << "2. View flood status\n";
        cout << "3. Evacuation recommendation\n";
        cout << "4. View emergency alerts\n";
        cout << "5. Clear alerts\n";
        cout << "6. Reset flood status\n";
        cout << "7. Exit\n";

        choice = readInt("Enter choice: ");

        // ---- Mark flooded area ----
        if (choice == 1) {
            string area;
            cout << "Enter area that is flooded: ";
            getline(cin, area);

            if (nameToNode.find(area) != nameToNode.end()) {
                g.markFlooded(nameToNode[area]); // Vector<bool> update
                alertQueue.push(area + " is FLOODED."); // Queue operation
            } else {
                cout << "Invalid area name.\n";
            }
        }

        // ---- View flood status ----
        else if (choice == 2) {
            g.displayFloodStatus(names); // Vector iteration
        }

        // ---- Evacuation recommendation ----
        else if (choice == 3) {
            string userArea;
            cout << "Enter your barangay location: ";
            getline(cin, userArea);

            if (nameToNode.find(userArea) == nameToNode.end()) {
                cout << "Invalid area name.\n";
                continue;
            }

            int userNode = nameToNode[userArea];

            // DSA Concept: Vector of pairs (distance, index)
            vector< pair<int,int> > ranked; 
            for (int i = 0; i < evacCenters.size(); i++) {
                int d = g.shortestDistance(userNode, evacCenters[i].node); // Dijkstra
                ranked.push_back(make_pair(d, i));
            }

            // DSA Concept: Sorting evacuation centers by distance
            sort(ranked.begin(), ranked.end());

            cout << "\n=== RECOMMENDED EVACUATION CENTERS ===\n";

            bool recommended = false;

            // Display ranked centers
            for (int i = 0; i < ranked.size(); i++) {
                int dist = ranked[i].first;
                int idx = ranked[i].second;

                EvacCenter &e = evacCenters[idx];

                if (dist == INT_MAX) {
                    cout << e.name << ": NO SAFE PATH\n";
                    continue;
                }

                cout << e.name 
                     << " | Distance: " << dist
                     << " | Family Size: " << e.currentPeople 
                     << "/" << e.capacity;

                if (e.currentPeople < e.capacity) {
                    cout << "  <-- Recommended!\n";
                    e.currentPeople++; // Update occupancy
                    recommended = true;
                    break; // Stop after recommending one center
                } else {
                    cout << "  (FULL)\n";
                }
            }

            if (!recommended) {
                cout << "\n⚠ ALL evacuation centers are FULL.\n";
            }
        }

        // ---- View emergency alerts ----
        else if (choice == 4) {
            cout << "\n--- EMERGENCY ALERTS ---\n";
            if (alertQueue.empty()) {
                cout << "No alerts.\n";
            } else {
                // Temporary queue copy to display alerts
                queue<string> temp = alertQueue;
                while (!temp.empty()) {
                    cout << "⚠ " << temp.front() << endl;
                    temp.pop();
                }
            }
        }

        // ---- Clear alerts ----
        else if (choice == 5) {
            while (!alertQueue.empty()) alertQueue.pop(); // Queue clear
            cout << "Alerts cleared.\n";
        }

        // ---- Reset flood status ----
        else if (choice == 6) {
            g.resetFlood(); //
