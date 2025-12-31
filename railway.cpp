#include <iostream> // input / output streams
#include <fstream> // file streams
#include <sstream>//
#include <string> // string class
#include <algorithm> // algorithm
#include <map> // hash map
#include <queue> // queue
#include <limits> // numeric_limits
#include <vector> // vector
#include <functional> // function
using namespace std;

// Route structure to represent edges in the graph (routes between stations)
struct Route {
    string destination;   // Destination station
    int distance;         // Distance to the destination station
    Route* next;          // Pointer to the next route (for chaining routes)

    Route(const string &dest, int dist) : destination(dest), distance(dist), next(nullptr) {}
};

// Station structure for graph nodes (stations in the railway system)
struct Station {
    string name;      // Name of the station
    Route* routes;    // List of routes from this station
    Station* next;    // Pointer to the next station (for chaining stations)

    Station(const string &name) : name(name), routes(nullptr), next(nullptr) {}
};

// Node structure for the linked list stack of history actions
struct StackNode {
    string action;     // Action taken in the system
    StackNode* next;   // Pointer to the next action in the stack

    StackNode(const string &action) : action(action), next(nullptr) {}
};

// Linked List Stack class for managing the history of actions in the system
class HistoryStack {
private:
    StackNode* top;  // Points to the top of the stack

public:
    HistoryStack() : top(nullptr) {}

    // Add a new action to the stack
    void push(const string &action) {
        StackNode* newNode = new StackNode(action);
        newNode->next = top;  // Point to the previous top node
        top = newNode;        // Update the top of the stack
    }

    // Remove the top action from the stack
    void pop() {
        if (!top) return;  // Empty stack
        StackNode* temp = top;
        top = top->next;    // Update the top to the next node
        delete temp;        // Free the memory of the removed node
    }

    // Display all actions in the stack
    void display() const {
        if (!top) {
            cout << "No actions recorded.\n";
            return;
        }
        StackNode* current = top;
        while (current) {
            cout << current->action << "\n";
            current = current->next;
        }
    }

    // Clear the stack
    void clear() {
        while (top) {
            pop();  // Remove each action
        }
    }

    ~HistoryStack() {
        clear();  // Cleanup memory before object destruction
    }
};

// Class to represent the railway system
class RailwaySystem {
private:
    Station* stations;           // Head of the linked list of stations
    HistoryStack history;        // History of actions using the linked list stack

    // Find a station by name
    Station* findStation(const string &name) {
        Station* current = stations;
        while (current) {
            if (current->name == name)
                return current;  // Station found
            current = current->next;
        }
        return nullptr;  // Station not found
    }

    // Add route between two stations (internal function)
    void addRouteInternal(const string &source, const string &destination, int distance) {
        Station* srcStation = findStation(source);
        if (!srcStation) return;

        // Create and add a new route to the source station
        Route* newRoute = new Route(destination, distance);
        newRoute->next = srcStation->routes;
        srcStation->routes = newRoute;
    }

    // Find route between two stations
    Route* findRoute(const string &source, const string &destination) {
        Station* station = findStation(source);
        if (!station) return nullptr;

        // Search for a route from the source station to the destination
        Route* route = station->routes;
        while (route) {
            if (route->destination == destination) {
                return route;  // Route found
            }
            route = route->next;
        }
        return nullptr;  // Route not found
    }

    // Helper function to add to history (records actions performed in the system)
    void addToHistory(const string &action) {
        history.push(action);
    }

    // Save stations and routes to a file (CSV format)
    void saveToFile() {
        ofstream outFile("railway_data.csv");
        if (!outFile) {
            cerr << "Error: Could not open file for saving data.\n";
            return;
        }

        Station* current = stations;
        while (current) {
            Route* route = current->routes;
            while (route) {
                outFile << current->name << "," << route->destination << "," << route->distance << "\n";
                route = route->next;
            }
            current = current->next;
        }

        outFile.close();
        if (outFile.fail()) {
            cerr << "Error: Failed to write data to the file.\n";
        } else {
            cout << "Data saved successfully.\n";
        }
    }

    // Load stations and routes from a file (CSV format)
    void loadFromFile() {
        ifstream inFile("railway_data.csv");
        if (!inFile) {
            cout << "No data file found. Starting with an empty system.\n";
            return;
        }

        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string source, destination, distanceStr;
            if (getline(ss, source, ',') && getline(ss, destination, ',') && getline(ss, distanceStr)) {
                int distance = stoi(distanceStr);
                addStation(source);
                addStation(destination);
                addRouteInternal(source, destination, distance);
                addRouteInternal(destination, source, distance);  // Bidirectional
            } else {
                cerr << "Error: Corrupted data in file.\n";
            }
        }

        inFile.close();
    }

public:
    RailwaySystem() : stations(nullptr) {
        loadFromFile();  // Load existing data when the system starts
    }

    ~RailwaySystem() {
        saveToFile();  // Save data before system shuts down
        while (stations) {
            Station* temp = stations;
            stations = stations->next;
            delete temp;  // Cleanup memory
        }
    }

    // Add a station to the system
    void addStation(const string &name) {
        if (findStation(name)) {
            cout << "Station '" << name << "' already exists.\n";
            return;
        }

        // Create and add the new station to the list
        Station* newStation = new Station(name);
        newStation->next = stations;
        stations = newStation;

        addToHistory("Added station: " + name);
        cout << "Station '" << name << "' added.\n";
    }

    // Add a route between two stations
    void addRoute(const string &source, const string &destination, int distance) {
        if (!findStation(source)) addStation(source);
        if (!findStation(destination)) addStation(destination);

        // Add the route in both directions (bidirectional)
        addRouteInternal(source, destination, distance);
        addRouteInternal(destination, source, distance);

        addToHistory("Added route: " + source + " to " + destination + " (" + to_string(distance) + " km)");
        cout << "Route from '" << source << "' to '" << destination << "' added.\n";
    }

    // Edit the distance of an existing route
    void editRoute(const string &source, const string &destination, int newDistance) {
        Route* route = findRoute(source, destination);
        if (!route) {
            cout << "Route from '" << source << "' to '" << destination << "' not found.\n";
            return;
        }

        // the distance for the route
        route->distance = newDistance;

        // reverse route as well (since the graph is undirected)
        Route* reverseRoute = findRoute(destination, source);
        if (reverseRoute) {
            reverseRoute->distance = newDistance;
        }

        addToHistory("Edited route: " + source + " to " + destination + " with new distance (" + to_string(newDistance) + " km)");
        cout << "Route from '" << source << "' to '" << destination << "' updated to " << newDistance << " km.\n";
    }

    // Display all stations and their routes
    void displayStations() {
        Station* current = stations;
        cout << "\n=========================\n";
        cout << "    Stations and Routes   \n";
        cout << "=========================\n";

        map<string, bool> displayedRoutes;

        while (current) {
            cout << "\n Station: " << current->name << "\n";
            cout << "-------------------------\n";

            Route* route = current->routes;
            if (!route) {
                cout << "  No routes available.\n";
            } else {
                while (route) {
                    if (displayedRoutes[route->destination] == false) {
                        cout << "   Route to: " << route->destination
                             << " | Distance: " << route->distance << " km\n";
                        displayedRoutes[route->destination] = true;
                    }
                    route = route->next;
                }
            }
            current = current->next;
        }

        cout << "\n=========================\n";
        cout << "   End of Station List   \n";
        cout << "=========================\n";
    }

    // Function to find the shortest path between two stations
    void findShortestPath(const string& source, const string &destination) {
        if (!findStation(source) || !findStation(destination)) {
            cout << "Either source or destination station does not exist.\n";
            return;
        }

        map<string, int> distances;
        map<string, string> predecessors;
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;

        Station* current = stations;
        while (current) {
            distances[current->name] = numeric_limits<int>::max();
            current = current->next;
        }

        distances[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            auto [currentDistance, currentStation] = pq.top();
            pq.pop();

            if (currentStation == destination) break;

            Station* station = findStation(currentStation);
            Route* route = station->routes;
            while (route) {
                int newDistance = currentDistance + route->distance;
                if (newDistance < distances[route->destination]) {
                    distances[route->destination] = newDistance;
                    predecessors[route->destination] = currentStation;
                    pq.push({newDistance, route->destination});
                }
                route = route->next;
            }
        }

        if (distances[destination] == numeric_limits<int>::max()) {
            cout << "No path found from '" << source << "' to '" << destination << "'.\n";
            return;
        }

        vector<string> path;
        for (string at = destination; !at.empty(); at = predecessors[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        cout << "\nShortest path from '" << source << "' to '" << destination << "':\n";
        for (const string &station : path) {
            cout << station;
            if (&station != &path.back()) cout << " -> ";
        }
        cout << "\nDistance: " << distances[destination] << " km\n";
    }

    // Function to display all possible paths between two stations
    void displayAllPaths(const string &source, const string &destination) {
        if (!findStation(source) || !findStation(destination)) {
            cout << "Either source or destination station does not exist.\n";
            return;
        }

        vector<string> path;
        map<string, bool> visited;

        // Helper function for DFS to find all paths
        function<void(string)> dfs = [&](string station) {
            visited[station] = true;
            path.push_back(station);

            if (station == destination) {
                // Print the path
                for (size_t i = 0; i < path.size(); ++i) {
                    cout << path[i];
                    if (i < path.size() - 1) cout << " -> ";
                }
                cout << endl;
            } else {
                Station* current = findStation(station);
                Route* route = current->routes;
                while (route) {
                    if (!visited[route->destination]) {
                        dfs(route->destination);
                    }
                    route = route->next;
                }
            }

            path.pop_back();
            visited[station] = false;
        };

        dfs(source);
    }

    // Display the history of actions performed in the system
    void displayHistory() {
        cout << "\n=========================\n";
        cout << "       Action History     \n";
        cout << "=========================\n";
        history.display();
        cout << "=========================\n";
    }

    // Clear history
    void clearHistory() {
        history.clear();
        cout << "History cleared.\n";
    }

    // Save data to file manually
    void manualSave() {
        saveToFile();
    }

    // Function to remove a station from the list of routes
    void deleteStation(const string& stationName) {
        Station* prevStation = nullptr;
        Station* currentStation = stations;

        // Search for the station to delete
        while (currentStation && currentStation->name != stationName) {
            prevStation = currentStation;
            currentStation = currentStation->next;
        }

        if (!currentStation) {
            cout << "Station '" << stationName << "' not found.\n";
            return;  // Station not found
        }

        // Remove all routes involving this station (from other stations)
        Station* tempStation = stations;
        while (tempStation) {
            Route* prevRoute = nullptr;
            Route* currentRoute = tempStation->routes;

            // Remove routes where this station is the destination
            while (currentRoute) {
                if (currentRoute->destination == stationName) {
                    if (prevRoute) {
                        prevRoute->next = currentRoute->next;  // Skip the current route
                    } else {
                        tempStation->routes = currentRoute->next;  // Update the head of the route list
                    }
                    delete currentRoute;  // Delete the route
                    break;  // Only remove one route at a time
                }
                prevRoute = currentRoute;
                currentRoute = currentRoute->next;
            }

            tempStation = tempStation->next;
        }

        // Now remove the station itself from the list of stations
        if (prevStation) {
            prevStation->next = currentStation->next;  // Remove the station from the list
        } else {
            stations = currentStation->next;  // The station is the first in the list
        }

        delete currentStation;  // Delete the station object
        cout << "Station '" << stationName << "' and all related routes have been deleted.\n";

        // Add to history
        addToHistory("Deleted station: " + stationName);
    }

};

// Function to display the menu
void menuDisplay() {
    cout << "\n";
    cout << "====================================\n";
    cout << " Railway Reservation System\n";
    cout << "====================================\n";
    cout << "1   Add Station\n";
    cout << "2  Add Route\n";
    cout << "3  Edit Route\n";
    cout << "4  Display Stations and Routes\n";
    cout << "5  Find Shortest Route\n";
    cout << "6  View Action History\n";
    cout << "7  Clear History\n";
    cout << "8  Save Data to File\n";
    cout << "9  Display All Paths\n";
    cout << "10 Delete Station\n";  // New option to delete a station
    cout << "0  Exit\n";
    cout << "====================================\n";
}

// Function to get the user's choice
int choice() {
    int option;
    cout << "Enter choice: ";
    cin >> option;
    return option;
}


int main() {
    RailwaySystem system;
    int userChoice;
    string station, source, destination;
    int distance;

    while (true) {
        menuDisplay();
        userChoice = choice();

        switch (userChoice) {
            case 1: {
                cout << "Enter station name: ";
                cin >> station;
                system.addStation(station);
                break;
            }
            case 2: {
                cout << "Enter source station: ";
                cin >> source;
                cout << "Enter destination station: ";
                cin >> destination;
                cout << "Enter distance: ";
                cin >> distance;
                system.addRoute(source, destination, distance);
                break;
            }
            case 3: {
                cout << "Enter source station: ";
                cin >> source;
                cout << "Enter destination station: ";
                cin >> destination;
                cout << "Enter new distance: ";
                cin >> distance;
                system.editRoute(source, destination, distance);
                break;
            }
            case 4:
                system.displayStations();
                break;
            case 5: {
                cout << "Enter source station: ";
                cin >> source;
                cout << "Enter destination station: ";
                cin >> destination;
                system.findShortestPath(source, destination);
                break;
            }
            case 6:
                system.displayHistory();
                break;
            case 7:
                system.clearHistory();
                break;
            case 8:
                system.manualSave();
                break;
            case 9: {
                cout << "Enter source station: ";
                cin >> source;
                cout << "Enter destination station: ";
                cin >> destination;
                system.displayAllPaths(source, destination);
                break;
            }
            case 10: {
                cout << "Enter station name to delete: ";
                cin >> station;
                system.deleteStation(station);
                break;
            }
            case 0:
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}
