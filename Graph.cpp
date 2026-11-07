#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Graph {
public:
    vector<string> names;
    vector<vector<int>> adjacency;
    unordered_map<string, int> personIndex;

    Graph() = default;

    int findPerson(const string& name) const {
        auto it = personIndex.find(name);
        return (it == personIndex.end()) ? -1 : it->second;
    }

    void addPerson(const string& name) {
        if (findPerson(name) != -1) return;

        personIndex[name] = static_cast<int>(names.size());
        names.push_back(name);
        adjacency.emplace_back();
    }

    void addFriendOneWay(int person, int friendPerson) {
        if (person < 0 || friendPerson < 0) return;
        if (person >= static_cast<int>(adjacency.size()) || friendPerson >= static_cast<int>(adjacency.size())) return;
        if (find(adjacency[person].begin(), adjacency[person].end(), friendPerson) != adjacency[person].end()) return;

        adjacency[person].push_back(friendPerson);
    }

    void addFriendship(const string& a, const string& b) {
        int personA = findPerson(a);
        int personB = findPerson(b);

        if (personA == -1 || personB == -1) return;

        addFriendOneWay(personA, personB);
        addFriendOneWay(personB, personA);
    }

    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Could not open " << filename << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();

            istringstream words(line);
            string person;
            if (!(words >> person)) continue;

            addPerson(person);

            string other;
            while (words >> other) {
                addPerson(other);
                addFriendship(person, other);
            }
        }
        return true;
    }

    void print() const {
        for (size_t i = 0; i < names.size(); ++i) {
            cout << names[i];
            for (int neighbor : adjacency[i]) {
                cout << " -> " << names[neighbor];
            }
            cout << endl;
        }
    }
};

// --- Utility functions for the project tasks ---

static bool hasPath(const Graph& graph, const string& start, const string& goal) {
    int startIndex = graph.findPerson(start);
    int goalIndex = graph.findPerson(goal);

    if (startIndex == -1 || goalIndex == -1) {
        return false;
    }

    if (startIndex == goalIndex) {
        return true;
    }

    vector<bool> visited(graph.names.size(), false);
    queue<int> q;
    q.push(startIndex);
    visited[startIndex] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int neighbor : graph.adjacency[current]) {
            if (!visited[neighbor]) {
                if (neighbor == goalIndex) {
                    return true;
                }
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    return false;
}

static void question2(const Graph& graph, const string& a, const string& b, const string& c, const string& d) {
    bool pathAB = hasPath(graph, a, b);
    bool pathCD = hasPath(graph, c, d);

    cout << "Question 2: checking connectivity between " << a << " and " << b
         << ", and between " << c << " and " << d << endl;

    if (pathAB && pathCD) {
        cout << "A path exists from " << a << " to " << b << " and from " << c << " to " << d << "." << endl;
    } else {
        cout << "At least one of the requested paths does not exist." << endl;
    }
}

static void visitAllAndReturn(const Graph& graph, const string& startName) {
    int start = graph.findPerson(startName);
    if (start == -1) {
        cout << "Start person not found: " << startName << endl;
        return;
    }

    vector<bool> visited(graph.names.size(), false);
    queue<int> q;
    q.push(start);
    visited[start] = true;

    vector<string> order;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        order.push_back(graph.names[current]);

        for (int neighbor : graph.adjacency[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    cout << "Task 3: traversal from " << startName << " visits: ";
    for (size_t i = 0; i < order.size(); ++i) {
        cout << order[i];
        if (i + 1 < order.size()) {
            cout << " -> ";
        }
    }
    cout << endl;

    bool allVisited = true;
    for (size_t i = 0; i < visited.size(); ++i) {
        if (!visited[i]) {
            allVisited = false;
            break;
        }
    }

    if (allVisited) {
        cout << "All nodes were visited and the traversal can return to the start node." << endl;
    } else {
        cout << "Not all nodes were reached from " << startName << "." << endl;
    }
}

// Question 1: Find the shortest path using BFS
void question1(Graph& graph, const string& startName, const string& endName, int x) {
    int start = graph.findPerson(startName);
    int end = graph.findPerson(endName);

    if (start == -1 || end == -1) {
        cout << "One or both users do not exist." << endl;
        return;
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

    if (parent[end] == -1) {
        cout << "No path exists." << endl;
        return;
    }

    vector<string> path;
    int current = end;

    while (current != start) {
        path.push_back(graph.names[current]);
        current = parent[current];
    }

    path.push_back(graph.names[start]);
    reverse(path.begin(), path.end());

    int connections = static_cast<int>(path.size()) - 1;

    if (connections < x) {
        cout << "Yes, they can connect in fewer than " << x << " connections." << endl;
        cout << "Shortest path: ";

        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i + 1 < path.size()) {
                cout << " -> ";
            }
        }

        cout << endl;
        cout << "Number of connections: " << connections << endl;
    }
    else {
        cout << "No path exists with fewer than " << x << " connections." << endl;
        cout << "The shortest path has " << connections << " connections." << endl;
    }
}

// --- Main Function ---
int main() {
    Graph g;
    if (!g.loadFromFile("inputs.txt")) {
        return 1;
    }

    g.print();

    cout << "\nTask 2:" << endl;
    question2(g, "A", "B", "C", "D");

    cout << "\nTask 3:" << endl;
    visitAllAndReturn(g, "A");

    cout << "\nTask 1:" << endl;
    question1(g, "A", "D", 3);
}

    cout << "\nTask 1:" << endl;
    question1(g, "A", "D", 3);

    return 0;
}
