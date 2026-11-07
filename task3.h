// task3.h
#ifndef TASK3_H
#define TASK3_H

#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "Graph.cpp"

using namespace std;

// Task 3: visit all reachable users and return to start
void visitAllAndReturn(Graph& graph, const string& startName) {
    int start = graph.findPerson(startName);
    if (start == -1) {
        cout << "Start user not found." << endl;
        return;
    }

    vector<int> reachable;
    queue<int> q;
    vector<bool> seen(graph.names.size(), false);

    q.push(start);
    seen[start] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        reachable.push_back(current);

        for (int neighbor : graph.adjacency[current]) {
            if (!seen[neighbor]) {
                seen[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    if (reachable.size() == 1) {
        cout << "Shortest cycle visiting all reachable users and returning to "
             << startName << ": " << startName << " -> " << startName << endl;
        cout << "Number of connections: 0" << endl;
        return;
    }

    vector<int> bestPath;
    vector<int> currentPath;
    vector<bool> visited(graph.names.size(), false);
    int minConnections = INT_MAX;

    currentPath.push_back(start);
    visited[start] = true;

    function<void(int)> dfs = [&](int current) {
        if (currentPath.size() == reachable.size()) {
            if (find(graph.adjacency[current].begin(), graph.adjacency[current].end(), start)
                != graph.adjacency[current].end()) {
                vector<int> cycle = currentPath;
                cycle.push_back(start);
                int connections = static_cast<int>(cycle.size()) - 1;
                if (connections < minConnections) {
                    minConnections = connections;
                    bestPath = cycle;
                }
            }
            return;
        }

        for (int neighbor : graph.adjacency[current]) {
            if (!seen[neighbor]) continue;
            if (visited[neighbor]) continue;

            currentPath.push_back(neighbor);
            visited[neighbor] = true;
            dfs(neighbor);
            visited[neighbor] = false;
            currentPath.pop_back();
        }
    };

    dfs(start);

    if (bestPath.empty()) {
        cout << "No cycle found that visits all reachable users and returns to "
             << startName << "." << endl;
    } else {
        cout << "Shortest cycle visiting all reachable users and returning to "
             << startName << ": ";
        for (size_t i = 0; i < bestPath.size(); ++i) {
            cout << graph.names[bestPath[i]];
            if (i + 1 < bestPath.size()) {
                cout << " -> ";
            }
        }
        cout << endl;
        cout << "Number of connections: " << minConnections << endl;
    }
}

#endif