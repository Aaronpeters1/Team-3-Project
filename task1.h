#ifndef TASK1_H
#define TASK1_H


#include <iostream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

//Question 1: Find the shortest path using BFS
void question1(const vector<vector<int>>& graph,
               const vector<string>& names,
               const unordered_map<string, int>& nameToNum,
               string startName, string endName, int x) {

    if (nameToNum.count(startName) == 0 || 
        nameToNum.count(endName) == 0) {
         cout << "One or both users do not exist." << endl;
         return;
    }
    
    int start = nameToNum.at(startName);
    int end = nameToNum.at(endName);
    
    // Queue for BFS
    queue<int> q;

    // Remember where each person came from
    vector<int> parent(graph.size(), -1);
    vector<bool> visited(graph.size(), false);

    q.push(start);
    visited[start] = true;

        
        // Search through the friendship network
        while (!q.empty()) {
            int current = q.front();
            q.pop();

            if (current == end) {
                break;
            }
            
            // look at the current person's friends
            for (int neighbor : graph[current]) {

                // Only visit people we haven't visited
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }    
        }       
    
    // Check if a path was found
    if (!visited[end]) {
        cout << "No path exists." << endl;
        return;
    }    
    
    // Build the shortest path
    vector<int> path;
    
    int current = end;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    
    reverse(path.begin(), path.end());

    int connections = path.size() - 1;

    // Check whether the path is shorter than x
    if (connections < x) {
        cout << "Yes, they can connect in fewer than "
             << x << " connections." << endl;

        cout << "Shortest path: ";

        for (int i = 0; i < path.size(); i++) {
            cout << names[path[i]];

            if (i < path.size() - 1) {
               cout << " -> ";
            }
        } 
            
        cout << endl;
        cout << "Number of connections: "
             << connections << endl;
    }
    else {
        cout << "No path exists with fewer than "
             << x << " connections." << endl;
             
        cout << "The shortest path has "
             << connections << " connections." << endl;
    }
}

#endif
        
        