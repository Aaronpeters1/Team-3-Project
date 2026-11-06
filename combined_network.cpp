#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>

using namespace std;

// --- Graph and PersonNode Classes ---
class PersonNode; // Forward declaration

class FriendNode {
public:
    PersonNode* person;
    FriendNode* next;
    FriendNode* previous;

    FriendNode() : person(nullptr), next(nullptr), previous(nullptr) {}
};

class PersonNode {
public:
    string name;
    FriendNode* friendsHead;
    FriendNode* friendsTail;
    PersonNode* next;
    PersonNode* previous;

    PersonNode() : friendsHead(nullptr), friendsTail(nullptr), next(nullptr), previous(nullptr) {}
};

class Graph {
public:
    PersonNode* head;
    PersonNode* tail;

    Graph() : head(nullptr), tail(nullptr) {}

    PersonNode* findPerson(const string& name) {
        PersonNode* current = head;
        while (current != nullptr) {
            if (current->name == name) return current;
            current = current->next;
        }
        return nullptr;
    }

    void addPerson(const string& name) {
        if (findPerson(name) != nullptr) return;

        PersonNode* node = new PersonNode();
        node->name = name;

        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            node->previous = tail;
            tail->next = node;
            tail = node;
        }
    }

    void addFriendOneWay(PersonNode* person, PersonNode* friendPerson) {
        FriendNode* check = person->friendsHead;
        while (check != nullptr) {
            if (check->person == friendPerson) return;
            check = check->next;
        }

        FriendNode* node = new FriendNode();
        node->person = friendPerson;

        if (person->friendsHead == nullptr) {
            person->friendsHead = node;
            person->friendsTail = node;
        } else {
            node->previous = person->friendsTail;
            person->friendsTail->next = node;
            person->friendsTail = node;
        }
    }

    void addFriendship(const string& a, const string& b) {
        PersonNode* personA = findPerson(a);
        PersonNode* personB = findPerson(b);
        if (personA == nullptr || personB == nullptr) return;

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

    void print() {
        PersonNode* person = head;
        while (person != nullptr) {
            cout << person->name;
            FriendNode* f = person->friendsHead;
            while (f != nullptr) {
                cout << " -> " << f->person->name;
                f = f->next;
            }
            cout << endl;
            person = person->next;
        }
    }
};

// --- Task 3: Visit All Reachable Users and Return to A ---
void visitAllAndReturn(Graph& graph, const string& startName) {
    PersonNode* start = graph.findPerson(startName);
    if (start == nullptr) {
        cout << "Start user not found." << endl;
        return;
    }

    vector<string> bestPath; // To store the best path
    vector<string> currentPath; // To store the current path during traversal
    unordered_map<string, bool> visited; // To track visited nodes
    int minConnections = INT_MAX; // To track the minimum number of connections

    // Helper function for DFS
    function<void(PersonNode*, int)> dfs = [&](PersonNode* current, int connections) {
        // Add the current node to the path and mark it as visited
        currentPath.push_back(current->name);
        visited[current->name] = true;

        // If we return to the start and have visited all nodes, check the path length
        if (current->name == startName && currentPath.size() > 1) {
            if (connections < minConnections && currentPath.size() == visited.size() + 1) {
                minConnections = connections;
                bestPath = currentPath;
            }
        } else {
            // Explore all unvisited neighbors
            FriendNode* friendNode = current->friendsHead;
            while (friendNode != nullptr) {
                if (!visited[friendNode->person->name] || (friendNode->person->name == startName && currentPath.size() == visited.size() + 1)) {
                    dfs(friendNode->person, connections + 1);
                }
                friendNode = friendNode->next;
            }
        }

        // Backtrack: remove the current node from the path and mark it as unvisited
        currentPath.pop_back();
        visited[current->name] = false;
    };

    // Start DFS from the starting node
    dfs(start, 0);

    // Output the result
    if (bestPath.empty()) {
        cout << "No cycle found that visits all reachable users and returns to " << startName << "." << endl;
    } else {
        cout << "Shortest cycle visiting all reachable users and returning to " << startName << ": ";
        for (size_t i = 0; i < bestPath.size(); i++) {
            cout << bestPath[i];
            if (i < bestPath.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
        cout << "Number of connections: " << minConnections << endl;
    }
}
// --- Task 2 Functions ---
vector<PersonNode*> shortestPath(Graph& graph, string startName, string endName) {
    PersonNode* start = graph.findPerson(startName);
    PersonNode* end = graph.findPerson(endName);

    vector<PersonNode*> path;

    if (start == nullptr || end == nullptr) {
        return path;
    }

    queue<PersonNode*> q;
    unordered_map<PersonNode*, PersonNode*> parent;

    q.push(start);
    parent[start] = nullptr;

    while (!q.empty()) {
        PersonNode* current = q.front();
        q.pop();

        if (current == end) {
            break;
        }

        FriendNode* friendNode = current->friendsHead;

        while (friendNode != nullptr) {
            PersonNode* neighbor = friendNode->person;

            if (parent.find(neighbor) == parent.end()) {
                parent[neighbor] = current;
                q.push(neighbor);
            }

            friendNode = friendNode->next;
        }
    }

    if (parent.find(end) == parent.end()) {
        return path;
    }

    PersonNode* current = end;

    while (current != nullptr) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    return path;
}

void addPath(vector<PersonNode*>& fullPath, const vector<PersonNode*>& part) {
    for (int i = 0; i < (int)part.size(); i++) {
        if (!fullPath.empty() && i == 0) {
            continue;
        }
        fullPath.push_back(part[i]);
    }
}

void question2(Graph& graph, string A, string B, string C, string D) {
    vector<PersonNode*> AB = shortestPath(graph, A, B);
    vector<PersonNode*> BC = shortestPath(graph, B, C);
    vector<PersonNode*> CD = shortestPath(graph, C, D);

    vector<PersonNode*> route1;

    if (!AB.empty() && !BC.empty() && !CD.empty()) {
        addPath(route1, AB);
        addPath(route1, BC);
        addPath(route1, CD);
    }

    vector<PersonNode*> AC = shortestPath(graph, A, C);
    vector<PersonNode*> CB = shortestPath(graph, C, B);
    vector<PersonNode*> BD = shortestPath(graph, B, D);

    vector<PersonNode*> route2;

    if (!AC.empty() && !CB.empty() && !BD.empty()) {
        addPath(route2, AC);
        addPath(route2, CB);
        addPath(route2, BD);
    }

    if (route1.empty() && route2.empty()) {
        cout << "No valid path exists." << endl;
        return;
    }

    vector<PersonNode*> bestRoute;

    if (route1.empty()) {
        bestRoute = route2;
    } else if (route2.empty()) {
        bestRoute = route1;
    } else if (route1.size() <= route2.size()) {
        bestRoute = route1;
    } else {
        bestRoute = route2;
    }

    cout << "Shortest path: ";

    for (int i = 0; i < (int)bestRoute.size(); i++) {
        cout << bestRoute[i]->name;

        if (i < (int)bestRoute.size() - 1) {
            cout << " -> ";
        }
    }

    cout << endl;
    cout << "Number of connections: " << bestRoute.size() - 1 << endl;
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

    // Task 3: Visit all reachable users starting and ending at A
    cout << "\nTask 3:" << endl;
    visitAllAndReturn(g, "A");


    return 0;
}