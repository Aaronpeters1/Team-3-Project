#ifndef TASK2_H
#define TASK2_H

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include "graph.cpp"

using namespace std;

class Graph {
public:
    vector<string> names;
    vector<vector<int>> adjacency;

    int findPerson(const string& name) const {
        auto it = find(names.begin(), names.end(), name);
        if (it == names.end()) {
            return -1;
        }
        return static_cast<int>(distance(names.begin(), it));
    }
};

vector<int> shortestPath(Graph& graph, const string& startName, const string& endName) {
    int start = graph.findPerson(startName);
    int end = graph.findPerson(endName);

    vector<int> path;
    if (start == -1 || end == -1) {
        return path;
    }

    queue<int> q;
    vector<int> parent(graph.names.size(), -1);

    q.push(start);
    parent[start] = start;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == end) {
            break;
        }

        for (int neighbor : graph.adjacency[current]) {
            if (parent[neighbor] == -1) {
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    if (parent[end] == -1 && end != start) {
        return path;
    }

    int current = end;
    while (current != start) {
        path.push_back(current);
        current = parent[current];
    }
    path.push_back(start);
    reverse(path.begin(), path.end());

    return path;
}

void addPath(vector<int>& fullPath, const vector<int>& part) {
    for (size_t i = 0; i < part.size(); ++i) {
        if (!fullPath.empty() && i == 0) {
            continue;
        }
        fullPath.push_back(part[i]);
    }
}

void question2(Graph& graph, const string& A, const string& B, const string& C, const string& D) {
    vector<int> AB = shortestPath(graph, A, B);
    vector<int> BC = shortestPath(graph, B, C);
    vector<int> CD = shortestPath(graph, C, D);

    vector<int> route1;
    if (!AB.empty() && !BC.empty() && !CD.empty()) {
        addPath(route1, AB);
        addPath(route1, BC);
        addPath(route1, CD);
    }

    vector<int> AC = shortestPath(graph, A, C);
    vector<int> CB = shortestPath(graph, C, B);
    vector<int> BD = shortestPath(graph, B, D);

    vector<int> route2;
    if (!AC.empty() && !CB.empty() && !BD.empty()) {
        addPath(route2, AC);
        addPath(route2, CB);
        addPath(route2, BD);
    }

    if (route1.empty() && route2.empty()) {
        cout << "No valid path exists." << endl;
        return;
    }

    vector<int> bestRoute = route1;
    if (route1.empty() || (!route2.empty() && route2.size() < route1.size())) {
        bestRoute = route2;
    }

    cout << "Shortest path: ";
    for (size_t i = 0; i < bestRoute.size(); ++i) {
        cout << graph.names[bestRoute[i]];
        if (i + 1 < bestRoute.size()) {
            cout << " -> ";
        }
    }

    cout << endl;
    cout << "Number of connections: " << bestRoute.size() - 1 << endl;
}

#endif
