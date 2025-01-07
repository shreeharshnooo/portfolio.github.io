#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <climits>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <map>
#include <limits>
#include <set>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <cmath>

using namespace std;

char adminf[20] = "Trip.txt";

// Struct to store tourist place information
struct TouristPlace {
    string name;
    string crowd_status;
    int cost;
    int rating;
    string location;
    string season;
    string accessibility;  // New field for accessibility (if applicable)
    vector<string> activities;  // List of activities for the place
    int popularity;
    std::string description;

TouristPlace(std::string n, std::string c, int co, int r, std::string l, std::string s, int p, std::string a, std::vector<std::string> act)
        : name(n), crowd_status(c), cost(co), rating(r), location(l), season(s), popularity(p), accessibility(a), activities(act) {}

    // Constructor without accessibility and activities
    TouristPlace(std::string n, std::string c, int co, int r, std::string l, std::string s, int p)
        : name(n), crowd_status(c), cost(co), rating(r), location(l), season(s), popularity(p), accessibility(""), activities({}) {}

    // Default constructor
    TouristPlace()
        : name(""), crowd_status(""), cost(0), rating(0), location(""), season(""), popularity(0), accessibility(""), activities({}) {}
};

class Graph1 {
public:
    unordered_map<string, vector<pair<string, int>>> adjList;  // Place name -> (neighbor place, cost)

    void addEdge(const string& from, const string& to, int cost);
    void dijkstra(const string& start, const string& destination);  // Declare the dijkstra function

    int getDistance(const string& from, const string& to);  // Function to get distance between two nodes
};

class Tourist {
public:
    void displayName();
    void displayCost();
    void displayCrowdStatus();
    void displayRating();
    void displayLocation();
    void selectTouristPlaces();
    void travelMap(Graph1& g1);
    void addTouristPlaces();
    void addEdgesToGraph(Graph1& g1);
    void calculateDistancesFromCurrentLocation(const string& currentLocation, Graph1& g1);
    void calculateAndDisplayShortestPath(const string& currentLocation, Graph1& g1);
    void displayDistancesFromCurrentLocation(const string& currentLocation, Graph1& g1);
    int calculateTotalCost();
    void displayPlacesByCrowdStatus();
    void filterPlacesByBudget();
    void displayHighestRatedPlaces();
    void addNewPlace();
    void viewPlaceDetails();
    void sortByRating();
    void filterByCrowdStatus();
    void sortByCost() ;
    void displayMostExpensivePlace();
    void displayCheapestPlace();
    void displayPlacesByRatingRange();
    void displayPlacesByLocation();
    void findPlacesByName();
    void displayPlacesByPopularity();
    void planCustomTour();
    void sortPlacesByCost();


    void displayPlacesByAccessibility();


    void displayPlacesBySeason();

    void generateTouristActivities();


    void displayPlacesWithActivity();


    void  displayPlacesInProximity();
// New function

    vector<TouristPlace>& getTouristPlaces();

private:
    vector<int> selectedPlaces;
    vector<TouristPlace> touristPlaces;  // Keep touristPlaces private
};

vector<TouristPlace>& Tourist::getTouristPlaces() {
    return touristPlaces;
}
void Graph1::addEdge(const string& from, const string& to, int cost) {
    adjList[from].push_back({to, cost});
    adjList[to].push_back({from, cost});  // Assuming undirected graph
}

void Graph1::dijkstra(const string& start, const string& destination) {
    unordered_map<string, int> dist;
    unordered_map<string, string> parent;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    for (auto& place : adjList) {
        dist[place.first] = INT_MAX;  // Initialize distances to infinity
    }

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        string u = pq.top().second;
        int uDist = pq.top().first;
        pq.pop();

        // Skip processing if we already found a shorter path
        if (uDist > dist[u]) continue;

        for (auto& neighbor : adjList[u]) {
            string v = neighbor.first;
            int weight = neighbor.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Reconstruct the shortest path
    if (dist[destination] == INT_MAX) {
        cout << "No path found from " << start << " to " << destination << endl;
    } else {
        cout << "Shortest path from " << start << " to " << destination << " is " << dist[destination] << " units long.\n";
        cout << "Path: ";
        string curr = destination;
        vector<string> path;
        while (curr != start) {
            path.push_back(curr);
            curr = parent[curr];
        }
        path.push_back(start);
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << path[i];
            if (i != 0) cout << " -> ";
        }
        cout << endl;
    }
}

int Graph1::getDistance(const string& from, const string& to) {
    if (from == to) return 0;  // Distance to itself is 0
    for (const auto& neighbor : adjList[from]) {
        if (neighbor.first == to) {
            return neighbor.second;
        }
    }
    return INT_MAX;  // Return a large value if no direct path exists
}
//
// Function to simulate travel map and calculate shortest path between tourist places
/**
 * @brief Simulates travel through the selected tourist places and calculates the shortest path between them.
 *
 * This function calculates the shortest path between each pair of selected tourist places using Dijkstra's algorithm.
 * It also calculates the total distance and cost for the entire trip and displays the complete path.
 *
 * @param g The graph representing the tourist places and their distances.
 */
void Tourist::travelMap(Graph1& g1) {
    if (selectedPlaces.size() < 2) {
        cout << "Please select at least two places to calculate the path.\n";
        return;
    }

    // Travel through the selected places
    for (size_t i = 0; i < selectedPlaces.size() - 1; i++) {
        string startPlace = touristPlaces[selectedPlaces[i]].name;  // Get the name of the start place
        string endPlace = touristPlaces[selectedPlaces[i + 1]].name;  // Get the name of the end place

        cout << "\nCalculating shortest path from " << startPlace << " to " << endPlace << "...\n";
        g1.dijkstra(startPlace, endPlace);
    }

    // Calculate the total distance and cost for the entire trip
    int totalDistance = 0;
    int totalCost = 0;
    string currentPlace = touristPlaces[selectedPlaces[0]].name;

    for (size_t i = 1; i < selectedPlaces.size(); i++) {
        string nextPlace = touristPlaces[selectedPlaces[i]].name;
        int distance = g1.getDistance(currentPlace, nextPlace);
        totalDistance += distance;
        totalCost += touristPlaces[selectedPlaces[i]].cost;
        currentPlace = nextPlace;
    }

    cout << "\nTotal distance for the trip: " << totalDistance << " KM\n";
    cout << "Total cost for the trip: " << totalCost << " Rs\n";

    // Display the entire path
    cout << "\nComplete path for the trip:\n";
    for (size_t i = 0; i < selectedPlaces.size(); i++) {
        cout << touristPlaces[selectedPlaces[i]].name;
        if (i != selectedPlaces.size() - 1) cout << " -> ";
    }
    cout << endl;
}

// Function to display the name of the tourist places void Tourist::displayName()
/**
 * @brief Displays the list of tourist places with their serial numbers and names.
 *
 * This function prints a formatted table of tourist places stored in the `touristPlaces`
 * container. Each place is displayed with a serial number and its name.
 *
 * The output format is as follows:
 * -------------------------------------------------------------------------------------------------
 * Sr No.          NAME
 * -------------------------------------------------------------------------------------------------
 * 1               PlaceName1
 * 2               PlaceName2
 * ...
 * -------------------------------------------------------------------------------------------------
 */
void Tourist::displayName() {
    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << "Sr No.\t\tNAME\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    int c = 1;
    for (const auto& place : touristPlaces) {
        cout << c++ << "\t\t" << place.name << endl;
    }
    cout << "-------------------------------------------------------------------------------------------------\n";
}

// Function to display the cost of the tourist places
/**
 * @brief Displays the cost of tourist places.
 *
 * This function prints a formatted table of tourist places and their associated costs.
 * The table includes a serial number, the name of the tourist place, and the cost.
 * The output is displayed in a tabular format with headers and separators for better readability.
 *
 * Example output:
 * -------------------------------------------------------------------------------------------------
 * Sr No.          NAME                    COST
 * -------------------------------------------------------------------------------------------------
 * 1               Place1                  100
 * 2               Place2                  200
 * ...
 * -------------------------------------------------------------------------------------------------
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
/**
 * @brief Displays the cost of tourist places.
 *
 * This function prints a formatted table of tourist places and their associated costs.
 * The table includes a serial number, the name of the tourist place, and the cost.
 * The output is displayed in a tabular format with headers and separators for better readability.
 *
 * Example output:
 * -------------------------------------------------------------------------------------------------
 * Sr No.          NAME                    COST
 * -------------------------------------------------------------------------------------------------
 * 1               Place1                  100
 * 2               Place2                  200
 * ...
 * -------------------------------------------------------------------------------------------------
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayCost() {
    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << "Sr No.\t\tNAME\t\t\tCOST\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    int c = 1;
    for (const auto& place : touristPlaces) {
        cout << c++ << "\t\t" << place.name << "\t\t" << place.cost << endl;
    }
    cout << "-------------------------------------------------------------------------------------------------\n";
}

// Function to display the crowd status of the tourist places
/**
 * @brief Displays the crowd status of tourist places.
 *
 * This function prints a formatted table of tourist places and their associated crowd status.
 * The table includes a serial number, the name of the tourist place, and the crowd status.
 * The output is displayed in a tabular format with headers and separators for better readability.
 *
 * Example output:
 * -------------------------------------------------------------------------------------------------
 * Sr No.          NAME                    CROWD STATUS
 * -------------------------------------------------------------------------------------------------
 * 1               Place1                  crowded
 * 2               Place2                  less crowded
 * ...
 * -------------------------------------------------------------------------------------------------
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayCrowdStatus() {
    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << "Sr No.\t\tNAME\t\t\tCROWD STATUS\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    int c = 1;
    for (const auto& place : touristPlaces) {
        cout << c++ << "\t\t" << place.name << "\t\t" << place.crowd_status << endl;
    }
    cout << "-------------------------------------------------------------------------------------------------\n";
}

// Function to display the rating of the tourist places
/**
 * @brief Displays the rating of tourist places.
 *
 * This function prints a formatted table of tourist places and their associated ratings.
 * The table includes a serial number, the name of the tourist place, and the rating.
 * The output is displayed in a tabular format with headers and separators for better readability.
 *
 * Example output:
 * -------------------------------------------------------------------------------------------------
 * Sr No.          NAME                    RATING
 * -------------------------------------------------------------------------------------------------
 * 1               Place1                  95
 * 2               Place2                  90
 * ...
 * -------------------------------------------------------------------------------------------------
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayRating() {
    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << "Sr No.\t\tNAME\t\t\tRATING\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    int c = 1;
    for (const auto& place : touristPlaces) {
        cout << c++ << "\t\t" << place.name << "\t\t" << place.rating << endl;
    }
    cout << "-------------------------------------------------------------------------------------------------\n";
}

// Function to display the location of the tourist places
/**
 * @brief Displays the list of tourist locations.
 *
 * This function prints a formatted table of tourist locations to the standard output.
 * The table includes a serial number, the name of the tourist place, and its location.
 * The table is enclosed within horizontal lines for better readability.
 *
 * Example output:
 * -------------------------------------------------------------------------------------------------
 * Sr No.          NAME                    LOCATION
 * -------------------------------------------------------------------------------------------------
 * 1               Eiffel Tower            Paris
 * 2               Statue of Liberty       New York
 * 3               Great Wall              China
 * -------------------------------------------------------------------------------------------------
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayLocation() {
    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << "Sr No.\t\tNAME\t\t\tLOCATION\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    int c = 1;
    for (const auto& place : touristPlaces) {
        cout << c++ << "\t\t" << place.name << "\t\t" << place.location << endl;
    }
    cout << "-------------------------------------------------------------------------------------------------\n";
}

// Function to allow the tourist to select places they want to visit
/**
 * @brief Allows the tourist to select places they want to visit.
 *
 * This function prompts the user to enter the number of places they wish to visit.
 * It then displays a list of available tourist places and allows the user to select
 * the desired places by entering the corresponding numbers. The selected places are
 * stored in the `selectedPlaces` vector.
 *
 * The function performs the following steps:
 * - Prompts the user to enter the number of places they want to visit.
 * - Validates the input to ensure it is a positive number.
 * - Checks if the number of places exceeds the available list size and adjusts if necessary.
 * - Clears any previously selected places.
 * - Prompts the user to select places by entering the corresponding numbers.
 * - Validates each selection to ensure it is within the valid range.
 * - Stores the selected place indices in the `selectedPlaces` vector.
 * - Displays the list of selected places.
 *
 * @note The function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::selectTouristPlaces() {
    int numPlaces;
    cout << "\nEnter the number of places you want to visit: ";
    cin >> numPlaces;

    if (numPlaces <= 0) {
        cout << "Invalid number of places. Please enter a positive number.\n";
        return;
    }

    // Ensure the number of places is within the available list size
    if (numPlaces > touristPlaces.size()) {
        cout << "We have only " << touristPlaces.size() << " places available. Selecting all places instead.\n";
        numPlaces = touristPlaces.size();
    }

    selectedPlaces.clear();
    int placeIndex;

    cout << "Select " << numPlaces << " places by entering the corresponding number from the list:\n";
    for (int i = 0; i < numPlaces; i++) {
        cout << "Enter place number " << i + 1 << ": ";
        cin >> placeIndex;

        // Ensure the index is valid
        if (placeIndex > 0 && placeIndex <= touristPlaces.size()) {
            selectedPlaces.push_back(placeIndex - 1);  // Store the index (adjusted for 0-based index)
        } else {
            cout << "Invalid choice. Please select a valid place number.\n";
            i--;  // Retry the current selection
        }
    }

    // Display the selected places
    cout << "\nYou have selected the following places to visit:\n";
    cout << "------------------------------------------------------------\n";
    for (int i : selectedPlaces) {
        cout << touristPlaces[i].name << " (Location: " << touristPlaces[i].location << ", Cost: " << touristPlaces[i].cost << ", Rating: " << touristPlaces[i].rating << ")\n";
    }
}

// Function to add tourist places from a file
/**
 * @brief Adds predefined tourist places to the touristPlaces vector.
 *
 * This function initializes the touristPlaces vector with a list of predefined tourist places.
 * Each tourist place is represented by a TouristPlace struct containing various attributes such as:
 * - name: The name of the tourist place.
 * - crowd_status: The crowd status of the place (e.g., "crowded", "less crowded").
 * - cost: The cost associated with visiting the place (in Rs).
 * - rating: The rating of the place (out of 100).
 * - location: The geographical location of the place.
 * - season: The best season to visit the place (e.g., "Winter", "Summer").
 * - popularity: The popularity score of the place (out of 10).
 *
 * The function populates the touristPlaces vector with a diverse set of tourist places across different locations,
 * crowd statuses, costs, ratings, seasons, and popularity scores.
 */
void Tourist::addTouristPlaces() {
    touristPlaces = {
         {"Taj Mahal", "crowded", 2000, 5, "Agra", "Winter", 9, "Wheelchairaccessible", {"sightseeing", "photography"}},
        {"Goa Beaches", "crowded", 1500, 4, "Goa", "Summer", 8, "Not wheelchairaccessible", {"swimming", "sunbathing"}},
        {"Leh Ladakh", "lesscrowded", 3000, 4, "Leh", "Summer", 7, "Limitedaccessibility", {"hiking", "sightseeing"}},
        {"Hampi", "lesscrowded", 1000, 4, "Karnataka", "Summer", 6, "Notwheelchairaccessible", {"hiking", "sightseeing"}},
        {"Jaipur", "crowded", 1200, 4, "Jaipur", "Winter", 5, "Wheelchairaccessible", {"sightseeing", "shopping"}},
        {"Andaman Islands", "less crowded", 2500, 4, "Andaman", "Winter", 6, "Limitedaccessibility", {"swimming", "scuba diving"}},
        {"Mumbai", "crowded", 800, 4, "Mumbai", "Monsoon", 9, "Wheelchairaccessible", {"sightseeing", "shopping"}},
        {"New Delhi", "crowded", 600, 4, "New Delhi", "Winter", 8, "Wheelchairaccessible", {"sightseeing", "shopping"}},
        {"Rajasthan Desert", "lesscrowded", 2000, 4, "Rajasthan", "Winter", 6, "Notwheelchairaccessible", {"hiking", "camel riding"}},
        {"Mysore Palace", "crowded", 1000, 4, "Mysore", "Winter", 7, "Wheelchairaccessible", {"sightseeing", "photography"}},
        {"Nagaland", "lesscrowded", 2500, 4, "Nagaland", "Winter", 6, "Limitedaccessibility", {"hiking", "cultural tours"}},
        {"Shimla", "lesscrowded", 1200, 4, "Shimla", "Winter", 8, "Limitedaccessibility", {"hiking", "sightseeing"}},
        {"Manali", "lesscrowded", 1400, 4, "Manali", "Winter", 9, "Limitedaccessibility", {"hiking", "skiing"}},
        {"Varanasi Ghats", "crowded", 800, 4, "Varanasi", "Winter", 8, "Limitedaccessibility", {"sightseeing", "boating"}},
        {"Chennai Marina Beach", "crowded", 500, 4, "Chennai", "Summer", 6, "Notwheelchairaccessible", {"swimming", "sunbathing"}},
        {"Bangalore Gardens", "crowded", 700, 4, "Bangalore", "Monsoon", 7, "Wheelchairaccessible", {"sightseeing", "walking"}},
        {"Kolkata Victoria Memorial", "crowded", 900, 4, "Kolkata", "Winter", 6, "Wheelchairaccessible", {"sightseeing", "photography"}},
        {"Pondicherry", "less crowded", 1000, 4, "Pondicherry", "Winter", 6, "Limitedaccessibility", {"swimming", "sightseeing"}},
        {"Hyderabad Charminar", "crowded", 850, 4, "Hyderabad", "Winter", 7, "Wheelchairaccessible", {"sightseeing", "shopping"}},
        {"Kerala Backwaters", "lesscrowded", 2000, 4, "Kerala", "Monsoon", 8, "Limitedaccessibility", {"boating", "sightseeing"}},
        {"Darjeeling", "less crowded", 1800, 4, "Darjeeling", "Winter", 7, "Not wheelchairaccessible", {"hiking", "sightseeing"}},
        {"Amritsar Golden Temple", "crowded", 950, 4, "Amritsar", "Winter", 8, "Wheelchairaccessible", {"sightseeing", "spiritual tours"}},
        {"Rishikesh", "lesscrowded", 1200, 4, "Rishikesh", "Winter", 9, "Limitedaccessibility", {"hiking", "yoga"}},
        {"Kanyakumari", "lesscrowded", 1500, 4, "Kanyakumari", "Summer", 8, "Limitedaccessibility", {"sightseeing", "swimming"}},
        {"Udaipur", "crowded", 1200, 4, "Rajasthan", "Winter", 6, "Wheelchairaccessible", {"sightseeing", "boating"}},
        {"Jaisalmer", "lesscrowded", 1100, 4, "Rajasthan", "Winter", 5, "Limitedaccessibility", {"hiking", "camel riding"}},
        {"Sikkim", "crowded", 1500, 4, "Darjeeling", "Winter", 8, "Limitedaccessibility", {"hiking", "sightseeing"}},
        {"Gokarna", "crowded", 1000, 4, "Karnataka", "Summer", 8, "Notwheelchairaccessible", {"swimming", "hiking"}},
        {"Srinagar", "lesscrowded", 2000, 4, "Kashmir", "Winter", 7, "Limitedaccessibility", {"hiking", "boating"}},
        {"Alleppey", "crowded", 1300, 4, "Kerala", "Monsoon", 8, "Limitedaccessibility", {"boating", "sightseeing"}},
        {"Havelock Island", "lesscrowded", 2000, 4, "Andaman", "Summer", 6, "Limitedaccessibility", {"swimming", "scuba diving"}},
        {"Varkala", "less crowded", 1200, 4, "Kerala", "Monsoon", 7, "Limitedaccessibility", {"swimming", "hiking"}},
        {"Ooty", "crowded", 1500, 4, "Tamil Nadu", "Winter", 9, "Limitedaccessibility", {"hiking", "sightseeing"}},
        {"Coorg", "lesscrowded", 1000, 4, "Karnataka", "Monsoon", 5, "Limitedaccessibility", {"hiking", "sightseeing"}},
        {"Ujjain", "lesscrowded", 1500, 4, "Madhya Pradesh", "Winter", 4, "Wheelchairaccessible", {"sightseeing", "spiritual tours"}}
    };
}

// Function to add edges between the places with random distances
/**
 * @brief Adds edges between tourist places with random distances.
 *
 * This function populates the graph with edges representing the distances between tourist places.
 * Each edge is assigned a random distance between 100 and 1100 km for demonstration purposes.
 * The graph is assumed to be undirected, so edges are added in both directions.
 *
 * @param g The graph representing the tourist places and their distances.
 */
void Tourist::addEdgesToGraph(Graph1& g1) {
    // Iterate through each pair of tourist places
    for (int i = 0; i < touristPlaces.size(); i++) {
        for (int j = i + 1; j < touristPlaces.size(); j++) {
            // Generate a random distance between 100 and 1100 km
            int distance = rand() % 1000 + 100;
            // Add an edge from place i to place j with the generated distance
            g1.addEdge(touristPlaces[i].name, touristPlaces[j].name, distance);
        }
    }
}

// Function to calculate distances from the current location
/**
 * @brief Calculates distances from the current location to all tourist places.
 *
 * This function adds edges from the current location to each tourist place in the graph.
 * Each edge is assigned a random distance between 100 and 1100 km for demonstration purposes.
 * The graph is assumed to be undirected, so edges are added in both directions.
 *
 * @param currentLocation The current location of the user.
 * @param g The graph representing the tourist places and their distances.
 */
void Tourist::calculateDistancesFromCurrentLocation(const string& currentLocation, Graph1& g1) {
    // Add edges from the current location to each tourist place with random distances
    for (const auto& place : touristPlaces) {
        int distance = rand() % 1000 + 100; // Random distance between 100 and 1100 km
        g1.addEdge(currentLocation, place.name, distance);
    }
}

// Function to calculate and display the shortest path visiting all selected places
/**
 * @brief Calculates and displays the shortest path visiting all selected places.
 *
 * This function calculates the shortest path that visits all selected tourist places
 * starting from the current location. It uses a greedy approach to find the nearest
 * unvisited place at each step. The function also calculates the total distance and
 * cost for the trip and displays the complete path.
 *
 * @param currentLocation The current location of the user.
 * @param g The graph representing the tourist places and their distances.
 */
void Tourist::calculateAndDisplayShortestPath(const string& currentLocation, Graph1& g1) {
    // Add edges from the current location to each tourist place
    calculateDistancesFromCurrentLocation(currentLocation, g1);

    vector<bool> visited(selectedPlaces.size(), false);  // Track visited places
    string current = currentLocation;  // Start from the current location
    int totalDistance = 0;  // Initialize total distance
    vector<string> path = {current};  // Initialize path with the starting location

    // Iterate through all selected places
    for (size_t i = 0; i < selectedPlaces.size(); ++i) {
        int nearest = -1;  // Index of the nearest unvisited place
        int nearestDistance = INT_MAX;  // Distance to the nearest unvisited place

        // Find the nearest unvisited place
        for (size_t j = 0; j < selectedPlaces.size(); ++j) {
            if (!visited[j]) {
                string placeName = touristPlaces[selectedPlaces[j]].name;
                int distance = g1.getDistance(current, placeName);
                if (distance < nearestDistance) {
                    nearest = j;
                    nearestDistance = distance;
                }
            }
        }

        // Visit the nearest place
        if (nearest != -1) {
            visited[nearest] = true;  // Mark the place as visited
            current = touristPlaces[selectedPlaces[nearest]].name;  // Update current location
            totalDistance += nearestDistance;  // Add distance to the total distance
            path.push_back(current);  // Add the place to the path
        }
    }

    // Return to the starting point
    totalDistance += g1.getDistance(current, currentLocation);
    path.push_back(currentLocation);

    // Calculate total cost
    int totalCost = calculateTotalCost();

    // Display the shortest path, total distance, and total cost
    cout << "Shortest path visiting all selected places:\n";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i != path.size() - 1) cout << " -> ";
    }
    cout << "\nTotal distance: " << totalDistance << " KM\n";
    cout << "Total cost: " << totalCost << " Rs\n";
}

// New function to display distances from current location to all tourist places
/**
 * @brief Displays the distances from the current location to all tourist places.
 *
 * This function prints the distances from the specified current location to all
 * tourist places stored in the `touristPlaces` vector. It uses the provided graph
 * object to calculate the distances between the current location and each tourist place.
 *
 * @param currentLocation The name of the current location from which distances are calculated.
 * @param g The graph object that contains the distance information between locations.
 *
 * The function outputs the distances in the following format:
 * "Distances from <currentLocation> to all tourist places:"
 * "------------------------------------------------------------"
 * "<currentLocation> to <place.name>: <distance> KM"
 * "------------------------------------------------------------"
 *
 * Example usage:
 * @code
 * Graph g;
 * // Assume g is populated with locations and distances
 * Tourist t;
 * t.displayDistancesFromCurrentLocation("New York", g);
 * @endcode
 */
/**
 * @brief Displays the distances from the current location to all tourist places.
 *
 * This function prints the distances from the specified current location to all
 * tourist places stored in the `touristPlaces` vector. It uses the provided graph
 * object to calculate the distances between the current location and each tourist place.
 *
 * @param currentLocation The name of the current location from which distances are calculated.
 * @param g The graph object that contains the distance information between locations.
 *
 * The function outputs the distances in the following format:
 * "Distances from <currentLocation> to all tourist places:"
 * "------------------------------------------------------------"
 * "<currentLocation> to <place.name>: <distance> KM"
 * "------------------------------------------------------------"
 *
 * Example usage:
 * @code
 * Graph g;
 * // Assume g is populated with locations and distances
 * Tourist t;
 * t.displayDistancesFromCurrentLocation("New York", g);
 * @endcode
 */
void Tourist::displayDistancesFromCurrentLocation(const string& currentLocation, Graph1& g1) {
    cout << "Distances from " << currentLocation << " to all tourist places:\n";
    cout << "------------------------------------------------------------\n";
    for (const auto& place : touristPlaces) {
        int distance = g1.getDistance(currentLocation, place.name);
        cout << currentLocation << " to " << place.name << ": " << distance << " KM\n";
    }
    cout << "------------------------------------------------------------\n";
}

// New function to calculate the total cost based on selected places
int Tourist::calculateTotalCost() {
    int totalCost = 0;
    for (int i : selectedPlaces) {
        totalCost += touristPlaces[i].cost;
    }
    return totalCost;
}
/**
 * @brief Displays tourist places filtered by crowd status.
 *
 * This function prompts the user to enter a crowd status (e.g., 'crowded' or 'less crowded')
 * and then filters the list of tourist places to display only those that match the specified
 * crowd status. The filtered list is displayed in a formatted table with headers and separators
 * for better readability.
 *
 * The function performs the following steps:
 * - Prompts the user to enter the desired crowd status.
 * - Iterates through the list of tourist places and checks if each place matches the specified crowd status.
 * - Displays the matching places in a tabular format with their serial number, name, location, cost, and rating.
 * - If no places match the specified crowd status, a message is displayed indicating that no places were found.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.displayPlacesByCrowdStatus();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayPlacesByCrowdStatus() {
    string status;
    cout << "Enter crowd status to filter (e.g., 'crowded' or 'less crowded'): ";
    cin >> status;

    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << "Places with crowd status '" << status << "':\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    int c = 1;
    for (const auto& place : touristPlaces) {
        if (status == place.crowd_status) {
            cout << c++ << ". " << place.name << " (Location: " << place.location << ", Cost: " << place.cost << ", Rating: " << place.rating << ")\n";
        }
    }

    if (c == 1) {
        cout << "No places found with the given crowd status.\n";
    }
    cout << "-------------------------------------------------------------------------------------------------\n";
}
/**
 * @brief Filters and displays tourist places within a specified budget.
 *
 * This function prompts the user to enter their maximum budget and then filters the list of tourist places
 * to display only those that have a cost less than or equal to the specified budget. The filtered list is
 * displayed in a formatted table with headers and separators for better readability.
 *
 * The function performs the following steps:
 * - Prompts the user to enter their maximum budget.
 * - Iterates through the list of tourist places and checks if each place's cost is within the specified budget.
 * - Displays the matching places in a tabular format with their serial number, name, location, cost, and rating.
 * - If no places match the specified budget, a message is displayed indicating that no places were found.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.filterPlacesByBudget();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::filterPlacesByBudget() {
    int budget;
    cout << "Enter your maximum budget: ";
    cin >> budget;

    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << "Places within your budget (<= " << budget << "):\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    int c = 1;
    for (const auto& place : touristPlaces) {
        if (place.cost <= budget) {
            cout << c++ << ". " << place.name << " (Location: " << place.location << ", Cost: " << place.cost << ", Rating: " << place.rating << ")\n";
        }
    }

    if (c == 1) {
        cout << "No places found within your budget.\n";
    }
    cout << "-------------------------------------------------------------------------------------------------\n";
}

/**
 * @brief Displays the highest-rated tourist places.
 *
 * This function iterates through the list of tourist places and determines the highest rating.
 * It then prints out all the places that have this highest rating along with their details.
 *
 * The details displayed for each place include:
 * - Name of the place
 * - Location of the place
 * - Cost associated with visiting the place
 * - Crowd status of the place
 *
 * The output is formatted with separators for better readability.
 */
void Tourist::displayHighestRatedPlaces() {
    int topRating = 0;
    for (const auto& place : touristPlaces) {
        if (place.rating > topRating) {
            topRating = place.rating;
        }
    }

    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << "Places with the highest rating (" << topRating << "):\n";
    cout << "-------------------------------------------------------------------------------------------------\n";
    for (const auto& place : touristPlaces) {
        if (place.rating == topRating) {
            cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << ", Crowd Status: " << place.crowd_status << ")\n";
        }
    }
    cout << "-------------------------------------------------------------------------------------------------\n";
}
/**
 * @brief Adds a new tourist place to the list.
 *
 * This function prompts the user to enter details for a new tourist place,
 * including the name, crowd status, cost, rating, and location. The new
 * place is then added to the `touristPlaces` vector.
 *
 * The function performs the following steps:
 * - Prompts the user to enter the name of the tourist place.
 * - Prompts the user to enter the crowd status (e.g., 'crowded' or 'less crowded').
 * - Prompts the user to enter the cost associated with visiting the place (in Rs).
 * - Prompts the user to enter the rating of the place (out of 100).
 * - Prompts the user to enter the location of the place.
 * - Adds the new place to the `touristPlaces` vector.
 * - Displays a success message indicating that the new place has been added.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.addNewPlace();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is already initialized.
 */
void Tourist::addNewPlace() {
    TouristPlace newPlace;
    cout << "Enter name of the tourist place: ";
    cin.ignore();  // To ignore any leftover newline character
    getline(cin, newPlace.name);
    cout << "Enter crowd status ('crowded' or 'less crowded'): ";
    cin >> newPlace.crowd_status;
    cout << "Enter cost (in Rs): ";
    cin >> newPlace.cost;
    cout << "Enter rating (out of 100): ";
    cin >> newPlace.rating;
    cout << "Enter location: ";
    cin.ignore();  // To ignore any leftover newline character
    getline(cin, newPlace.location);

    // Add the new place to the list of tourist places
    touristPlaces.push_back(newPlace);
    cout << "New tourist place added successfully!\n";
}
/**
 * @brief Displays detailed information about a specific tourist place.
 *
 * This function allows the user to view detailed information about a specific tourist place.
 * It first checks if there are any tourist places available. If not, it displays a message
 * indicating that no tourist places are available. If tourist places are available, it displays
 * a list of available places with their serial numbers. The user is then prompted to enter the
 * serial number of the place they want to view details for. The function validates the input and
 * displays the details of the selected place, including its name, crowd status, cost, rating, and
 * location. If the user enters an invalid serial number, an error message is displayed.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.viewPlaceDetails();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::viewPlaceDetails() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to view details.\n";
        return;
    }

    int placeIndex;
    cout << "\nAvailable Places:\n";
    for (size_t i = 0; i < touristPlaces.size(); ++i) {
        cout << i + 1 << ". " << touristPlaces[i].name << "\n";
    }

    cout << "\nEnter the serial number of the place to view details: ";
    cin >> placeIndex;

    if (placeIndex > 0 && placeIndex <= touristPlaces.size()) {
        const auto& place = touristPlaces[placeIndex - 1];
        cout << "\nDetails of " << place.name << ":\n";
        cout << "------------------------------------------------------------\n";
        cout << "Name: " << place.name << "\n";
        cout << "Crowd Status: " << place.crowd_status << "\n";
        cout << "Cost: " << place.cost << " Rs\n";
        cout << "Rating: " << place.rating << " out of 100\n";
        cout << "Location: " << place.location << "\n";
        cout << "Season: " << place.season << "\n";
        cout << "Popularity: " << place.popularity << "\n";
        cout << "Accessibility: " << place.accessibility << "\n";
        cout << "Activities: ";
        for (const auto& activity : place.activities) {
            cout << activity;
            if (&activity != &place.activities.back()) cout << ", ";
        }
        cout << "\nDescription: " << place.description << "\n";
        cout << "------------------------------------------------------------\n";
    } else {
        cout << "Invalid serial number. Please try again.\n";
    }
}
/**
 * @brief Sorts the tourist places by their rating in descending order.
 *
 * This function sorts the list of tourist places based on their ratings, from highest to lowest.
 * It first checks if there are any tourist places available to sort. If the list is empty, it
 * displays a message indicating that no tourist places are available. If there are tourist places
 * available, it sorts them using the C++ Standard Library's `sort` function with a custom comparator.
 * The comparator compares the ratings of two tourist places and orders them in descending order.
 *
 * After sorting, the function displays the sorted list of tourist places along with their ratings.
 * The output is formatted with separators for better readability.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.sortByRating();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::sortByRating() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to sort by rating.\n";
        return;
    }

    // Sort the tourist places in descending order of rating
    sort(touristPlaces.begin(), touristPlaces.end(), [](const TouristPlace& a, const TouristPlace& b) {
        return a.rating > b.rating;
    });

    // Display the sorted list of tourist places
    cout << "\nTourist places sorted by rating (highest to lowest):\n";
    cout << "------------------------------------------------------------\n";
    for (const auto& place : touristPlaces) {
        cout << place.name << " (Rating: " << place.rating << ")\n";
    }
    cout << "------------------------------------------------------------\n";
}
/**
 * @brief Filters and displays tourist places based on crowd status.
 *
 * This function prompts the user to enter their crowd preference (e.g., 'crowded' or 'less crowded')
 * and then filters the list of tourist places to display only those that match the specified crowd status.
 * The filtered list is displayed in a formatted table with headers and separators for better readability.
 *
 * The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Prompts the user to enter their crowd preference.
 * - Iterates through the list of tourist places and checks if each place matches the specified crowd status.
 * - Displays the matching places in a tabular format with their name, location, cost, and rating.
 * - If no places match the specified crowd status, a message is displayed indicating that no places were found.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.filterByCrowdStatus();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::filterByCrowdStatus() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to filter by crowd status.\n";
        return;
    }

    string crowdPreference;
    cout << "\nEnter your crowd preference (crowded/less crowded): ";
    cin.ignore();  // To ignore any leftover newline character
    getline(cin, crowdPreference);

    cout << "\nPlaces with crowd status '" << crowdPreference << "':\n";
    cout << "------------------------------------------------------------\n";
    int count = 0;
    for (const auto& place : touristPlaces) {
        if (place.crowd_status == crowdPreference) {
            cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ")\n";
            count++;
        }
    }
    if (count == 0) {
        cout << "No places found with the specified crowd status.\n";
    }
    cout << "------------------------------------------------------------\n";
}
/**
 * @brief Sorts the tourist places by their cost in ascending order.
 *
 * This function sorts the list of tourist places based on their costs, from lowest to highest.
 * It first checks if there are any tourist places available to sort. If the list is empty, it
 * displays a message indicating that no tourist places are available. If there are tourist places
 * available, it sorts them using the C++ Standard Library's `sort` function with a custom comparator.
 * The comparator compares the costs of two tourist places and orders them in ascending order.
 *
 * After sorting, the function displays the sorted list of tourist places along with their costs.
 * The output is formatted with separators for better readability.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.sortByCost();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::sortByCost() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to sort by cost.\n";
        return;
    }

    // Sort the tourist places in ascending order of cost
    sort(touristPlaces.begin(), touristPlaces.end(), [](const TouristPlace& a, const TouristPlace& b) {
        return a.cost < b.cost;  // Sort in ascending order of cost
    });

    // Display the sorted list of tourist places
    cout << "\nTourist places sorted by cost:\n";
    cout << "------------------------------------------------------------\n";
    for (const auto& place : touristPlaces) {
        cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ")\n";
    }
    cout << "------------------------------------------------------------\n";
}
/**
 * @brief Displays the most expensive tourist place.
 *
 * This function finds and displays the tourist place with the highest cost from the list of tourist places.
 * It first checks if there are any tourist places available. If the list is empty, it displays a message
 * indicating that no tourist places are available. If there are tourist places available, it uses the
 * C++ Standard Library's `max_element` function with a custom comparator to find the place with the highest cost.
 *
 * The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Uses `max_element` with a custom comparator to find the tourist place with the highest cost.
 * - Displays the details of the most expensive tourist place, including its name, location, cost, and rating.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.displayMostExpensivePlace();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayMostExpensivePlace() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to display the most expensive one.\n";
        return;
    }

    // Find the tourist place with the highest cost
    auto mostExpensive = *max_element(touristPlaces.begin(), touristPlaces.end(), [](const TouristPlace& a, const TouristPlace& b) {
        return a.cost < b.cost;
    });

    // Display the details of the most expensive tourist place
    cout << "\nMost expensive tourist place:\n";
    cout << "------------------------------------------------------------\n";
    cout << "Name: " << mostExpensive.name << "\n";
    cout << "Location: " << mostExpensive.location << "\n";
    cout << "Cost: " << mostExpensive.cost << " Rs\n";
    cout << "Rating: " << mostExpensive.rating << " out of 100\n";
    cout << "Crowd Status: " << mostExpensive.crowd_status << "\n";
    cout << "Season: " << mostExpensive.season << "\n";
    cout << "Popularity: " << mostExpensive.popularity << "\n";
    cout << "Accessibility: " << mostExpensive.accessibility << "\n";
    cout << "Activities: ";
    for (const auto& activity : mostExpensive.activities) {
        cout << activity;
        if (&activity != &mostExpensive.activities.back()) cout << ", ";
    }
    cout << "\nDescription: " << mostExpensive.description << "\n";
    cout << "------------------------------------------------------------\n";
}
/**
 * @brief Displays the cheapest tourist place.
 *
 * This function finds and displays the tourist place with the lowest cost from the list of tourist places.
 * It first checks if there are any tourist places available. If the list is empty, it displays a message
 * indicating that no tourist places are available. If there are tourist places available, it uses the
 * C++ Standard Library's `min_element` function with a custom comparator to find the place with the lowest cost.
 *
 * The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Uses `min_element` with a custom comparator to find the tourist place with the lowest cost.
 * - Displays the details of the cheapest tourist place, including its name, location, cost, rating, crowd status,
 *   season, popularity, accessibility, activities, and description.
 * - Displays additional details such as nearby attractions, recommended activities, and travel tips (assuming
 *   methods to get these details are available).
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.displayCheapestPlace();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayCheapestPlace() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to display the cheapest one.\n";
        return;
    }

    // Find the tourist place with the lowest cost
    auto cheapest = *min_element(touristPlaces.begin(), touristPlaces.end(), [](const TouristPlace& a, const TouristPlace& b) {
        return a.cost < b.cost;
    });

    // Display the details of the cheapest tourist place
    cout << "\nCheapest tourist place:\n";
    cout << "------------------------------------------------------------\n";
    cout << "Name: " << cheapest.name << "\n";
    cout << "Location: " << cheapest.location << "\n";
    cout << "Cost: " << cheapest.cost << " Rs\n";
    cout << "Rating: " << cheapest.rating << " out of 100\n";
    cout << "Crowd Status: " << cheapest.crowd_status << "\n";
    cout << "Season: " << cheapest.season << "\n";
    cout << "Popularity: " << cheapest.popularity << "\n";
    cout << "Accessibility: " << cheapest.accessibility << "\n";
    cout << "Activities: ";
    for (const auto& activity : cheapest.activities) {
        cout << activity;
        if (&activity != &cheapest.activities.back()) cout << ", ";
    }
    cout << "\nDescription: " << cheapest.description << "\n";
    cout << "------------------------------------------------------------\n";

    // Additional details (assuming methods to get these details are available)
    cout << "\nAdditional Details:\n";
    cout << "------------------------------------------------------------\n";
    cout << "Nearby Attractions: ";
    // Assuming we have a method to get nearby attractions
    // vector<string> nearbyAttractions = getNearbyAttractions(cheapest.name);
    // for (const auto& attraction : nearbyAttractions) {
    //     cout << attraction;
    //     if (&attraction != &nearbyAttractions.back()) cout << ", ";
    // }
    cout << "\n------------------------------------------------------------\n";

    cout << "Recommended Activities: ";
    // Assuming we have a method to get recommended activities
    // vector<string> recommendedActivities = getRecommendedActivities(cheapest.name);
    // for (const auto& activity : recommendedActivities) {
    //     cout << activity;
    //     if (&activity != &recommendedActivities.back()) cout << ", ";
    // }
    cout << "\n------------------------------------------------------------\n";

    cout << "Travel Tips: ";
    // Assuming we have a method to get travel tips
    // vector<string> travelTips = getTravelTips(cheapest.name);
    // for (const auto& tip : travelTips) {
    //     cout << tip;
    //     if (&tip != &travelTips.back()) cout << ", ";
    // }
    cout << "\n------------------------------------------------------------\n";
}
/**
 * @brief Displays tourist places within a specified rating range.
 *
 * This function prompts the user to enter a minimum and maximum rating, and then filters
 * the list of tourist places to display only those that fall within the specified rating range.
 * The filtered list is displayed in a formatted table with headers and separators for better readability.
 *
 * The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Prompts the user to enter the minimum and maximum rating.
 * - Iterates through the list of tourist places and checks if each place's rating falls within the specified range.
 * - Displays the matching places in a tabular format with their name, location, cost, rating, crowd status, season, popularity, accessibility, activities, and description.
 * - If no places match the specified rating range, a message is displayed indicating that no places were found.
 * - Prompts the user if they want to see places with ratings outside the specified range and displays them if the user agrees.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.displayPlacesByRatingRange();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayPlacesByRatingRange() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to filter by rating range.\n";
        return;
    }

    double minRating, maxRating;
    cout << "\nEnter minimum rating: ";
    cin >> minRating;
    cout << "Enter maximum rating: ";
    cin >> maxRating;

    cout << "\nPlaces with ratings between " << minRating << " and " << maxRating << ":\n";
    cout << "------------------------------------------------------------\n";
    int count = 0;
    for (const auto& place : touristPlaces) {
        if (place.rating >= minRating && place.rating <= maxRating) {
            cout << "Name: " << place.name << "\n";
            cout << "Location: " << place.location << "\n";
            cout << "Cost: " << place.cost << " Rs\n";
            cout << "Rating: " << place.rating << " out of 100\n";
            cout << "Crowd Status: " << place.crowd_status << "\n";
            cout << "Season: " << place.season << "\n";
            cout << "Popularity: " << place.popularity << "\n";
            cout << "Accessibility: " << place.accessibility << "\n";
            cout << "Activities: ";
            for (const auto& activity : place.activities) {
                cout << activity;
                if (&activity != &place.activities.back()) cout << ", ";
            }
            cout << "\nDescription: " << place.description << "\n";
            cout << "------------------------------------------------------------\n";
            count++;
        }
    }
    if (count == 0) {
        cout << "No places found within the specified rating range.\n";
    }
    cout << "------------------------------------------------------------\n";

    // Additional functionality: Display places with ratings outside the specified range
    cout << "\nWould you like to see places with ratings outside the specified range? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        cout << "\nPlaces with ratings outside the range " << minRating << " and " << maxRating << ":\n";
        cout << "------------------------------------------------------------\n";
        count = 0;
        for (const auto& place : touristPlaces) {
            if (place.rating < minRating || place.rating > maxRating) {
                cout << "Name: " << place.name << "\n";
                cout << "Location: " << place.location << "\n";
                cout << "Cost: " << place.cost << " Rs\n";
                cout << "Rating: " << place.rating << " out of 100\n";
                cout << "Crowd Status: " << place.crowd_status << "\n";
                cout << "Season: " << place.season << "\n";
                cout << "Popularity: " << place.popularity << "\n";
                cout << "Accessibility: " << place.accessibility << "\n";
                cout << "Activities: ";
                for (const auto& activity : place.activities) {
                    cout << activity;
                    if (&activity != &place.activities.back()) cout << ", ";
                }
                cout << "\nDescription: " << place.description << "\n";
                cout << "------------------------------------------------------------\n";
                count++;
            }
        }
        if (count == 0) {
            cout << "No places found outside the specified rating range.\n";
        }
        cout << "------------------------------------------------------------\n";
    }
}
/**
 * @brief Displays tourist places filtered by location.
 *
 * This function prompts the user to enter a location and then filters the list of tourist places
 * to display only those that match the specified location. The filtered list is displayed in a
 * formatted table with headers and separators for better readability.
 *
 * The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Prompts the user to enter the desired location.
 * - Iterates through the list of tourist places and checks if each place matches the specified location.
 * - Displays the matching places in a tabular format with their name, cost, and rating.
 * - If no places match the specified location, a message is displayed indicating that no places were found.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.displayPlacesByLocation();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayPlacesByLocation() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to filter by location.\n";
        return;
    }

    string location;
    cout << "\nEnter location to filter places: ";
    cin.ignore();  // To ignore any leftover newline character
    getline(cin, location);

    cout << "\nPlaces in " << location << ":\n";
    cout << "------------------------------------------------------------\n";
    int count = 0;
    for (const auto& place : touristPlaces) {
        if (place.location == location) {
            cout << place.name << " (Cost: " << place.cost << " Rs, Rating: " << place.rating << ")\n";
            count++;
        }
    }
    if (count == 0) {
        cout << "No places found in the specified location.\n";
    }
    cout << "------------------------------------------------------------\n";
}
/**
 * @brief Finds and displays tourist places by name.
 *
 * This function allows the user to search for tourist places by their name. It prompts the user to enter
 * a name or a part of the name of the tourist place they are looking for. The function then searches through
 * the list of tourist places and displays those that match the entered name. The search is case-sensitive and
 * matches any part of the name.
 *
 * The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Prompts the user to enter the name or part of the name of the tourist place.
 * - Iterates through the list of tourist places and checks if each place's name contains the entered string.
 * - Displays the matching places in a tabular format with their name, location, cost, and rating.
 * - If no places match the entered name, a message is displayed indicating that no places were found.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.findPlacesByName();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::findPlacesByName() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to search.\n";
        return;
    }

    string name;
    cout << "\nEnter the name of the tourist place: ";
    cin.ignore();  // To ignore any leftover newline character
    getline(cin, name);

    cout << "\nPlaces matching '" << name << "':\n";
    cout << "------------------------------------------------------------\n";
    int count = 0;
    for (const auto& place : touristPlaces) {
        if (place.name.find(name) != string::npos) {  // If name contains the searched string
            cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ")\n";
            count++;
        }
    }
    if (count == 0) {
        cout << "No places found with the specified name.\n";
    }
    cout << "------------------------------------------------------------\n";
}
/**
 * @brief Displays tourist places filtered by popularity.
 *
 * This function prompts the user to enter a minimum popularity score (on a scale of 1 to 10)
 * and then filters the list of tourist places to display only those that have a popularity
 * score greater than or equal to the specified value. The filtered list is displayed in a
 * formatted table with headers and separators for better readability.
 *
 * The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Prompts the user to enter the minimum popularity score.
 * - Iterates through the list of tourist places and checks if each place's popularity score
 *   is greater than or equal to the specified value.
 * - Displays the matching places in a tabular format with their name, location, cost, rating,
 *   and popularity score.
 * - If no places match the specified popularity score, a message is displayed indicating that
 *   no places were found.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.displayPlacesByPopularity();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayPlacesByPopularity() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to filter by popularity.\n";
        return;
    }

    cout << "\nEnter the minimum popularity score (1 to 10): ";
    int minPopularity;
    cin >> minPopularity;

    cout << "\nPlaces with popularity greater than or equal to " << minPopularity << ":\n";
    cout << "------------------------------------------------------------\n";
    int count = 0;
    for (const auto& place : touristPlaces) {
        if (place.popularity >= minPopularity) {
            cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ", Popularity: " << place.popularity << ")\n";
            count++;
        }
    }
    if (count == 0) {
        cout << "No places found with the specified popularity score.\n";
    }
    cout << "------------------------------------------------------------\n";
}
/**
 * @brief Plans a custom tour by allowing the user to select tourist places.
 *
 * This function allows the user to create a custom tour by selecting tourist places from the available list.
 * The user is prompted to enter the name of the place they want to add to their custom tour. The function
 * checks if the entered place exists in the list of tourist places. If the place is found, it is added to
 * the custom tour. The user can continue adding places to the custom tour until they choose to stop.
 *
 * The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Prompts the user to enter the name of the place to add to the custom tour.
 * - Searches for the entered place in the list of tourist places.
 * - If the place is found, it is added to the custom tour and a confirmation message is displayed.
 * - If the place is not found, an error message is displayed.
 * - Prompts the user if they want to add another place to the custom tour.
 * - Displays the list of places in the custom tour after the user has finished adding places.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.planCustomTour();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::planCustomTour() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to plan a custom tour.\n";
        return;
    }

    vector<TouristPlace> customTour;  // Vector to store the custom tour places
    char choice;

    do {
        string placeName;
        cout << "\nEnter the name of the place to add to your custom tour: ";
        cin.ignore();  // To ignore any leftover newline character
        getline(cin, placeName);

        // Find the place in the list of tourist places
        auto it = find_if(touristPlaces.begin(), touristPlaces.end(), [&](const TouristPlace& place) {
            return place.name == placeName;
        });

        if (it != touristPlaces.end()) {
            customTour.push_back(*it);  // Add the place to the custom tour
            cout << "\n" << placeName << " added to your custom tour.\n";
        } else {
            cout << "\nPlace not found! Try again.\n";
        }

        cout << "\nWould you like to add another place to your tour? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    // Display the custom tour
    cout << "\nYour Custom Tour:\n";
    cout << "------------------------------------------------------------\n";
    for (const auto& place : customTour) {
        cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ")\n";
    }
    cout << "------------------------------------------------------------\n";
}
/**
 * @brief Displays tourist places filtered by accessibility features.
 *
 * This function prompts the user to enter an accessibility feature (e.g., wheelchair, pet-friendly)
 * and then filters the list of tourist places to display only those that match the specified feature.
 * The filtered list is displayed in a formatted table with headers and separators for better readability.
 *
 * The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Prompts the user to enter the desired accessibility feature.
 * - Iterates through the list of tourist places and checks if each place matches the specified feature.
 * - Displays the matching places in a tabular format with their name, location, cost, rating, and accessibility feature.
 * - If no places match the specified feature, a message is displayed indicating that no places were found.
 * - Prompts the user if they want to see places with other accessibility features and displays them if the user agrees.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.displayPlacesByAccessibility();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayPlacesByAccessibility() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to filter by accessibility.\n";
        return;
    }

    string accessibilityFeature;
    cout << "\nEnter the accessibility feature (e.g., Wheelchair,Notwheelchair,Limitedaccessibility): ";
    cin.ignore();  // To ignore any leftover newline character
    getline(cin, accessibilityFeature);

    cout << "\nPlaces with accessibility feature '" << accessibilityFeature << "':\n";
    cout << "------------------------------------------------------------\n";
    int count = 0;
    for (const auto& place : touristPlaces) {
        if (place.accessibility.find(accessibilityFeature) != string::npos) {  // If feature matches
            cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ", Accessibility: " << place.accessibility << ")\n";
            count++;
        }
    }
    if (count == 0) {
        cout << "No places found with the specified accessibility feature.\n";
    }
    cout << "------------------------------------------------------------\n";

    // Additional functionality: Display places with other accessibility features
    cout << "\nWould you like to see places with other accessibility features? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        vector<string> otherFeatures = {"Limitedaccessibility","Wheelchair","Notwheelchair"};
        otherFeatures.erase(remove(otherFeatures.begin(), otherFeatures.end(), accessibilityFeature), otherFeatures.end());  // Remove the already selected feature

        for (const auto& feature : otherFeatures) {
            cout << "\nPlaces with accessibility feature '" << feature << "':\n";
            cout << "------------------------------------------------------------\n";
            count = 0;
            for (const auto& place : touristPlaces) {
                if (place.accessibility.find(feature) != string::npos) {  // If feature matches
                    cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ", Accessibility: " << place.accessibility << ")\n";
                    count++;
                }
            }
            if (count == 0) {
                cout << "No places found with the specified accessibility feature.\n";
            }
            cout << "------------------------------------------------------------\n";
        }
    }
}
/**
 * @brief Displays tourist places filtered by season.
 *
 * This function prompts the user to enter a season (e.g., summer, winter, rainy)
 * and then filters the list of tourist places to display only those that match
 * the specified season. The filtered list is displayed in a formatted table with
 * headers and separators for better readability.
 *
 * The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Prompts the user to enter the desired season.
 * - Iterates through the list of tourist places and checks if each place matches the specified season.
 * - Displays the matching places in a tabular format with their name, location, cost, rating, and season.
 * - If no places match the specified season, a message is displayed indicating that no places were found.
 * - Prompts the user if they want to see places recommended for other seasons and displays them if the user agrees.
 *
 * Example usage:
 * @code
 * Tourist t;
 * t.displayPlacesBySeason();
 * @endcode
 *
 * @note This function assumes that the `touristPlaces` container is populated with valid data.
 */
void Tourist::displayPlacesBySeason() {
    if (touristPlaces.empty()) {
        cout << "\n No tourist places available to filter by season.\n";
        return;
    }

    string season;
    cout << "\nEnter the season (e.g., summer, winter, Monsoon): ";
    cin.ignore();  // To ignore any leftover newline character
    getline(cin, season);

    // Normalize the input season (convert to lowercase and trim)
    transform(season.begin(), season.end(), season.begin(), ::tolower);
    season.erase(season.find_last_not_of(" \t\n\r\f\v") + 1); // Trim trailing spaces
    season.erase(0, season.find_first_not_of(" \t\n\r\f\v")); // Trim leading spaces

    cout << "\nPlaces recommended for " << season << " season:\n";
    cout << "------------------------------------------------------------\n";
    int count = 0;
    for (const auto& place : touristPlaces) {
        string placeSeason = place.season;
        transform(placeSeason.begin(), placeSeason.end(), placeSeason.begin(), ::tolower);

        if (placeSeason == season) {  // If season matches
            cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ", Season: " << place.season << ")\n";
            count++;
        }
    }
    if (count == 0) {
        cout << "No places found for the specified season.\n";
    }
    cout << "------------------------------------------------------------\n";

    // Additional functionality: Display places for other seasons
    cout << "\nWould you like to see places recommended for other seasons? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        vector<string> seasons = {"summer", "winter", "Monsoon"};
        seasons.erase(remove(seasons.begin(), seasons.end(), season), seasons.end());  // Remove the already selected season

        for (const auto& otherSeason : seasons) {
            cout << "\nPlaces recommended for " << otherSeason << " season:\n";
            cout << "------------------------------------------------------------\n";
            count = 0;

            for (const auto& place : touristPlaces) {
                string placeSeason = place.season;
                transform(placeSeason.begin(), placeSeason.end(), placeSeason.begin(), ::tolower);

                if (placeSeason == otherSeason) {  // If season matches
                    cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ", Season: " << place.season << ")\n";
                    count++;
                }
            }
            if (count == 0) {
                cout << "No places found for the specified season.\n";
            }
            cout << "------------------------------------------------------------\n";
        }
    }
}
//void Tourist::generateTouristActivities() {
// This function is responsible for generating a list of activities for a tourist.
// It is a member function of the Tourist class.
// The function does not take any parameters and does not return any value.
//
// The purpose of this function is to create a personalized itinerary for a tourist
// based on various factors such as:
// 1. Tourist's preferences: The function might consider the tourist's interests
//    (e.g., adventure, relaxation, culture) to select appropriate activities.
// 2. Location: The function could take into account the tourist's current location
//    or destination to suggest relevant activities available in that area.
// 3. Time of year: Seasonal activities might be considered, such as skiing in winter
//    or beach activities in summer.
// 4. Duration of stay: The function might generate activities based on the length
//    of the tourist's stay, ensuring a balanced itinerary.
// 5. Budget: The function could consider the tourist's budget to suggest activities
//    that are affordable and within their financial plan.
//
// The implementation of this function might involve:
// - Accessing a database or an API to retrieve a list of available activities.
// - Filtering and sorting the activities based on the criteria mentioned above.
// - Storing the generated list of activities in a member variable of the Tourist class
//   for later use or display.
//
// Example usage:
// Tourist tourist;
// tourist.generateTouristActivities();
//
// Note: The actual implementation details are not provided here as the function body
// is commented out. The above description is based on the function's intended purpose
// and typical use cases for such a function in a tourism-related application.
//
void Tourist::generateTouristActivities() {
    if (touristPlaces.empty()) {//
        cout << "\nNo tourist places available to generate activities.\n";
        return;
    }

    cout << "\nEnter the name of the tourist place to generate activities: ";
    string placeName;
    cin.ignore();  // To ignore any leftover newline character
    getline(cin, placeName);

    auto it = find_if(touristPlaces.begin(), touristPlaces.end(), [&](const TouristPlace& place) {
        return place.name == placeName;
    });

    if (it != touristPlaces.end()) {
        cout << "\nAvailable activities at " << placeName << ":\n";
        cout << "------------------------------------------------------------\n";
        for (const auto& activity : it->activities) {
            cout << "- " << activity << "\n";
        }
        cout << "------------------------------------------------------------\n";

        char choice;
        cout << "\nWould you like to add a new activity to this place? (y/n): ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            cout << "Enter the new activity: ";
            string newActivity;
            cin.ignore();  // To ignore any leftover newline character
            getline(cin, newActivity);
            it->activities.push_back(newActivity);
            cout << "New activity added successfully!\n";
        }

        cout << "\nWould you like to remove an existing activity from this place? (y/n): ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            cout << "Enter the activity to remove: ";
            string removeActivity;
            cin.ignore();  // To ignore any leftover newline character
            getline(cin, removeActivity);

            auto activityIt = find(it->activities.begin(), it->activities.end(), removeActivity);
            if (activityIt != it->activities.end()) {
                it->activities.erase(activityIt);
                cout << "Activity removed successfully!\n";
            } else {
                cout << "Activity not found!\n";
            }
        }
    } else {
        cout << "\nPlace not found!\n";
    }
}
void Tourist::sortPlacesByCost() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to sort by cost.\n";
        return;
    }

    cout << "\nSort by cost (1 for Ascending, 2 for Descending): ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        sort(touristPlaces.begin(), touristPlaces.end(), [](const TouristPlace& a, const TouristPlace& b) {
            return a.cost < b.cost;
        });
        cout << "\nPlaces sorted by cost in ascending order.\n";
    } else if (choice == 2) {
        sort(touristPlaces.begin(), touristPlaces.end(), [](const TouristPlace& a, const TouristPlace& b) {
            return a.cost > b.cost;
        });
        cout << "\nPlaces sorted by cost in descending order.\n";
    } else {
        cout << "Invalid choice.\n";
    }

    // Display the sorted places
    for (const auto& place : touristPlaces) {
        cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ")\n";
    }
}
/**
 * @brief Displays a list of tourist places that offer a specified activity.
 *
 * This function prompts the user to enter an activity (e.g., hiking, swimming)
 * and then searches through the list of tourist places to find and display
 * those that offer the specified activity. If no tourist places are available
 * or no places offer the specified activity, appropriate messages are displayed.
 *
 * @note The function assumes that the list of tourist places (touristPlaces)
 *       and each place's activities are already populated.
 */
/**
 * @brief Displays tourist places that offer a specific activity.
 *
 * This function prompts the user to enter an activity (e.g., hiking, swimming)
 * and then searches through the list of tourist places to find and display
 * those that offer the specified activity. If no places are found offering
 * the activity, a message is displayed indicating so.
 *
 * @note If there are no tourist places available, the function will notify
 * the user and return immediately.
 */
void Tourist::displayPlacesWithActivity() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to filter by activity.\n";
        return;
    }

    string activity;
    cout << "\nEnter the activity to search for (e.g., hiking, swimming): ";
    cin.ignore();
    getline(cin, activity);

    cout << "\nPlaces offering activity '" << activity << "':\n";
    cout << "------------------------------------------------------------\n";
    int count = 0;
    for (const auto& place : touristPlaces) {
        for (const auto& act : place.activities) {
            if (act == activity) {
                cout << place.name << " (Location: " << place.location << ", Cost: " << place.cost << " Rs, Rating: " << place.rating << ", Activities: ";
                for (const auto& a : place.activities) {
                    cout << a;
                    if (&a != &place.activities.back()) cout << ", ";
                }
                cout << ")\n";
                count++;
                break;
            }
        }
    }
    if (count == 0) {
        cout << "No places found offering this activity.\n";
    }
    cout << "------------------------------------------------------------\n";
}
/**
 * @brief Displays tourist places within a specified proximity and filters.
 *
 * This function prompts the user to enter their current location and a maximum distance
 * they are willing to travel. It also allows the user to specify additional filters such as
 * crowd preference, minimum rating, and preferred season. The function then displays the
 * tourist places that match the specified criteria.
 *
 * @details The function performs the following steps:
 * - Checks if the list of tourist places is empty and displays a message if it is.
 * - Prompts the user to enter their current location.
 * - Prompts the user to enter the maximum distance they are willing to travel.
 * - Prompts the user to enter additional filters such as crowd preference, minimum rating, and preferred season.
 * - Simulates the distance calculation (for demonstration purposes) and checks if each place is within the specified distance.
 * - Checks if each place matches the additional filters.
 * - Displays the places that match the proximity and filters.
 * - Displays a message if no places are found within the specified filters.
 *
 * @note The distance calculation is simulated using a random number generator for demonstration purposes.
 *       Replace this with a real distance calculation in a production environment.
 */
void Tourist::displayPlacesInProximity() {
    if (touristPlaces.empty()) {
        cout << "\nNo tourist places available to display in proximity.\n";
        return;
    }

    string currentLocation;
    cout << "Enter your current location: ";
    cin.ignore();  // Clear the buffer from previous inputs
    getline(cin, currentLocation);

    int distanceThreshold;
    cout << "Enter the maximum distance you're willing to travel (in km): ";
    cin >> distanceThreshold;

    // Ask for additional filters (crowd status, rating, season)
    string crowdPreference, seasonPreference;
    int ratingThreshold;

    cout << "Enter crowd preference (crowded/less crowded) or press enter to skip: ";
    cin.ignore();  // Clear the input buffer
    getline(cin, crowdPreference);

    cout << "Enter the minimum rating you prefer (1-100) or press enter to skip: ";
    cin >> ratingThreshold;
    if (ratingThreshold < 1) ratingThreshold = 1;  // Default to 1 if not provided

    cout << "Enter preferred season (e.g., Summer, Monsoon) or press enter to skip: ";
    cin.ignore();  // Clear the input buffer
    getline(cin, seasonPreference);

    // Seed random number generator to simulate distances
    srand(time(0));

    cout << "\nPlaces within " << distanceThreshold << " km from " << currentLocation << ":\n";
    cout << "------------------------------------------------------------\n";

    bool found = false; // Flag to check if any place is found within the threshold

    // Simulate the distance check and display places within the distance
    for (const auto& place : touristPlaces) {
        // Random distance (for demonstration purposes, replace with real calculation)
        int distance = rand() % 500;  // Generate a random distance between 0 and 500 km
        bool isWithinDistance = (distance <= distanceThreshold);

        // Check for the additional filters
        bool matchesCrowd = crowdPreference.empty() || place.crowd_status == crowdPreference;
        bool matchesRating = place.rating >= ratingThreshold;
        bool matchesSeason = seasonPreference.empty() || place.season == seasonPreference;

        // If the place matches the proximity and filters, display it
        if (isWithinDistance && matchesCrowd && matchesRating && matchesSeason) {
            found = true;
            cout << place.name << " (Location: " << place.location << ", Distance: " << distance << " km, Cost: "
                 << place.cost << " Rs, Rating: " << place.rating << "/100, Crowd: " << place.crowd_status
                 << ", Season: " << place.season << ", Popularity: " << place.popularity << "%)\n";
        }
    }

    if (!found) {
        cout << "No places found within the specified filters.\n";
    }

    cout << "------------------------------------------------------------\n";
}
// The main function serves as the entry point for the Tourist System application.
// It displays a welcome message and prompts the user to press Enter to continue.
// Once the user presses Enter, the screen is cleared, and the main menu is displayed.
// The user is prompted to enter their current location, which is used for distance calculations.
// The main menu provides various options for the user to interact with the application.
// The user can choose to display tourist place names, costs, crowd status, ratings, and locations.
// The user can select places to visit and calculate the travel map and shortest path.
// The user can display distances from the current location to all tourist places.
// The user can filter places by crowd status, budget, rating range, location, and popularity.
// The user can add a new tourist place and view details of a specific tourist place.
// The user can sort tourist places by rating and cost, and display the most expensive and cheapest places.
// The user can plan a custom tour and display places by accessibility and seasonal preferences.
// The user can generate tourist activities and display places offering a specific activity.
// The user can display tourist places within a specified proximity and filters.
// The application uses a Graph class to represent the tourist places and their distances.
// The Graph class provides methods to add edges, calculate shortest paths using Dijkstra's algorithm, and get distances.
// The Tourist class provides methods to manage and display tourist places, and interact with the user.
// The Tourist class maintains a list of tourist places and selected places for the tour.
// The Tourist class provides methods to add tourist places, select places to visit, and calculate distances.
// The Tourist class provides methods to filter and sort tourist places based on various criteria.
// The Tourist class provides methods to display tourist places by crowd status, budget, rating range, location, and popularity.
// The Tourist class provides methods to add a new tourist place and view details of a specific tourist place.
// The Tourist class provides methods to plan a custom tour and display places by accessibility and seasonal preferences.
// The Tourist class provides methods to generate tourist activities and display places offering a specific activity.
// The Tourist class provides methods to display tourist places within a specified proximity and filters.
// The main function initializes the Tourist and Graph objects and sets up the graph with distances between tourist places.
// The main function enters a loop to display the main menu and handle user input.
// The user can choose an option from the menu and the corresponding method in the Tourist class is called.
// The main function handles invalid choices by exiting the application.
// The application uses standard input and output for user interaction.
// The application uses the C++ Standard Library for various functionalities such as I/O, strings, vectors, and algorithms.
// The application uses the <cstdlib> and <ctime> headers for random number generation.
// The application uses the <climits> header for the INT_MAX constant used in distance calculations.
// The application uses the <unordered_map> and <queue> headers for the Graph class implementation.
// The application uses the <algorithm> header for sorting and finding elements in the Tourist class.
// The application uses the <sstream> header for string stream operations.
// The application uses the <fstream> header for file operations (not used in the current code).
// The application uses the <iostream> header for standard input and output operations.
// The application uses the <string> header for string operations.
// The application uses the <vector> header for dynamic arrays.
// The application uses the <unordered_map> header for hash maps.
// The application uses the <queue> header for priority queues.
// The application uses the <climits> header for integer limits.
// The application uses the <sstream> header for string streams.
// The application uses the <cstdlib> header for general utilities.
// The application uses the <ctime> header for time-related functions.
// The application uses the <algorithm> header for algorithms.
// The application uses the <fstream> header for file streams.
// The application uses the <iostream> header for I/O streams.
// The application uses the <string> header for strings.
// The application uses the <vector> header for vectors.
// The application uses the <unordered_map> header for hash maps.
// The application uses the <queue> header for queues.
// The application uses the <climits> header for limits.
// The application uses the <sstream> header for string streams.
// The application uses the <cstdlib> header for utilities.
// The application uses the <ctime> header for time functions.

// ----------- Constants and Definitions -----------
#define INF INT_MAX

// Constants for setw values
const int ID_WIDTH = 5;
const int USER_NAME_WIDTH = 20;
const int CITY_WIDTH = 15;
const int MODE_WIDTH = 15;
const int STATUS_WIDTH = 10;

// Transportation Mode Class
class TransportationMode {
public:
    int id;
    string name;
    int capacity;
    bool available;

    TransportationMode(int id, string name, int capacity, bool available)
        : id(id), name(name), capacity(capacity), available(available) {}

    void display() const {
        cout << setw(5) << id << setw(15) << name << setw(10) << capacity << setw(10) << (available ? "Yes" : "No") << endl;
    }
};

// Booking Structure
class Booking {
public:
    int id;
    string userName;
    int sourceCityID;
    int destCityID;
    string modeOfTransport;
    string status;

 Booking() : id(0), sourceCityID(0), destCityID(0) {}

    Booking(int id, string userName, int sourceCityID, int destCityID, string modeOfTransport, string status)
        : id(id), userName(userName), sourceCityID(sourceCityID), destCityID(destCityID), modeOfTransport(modeOfTransport), status(status) {}

    void display(const map<int, string>& cityNames) const {
        cout << setw(5) << id << setw(20) << userName << setw(15) << cityNames.at(sourceCityID)
             << setw(15) << cityNames.at(destCityID) << setw(15) << modeOfTransport << setw(10) << status << endl;
    }
};


// Graph Class for City and Route Management
class Graph {
private:
    vector<vector<int>> adjMatrix;
    map<int, string> cityNames;
    vector<TransportationMode> modes;
    unordered_map<int, Booking> bookings;
    int bookingCounter;


public:
    Graph(int cityCount) : adjMatrix(cityCount, vector<int>(cityCount, 0)), bookingCounter(1000) {}

    void addCity(int id, const string& name) {
        cityNames[id] = name;
    }

    /**
     * @brief Adds a route between two nodes in the adjacency matrix.
     *
     * This function updates the adjacency matrix to reflect the distance between
     * the source node and the destination node. It assumes that the routes are
     * bidirectional, meaning the distance from the source to the destination is
     * the same as the distance from the destination to the source.
     *
     * @param src The source node index.
     * @param dest The destination node index.
     * @param distance The distance between the source and destination nodes.
     */
    void addRoute(int src, int dest, int distance) {
        adjMatrix[src][dest] = distance;
        adjMatrix[dest][src] = distance; // Assuming bidirectional routes
    }

    /**
     * @brief Adds a new mode to the collection.
     *
     * This function creates a new mode with the specified parameters and adds it
     * to the modes collection. Each mode is characterized by an ID, a name, a
     * capacity, and an availability status.
     *
     * @param id An integer representing the unique identifier for the mode.
     * @param name A constant reference to a string representing the name of the mode.
     * @param capacity An integer representing the capacity of the mode.
     * @param available A boolean indicating whether the mode is available or not.
     */
    void addMode(int id, const string& name, int capacity, bool available) {
        modes.emplace_back(id, name, capacity, available);
    }

    /**
     * @brief Books a ticket for the user with the specified details.
     *
     * This function attempts to book a ticket for the user with the given
     * source and destination locations using the specified mode of transport.
     * If the mode of transport is available, a new booking is created and
     * confirmed. Otherwise, the booking fails and an appropriate message is displayed.
     *
     * @param userName The name of the user booking the ticket.
     * @param src The source location identifier.
     * @param dest The destination location identifier.
     * @param modeOfTransport The mode of transport to be used for the booking.
     *
     * @note The function assumes that the modes of transport are stored in a
     *       container named 'modes' and that each mode has a 'name' and 'available'
     *       attribute. It also assumes that bookings are stored in a map named 'bookings'
     *       and that there is a global booking counter named 'bookingCounter'.
     *
     * @return void
     */
  void bookTicket(const string& userName, int src, int dest, const string& modeOfTransport) {
        auto it = std::find_if(modes.begin(), modes.end(), [&](const TransportationMode& mode) {
            return mode.name == modeOfTransport && mode.available;
        });

        if (it != modes.end()) {
            Booking newBooking(bookingCounter++, userName, src, dest, modeOfTransport, "Confirmed");
            bookings[newBooking.id] = newBooking;
            cout << "Booking successful. Booking ID: " << newBooking.id << endl;
        } else {
            cout << "Booking failed. Mode of transport " << modeOfTransport << " is not available.\n";
        }
    }

    /**
     * @brief Displays the list of cities.
     *
     * This function prints the list of cities stored in the cityNames map.
     * Each city is displayed with its ID and name in the format:
     * "City ID: <id>, Name: <name>"
     *
     * The function outputs the list to the standard output stream (cout).
     * It begins by printing a header "--- Cities ---" followed by each city's details.
     *
     * @note This function does not modify any member variables and is marked as const.
     */
    void displayCities() const {
        cout << "\n--- Cities ---\n";
        for (const auto& [id, name] : cityNames) {
            cout << "City ID: " << id << ", Name: " << name << endl;
        }
    }

    /**
     * @brief Displays the list of modes with their details.
     *
     * This function prints a formatted table header with columns for ID, Name,
     * Capacity, and Availability. It then iterates through the list of modes
     * and calls the display method on each mode to print its details.
     *
     * @note This function assumes that the 'modes' container and the 'display'
     * method of each mode are properly defined and accessible within the scope
     * of this function.
     */
    void displayModes() const {
        cout << setw(5) << "ID" << setw(15) << "Name" << setw(10) << "Capacity" << setw(10) << "Available" << endl;
        for (const auto& mode : modes) {
            mode.display();
        }
    }

    /**
     * @brief Displays all bookings in a formatted table.
     *
     * This function prints a header row with column names for ID, User Name, Source,
     * Destination, Mode, and Status. It then iterates through the bookings map and
     * calls the display method on each booking to print its details.
     *
     * The function assumes that the 'bookings' container and the 'display' method
     * of each booking are properly defined and accessible within the scope of this function.
     *
     * @note This function does not modify any member variables and is marked as const.
     */
    void viewBookings() const {
        cout << setw(ID_WIDTH) << "ID" << setw(USER_NAME_WIDTH) << "User Name" << setw(CITY_WIDTH) << "Source" << setw(CITY_WIDTH) << "Destination"
             << setw(MODE_WIDTH) << "Mode" << setw(STATUS_WIDTH) << "Status" << endl;
        for (const auto& [id, booking] : bookings) {
            booking.display(cityNames);
        }
    }

    /**
     * @brief Finds the shortest path between two cities using Dijkstra's algorithm.
     *
     * This function calculates the shortest path from the source city to the destination city
     * using Dijkstra's algorithm. It maintains a priority queue to explore the shortest distances
     * and updates the distance and parent vectors accordingly. If a path is found, it prints the
     * shortest distance and the path. If no path is found, it notifies the user.
     *
     * @param src The source city ID.
     * @param dest The destination city ID.
     */
    void findShortestPath(int src, int dest) const {
        vector<int> dist(adjMatrix.size(), INF); // Distance vector initialized to infinity
        vector<bool> visited(adjMatrix.size(), false); // Visited vector to keep track of visited nodes
        vector<int> parent(adjMatrix.size(), -1); // Parent vector to reconstruct the path

        // Priority queue to store (distance, node) pairs, with the smallest distance at the top
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, src}); // Start with the source node
        dist[src] = 0; // Distance to the source node is 0

        while (!pq.empty()) {
            int u = pq.top().second; // Get the node with the smallest distance
            pq.pop();

            if (visited[u]) continue; // Skip if the node is already visited
            visited[u] = true; // Mark the node as visited

            // Explore all adjacent nodes
            for (int v = 0; v < adjMatrix.size(); v++) {
                // If there is an edge and the node is not visited, and a shorter path is found
                if (adjMatrix[u][v] && !visited[v] && dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v]; // Update the distance
                    parent[v] = u; // Update the parent
                    pq.push({dist[v], v}); // Push the updated distance and node to the priority queue

            }
        }

        // If the destination node is unreachable
        if (dist[dest] == INF) {
            cout << "No route found from " << cityNames.at(src) << " to " << cityNames.at(dest) << ".\n";
            return;
        }

        // Print the shortest distance and path
        cout << "Shortest distance: " << dist[dest] << " km\nPath: ";
        vector<int> path;
        for (int current = dest; current != -1; current = parent[current]) {
            path.push_back(current); // Reconstruct the path from destination to source
        }
        reverse(path.begin(), path.end()); // Reverse the path to get the correct order
        for (int city : path) {
            cout << cityNames.at(city) << " "; // Print the city names in the path
        }
        cout << endl;


        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, src});
        dist[src] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (visited[u]) continue;
            visited[u] = true;

            for (int v = 0; v < adjMatrix.size(); v++) {
                if (adjMatrix[u][v] && !visited[v] && dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        // Check if the destination node is unreachable
        if (dist[dest] == INF) {
            cout << "No route found from " << cityNames.at(src) << " to " << cityNames.at(dest) << ".\n";
            return;
        }
    }

        cout << "Shortest distance: " << dist[dest] << " km\nPath: ";
        vector<int> path;
        for (int current = dest; current != -1; current = parent[current]) {
            path.push_back(current);
        }
        reverse(path.begin(), path.end());
        for (int city : path) {
            cout << cityNames.at(city) << " ";
        }
        cout << endl;
    }
   void updateBookingStatus(int bookingID, const string& newStatus) {
        auto it = bookings.find(bookingID);
        if (it != bookings.end()) {
            it->second.status = newStatus;
            cout << "Booking ID " << bookingID << " updated to status: " << newStatus << endl;
        } else {
            cout << "Error: Booking ID " << bookingID << " not found." << endl;
        }
    }
    /**
     * @brief Updates the status of a booking.
     *
     * This function searches for a booking by its ID and updates its status
     * if the booking is found. If the booking ID is not found, it prints an error message.
     *
     * @param bookingID The ID of the booking to be updated.
     * @param newStatus The new status to be assigned to the booking.
     */

    void deleteBooking(int bookingID) {
        auto it = bookings.find(bookingID);
        if (it != bookings.end()) {
            bookings.erase(it);
            cout << "Deleted booking. Booking ID: " << bookingID << endl;
        } else {
            cout << "Booking ID not found.\n";
        }
    }
    /**
     * @brief Updates the availability status of a transportation mode.
     *
     * This function searches for a transportation mode by its ID and updates its availability status.
     * If the mode is found, its availability is set to the provided value, and a confirmation message is displayed.
     * If the mode ID is not found, an error message is displayed.
     *
     * @param modeID The ID of the transportation mode to be updated.
     * @param availability The new availability status (true for available, false for not available).
     */
    void updateModeAvailability(int modeID, bool availability) {
        for (auto& mode : modes) {
            if (mode.id == modeID) {
                mode.available = availability;
                cout << "Updated mode availability. Mode ID: " << modeID << ", Available: " << (availability ? "Yes" : "No") << endl;
                return;
            }
        }
        cout << "Mode ID not found.\n";
    }

    /**
     * @brief Displays the routes from a given city.
     *
     * This function takes a city ID and prints all the routes originating from that city.
     * It first checks if the city ID exists in the cityNames map. If the city ID is not found,
     * it prints an error message. If the city ID is found, it prints the city name and all the
     * routes from that city along with the distances in kilometers.
     *
     * @param cityID The ID of the city for which routes are to be displayed.
     */
    void displayCityRoutes(int cityID) const {
        auto it = cityNames.find(cityID);
        if (it == cityNames.end()) {
            cout << "City ID not found.\n";
            return;
        }
        cout << "\n--- Routes from " << it->second << " ---\n";
        for (int i = 0; i < adjMatrix.size(); i++) {
            if (adjMatrix[cityID][i] != 0) {
                cout << "Route to " << cityNames.at(i) << ": " << adjMatrix[cityID][i] << " km\n";
            }
        }
    }

    /**
     * @brief Displays the available modes.
     *
     * This function prints a table header with columns for ID, Name, Capacity, and Availability.
     * It then iterates through the list of modes and displays each mode that is available.
     * If no modes are available, it prints a message indicating that no modes are available.
     *
     * @note This function assumes that the `modes` container and the `display` method of each mode are properly defined.
     */
    void displayAvailableModes() const {
        cout << setw(5) << "ID" << setw(15) << "Name" << setw(10) << "Capacity" << setw(10) << "Available" << endl;
        bool anyAvailable = false;
        for (const auto& mode : modes) {
            if (mode.available) {
                mode.display();
                anyAvailable = true;
            }
        }
        if (!anyAvailable) {
            cout << "No available modes.\n";
        }
    }

    /**
     * @brief Displays all the routes between cities along with their distances.
     *
     * This function iterates through the adjacency matrix to find and display all
     * the routes between cities. It prints the routes in the format:
     * "Route from [City1] to [City2]: [Distance] km".
     *
     * The adjacency matrix (adjMatrix) is assumed to be a square matrix where
     * adjMatrix[i][j] represents the distance between city i and city j. A value
     * of 0 indicates no direct route between the cities. The city names are stored
     * in the vector cityNames, where the index corresponds to the city.
     *
     * @note This function assumes that the adjacency matrix and city names vector
     * are properly initialized and have matching sizes.
     */
    void displayRoutes() const {
        cout << "\n--- Routes ---\n";
        for (int i = 0; i < adjMatrix.size(); i++) {
            for (int j = i + 1; j < adjMatrix.size(); j++) {
                if (adjMatrix[i][j] != 0) {
                    cout << "Route from " << cityNames.at(i) << " to " << cityNames.at(j) << ": " << adjMatrix[i][j] << " km\n";
                }
            }
        }
    }

    /**
     * @brief Counts the total number of bookings.
     *
     * This function checks if there are any bookings available in the system.
     * If there are no bookings, it prints a message indicating that no bookings are available.
     * If there are bookings, it prints the total number of bookings.
     *
     * @note This function does not modify any member variables and is marked as const.
     */
    void countBookings() const {
        if (bookings.empty()) {
            cout << "No bookings available." << endl;
        } else {
            cout << "Total number of bookings: " << bookings.size() << endl;
        }
    }

    /**
     * @brief Finds the longest path from a source city to a destination city in a weighted directed graph.
     *
     * This function uses a modified version of Dijkstra's algorithm to find the longest path from the source city
     * to the destination city. It maintains a priority queue to explore the nodes with the highest distance first.
     *
     * @param src The index of the source city.
     * @param dest The index of the destination city.
     *
     * The function prints the longest distance and the path from the source city to the destination city.
     * If no path is found, it prints a message indicating that no route was found.
     *
     * The function assumes that the adjacency matrix `adjMatrix` and the city names `cityNames` are defined
     * as member variables of the class. The adjacency matrix should represent the weights of the edges between
     * the cities, and the city names should map the indices to the city names.
     */
    void findLongestPath(int src, int dest) const {
        vector<int> dist(adjMatrix.size(), -INF);
        vector<bool> visited(adjMatrix.size(), false);
        vector<int> parent(adjMatrix.size(), -1);

        priority_queue<pair<int, int>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (visited[u]) continue;
            visited[u] = true;

            for (int v = 0; v < adjMatrix.size(); v++) {
                if (adjMatrix[u][v] && !visited[v] && dist[u] + adjMatrix[u][v] > dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[dest] == -INF) {
            cout << "No route found from " << cityNames.at(src) << " to " << cityNames.at(dest) << ".\n";
            return;
        }

        cout << "Longest distance: " << dist[dest] << " km\nPath: ";
        vector<int> path;
        for (int current = dest; current != -1; current = parent[current]) {
            path.push_back(current);
        }
        reverse(path.begin(), path.end());
        for (int city : path) {
            cout << cityNames.at(city) << " ";
        }
        cout << endl;
    }




    /**
     * @brief Displays the details of a city.
     *
     * This function takes a city ID as input and searches for the city in the cityNames map.
     * If the city is found, it prints the city ID and name. If the city is not found, it prints
     * an error message indicating that the city ID was not found.
     *
     * @param cityID The ID of the city to display details for.
     */
    void displayCityDetails(int cityID) const {
        auto it = cityNames.find(cityID); // Search for the city ID in the cityNames map
        if (it != cityNames.end()) { // If the city is found
            cout << "City ID: " << cityID << ", Name: " << it->second << endl; // Print the city ID and name
        } else { // If the city is not found
            cout << "City ID not found.\n"; // Print an error message
        }
    }

    /**
     * @brief Updates the name of a city.
     *
     * This function searches for a city by its ID and updates its name if the city is found.
     * If the city ID is not found, it prints an error message.
     *
     * @param cityID The ID of the city to be updated.
     * @param newName The new name to be assigned to the city.
     */
    void updateCityName(int cityID, const string& newName) {
        auto it = cityNames.find(cityID);
        if (it != cityNames.end()) {
            it->second = newName;
            cout << "Updated city name. City ID: " << cityID << ", New Name: " << newName << endl;
        } else {
            cout << "City ID not found.\n";
        }
    }

    /**
     * @brief Updates the distance of an existing route between two cities.
     *
     * This function updates the distance of a route between the source city (src)
     * and the destination city (dest) in the adjacency matrix. If the route exists,
     * the distance is updated to the newDistance value. The function assumes that
     * the routes are bidirectional, so it updates both adjMatrix[src][dest] and
     * adjMatrix[dest][src].
     *
     * @param src The index of the source city in the adjacency matrix.
     * @param dest The index of the destination city in the adjacency matrix.
     * @param newDistance The new distance to be set for the route.
     *
     * @note If the route does not exist (i.e., adjMatrix[src][dest] is 0), the
     * function will print a message indicating that the route was not found.
     *
     * @example
     * updateRouteDistance(0, 1, 150);
     * // Updates the distance between the city at index 0 and the city at index 1 to 150 km.
     */
    void updateRouteDistance(int src, int dest, int newDistance) {
        if (adjMatrix[src][dest] != 0) {
            adjMatrix[src][dest] = newDistance;
            adjMatrix[dest][src] = newDistance; // Assuming bidirectional routes
            cout << "Updated route distance. From " << cityNames.at(src) << " to " << cityNames.at(dest) << ": " << newDistance << " km\n";
        } else {
            cout << "Route between " << cityNames.at(src) << " and " << cityNames.at(dest) << " not found.\n";
        }
    }

    /**
     * @brief Deletes a city from the system.
     *
     * This function removes a city from the cityNames map and updates the adjacency matrix
     * to reflect the removal of the city. It sets the distances to and from the city to 0,
     * effectively removing all routes associated with the city.
     *
     * @param cityID The ID of the city to be deleted.
     *
     * @note If the city ID is not found in the cityNames map, the function prints an error message.
     */
    void deleteCity(int cityID) {
        if (cityNames.erase(cityID)) { // Remove the city from the cityNames map
            for (auto& row : adjMatrix) {
                row[cityID] = 0; // Set the distances to the city to 0
            }
            adjMatrix[cityID].assign(adjMatrix.size(), 0); // Set the distances from the city to 0
            cout << "Deleted city. City ID: " << cityID << endl; // Print confirmation message
        } else {
            cout << "City ID not found.\n"; // Print error message if city ID is not found
        }
    }

    /**
     * @brief Displays the total number of cities.
     *
     * This function prints the total number of cities currently stored in the cityNames map.
     * It outputs the count to the standard output stream (cout) in the format:
     * "Total number of cities: <count>"
     *
     * @note This function does not modify any member variables and is marked as const.
     */
    void displayTotalCities() const {
        cout << "Total number of cities: " << cityNames.size() << endl;
    }

    /**
     * @brief Displays the total number of routes in the adjacency matrix.
     *
     * This function iterates through the adjacency matrix (adjMatrix) and counts
     * the number of routes (edges) where the distance is greater than zero. Since
     * the routes are bidirectional, the total count is divided by 2 to avoid
     * double-counting.
     *
     * @note The adjacency matrix (adjMatrix) is assumed to be a member variable
     *       of the class and should be a square matrix where each element represents
     *       the distance between nodes.
     */
    void displayTotalRoutes() const {
        int totalRoutes = 0;
        for (const auto& row : adjMatrix) {
            totalRoutes += count_if(row.begin(), row.end(), [](int distance) { return distance > 0; });
        }
        cout << "Total number of routes: " << totalRoutes / 2 << endl; // Dividing by 2 because routes are bidirectional
    }

    /**
     * @brief Displays the total number of transportation modes.
     *
     * This function prints the total number of transportation modes currently stored in the modes vector.
     * It outputs the count to the standard output stream (cout) in the format:
     * "Total number of transportation modes: <count>"
     *
     * @note This function does not modify any member variables and is marked as const.
     */
    void displayTotalModes() const {
        cout << "Total number of transportation modes: " << modes.size() << endl;
    }

    /**
     * @brief Displays the details of a booking based on the provided booking ID.
     *
     * This function searches for the booking ID in the bookings map. If the booking ID is found,
     * it displays the booking details using the display method of the booking object. If the
     * booking ID is not found, it prints a message indicating that the booking ID was not found.
     *
     * @param bookingID The ID of the booking to be displayed.
     */
    void displayBookingDetails(int bookingID) const {
        auto it = bookings.find(bookingID);
        if (it != bookings.end()) {
            it->second.display(cityNames);
        } else {
            cout << "Booking ID not found.\n";
        }
    }


    /**
     * @brief Adds a new city to the system.
     *
     * This function adds a new city to the cityNames map and updates the adjacency matrix
     * to accommodate the new city. If the city ID already exists, it prints an error message.
     *
     * @param id The ID of the new city to be added.
     * @param name The name of the new city to be added.
     *
     * @note The function assumes that the cityNames map and the adjacency matrix (adjMatrix)
     *       are properly initialized and have matching sizes.
     */
    void addNewCity(int id, const string& name) {
        if (cityNames.find(id) == cityNames.end()) { // Check if the city ID does not already exist
            cityNames[id] = name; // Add the new city to the cityNames map
            for (auto& row : adjMatrix) {
                row.push_back(0); // Add a new column for the new city in each row of the adjacency matrix
            }
            adjMatrix.push_back(vector<int>(adjMatrix.size() + 1, 0)); // Add a new row for the new city in the adjacency matrix
            cout << "Added new city. City ID: " << id << ", Name: " << name << endl; // Print confirmation message
        } else {
            cout << "City ID already exists.\n"; // Print error message if the city ID already exists
        }
    }

    /**
     * @brief Adds a new mode of transportation to the system.
     *
     * This function creates a new TransportationMode object with the specified parameters
     * and adds it to the modes vector. It also prints a confirmation message indicating
     * that the new mode has been added.
     *
     * @param id The unique identifier for the new mode.
     * @param name The name of the new mode.
     * @param capacity The capacity of the new mode.
     * @param available The availability status of the new mode (true for available, false for not available).
     *
     * @note The function assumes that the modes vector is properly initialized and accessible.
     */
    void addNewMode(int id, const string& name, int capacity, bool available) {
        modes.emplace_back(id, name, capacity, available);
        cout << "Added new mode. Mode ID: " << id << ", Name: " << name << endl;
    }

    /**
     * @brief Removes an existing route between two cities.
     *
     * This function removes the route between the source city (src) and the destination city (dest)
     * by setting the distance in the adjacency matrix to 0. It assumes that the routes are bidirectional,
     * so it updates both adjMatrix[src][dest] and adjMatrix[dest][src].
     *
     * If the route exists, it prints a confirmation message indicating that the route has been removed.
     * If the route does not exist, it prints an error message.
     *
     * @param src The index of the source city in the adjacency matrix.
     * @param dest The index of the destination city in the adjacency matrix.
     *
     * @note The function assumes that the adjacency matrix (adjMatrix) and the city names (cityNames)
     *       are properly initialized and have matching sizes.
     */
    void removeRoute(int src, int dest) {
        if (adjMatrix[src][dest] != 0) {
            adjMatrix[src][dest] = 0;
            adjMatrix[dest][src] = 0; // Assuming bidirectional routes
            cout << "Removed route between " << cityNames.at(src) << " and " << cityNames.at(dest) << endl;
        } else {
            cout << "Route between " << cityNames.at(src) << " and " << cityNames.at(dest) << " does not exist.\n";
        }
    }

    /**
     * @brief Displays the names of the cities.
     *
     * This function prints the list of city names along with their corresponding IDs.
     * It iterates through the `cityNames` map and outputs each city's ID and name
     * in a formatted manner.
     *
     * @note This function does not modify any member variables and can be called on
     *       a constant instance of the class.
     */
    void displayCityNames() const {
        cout << "\n--- City Names ---\n";
        for (const auto& [id, name] : cityNames) {
            cout << "City ID: " << id << ", Name: " << name << endl;
        }
    }


    /**
     * @brief Displays the details of a transportation mode.
     *
     * This function searches for a transportation mode by its ID and displays its details
     * if the mode is found. The details include the mode ID, name, capacity, and availability status.
     * If the mode ID is not found, it prints an error message.
     *
     * @param modeID The ID of the transportation mode to be displayed.
     */
    void displayModeDetails(int modeID) const {
        auto it = find_if(modes.begin(), modes.end(), [modeID](const TransportationMode& mode) {
            return mode.id == modeID;
        });

        if (it != modes.end()) {
            cout << "Mode ID: " << it->id << "\nName: " << it->name << "\nCapacity: " << it->capacity << "\nAvailable: " << (it->available ? "Yes" : "No") << "\n";
        } else {
            cout << "Mode ID not found.\n";
        }
    }

    /**
     * @brief Updates the capacity of a transportation mode identified by its mode ID.
     *
     * This function searches for a transportation mode with the specified mode ID
     * within the 'modes' container. If found, it updates the capacity of that mode
     * to the new specified capacity and prints a confirmation message. If the mode
     * ID is not found, it prints an error message.
     *
     * @param modeID The ID of the transportation mode to be updated.
     * @param newCapacity The new capacity to be set for the transportation mode.
     */
    void updateModeCapacity(int modeID, int newCapacity) {
        auto it = find_if(modes.begin(), modes.end(), [modeID](const TransportationMode& mode) {
            return mode.id == modeID;
        });

        if (it != modes.end()) {
            it->capacity = newCapacity;
            cout << "Updated mode capacity. Mode ID: " << modeID << ", New Capacity: " << newCapacity << endl;
        } else {
            cout << "Mode ID not found.\n";
        }
    }


    /**
     * @brief Displays the count of cities.
     *
     * This function prints the number of cities stored in the cityNames container
     * to the standard output. It uses the size() method of the cityNames container
     * to determine the count and outputs it using the cout stream.
     *
     * @note This function does not modify any member variables and can be called
     * on a const instance of the class.
     */
    void displayCityCount() const {
        cout << "Number of cities: " << cityNames.size() << endl;
    }

    /**
     * @brief Displays the count of transportation modes.
     *
     * This function outputs the number of transportation modes currently stored
     * in the 'modes' container to the standard output stream. It is a constant
     * member function, meaning it does not modify any member variables of the class.
     */
    void displayModeCount() const {
        cout << "Number of transportation modes: " << modes.size() << endl;
    }

    /**
     * @brief Displays the total number of bookings.
     *
     * This function prints the number of bookings currently stored
     * in the bookings container to the standard output.
     * It is a constant member function, meaning it does not modify
     * any member variables of the class.
     */
    void displayBookingCount() const {
        cout << "Number of bookings: " << bookings.size() << endl;
    }

    /**
     * @brief Displays the number of routes in the adjacency matrix.
     *
     * This function iterates through the upper triangular part of the adjacency matrix
     * to count the number of non-zero entries, which represent the routes between nodes.
     * It then prints the total number of routes to the standard output.
     *
     * @note The adjacency matrix is assumed to be a square matrix where a non-zero entry
     *       at adjMatrix[i][j] indicates a route between node i and node j.
     */
    void displayRouteCount() const {
        int routeCount = 0;
        for (int i = 0; i < adjMatrix.size(); ++i) {
            for (int j = i + 1; j < adjMatrix.size(); ++j) {
                if (adjMatrix[i][j] != 0) {
                    ++routeCount;
                }
            }
        }
        cout << "Number of routes: " << routeCount << endl;
    }



    /**
     * @brief Displays the most connected city.
     *
     * This function iterates through the adjacency matrix to find the city with the most connections.
     * It counts the number of connections for each city and keeps track of the city with the maximum connections.
     * If a city with connections is found, it prints the city name and the number of connections.
     * If no connected cities are found, it prints an appropriate message.
     *
     * @note The adjacency matrix (adjMatrix) is assumed to be a member variable of the class and should be a square matrix.
     *       The city names (cityNames) are stored in a map where the key is the city ID and the value is the city name.
     */
    void displayMostConnectedCity() const {
        int maxConnections = 0;
        int mostConnectedCity = -1;
        for (int i = 0; i < adjMatrix.size(); ++i) {
            int connections = count_if(adjMatrix[i].begin(), adjMatrix[i].end(), [](int distance) { return distance > 0; });
            if (connections > maxConnections) {
                maxConnections = connections;
                mostConnectedCity = i;
            }
        }
        if (mostConnectedCity != -1) {
            cout << "Most connected city: " << cityNames.at(mostConnectedCity) << " with " << maxConnections << " connections.\n";
        } else {
            cout << "No connected cities found.\n";
        }
    }

    /**
     * @brief Displays the least connected city.
     *
     * This function iterates through the adjacency matrix to find the city with the least number of connections.
     * It counts the number of connections for each city and keeps track of the city with the minimum connections.
     * If a city with connections is found, it prints the city name and the number of connections.
     * If no connected cities are found, it prints an appropriate message.
     *
     * @note The adjacency matrix (adjMatrix) is assumed to be a member variable of the class and should be a square matrix.
     *       The city names (cityNames) are stored in a map where the key is the city ID and the value is the city name.
     */
    void displayLeastConnectedCity() const {
        int minConnections = INT_MAX; // Initialize the minimum connections to the maximum possible integer value
        int leastConnectedCity = -1; // Initialize the least connected city to an invalid value

        // Iterate through each city in the adjacency matrix
        for (int i = 0; i < adjMatrix.size(); ++i) {
            // Count the number of connections for the current city
            int connections = count_if(adjMatrix[i].begin(), adjMatrix[i].end(), [](int distance) { return distance > 0; });

            // Update the least connected city if the current city has fewer connections and is connected to at least one other city
            if (connections < minConnections && connections > 0) {
                minConnections = connections;
                leastConnectedCity = i;
            }
        }

        // Check if a least connected city was found
        if (leastConnectedCity != -1) {
            // Print the least connected city and the number of connections
            cout << "Least connected city: " << cityNames.at(leastConnectedCity) << " with " << minConnections << " connections.\n";
        } else {
            // Print a message indicating that no connected cities were found
            cout << "No connected cities found.\n";
        }
    }

    /**
     * @brief Finds and displays all paths from the source city to the destination city.
     *
     * This function initializes the visited vector and path vector, and then calls the
     * utility function findAllPathsUtil to recursively find and display all paths from
     * the source city to the destination city. It prints each path as a sequence of city names.
     *
     * @param src The source city ID.
     * @param dest The destination city ID.
     */
    void findAllPaths(int src, int dest) const {
        vector<bool> visited(adjMatrix.size(), false); // Vector to keep track of visited cities
        vector<int> path; // Vector to store the current path
        cout << "All paths from " << cityNames.at(src) << " to " << cityNames.at(dest) << ":\n";
        findAllPathsUtil(src, dest, visited, path); // Call the utility function to find all paths
    }

    /**
     * @brief Utility function to find all paths from the source city to the destination city.
     *
     * This function is a recursive utility that finds all paths from the source city (src) to the
     * destination city (dest). It uses a depth-first search (DFS) approach to explore all possible
     * paths. The function maintains a visited vector to keep track of visited cities and a path
     * vector to store the current path.
     *
     * @param src The source city ID.
     * @param dest The destination city ID.
     * @param visited A reference to a vector of booleans indicating whether a city has been visited.
     * @param path A reference to a vector of integers representing the current path.
     */
    void findAllPathsUtil(int src, int dest, vector<bool>& visited, vector<int>& path) const {
        visited[src] = true; // Mark the current city as visited
        path.push_back(src); // Add the current city to the path

        // If the current city is the destination, print the path
        if (src == dest) {
            for (int city : path) {
                cout << cityNames.at(city) << " ";
            }
            cout << endl;
        } else {
            // Recur for all the cities adjacent to the current city
            for (int i = 0; i < adjMatrix.size(); ++i) {
                if (adjMatrix[src][i] != 0 && !visited[i]) {
                    findAllPathsUtil(i, dest, visited, path);
                }
            }
        }

        path.pop_back(); // Remove the current city from the path
        visited[src] = false; // Mark the current city as unvisited
    }

    /**
     * @brief Finds the cheapest path between two cities using Dijkstra's algorithm.
     *
     * This function calculates the cheapest path from the source city to the destination city
     * using Dijkstra's algorithm. It maintains a priority queue to explore the shortest distances
     * and updates the distance and parent vectors accordingly. If a path is found, it prints the
     * cheapest distance and the path. If no path is found, it notifies the user.
     *
     * @param src The source city ID.
     * @param dest The destination city ID.
     */
    void findCheapestPath(int src, int dest) const {
        vector<int> dist(adjMatrix.size(), INF); // Distance vector initialized to infinity
        vector<int> parent(adjMatrix.size(), -1); // Parent vector to reconstruct the path
        vector<bool> visited(adjMatrix.size(), false); // Visited vector to keep track of visited nodes

        // Priority queue to store (distance, node) pairs, with the smallest distance at the top
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, src}); // Start with the source node
        dist[src] = 0; // Distance to the source node is 0

        while (!pq.empty()) {
            int u = pq.top().second; // Get the node with the smallest distance
            pq.pop();

            if (visited[u]) continue; // Skip if the node is already visited
            visited[u] = true; // Mark the node as visited

            // Explore all adjacent nodes
            for (int v = 0; v < adjMatrix.size(); ++v) {
                // If there is an edge and the node is not visited, and a shorter path is found
                if (adjMatrix[u][v] && !visited[v] && dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v]; // Update the distance
                    parent[v] = u; // Update the parent
                    pq.push({dist[v], v}); // Push the updated distance and node to the priority queue
                }
            }
        }

        // If the destination node is unreachable
        if (dist[dest] == INF) {
            cout << "No route found from " << cityNames.at(src) << " to " << cityNames.at(dest) << ".\n";
        } else {
            // Print the cheapest distance and path
            cout << "Cheapest distance: " << dist[dest] << " km\nPath: ";
            vector<int> path;
            for (int current = dest; current != -1; current = parent[current]) {
                path.push_back(current); // Reconstruct the path from destination to source
            }
            reverse(path.begin(), path.end()); // Reverse the path to get the correct order
            for (int city : path) {
                cout << cityNames.at(city) << " "; // Print the city names in the path
            }
            cout << endl;
        }
    }


    /**
     * @brief Displays the isolated cities.
     *
     * This function iterates through the cityNames map and checks each city to determine if it is isolated.
     * A city is considered isolated if it has no direct routes to any other city, which is determined by
     * checking if all distances in the adjacency matrix row for that city are zero. If isolated cities are found,
     * their IDs and names are printed. If no isolated cities are found, a message is printed indicating this.
     *
     * @note The adjacency matrix (adjMatrix) and city names (cityNames) are assumed to be member variables of the class.
     */
    void displayIsolatedCities() const {
        cout << "\n--- Isolated Cities ---\n";
        bool found = false;
        for (const auto& [id, name] : cityNames) {
            bool isolated = all_of(adjMatrix[id].begin(), adjMatrix[id].end(), [](int distance) { return distance == 0; });
            if (isolated) {
                cout << "City ID: " << id << ", Name: " << name << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No isolated cities found.\n";
        }
    }

    /**
     * @brief Displays the direct routes from a given city.
     *
     * This function takes a city ID and prints all the direct routes from that city
     * to other cities. If the city ID is not found in the cityNames map, it will
     * print an error message. If there are no direct routes from the given city,
     * it will indicate that no direct routes were found.
     *
     * @param cityID The ID of the city for which to display direct routes.
     *
     * @note The function assumes that cityNames is a map that associates city IDs
     *       with city names, and adjMatrix is an adjacency matrix representing the
     *       distances between cities.
     *
     * @warning Ensure that the cityID provided is valid and exists in the cityNames
     *          map to avoid incorrect outputs.
     */
    void displayDirectRoutes(int cityID) const {
        auto it = cityNames.find(cityID);
        if (it == cityNames.end()) {
            cout << "City ID not found.\n";
            return;
        }
        cout << "\n--- Direct Routes from " << it->second << " ---\n";
        bool found = false;
        for (int i = 0; i < adjMatrix.size(); ++i) {
            if (adjMatrix[cityID][i] != 0) {
                cout << "Route to " << cityNames.at(i) << ": " << adjMatrix[cityID][i] << " km\n";
                found = true;
            }
        }
        if (!found) {
            cout << "No direct routes found.\n";
        }
    }

    /**
     * @brief Displays the cities connected by a specific mode of transport.
     *
     * This function takes the name of a mode of transport and prints all the cities
     * that are connected by that mode. It first searches for the mode in the modes
     * vector. If the mode is found, it iterates through the adjacency matrix to find
     * all routes and prints the cities connected by the specified mode. If the mode
     * is not found, it prints an error message. If no cities are connected by the
     * specified mode, it prints a message indicating this.
     *
     * @param modeName The name of the mode of transport to search for.
     */
    void displayCitiesByMode(const string& modeName) const {
        cout << "\n--- Cities connected by " << modeName << " ---\n";

        // Search for the mode in the modes vector
        auto it = find_if(modes.begin(), modes.end(), [&](const TransportationMode& mode) {
            return mode.name == modeName;
        });

        // If the mode is not found, print an error message
        if (it == modes.end()) {
            cout << "Mode of transport not found.\n";
            return;
        }

        bool found = false;
        // Iterate through the adjacency matrix to find all routes
        for (int i = 0; i < adjMatrix.size(); ++i) {
            for (int j = i + 1; j < adjMatrix.size(); ++j) {
                if (adjMatrix[i][j] != 0) {
                    cout << cityNames.at(i) << " - " << cityNames.at(j) << endl;
                    found = true;
                }
            }
        }

        // If no cities are connected by the specified mode, print a message
        if (!found) {
            cout << "No cities connected by " << modeName << ".\n";
        }
    }

    /**
     * @brief Retrieves the city ID based on the provided city name.
     *
     * This function searches for a city by its name in the cityNames map.
     * If the city is found, it prints the city ID. If the city name is not found,
     * it prints an error message indicating that the city name was not found.
     *
     * @param cityName The name of the city to search for.
     */
    void getCityIDByName(const string& cityName) const {
        // Search for the city name in the cityNames map
        auto it = find_if(cityNames.begin(), cityNames.end(), [&](const pair<int, string>& city) {
            return city.second == cityName;
        });

        // If the city is found, print the city ID
        if (it != cityNames.end()) {
            cout << "City ID for " << cityName << " is " << it->first << endl;
        } else {
            // If the city name is not found, print an error message
            cout << "City name not found.\n";
        }
    }



    /**
     * @brief Displays the city with the longest route.
     *
     * This function iterates through the adjacency matrix (adjMatrix) to find the longest route between any two cities.
     * It keeps track of the longest route found and the corresponding city index.
     * If a route is found, it prints the name of the city with the longest route and the length of that route.
     * If no routes are found, it prints a message indicating that no routes were found.
     *
     * @note This function assumes that adjMatrix is a square matrix and that cityNames contains the names of the cities
     *       corresponding to the indices in adjMatrix.
     */
    void displayCityWithLongestRoute() const {
        int longestRoute = 0;
        int cityWithLongestRoute = -1;
        for (int i = 0; i < adjMatrix.size(); ++i) {
            for (int j = 0; j < adjMatrix.size(); ++j) {
                if (adjMatrix[i][j] > longestRoute) {
                    longestRoute = adjMatrix[i][j];
                    cityWithLongestRoute = i;
                }
            }
        }
        if (cityWithLongestRoute != -1) {
            cout << "City with the longest route: " << cityNames.at(cityWithLongestRoute) << " with a route of " << longestRoute << " km.\n";
        } else {
            cout << "No routes found.\n";
        }
    }



};

/**
 * @brief Main function for the Transportation Management System.
 *
 * This function initializes the transportation system with cities, routes, and modes of transport.
 * It then presents a menu to the user to perform various operations such as viewing cities,
 * booking tickets, finding paths, and updating information.
 *
 * @return int Returns 0 upon successful execution.
 */
/**
 * Main function for the Transportation Management System.
 *
 * This function initializes the system with cities, routes, and transportation modes.
 * It then displays a menu for the user to interact with the system, allowing various operations
 * such as viewing cities, booking tickets, finding paths, and updating information.
 *
 * The menu options are:
 * 1. View Cities - Displays all cities in the system.
 * 2. View Transportation Modes - Displays all available transportation modes.
 * 3. Add Booking - Allows the user to book a ticket between two cities using a specified mode of transport.
 * 4. View Bookings - Displays all current bookings.
 * 5. Find Shortest Path - Finds the shortest path between two cities.
 * 6. Update Booking Status - Updates the status of a booking.
 * 7. Delete Booking - Deletes a booking by its ID.
 * 8. Update Mode Availability - Updates the availability status of a transportation mode.
 * 9. View Routes - Displays all routes between cities.
 * 10. Display City Routes - Displays all routes for a specific city.
 * 11. Display Available Modes - Displays all currently available transportation modes.
 * 12. Count Bookings - Displays the total number of bookings.
 * 13. Find Longest Path - Finds the longest path between two cities.
 * 14. Display City Details - Displays detailed information for a specific city.
 * 15. Update City Name - Updates the name of a city.
 * 16. Update Route Distance - Updates the distance of a route between two cities.
 * 17. Delete City - Deletes a city from the system.
 * 18. Display Total Cities - Displays the total number of cities.
 * 19. Display Total Routes - Displays the total number of routes.
 * 20. Display Total Modes - Displays the total number of transportation modes.
 * 21. Display Booking Details - Displays detailed information for a specific booking.
 * 22. Add New City - Adds a new city to the system.
 * 23. Add New Mode - Adds a new transportation mode to the system.
 * 24. Remove Route - Removes a route between two cities.
 * 25. Display City Names - Displays the names of all cities.
 * 26. Display Mode Details - Displays detailed information for a specific transportation mode.
 * 27. Update Mode Capacity - Updates the capacity of a transportation mode.
 * 28. Display City Count - Displays the total number of cities.
 * 29. Display Mode Count - Displays the total number of transportation modes.
 * 30. Display Booking Count - Displays the total number of bookings.
 * 31. Display Route Count - Displays the total number of routes.
 * 32. Display Most Connected City - Displays the city with the most connections.
 * 33. Display Least Connected City - Displays the city with the least connections.
 * 34. Find All Paths - Finds all possible paths between two cities.
 * 35. Find Cheapest Path - Finds the cheapest path between two cities.
 * 36. Display Isolated Cities - Displays cities with no connections.
 * 37. Display Direct Routes - Displays direct routes for a specific city.
 * 38. Display Cities by Mode - Displays cities that can be reached using a specific mode of transport.
 * 39. Get City ID by Name - Retrieves the ID of a city by its name.
 * 40. Display City with Longest Route - Displays the city with the longest route.
 * 41. Exit - Exits the program.
 */
using namespace std;

// Struct to store location data
struct Location {
    string name;
    int connectivityStatus; // 1: Good, 2: Medium, 3: Bad
    bool hasJammer;         // true if location has a jammer, false otherwise
    bool nearMilitaryBase;  // true if location is near a military base, false otherwise
    string nearestMetroStation; // Nearest metro station name
};
// 1. Simulate node bootup
void simulateNodeBootup(int nodeID) {
    cout << "Simulating bootup process for node " << nodeID << "." << endl;
}

// 2. Validate network configuration
void validateNetworkConfiguration() {
    cout << "Validating network configuration for errors." << endl;
}

// 3. Simulate packet retransmission
void simulatePacketRetransmission(int packetID) {
    cout << "Simulating retransmission for packet " << packetID << "." << endl;
}

// 4. Calculate average network speed
double calculateAverageSpeed(int totalData, double totalTime) {
    cout << "Calculating average network speed." << endl;
    return totalData / totalTime; // Dummy calculation
}

// 5. Identify isolated nodes
void identifyIsolatedNodes() {
    cout << "Identifying isolated nodes in the network." << endl;
}

// 6. Perform redundancy checks
void performRedundancyChecks() {
    cout << "Performing redundancy checks in the network." << endl;
}

// 7. Generate connectivity matrix
void generateConnectivityMatrix() {
    cout << "Generating connectivity matrix for the network." << endl;
}

// 8. Predict network load
void predictNetworkLoad() {
    cout << "Predicting future network load." << endl;
}

// 9. Monitor device health
void monitorDeviceHealth(int deviceID) {
    cout << "Monitoring health of device " << deviceID << "." << endl;
}

// 10. Simulate power failure
void simulatePowerFailure(int regionID) {
    cout << "Simulating power failure in region " << regionID << "." << endl;
}

// 11. Update node firmware
void updateNodeFirmware(int nodeID) {
    cout << "Updating firmware for node " << nodeID << "." << endl;
}

// 12. Calculate signal-to-noise ratio
double calculateSignalToNoiseRatio(double signal, double noise) {
    cout << "Calculating signal-to-noise ratio." << endl;
    return 10 * log10(signal / noise); // Dummy calculation
}

// 13. Generate node heatmap
void generateNodeHeatmap() {
    cout << "Generating heatmap for node activity." << endl;
}

// 14. Simulate high-priority traffic
void simulateHighPriorityTraffic() {
    cout << "Simulating high-priority traffic flow." << endl;
}

// 15. Analyze downtime patterns
void analyzeDowntimePatterns() {
    cout << "Analyzing downtime patterns for nodes and regions." << endl;
}

// 16. Estimate signal range
double estimateSignalRange(double power, double frequency) {
    cout << "Estimating signal range based on power and frequency." << endl;
    return sqrt(power) * frequency; // Dummy calculation
}

// 17. Simulate new device onboarding
void simulateDeviceOnboarding(int deviceID) {
    cout << "Simulating onboarding process for device " << deviceID << "." << endl;
}

// 18. Generate node failure report
void generateNodeFailureReport() {
    cout << "Generating report for node failures." << endl;
}

// 19. Evaluate load distribution
void evaluateLoadDistribution() {
    cout << "Evaluating distribution of load across nodes." << endl;
}

// 20. Test network scalability
void testNetworkScalability() {
    cout << "Testing network scalability under increased load." << endl;
}
// 1. Add a connection between nodes
void addConnection(int node1, int node2) {
    cout << "Adding connection between node " << node1 << " and node " << node2 << "." << endl;
}

// 2. Remove a connection
void removeConnection(int node1, int node2) {
    cout << "Removing connection between node " << node1 << " and node " << node2 << "." << endl;
}

// 3. Calculate network density
double calculateNetworkDensity(int totalNodes, int totalEdges) {
    cout << "Calculating network density." << endl;
    return static_cast<double>(totalEdges) / (totalNodes * (totalNodes - 1) / 2);
}

// 4. Perform network-wide broadcast
void broadcastMessage(const string &message) {
    cout << "Broadcasting message: " << message << endl;
}

// 5. Monitor packet loss
void monitorPacketLoss() {
    cout << "Monitoring packet loss across the network." << endl;
}

// 6. Find a minimum spanning tree
void findMinimumSpanningTree() {
    cout << "Finding the minimum spanning tree using Kruskal's or Prim's algorithm." << endl;
}

// 7. Simulate data transfer rate
void simulateDataTransferRate(int nodeID) {
    cout << "Simulating data transfer rate for node " << nodeID << "." << endl;
}

// 8. Identify high-traffic nodes
void identifyHighTrafficNodes() {
    cout << "Identifying high-traffic nodes." << endl;
}

// 9. Check node availability
bool checkNodeAvailability(int nodeID) {
    cout << "Checking availability of node " << nodeID << "." << endl;
    return true; // Dummy result
}

// 10. Perform round-robin scheduling
void performRoundRobinScheduling() {
    cout << "Performing round-robin scheduling for tasks." << endl;
}

// 11. Simulate signal degradation
void simulateSignalDegradation() {
    cout << "Simulating signal degradation over distance." << endl;
}

// 12. Update network topology
void updateNetworkTopology() {
    cout << "Updating network topology based on changes." << endl;
}

// 13. Measure bandwidth usage
void measureBandwidthUsage() {
    cout << "Measuring bandwidth usage across the network." << endl;
}

// 14. Analyze historical traffic data
void analyzeHistoricalTrafficData() {
    cout << "Analyzing historical traffic data for patterns." << endl;
}

// 15. Simulate a network outage
void simulateNetworkOutage() {
    cout << "Simulating network outage in a region." << endl;
}

// 16. Repair broken connections
void repairBrokenConnections() {
    cout << "Repairing broken connections." << endl;
}

// 17. Map network coverage
void mapNetworkCoverage() {
    cout << "Mapping network coverage for analysis." << endl;
}

// 18. Simulate multi-hop communication
void simulateMultiHopCommunication(int sourceNode, int destinationNode) {
    cout << "Simulating multi-hop communication from " << sourceNode << " to " << destinationNode << "." << endl;
}

// 19. Monitor real-time traffic
void monitorRealTimeTraffic() {
    cout << "Monitoring real-time traffic across the network." << endl;
}

// 20. Evaluate fault tolerance
void evaluateFaultTolerance() {
    cout << "Evaluating network fault tolerance." << endl;
}

// 21. Perform dynamic routing
void performDynamicRouting() {
    cout << "Performing dynamic routing based on real-time conditions." << endl;
}

// 22. Optimize resource allocation
void optimizeResourceAllocation() {
    cout << "Optimizing resource allocation across nodes." << endl;
}

// 23. Simulate wireless signal interference
void simulateSignalInterference() {
    cout << "Simulating wireless signal interference." << endl;
}

// 24. Perform congestion control
void performCongestionControl() {
    cout << "Performing congestion control in high-traffic areas." << endl;
}

// 25. Check load balancing status
void checkLoadBalancingStatus() {
    cout << "Checking load balancing status." << endl;
}

// 26. Log network activity
void logNetworkActivity() {
    cout << "Logging network activity for future analysis." << endl;
}

// 27. Simulate server downtime
void simulateServerDowntime(int serverID) {
    cout << "Simulating downtime for server " << serverID << "." << endl;
}

// 28. Detect malicious activity
void detectMaliciousActivity() {
    cout << "Detecting malicious activity in the network." << endl;
}

// 29. Monitor QoS (Quality of Service)
void monitorQoS() {
    cout << "Monitoring Quality of Service parameters." << endl;
}

// 30. Identify critical nodes
void identifyCriticalNodes() {
    cout << "Identifying critical nodes in the network graph." << endl;
}

// 31. Perform hierarchical clustering
void performHierarchicalClustering() {
    cout << "Performing hierarchical clustering of network nodes." << endl;
}

// 32. Optimize energy consumption
void optimizeEnergyConsumption() {
    cout << "Optimizing energy consumption across the network." << endl;
}

// 33. Simulate network upgrade
void simulateNetworkUpgrade() {
    cout << "Simulating a network upgrade scenario." << endl;
}

// 34. Monitor geographic coverage
void monitorGeographicCoverage() {
    cout << "Monitoring geographic network coverage." << endl;
}

// 35. Predict node failure
void predictNodeFailure() {
    cout << "Predicting potential node failures." << endl;
}

// 36. Perform depth-limited search
void performDepthLimitedSearch(int startNode, int depthLimit) {
    cout << "Performing depth-limited search starting from node " << startNode << " with depth limit " << depthLimit << "." << endl;
}

// 37. Detect redundant paths
void detectRedundantPaths() {
    cout << "Detecting redundant paths in the network." << endl;
}

// 38. Configure network settings
void configureNetworkSettings() {
    cout << "Configuring network settings for optimization." << endl;
}

// 39. Simulate latency spikes
void simulateLatencySpikes() {
    cout << "Simulating latency spikes in the network." << endl;
}

// 40. Generate traffic heatmap
void generateTrafficHeatmap() {
    cout << "Generating a heatmap of network traffic." << endl;
}

// 41. Perform node clustering
void performNodeClustering() {
    cout << "Performing clustering of network nodes." << endl;
}

// 42. Monitor energy usage
void monitorEnergyUsage() {
    cout << "Monitoring energy usage of network components." << endl;
}

// 43. Analyze packet flow
void analyzePacketFlow() {
    cout << "Analyzing the flow of packets through the network." << endl;
}

// 44. Perform static routing
void performStaticRouting() {
    cout << "Performing static routing for fixed paths." << endl;
}

// 45. Detect network anomalies
void detectNetworkAnomalies() {
    cout << "Detecting anomalies in the network." << endl;
}

// 46. Simulate network scalability
void simulateNetworkScalability() {
    cout << "Simulating scalability for larger networks." << endl;
}

// 47. Detect network loops
void detectNetworkLoops() {
    cout << "Detecting loops in the network topology." << endl;
}

// 48. Map latency across regions
void mapLatencyAcrossRegions() {
    cout << "Mapping latency across network regions." << endl;
}

// 49. Simulate edge device behavior
void simulateEdgeDeviceBehavior() {
    cout << "Simulating behavior of edge devices in the network." << endl;
}

// 50. Manage network session
void manageNetworkSession() {
    cout << "Managing a network session for user interactions." << endl;
}

// Function to display connectivity status
void displayStatus(const string& locationName, int status) {
    cout << "Connectivity status for " << locationName << ": ";
    if (status == 1) {
        cout << "Good" << endl;
    } else if (status == 2) {
        cout << "Medium" << endl;
    } else if (status == 3) {
        cout << "Bad" << endl;
    } else {
        cout << "Unknown" << endl;
    }
}
// 1. Add a network node
void addNetworkNode(int nodeID, const string &location) {
    cout << "Adding node " << nodeID << " at location " << location << "." << endl;
}

// 2. Remove a network node
void removeNetworkNode(int nodeID) {
    cout << "Removing node " << nodeID << "." << endl;
}

// 3. Calculate distance between two nodes
double calculateDistance(int node1, int node2) {
    cout << "Calculating distance between nodes " << node1 << " and " << node2 << "." << endl;
    return sqrt(node1 * node2); // Dummy calculation
}

// 4. Simulate a network scan
vector<int> scanForConnections(int nodeID) {
    cout << "Scanning for connections near node " << nodeID << "." << endl;
    return {1, 2, 3}; // Dummy nearby nodes
}

// 5. Allocate resources to a region
void allocateResources(int regionID, double bandwidth) {
    cout << "Allocating " << bandwidth << " Mbps to region " << regionID << "." << endl;
}

// 6. Monitor signal strength
void monitorSignalStrength(int towerID) {
    cout << "Monitoring signal strength for tower " << towerID << "." << endl;
}

// 7. Perform DFS on a network graph
void performDFS(int startNode, const vector<vector<int>> &graph) {
    cout << "Performing DFS starting from node " << startNode << "." << endl;
}

// 8. Perform BFS on a network graph
void performBFS(int startNode, const vector<vector<int>> &graph) {
    cout << "Performing BFS starting from node " << startNode << "." << endl;
}

// 9. Optimize tower placement
void optimizeTowerPlacement() {
    cout << "Optimizing tower placement for maximum coverage." << endl;
}

// 10. Update routing table
void updateRoutingTable(int nodeID) {
    cout << "Updating routing table for node " << nodeID << "." << endl;
}

// 11. Simulate bandwidth distribution
void distributeBandwidth(int nodeID, double bandwidth) {
    cout << "Distributing " << bandwidth << " Mbps to node " << nodeID << "." << endl;
}

// 12. Evaluate network latency
double evaluateLatency(int node1, int node2) {
    cout << "Evaluating latency between nodes " << node1 << " and " << node2 << "." << endl;
    return rand() % 50; // Dummy latency
}

// 13. Find shortest path using Dijkstra�s algorithm
void findShortestPath(int startNode, int endNode) {
    cout << "Finding shortest path from node " << startNode << " to node " << endNode << " using Dijkstra's algorithm." << endl;
}

// 14. Check network connectivity
bool isNetworkConnected(int regionID) {
    cout << "Checking network connectivity for region " << regionID << "." << endl;
    return true; // Dummy result
}

// 15. Perform load balancing
void performLoadBalancing() {
    cout << "Performing load balancing across the network." << endl;
}

// 16. Simulate packet transmission
void simulatePacketTransmission(int sourceNode, int destinationNode) {
    cout << "Simulating packet transmission from node " << sourceNode << " to node " << destinationNode << "." << endl;
}

// 17. Analyze network traffic
void analyzeTraffic(int nodeID) {
    cout << "Analyzing network traffic at node " << nodeID << "." << endl;
}

// 18. Detect bottlenecks in the network
void detectBottlenecks() {
    cout << "Detecting bottlenecks in the network." << endl;
}

// 19. Predict future network demand
void predictNetworkDemand() {
    cout << "Predicting future network demand." << endl;
}

// 20. Generate network report
void generateNetworkReport() {
    cout << "Generating network report for analysis." << endl;
}
// Function to simulate the addition of a new tower
void addTower(int towerID, double latitude, double longitude) {
    cout << "Adding tower with ID " << towerID << " at coordinates (" << latitude << ", " << longitude << ")." << endl;
}

// Function to calculate signal strength between two nodes
double calculateSignalStrength(int node1, int node2) {
    cout << "Calculating signal strength between nodes " << node1 << " and " << node2 << "." << endl;
    return rand() % 100; // Dummy signal strength
}

// Function to simulate a connection test between two nodes
bool testConnection(int node1, int node2) {
    cout << "Testing connection between nodes " << node1 << " and " << node2 << "." << endl;
    return (node1 + node2) % 2 == 0; // Dummy connection result
}

// Function to allocate bandwidth to a tower
void allocateBandwidth(int towerID, double bandwidth) {
    cout << "Allocating " << bandwidth << " Mbps bandwidth to tower " << towerID << "." << endl;
}

// Function to simulate identifying isolated regions
vector<int> identifyIsolatedRegions() {
    cout << "Identifying isolated regions." << endl;
    return {1, 3, 7}; // Dummy isolated region IDs
}

// Function to find the optimal network layout using Kruskal's algorithm
void optimizeNetworkLayout() {
    cout << "Running Kruskal's algorithm for optimal network layout." << endl;
    // Placeholder for graph processing
}

// Function to expand network coverage to underserved areas
void expandCoverage() {
    cout << "Expanding coverage to underserved areas." << endl;
    // Placeholder for coverage logic
}

// Function to update connection priority in a priority queue (min-heap)
void updateConnectionPriority(priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> &pq, int node, int priority) {
    cout << "Updating priority for node " << node << " to " << priority << "." << endl;
    // Since C++ priority queues do not allow direct updates, you'd need to recreate the queue.
    vector<pair<int, int>> temp;

    // Transfer elements to a temporary container
    while (!pq.empty()) {
        temp.push_back(pq.top());
        pq.pop();
    }

    // Update priority or add a new node
    temp.push_back({priority, node});

    // Rebuild the priority queue
    for (auto &item : temp) {
        pq.push(item);
    }
}

// Function to simulate monitoring of connected components using Union-Find
void monitorConnectedComponents() {
    cout << "Monitoring connected components using Union-Find." << endl;
    // Placeholder for Union-Find operations
}

// Function to generate a random connectivity status
int generateRandomConnectivityStatus() {
    return rand() % 3 + 1; // Generates a random number between 1 and 3
}
// Function to simulate the activation of a jammer at a location
void activateJammer(Location& location) {
    location.hasJammer = true;
    cout << location.name << " now has a jammer activated." << endl;
}
void performNetworkSecurityCheck(const vector<Location>& locations) {
    cout << "Performing network security check..." << endl;

    // Simulate security checks (e.g., firewall status, intrusion detection)
    for (const auto& location : locations) {
        cout << "Checking security for location " << location.name << "..." << endl;
        // If location has poor connectivity, assume higher vulnerability
        if (location.connectivityStatus == 3) {
            cout << "Warning: Location " << location.name << " is vulnerable to security breaches due to poor connectivity." << endl;
            // Simulate remedial action (e.g., strengthen firewall, implement encryption)
            cout << "Implementing stronger security measures for location " << location.name << "..." << endl;
        } else {
            cout << "Location " << location.name << " has adequate security." << endl;
        }
    }
}

void handleNetworkFailureRecovery(vector<Location>& locations) {
    cout << "Detecting network failures and recovering..." << endl;

    // Simulate failure detection and recovery
    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {  // Poor connectivity indicates potential failure
            cout << "Network failure detected at " << location.name << ". Attempting recovery..." << endl;
            // Reroute traffic to backup network, boost signal, or activate backup resources
            cout << "Switching to backup network and deploying additional resources for location " << location.name << "..." << endl;
            location.connectivityStatus = 2; // Simulate recovery to medium connectivity
        }
        displayStatus(location.name, location.connectivityStatus);
    }
}


// Function to simulate the construction of a military base near a location
void constructMilitaryBase(Location& location) {
    location.nearMilitaryBase = true;
    cout << location.name << " is now near a newly constructed military base." << endl;
}
 // For sorting and other algorithms

// Function to automatically detect network faults and trigger recovery actions
void detectAndHandleNetworkFaults(vector<Location>& locations) {
    cout << "Automatically detecting and handling network faults..." << endl;

    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {
            // Critical fault detected, initiate recovery
            cout << "Fault detected at location " << location.name << ". Initiating fault recovery protocols." << endl;
            cout << "Triggering failover to backup network..." << endl;
            // Switch to backup network
            location.connectivityStatus = 2; // Attempt to resolve by switching to a backup
        } else if (location.connectivityStatus == 2) {
            // Medium connectivity: Monitor for possible issues
            cout << "Monitoring network stability at location " << location.name << "..." << endl;
        } else {
            // Optimal condition: No issues detected
            cout << "Location " << location.name << " is stable. No issues detected." << endl;
        }

        displayStatus(location.name, location.connectivityStatus);
    }
}


// Function to manage multi-tier network infrastructure (e.g., local, regional, global networks)
void manageMultiTierNetwork(vector<Location>& locations) {
    cout << "Managing multi-tier network infrastructure..." << endl;

    // Group locations by tiers (local, regional, global)
    vector<Location> localTier, regionalTier, globalTier;

    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {
            // Poor connectivity, critical locations: Global Tier
            globalTier.push_back(location);
        } else if (location.connectivityStatus == 2) {
            // Medium connectivity, important locations: Regional Tier
            regionalTier.push_back(location);
        } else {
            // Good connectivity: Local Tier
            localTier.push_back(location);
        }
    }

    cout << "Local Tier (" << localTier.size() << " locations):" << endl;
    for (auto& location : localTier) {
        displayStatus(location.name, location.connectivityStatus);
    }

    cout << "Regional Tier (" << regionalTier.size() << " locations):" << endl;
    for (auto& location : regionalTier) {
        displayStatus(location.name, location.connectivityStatus);
    }

    cout << "Global Tier (" << globalTier.size() << " locations):" << endl;
    for (auto& location : globalTier) {
        displayStatus(location.name, location.connectivityStatus);
    }
}

// Function to integrate machine learning (ML) for predictive analytics (simplified approach)
void integrateMachineLearningForPrediction(vector<Location>& locations, const vector<vector<int>>& historicalData) {
    cout << "Integrating Machine Learning (ML) for predictive analytics..." << endl;

    for (size_t i = 0; i < locations.size(); ++i) {
        // A basic approach to predicting future status based on historical trends
        int lastStatus = historicalData[i].back();
        int trend = 0;

        // Simple trend calculation: Check if the connectivity status has been improving or degrading
        for (size_t j = 1; j < historicalData[i].size(); ++j) {
            if (historicalData[i][j] < historicalData[i][j - 1]) {
                trend--; // Decreasing connectivity
            } else if (historicalData[i][j] > historicalData[i][j - 1]) {
                trend++; // Improving connectivity
            }
        }

        // Simple prediction based on trend: if the trend is negative, predict issues
        if (trend < 0) {
            cout << "Location " << locations[i].name << " predicted to experience connectivity issues." << endl;
            locations[i].connectivityStatus = 3; // Predict and mark for potential failure
        } else {
            cout << "Location " << locations[i].name << " predicted to maintain stable connectivity." << endl;
            locations[i].connectivityStatus = 1; // Predict stable or good connectivity
        }

        displayStatus(locations[i].name, locations[i].connectivityStatus);
    }
}

// Function to generate advanced network reports
void generateAdvancedNetworkReports(const vector<Location>& locations) {
    cout << "Generating advanced network reports..." << endl;

    int totalLocations = locations.size();
    int poorConnectivityCount = 0, mediumConnectivityCount = 0, goodConnectivityCount = 0;

    for (const auto& location : locations) {
        if (location.connectivityStatus == 3) {
            poorConnectivityCount++;
        } else if (location.connectivityStatus == 2) {
            mediumConnectivityCount++;
        } else {
            goodConnectivityCount++;
        }
    }

    cout << "Network Report Summary:" << endl;
    cout << "Total Locations: " << totalLocations << endl;
    cout << "Locations with Poor Connectivity: " << poorConnectivityCount << endl;
    cout << "Locations with Medium Connectivity: " << mediumConnectivityCount << endl;
    cout << "Locations with Good Connectivity: " << goodConnectivityCount << endl;

    // Additional detailed report can be generated for individual locations
    cout << "Detailed Location Status:" << endl;
    for (const auto& location : locations) {
        cout << location.name << " - Status: " << (location.connectivityStatus == 1 ? "Good" : (location.connectivityStatus == 2 ? "Medium" : "Poor")) << endl;
    }
}



// Function to automatically update system configurations based on network conditions
void autoUpdateSystemConfiguration(vector<Location>& locations) {
    cout << "Automatically updating system configurations based on network conditions..." << endl;

    // Adjust configurations based on connectivity status of each location
    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {
            cout << "Location " << location.name << " is experiencing poor connectivity." << endl;
            cout << "Applying additional failover measures, boosting signal, or switching to backup systems." << endl;
            // Apply backup configurations
        } else if (location.connectivityStatus == 2) {
            cout << "Location " << location.name << " has medium connectivity." << endl;
            cout << "Optimizing configurations for stable performance." << endl;
            // Apply optimization measures
        } else {
            cout << "Location " << location.name << " is operating normally." << endl;
            // No need for updates for optimal performance
        }

        displayStatus(location.name, location.connectivityStatus);
    }
}

// Function to check the connectivity based on time of day
int checkConnectivityByTimeOfDay(const Location& location) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int hour = ltm->tm_hour;

    if (hour >= 9 && hour <= 17) {
        cout << location.name << " has better connectivity during office hours." << endl;
        return 1; // Good connectivity during working hours
    } else {
        cout << location.name << " might have worse connectivity after office hours." << endl;
        return 3; // Bad connectivity after working hours
    }
}
// Function to prioritize locations based on their connectivity status and urgency
void prioritizeLocationsForMaintenance(vector<Location>& locations) {
    vector<Location> highPriority, mediumPriority, lowPriority;

    // Categorize locations by priority
    for (const auto& location : locations) {
        if (location.connectivityStatus == 3) {
            highPriority.push_back(location); // Critical - needs immediate attention
        } else if (location.connectivityStatus == 2) {
            mediumPriority.push_back(location); // Needs attention soon
        } else {
            lowPriority.push_back(location); // Good - not urgent
        }
    }

    // Print high priority maintenance tasks
    cout << "High Priority Maintenance Locations (Critical Connectivity Issues):" << endl;
    for (const auto& loc : highPriority) {
        cout << loc.name << endl;
    }

    // Print medium priority maintenance tasks
    cout << "Medium Priority Maintenance Locations (Moderate Connectivity Issues):" << endl;
    for (const auto& loc : mediumPriority) {
        cout << loc.name << endl;
    }

    // Print low priority maintenance tasks
    cout << "Low Priority Maintenance Locations (Good Connectivity):" << endl;
    for (const auto& loc : lowPriority) {
        cout << loc.name << endl;
    }
}

// Function to simulate real-time network optimization based on location needs
void optimizeNetworkBasedOnLocationNeeds(vector<Location>& locations) {
    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {
            // Optimize network by adding more access points or other measures
            cout << "Optimizing network for location " << location.name << " with poor connectivity." << endl;
            location.connectivityStatus = 2; // After optimization, improve connectivity
            displayStatus(location.name, location.connectivityStatus);
        } else if (location.connectivityStatus == 2) {
            // Minor improvements for medium connectivity
            cout << "Minor optimization in progress for location " << location.name << endl;
            location.connectivityStatus = 1; // After minor optimization, improve to full
            displayStatus(location.name, location.connectivityStatus);
        }
    }
}

// Function to handle the simulation of a network-wide upgrade across all locations
void simulateNetworkUpgrade(vector<Location>& locations) {
    cout << "Simulating a network-wide upgrade across all locations..." << endl;
    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {
            cout << "Critical upgrade for location " << location.name << endl;
            location.connectivityStatus = 2; // Critical upgrade improves connectivity
        } else if (location.connectivityStatus == 2) {
            cout << "Upgrading location " << location.name << " to full connectivity." << endl;
            location.connectivityStatus = 1; // Medium upgrade improves to full connectivity
        } else {
            cout << "Location " << location.name << " already has full connectivity." << endl;
        }
        displayStatus(location.name, location.connectivityStatus);
    }
}

// Function to simulate periodic testing of network stability and suggest corrective actions
void testNetworkStabilityAndSuggestActions(vector<Location>& locations) {
    for (auto& location : locations) {
        // Simulate a periodic network stability test (could be based on a schedule or real-time checks)
        cout << "Testing network stability for location " << location.name << "..." << endl;

        if (location.connectivityStatus == 3) {
            cout << "Location " << location.name << " is experiencing poor network stability." << endl;
            // Suggest corrective actions based on the status
            if (location.hasJammer) {
                cout << "Consider relocating jammer or adjusting interference frequency." << endl;
            } else if (location.nearMilitaryBase) {
                cout << "Investigate military base interference and adjust network parameters." << endl;
            } else {
                cout << "Consider upgrading local network infrastructure." << endl;
            }
        } else if (location.connectivityStatus == 2) {
            cout << "Location " << location.name << " has moderate network stability." << endl;
            cout << "Recommend monitoring the situation or performing minor optimizations." << endl;
        } else {
            cout << "Location " << location.name << " has excellent network stability." << endl;
            cout << "No immediate corrective actions needed." << endl;
        }
    }
}

// Function to visualize the average connectivity across all locations
void visualizeAverageConnectivity(const vector<Location>& locations) {
    int totalStatus = 0;
    int totalLocations = locations.size();

    // Sum the connectivity status for average calculation
    for (const auto& location : locations) {
        totalStatus += location.connectivityStatus;
    }

    // Calculate average connectivity status
    double averageConnectivity = static_cast<double>(totalStatus) / totalLocations;
    cout << "Average Connectivity Status across all locations: ";
    if (averageConnectivity <= 1.5) {
        cout << "Good (Average Status: " << averageConnectivity << ")" << endl;
    } else if (averageConnectivity <= 2.5) {
        cout << "Medium (Average Status: " << averageConnectivity << ")" << endl;
    } else {
        cout << "Bad (Average Status: " << averageConnectivity << ")" << endl;
    }
}

// Function to simulate a deep analysis of connectivity trends and provide actionable insights
void analyzeConnectivityTrendsAndProvideInsights(const vector<Location>& locations, const vector<vector<int>>& connectivityOverTime) {
    cout << "Performing deep analysis of connectivity trends and providing insights..." << endl;

    for (size_t i = 0; i < locations.size(); ++i) {
        int poorConnectivityCount = 0;
        int totalPeriodCount = connectivityOverTime[i].size();

        // Count the number of periods with poor connectivity
        for (const auto& status : connectivityOverTime[i]) {
            if (status == 3) {
                poorConnectivityCount++;
            }
        }

        double poorConnectivityPercentage = (static_cast<double>(poorConnectivityCount) / totalPeriodCount) * 100;
        cout << "Location " << locations[i].name << " had poor connectivity "
             << poorConnectivityPercentage << "% of the time." << endl;

        if (poorConnectivityPercentage > 50) {
            cout << "Recommendation: Immediate infrastructure improvements required at "
                 << locations[i].name << endl;
        } else if (poorConnectivityPercentage > 20) {
            cout << "Recommendation: Monitoring and periodic maintenance required for "
                 << locations[i].name << endl;
        } else {
            cout << "Recommendation: No major issues detected at "
                 << locations[i].name << ". Continue regular monitoring." << endl;
        }
    }
}

// Function to simulate an emergency situation and deploy additional resources to the affected location
void simulateEmergencyAndDeployResources(vector<Location>& locations, const string& emergencyLocation) {
    bool locationFound = false;

    // Find the location that is in an emergency state
    for (auto& location : locations) {
        if (location.name == emergencyLocation) {
            locationFound = true;
            cout << "Emergency detected at location " << location.name << "!" << endl;

            // Deploy additional resources (e.g., extra access points, equipment)
            cout << "Deploying additional network resources to " << location.name << " to restore connectivity." << endl;
            location.connectivityStatus = 2; // Assume that emergency resources improve the situation

            displayStatus(location.name, location.connectivityStatus);
            break;
        }
    }

    if (!locationFound) {
        cout << "No emergency situation found at location " << emergencyLocation << endl;
    }
}

// Function to simulate a connectivity test result
string performConnectivityTest(const Location& location) {
    int status = location.connectivityStatus;
    if (status == 1) {
        return "Test result for " + location.name + ": Excellent connectivity!";
    } else if (status == 2) {
        return "Test result for " + location.name + ": Moderate connectivity.";
    } else {
        return "Test result for " + location.name + ": Poor connectivity.";
    }
}

// Function to log the results of a connectivity test to a file
void logConnectivityTestResultsToFile(const vector<Location>& locations) {
    ofstream logFile("connectivity_log.txt", ios::app);
    for (const auto& location : locations) {
        logFile << location.name << ": ";
        logFile << performConnectivityTest(location) << endl;
    }
    cout << "Connectivity test results logged to connectivity_log.txt." << endl;
}

// Function to display locations with a specific connectivity status
void displayLocationsWithSpecificStatus(const vector<Location>& locations, int status) {
    cout << "Displaying locations with connectivity status: " << status << endl;
    for (const auto& location : locations) {
        if (location.connectivityStatus == status) {
            cout << location.name << " - " << performConnectivityTest(location) << endl;
        }
    }
}

// Function to calculate the average connectivity status for all locations
double calculateAverageConnectivity(const vector<Location>& locations) {
    int totalStatus = 0;
    int count = 0;
    for (const auto& location : locations) {
        totalStatus += location.connectivityStatus;
        count++;
    }
    return count > 0 ? static_cast<double>(totalStatus) / count : 0;
}


// Function to simulate random connectivity issues at locations
void simulateRandomConnectivityIssues(vector<Location>& locations) {
    srand(time(0));
    for (auto& location : locations) {
        if (rand() % 5 == 0) { // 20% chance to introduce connectivity issue
            location.connectivityStatus = 3; // Set connectivity to "Bad"
            cout << "Connectivity issue at " << location.name << ". Status set to Bad." << endl;
        }
    }
}

// Function to display connectivity history for a location
void checkLocationConnectivityHistory(const string& locationName, const vector<Location>& locations) {
    bool found = false;
    for (const auto& location : locations) {
        if (location.name == locationName) {
            found = true;
            cout << "Connectivity history for " << locationName << ":\n";
            cout << "Current status: " << (location.connectivityStatus == 1 ? "Good" :
                                         location.connectivityStatus == 2 ? "Medium" : "Bad") << endl;
            break;
        }
    }
    if (!found) {
        cout << "Location " << locationName << " not found." << endl;
    }
}
// Function to simulate high traffic affecting connectivity status
void simulateHighTraffic(vector<Location>& locations) {
    srand(time(0));
    for (auto& location : locations) {
        if (rand() % 4 == 0) { // 25% chance to simulate high traffic
            location.connectivityStatus = 3; // Set to Bad connectivity due to traffic
            cout << "High traffic detected at " << location.name << ". Connectivity set to Bad." << endl;
        }
    }
}
// Function to send alerts based on connectivity status
void sendConnectivityAlert(const Location& location) {
    if (location.connectivityStatus == 3) {
        cout << "Alert: Poor connectivity at " << location.name << "!" << endl;
    } else if (location.connectivityStatus == 2) {
        cout << "Warning: Moderate connectivity at " << location.name << "." << endl;
    } else {
        cout << "Notification: Good connectivity at " << location.name << "." << endl;
    }
}

// Function to backup locations data to a JSON file format
void backupLocationsToJson(const vector<Location>& locations) {
    ofstream jsonFile("locations_backup.json");
    jsonFile << "[\n";
    for (size_t i = 0; i < locations.size(); i++) {
        const auto& location = locations[i];
        jsonFile << "  {\n";
        jsonFile << "    \"name\": \"" << location.name << "\",\n";
        jsonFile << "    \"connectivityStatus\": " << location.connectivityStatus << ",\n";
        jsonFile << "    \"hasJammer\": " << location.hasJammer << ",\n";
        jsonFile << "    \"nearMilitaryBase\": " << location.nearMilitaryBase << ",\n";
        jsonFile << "    \"nearestMetroStation\": \"" << location.nearestMetroStation << "\"\n";
        jsonFile << "  }";
        if (i != locations.size() - 1) {
            jsonFile << ",";
        }
        jsonFile << "\n";
    }
    jsonFile << "]\n";
    cout << "Locations data backed up to locations_backup.json." << endl;
}
// Function to load locations data from a JSON file format
void loadLocationsFromJson(vector<Location>& locations) {
    ifstream jsonFile("locations_backup.json");
    string line;
    while (getline(jsonFile, line)) {
        if (line.find("name") != string::npos) {
            Location loc;
            loc.name = line.substr(line.find(":") + 2, line.find_last_of("\"") - line.find(":") - 2);
            getline(jsonFile, line); loc.connectivityStatus = stoi(line.substr(line.find(":") + 1));
            getline(jsonFile, line); loc.hasJammer = line.find("true") != string::npos;
            getline(jsonFile, line); loc.nearMilitaryBase = line.find("true") != string::npos;
            getline(jsonFile, line); loc.nearestMetroStation = line.substr(line.find(":") + 2, line.find_last_of("\"") - line.find(":") - 2);
            locations.push_back(loc);
        }
    }
    cout << "Locations data loaded from locations_backup.json." << endl;
}
// Function to check if a location is near a specific metro station
bool isNearMetroStation(const Location& location, const string& metroStation) {
    return location.nearestMetroStation == metroStation;
}

// Function to get a list of all locations with a specific connectivity status
vector<Location> filterLocationsByConnectivity(const vector<Location>& locations, int status) {
    vector<Location> filteredLocations;
    for (const auto& location : locations) {
        if (location.connectivityStatus == status) {
            filteredLocations.push_back(location);
        }
    }
    return filteredLocations;
}

// Function to count the number of locations near a specific metro station
int countLocationsNearMetroStation(const vector<Location>& locations, const string& metroStation) {
    int count = 0;
    for (const auto& location : locations) {
        if (isNearMetroStation(location, metroStation)) {
            count++;
        }
    }
    return count;
}

// Function to check if a location is close to a military base
bool isNearMilitaryBase(const Location& location) {
    return location.nearMilitaryBase;
}

// Function to find all locations near a military base
vector<Location> filterLocationsNearMilitaryBase(const vector<Location>& locations) {
    vector<Location> filteredLocations;
    for (const auto& location : locations) {
        if (location.nearMilitaryBase) {
            filteredLocations.push_back(location);
        }
    }
    return filteredLocations;
}

// Function to calculate the average connectivity status of all locations
double calculateAverageConnectivityStatus(const vector<Location>& locations) {
    int totalStatus = 0;
    for (const auto& location : locations) {
        totalStatus += location.connectivityStatus;
    }
    return locations.empty() ? 0.0 : static_cast<double>(totalStatus) / locations.size();
}
// Function to fetch the location with the lowest connectivity status
Location getWorstConnectivityLocation(const vector<Location>& locations) {
    Location worstLocation = locations[0];
    for (const auto& location : locations) {
        if (location.connectivityStatus > worstLocation.connectivityStatus) {
            worstLocation = location;
        }
    }
    return worstLocation;
}

// Function to simulate a network outage for a location
void simulateNetworkOutage(vector<Location>& locations, const string& locationName) {
    for (auto& location : locations) {
        if (location.name == locationName) {
            location.connectivityStatus = 3; // Simulating bad connectivity
            cout << "Network outage simulated for location: " << locationName << endl;
            displayStatus(location.name, location.connectivityStatus);
            break;
        }
    }
}

// Function to simulate the restoration of connectivity for a location
void restoreConnectivity(vector<Location>& locations, const string& locationName) {
    for (auto& location : locations) {
        if (location.name == locationName) {
            location.connectivityStatus = 1; // Simulating full connectivity restoration
            cout << "Connectivity restored for location: " << locationName << endl;
            displayStatus(location.name, location.connectivityStatus);
            break;
        }
    }
}

// Function to display a report of locations experiencing poor connectivity
void displayPoorConnectivityReport(const vector<Location>& locations) {
    bool found = false;
    for (const auto& location : locations) {
        if (location.connectivityStatus == 3) {
            if (!found) {
                cout << "Locations experiencing poor connectivity:" << endl;
                found = true;
            }
            cout << location.name << " - ";
            displayStatus(location.name, location.connectivityStatus);
        }
    }
    if (!found) {
        cout << "All locations have acceptable connectivity." << endl;
    }
}

// Function to get locations that have connectivity status greater than or equal to a given threshold
vector<Location> filterLocationsByConnectivityThreshold(const vector<Location>& locations, int threshold) {
    vector<Location> filteredLocations;
    for (const auto& location : locations) {
        if (location.connectivityStatus <= threshold) {
            filteredLocations.push_back(location);
        }
    }
    return filteredLocations;
}

// Function to recommend possible upgrades for locations with poor connectivity
void recommendUpgradeForPoorConnectivity(vector<Location>& locations) {
    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {
            cout << "Connectivity upgrade recommended for location: " << location.name << endl;
            cout << "Potential solutions: ";
            if (location.hasJammer) {
                cout << "Consider removing the jammer or relocating to a new area." << endl;
            } else if (location.nearMilitaryBase) {
                cout << "Check military base interference and explore alternatives." << endl;
            } else {
                cout << "Improve network infrastructure in the area." << endl;
            }
        }
    }
}

// Function to compare connectivity status between two locations
void compareLocationsConnectivity(const vector<Location>& locations, const string& locationName1, const string& locationName2) {
    Location loc1, loc2;
    bool found1 = false, found2 = false;

    for (const auto& location : locations) {
        if (location.name == locationName1) {
            loc1 = location;
            found1 = true;
        }
        if (location.name == locationName2) {
            loc2 = location;
            found2 = true;
        }
    }

    if (found1 && found2) {
        cout << "Comparing connectivity between " << locationName1 << " and " << locationName2 << ":" << endl;
        displayStatus(locationName1, loc1.connectivityStatus);
        displayStatus(locationName2, loc2.connectivityStatus);
    } else {
        if (!found1) {
            cout << locationName1 << " not found!" << endl;
        }
        if (!found2) {
            cout << locationName2 << " not found!" << endl;
        }
    }
}

// Function to simulate weather conditions affecting connectivity (e.g., during storms or high traffic)
void simulateWeatherImpactOnConnectivity(vector<Location>& locations, const string& weatherCondition) {
    for (auto& location : locations) {
        if (weatherCondition == "storm") {
            if (location.connectivityStatus == 1) {
                location.connectivityStatus = 2; // Storm causes moderate disruption
            } else if (location.connectivityStatus == 2) {
                location.connectivityStatus = 3; // Storm worsens connectivity
            }
        } else if (weatherCondition == "clear") {
            if (location.connectivityStatus == 3) {
                location.connectivityStatus = 2; // Clear weather restores some connectivity
            } else if (location.connectivityStatus == 2) {
                location.connectivityStatus = 1; // Clear weather restores full connectivity
            }
        }
        cout << "Weather impact on location " << location.name << " due to " << weatherCondition << ": ";
        displayStatus(location.name, location.connectivityStatus);
    }
}

// Function to generate a summary of connectivity data for all locations
void generateConnectivitySummary(const vector<Location>& locations) {
    cout << "Connectivity Summary Report:" << endl;
    cout << "---------------------------------" << endl;

    int totalLocations = locations.size();
    int goodCount = 0, mediumCount = 0, badCount = 0;
    for (const auto& location : locations) {
        if (location.connectivityStatus == 1) goodCount++;
        else if (location.connectivityStatus == 2) mediumCount++;
        else if (location.connectivityStatus == 3) badCount++;
    }

    double goodPercentage = (goodCount / static_cast<double>(totalLocations)) * 100;
    double mediumPercentage = (mediumCount / static_cast<double>(totalLocations)) * 100;
    double badPercentage = (badCount / static_cast<double>(totalLocations)) * 100;

    cout << "Good Connectivity: " << goodPercentage << "%" << endl;
    cout << "Medium Connectivity: " << mediumPercentage << "%" << endl;
    cout << "Bad Connectivity: " << badPercentage << "%" << endl;
    cout << "---------------------------------" << endl;
}

// Function to simulate a backup connectivity system if the primary system fails
void simulateBackupSystem(vector<Location>& locations) {
    cout << "Simulating backup connectivity system..." << endl;
    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {
            cout << "Location " << location.name << " is now using backup connectivity." << endl;
            location.connectivityStatus = 2; // Backup system restores connectivity
            displayStatus(location.name, location.connectivityStatus);
        }
    }
}

// Function to visualize connectivity trends over a range of dates (simplified)
void visualizeConnectivityTrendsOverTime(const vector<Location>& locations, const vector<vector<int>>& connectivityOverTime) {
    cout << "Visualizing Connectivity Trends Over Time:" << endl;

    for (size_t i = 0; i < locations.size(); ++i) {
        cout << "Location: " << locations[i].name << " | Trend: ";
        for (const auto& status : connectivityOverTime[i]) {
            cout << (status == 1 ? 'G' : status == 2 ? 'M' : 'B') << " ";
        }
        cout << endl;
    }
}

// Function to simulate a daily report for all locations


// Function to check if a location is experiencing a connectivity issue
bool isConnectivityIssue(const Location& location) {
    return location.connectivityStatus == 3;
}

// Function to find the locations experiencing connectivity issues
vector<Location> findConnectivityIssues(const vector<Location>& locations) {
    vector<Location> locationsWithIssues;
    for (const auto& location : locations) {
        if (isConnectivityIssue(location)) {
            locationsWithIssues.push_back(location);
        }
    }
    return locationsWithIssues;
}







// Function to generate a monthly report for connectivity status changes
void generateMonthlyReport(const vector<Location>& locations, const vector<int>& connectivityChanges) {
    cout << "Monthly Connectivity Report:" << endl;
    cout << "---------------------------------" << endl;
    for (size_t i = 0; i < locations.size(); ++i) {
        cout << locations[i].name << " - Previous Status: " << connectivityChanges[i] << " - Current Status: ";
        displayStatus(locations[i].name, locations[i].connectivityStatus);
        cout << "---------------------------------" << endl;
    }
}

// Function to store connectivity changes for the month
vector<int> trackConnectivityChanges(const vector<Location>& locations) {
    vector<int> connectivityChanges;
    for (const auto& location : locations) {
        connectivityChanges.push_back(location.connectivityStatus);
    }
    return connectivityChanges;
}

// Function to analyze the trend of connectivity over time for a given location
void analyzeConnectivityTrend(const vector<Location>& locations, const string& locationName) {
    vector<int> connectivityHistory;
    for (const auto& location : locations) {
        if (location.name == locationName) {
            connectivityHistory.push_back(location.connectivityStatus);
        }
    }

    cout << "Connectivity Trend for " << locationName << ":" << endl;
    for (int status : connectivityHistory) {
        displayStatus(locationName, status);
    }
}

// Function to simulate the addition of new locations
void addNewLocation(vector<Location>& locations, const string& name, int connectivityStatus, bool hasJammer, bool nearMilitaryBase, const string& nearestMetroStation) {
    Location newLocation = {name, connectivityStatus, hasJammer, nearMilitaryBase, nearestMetroStation};
    locations.push_back(newLocation);
    cout << "New location " << name << " added successfully." << endl;
}

// Function to get the location with the highest connectivity status
Location getBestConnectivityLocation(const vector<Location>& locations) {
    Location bestLocation = locations[0];
    for (const auto& location : locations) {
        if (location.connectivityStatus < bestLocation.connectivityStatus) {
            bestLocation = location;
        }
    }
    return bestLocation;
}

// Function to update the status of multiple locations at once


// Function to calculate the ratio of locations with good, medium, and bad connectivity
void calculateConnectivityRatios(const vector<Location>& locations) {
    int goodCount = 0, mediumCount = 0, badCount = 0;
    for (const auto& location : locations) {
        if (location.connectivityStatus == 1) goodCount++;
        else if (location.connectivityStatus == 2) mediumCount++;
        else if (location.connectivityStatus == 3) badCount++;
    }

    double totalLocations = locations.size();
    cout << "Connectivity Ratios:" << endl;
    cout << "Good: " << (goodCount / totalLocations) * 100 << "%" << endl;
    cout << "Medium: " << (mediumCount / totalLocations) * 100 << "%" << endl;
    cout << "Bad: " << (badCount / totalLocations) * 100 << "%" << endl;
}

// Function to get the location with the most recent connectivity update
Location getMostRecentConnectivityUpdate(const vector<Location>& locations, const vector<time_t>& timestamps) {
    time_t latestTime = timestamps[0];
    Location latestLocation = locations[0];

    for (size_t i = 1; i < locations.size(); ++i) {
        if (timestamps[i] > latestTime) {
            latestTime = timestamps[i];
            latestLocation = locations[i];
        }
    }

    return latestLocation;
}

// Function to simulate updates to connectivity based on external events (e.g., weather)
void simulateConnectivityBasedOnEvents(vector<Location>& locations) {
    srand(time(0));
    for (auto& location : locations) {
        // Simulate weather effect on connectivity
        int eventImpact = rand() % 3; // Randomly simulate low, medium, or high impact
        location.connectivityStatus = (location.connectivityStatus + eventImpact) % 3 + 1;
        cout << "Location " << location.name << " updated due to external event. New status: ";
        displayStatus(location.name, location.connectivityStatus);
    }
}

// Function to visualize connectivity data over time (line chart simulation)
void visualizeConnectivityOverTime(const vector<Location>& locations, const vector<vector<int>>& timeSeriesData) {
    cout << "Connectivity Visualization Over Time:" << endl;
    for (size_t i = 0; i < locations.size(); ++i) {
        cout << locations[i].name << ": ";
        for (size_t j = 0; j < timeSeriesData[i].size(); ++j) {
            cout << (timeSeriesData[i][j] == 1 ? 'G' : timeSeriesData[i][j] == 2 ? 'M' : 'B') << " ";
        }
        cout << endl;
    }
}

// Function to provide a recommendation for improving connectivity at a given location
void recommendConnectivityImprovement(const vector<Location>& locations, const string& locationName) {
    bool found = false;
    for (const auto& location : locations) {
        if (location.name == locationName) {
            found = true;
            if (location.connectivityStatus == 3) {
                cout << "To improve connectivity at " << locationName << ", consider relocating to a better-connected area." << endl;
            } else {
                cout << "The connectivity at " << locationName << " is already acceptable." << endl;
            }
            break;
        }
    }

    if (!found) {
        cout << "Location " << locationName << " not found!" << endl;
    }
}






// Function to display additional information (jammer or military base)
void displayAdditionalInfo(const Location& location) {
    if (location.hasJammer) {
        cout << location.name << " has a jammer affecting connectivity." << endl;
    } else {
        cout << location.name << " does not have a jammer." << endl;
    }

    if (location.nearMilitaryBase) {
        cout << location.name << " is near a military base." << endl;
    } else {
        cout << location.name << " is not near a military base." << endl;
    }

    // Display nearest metro station
    cout << "Nearest metro station to " << location.name << " is: " << location.nearestMetroStation << endl;
}

// Function to check if a location exists in the list
bool locationExists(const string& userInput, const vector<Location>& locations) {
    for (int i = 0; i < locations.size(); i++) {
        if (locations[i].name == userInput) {
            return true;
        }
    }
    return false;
}






// Function to find the connectivity status of a location
int getConnectivityStatus(const string& userInput, const vector<Location>& locations) {
    for (int i = 0; i < locations.size(); i++) {
        if (locations[i].name == userInput) {
            return locations[i].connectivityStatus;
        }
    }
    return -1; // Location not found
}

// Function to update the connectivity status of a location
void updateConnectivityStatus(const string& locationName, vector<Location>& locations, int newStatus) {
    for (int i = 0; i < locations.size(); i++) {
        if (locations[i].name == locationName) {
            locations[i].connectivityStatus = newStatus;
            cout << locationName << " connectivity status updated." << endl;
            return;
        }
    }
    cout << "Location not found!" << endl;
}

// Function to remove a location from the list
void removeLocation(const string& locationName, vector<Location>& locations) {
    for (auto it = locations.begin(); it != locations.end(); ++it) {
        if (it->name == locationName) {
            locations.erase(it);
            cout << locationName << " has been removed from the list." << endl;
            return;
        }
    }
    cout << "Location not found!" << endl;
}

// Function to display a statistics report
void displayStatistics(const vector<Location>& locations) {
    int goodCount = 0, mediumCount = 0, badCount = 0;
    for (const auto& location : locations) {
        if (location.connectivityStatus == 1) goodCount++;
        else if (location.connectivityStatus == 2) mediumCount++;
        else if (location.connectivityStatus == 3) badCount++;
    }

    cout << "Connectivity statistics:" << endl;
    cout << "Good connectivity: " << goodCount << endl;
    cout << "Medium connectivity: " << mediumCount << endl;
    cout << "Bad connectivity: " << badCount << endl;
}

// Function to sort locations alphabetically by name
void sortLocationsByName(vector<Location>& locations) {
    sort(locations.begin(), locations.end(), [](const Location& a, const Location& b) {
        return a.name < b.name;
    });
    cout << "Locations sorted alphabetically." << endl;
}




// Function to sort locations based on connectivity status (ascending)
void sortLocationsByConnectivity(vector<Location>& locations) {
    sort(locations.begin(), locations.end(), [](const Location& a, const Location& b) {
        return a.connectivityStatus < b.connectivityStatus;
    });
    cout << "Locations sorted by connectivity status." << endl;
}





// Function to display all locations and their details
void displayAllLocations(const vector<Location>& locations) {
    cout << "Detailed report of all locations:" << endl;
    for (const auto& location : locations) {
        cout << location.name << " - Connectivity: ";
        displayStatus(location.name, location.connectivityStatus);
        displayAdditionalInfo(location);
        cout << "------------------------" << endl;
    }
}
double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}






// Function to take two inputs from the user and calculate distance
void getDistanceBetweenLocations(const vector<Location>& locations) {
    string location1, location2;
    cout << "Enter the first location: ";
    getline(cin, location1);
    cout << "Enter the second location: ";
    getline(cin, location2);

    // Mock coordinates for demonstration
    map<string, pair<double, double>> locationCoordinates = {
        {"SampigeRoad", {12.9869, 77.5661}},
        {"BengaluruNorth", {13.0308, 77.5619}},
        {"YelahankaNewTown", {13.1012, 77.5946}},
        {"Marathahalli", {12.9606, 77.6974}},
        {"Bellandur", {12.9260, 77.6762}},
        {"BangaloreITHub", {12.8494, 77.6767}},
        {"HosurRoad", {12.9001, 77.6343}},
        {"IndiranagarExtension", {12.9758, 77.6408}},
        {"Agrahara", {12.9964, 77.6040}},
        {"Kadubeesanahalli", {12.9344, 77.6882}}
    };

    // Check if both locations are in the map
    if (locationCoordinates.count(location1) && locationCoordinates.count(location2)) {
        // Retrieve the coordinates for each location
        pair<double, double> loc1 = locationCoordinates[location1];
        pair<double, double> loc2 = locationCoordinates[location2];

        // Calculate the distance
        double distance = calculateDistance(loc1.first, loc1.second, loc2.first, loc2.second);

        // Output the result
        cout << "The distance between " << location1 << " and " << location2 << " is approximately "
             << distance << " km." << endl;
    } else {
        cout << "One or both locations not found in the database!" << endl;
    }
}







// Function to suggest the mode of transport based on distance
void suggestModeOfTransport(double distance) {
    cout << "Suggested mode of transport for a distance of " << distance << " km:" << endl;
    if (distance <= 5) {
        cout << "Walking or cycling would be suitable." << endl;
    } else if (distance <= 15) {
        cout << "Consider using a two-wheeler or car." << endl;
    } else if (distance <= 50) {
        cout << "Public transport like buses or metro is recommended." << endl;
    } else {
        cout << "Long distances are best covered by train or flight." << endl;
    }
}



// Function to display the options to the user
void displayOptions() {
    cout << "Please select an option:" << endl;
    cout << "1. Check connectivity status for a location" << endl;
    cout << "2. Update connectivity status" << endl;
    cout << "3. Remove a location" << endl;
    cout << "4. Display statistics report" << endl;
    cout << "5. Sort locations by name" << endl;
    cout << "6. Sort locations by connectivity" << endl;
    cout << "7. Display detailed report of all locations" << endl;
    cout << "8. Exit" << endl;
    cout << "9. Search locations by connectivity status" << endl;
    cout << "10. Search locations with jammer" << endl;
    cout << "11. Predict connectivity status for a location" << endl;
    cout << "12. Sort locations by multiple criteria" << endl;
    cout << "13. Display a connectivity heatmap" << endl;
    cout << "14. Suggest better location for better connectivity" << endl;
    cout << "15. Simulate live connectivity updates" << endl;
    cout << "16. Save locations to file" << endl;
    cout << "17. Load locations from file" << endl;
    cout << "18. Calculate distance between two locations" << endl;

}


// Function to get user input for location name
string getLocationInput() {
    string userInput;
    cout << "Enter the location name: ";
    getline(cin, userInput);
    return userInput;
}








// Additional functions to enhance functionality:

// Function to search locations by connectivity status
void searchByConnectivityStatus(const vector<Location>& locations, int status) {
    cout << "Searching locations with connectivity status: " << status << endl;
    for (const auto& location : locations) {
        if (location.connectivityStatus == status) {
            cout << location.name << " - Connectivity: ";
            displayStatus(location.name, location.connectivityStatus);
        }
    }
}







// Function to search locations with a jammer
void searchByJammer(const vector<Location>& locations, bool hasJammer) {
    cout << "Searching locations with jammer: " << (hasJammer ? "Yes" : "No") << endl;
    for (const auto& location : locations) {
        if (location.hasJammer == hasJammer) {
            cout << location.name << " - Connectivity: ";
            displayStatus(location.name, location.connectivityStatus);
        }
    }
}

// Function to predict the connectivity status of a location based on similar locations
int predictConnectivityStatus(const vector<Location>& locations, const string& locationName) {
    int totalStatus = 0;
    int count = 0;
    for (const auto& location : locations) {
        if (location.name != locationName) {
            totalStatus += location.connectivityStatus;
            count++;
        }
    }
    return count > 0 ? totalStatus / count : 1; // Default to "Good" if no similar locations
}

// Function to sort locations by multiple criteria (connectivity status and jammer presence)
void sortLocationsByMultipleCriteria(vector<Location>& locations) {
    sort(locations.begin(), locations.end(), [](const Location& a, const Location& b) {
        if (a.connectivityStatus == b.connectivityStatus) {
            return a.hasJammer < b.hasJammer;
        }
        return a.connectivityStatus < b.connectivityStatus;
    });
    cout << "Locations sorted by connectivity status and jammer presence." << endl;
}

// Function to display a connectivity heatmap (good = G, medium = M, bad = B)
void displayConnectivityHeatmap(const vector<Location>& locations) {
    cout << "Connectivity Heatmap (Good = G, Medium = M, Bad = B):" << endl;
    for (const auto& location : locations) {
        char statusChar = (location.connectivityStatus == 1) ? 'G' :
                          (location.connectivityStatus == 2) ? 'M' : 'B';
        cout << location.name << ": " << statusChar << endl;
    }
}

// Function to suggest a better location based on connectivity status
void suggestBetterLocation(const vector<Location>& locations, const string& currentLocation) {
    int currentStatus = getConnectivityStatus(currentLocation, locations);
    string suggestedLocation;
    int highestStatus = currentStatus;

    for (const auto& location : locations) {
        if (location.connectivityStatus > highestStatus) {
            highestStatus = location.connectivityStatus;
            suggestedLocation = location.name;
        }
    }

    if (!suggestedLocation.empty()) {
        cout << "Based on your current location, you can try " << suggestedLocation << " for better connectivity." << endl;
    } else {
        cout << "No better location found." << endl;
    }
}







// Function to simulate live connectivity updates (randomized)
void simulateLiveConnectivityUpdates(vector<Location>& locations) {
    srand(time(0));
    for (auto& location : locations) {
        int randomChange = rand() % 3 + 1; // Randomly change connectivity status
        location.connectivityStatus = randomChange;
        cout << "Updated " << location.name << " connectivity status to ";
        displayStatus(location.name, location.connectivityStatus);
    }
}







// Function to save locations data to a file
void saveLocationsToFile(const vector<Location>& locations) {
    ofstream outFile("locations.txt");
    for (const auto& location : locations) {
        outFile << location.name << "," << location.connectivityStatus << ","
                << location.hasJammer << "," << location.nearMilitaryBase << ","
                << location.nearestMetroStation << endl;
    }
    cout << "Locations data saved to file." << endl;
}
// Function to predict future connectivity issues based on historical data
void predictConnectivityIssues(vector<Location>& locations, const vector<vector<int>>& historicalConnectivityData) {
    cout << "Predicting future connectivity issues based on historical data..." << endl;

    for (size_t i = 0; i < locations.size(); ++i) {
        int lastStatus = historicalConnectivityData[i].back(); // Latest connectivity status
        int totalPoorConnectivity = 0;

        // Analyze historical data to predict future issues
        for (size_t j = 0; j < historicalConnectivityData[i].size(); ++j) {
            if (historicalConnectivityData[i][j] == 3) { // Poor connectivity status
                totalPoorConnectivity++;
            }
        }

        // Simple prediction: If more than 50% of the historical data shows poor connectivity, predict future issues
        double poorConnectivityPercentage = (static_cast<double>(totalPoorConnectivity) / historicalConnectivityData[i].size()) * 100;
        if (poorConnectivityPercentage > 50) {
            cout << "Location " << locations[i].name << " is likely to experience future connectivity issues." << endl;
            cout << "Recommendation: Consider proactive maintenance or upgrades." << endl;
        } else {
            cout << "Location " << locations[i].name << " is unlikely to experience future connectivity issues." << endl;
        }

        // If current status is poor and historical data also suggests issues, predict future problems
        if (lastStatus == 3 && poorConnectivityPercentage > 50) {
            cout << "Location " << locations[i].name << " is predicted to have connectivity problems in the near future." << endl;
        }
    }
}

// Function to optimize resource allocation based on real-time network conditions and priorities
void optimizeResourceAllocation(vector<Location>& locations) {
    cout << "Optimizing resource allocation based on real-time network conditions..." << endl;

    // Simulate resource allocation: locations with poor connectivity get more resources
    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {
            // High resource allocation for critical locations
            cout << "Allocating additional resources (e.g., bandwidth, backup power) to location " << location.name << endl;
            location.connectivityStatus = 2; // Improving connectivity status
        } else if (location.connectivityStatus == 2) {
            // Moderate resource allocation for medium-priority locations
            cout << "Allocating moderate resources to location " << location.name << endl;
            location.connectivityStatus = 1; // Improving to full connectivity
        } else {
            // No additional resources needed for good connectivity
            cout << "Location " << location.name << " has good connectivity. No additional resources required." << endl;
        }

        displayStatus(location.name, location.connectivityStatus);
    }
}

// Function to simulate and improve the system's resilience to external threats (e.g., jamming, interference)
void improveSystemResilience(vector<Location>& locations) {
    cout << "Simulating resilience improvement against external threats (e.g., jamming, interference)..." << endl;

    for (auto& location : locations) {
        if (location.hasJammer) {
            cout << "Location " << location.name << " is under threat of jamming." << endl;
            cout << "Activating countermeasures: adjusting frequencies, improving encryption, and deploying backup networks." << endl;
            // Improving the location's connectivity by countering interference
            location.connectivityStatus = 1; // Improving from poor to full connectivity
        } else if (location.nearMilitaryBase) {
            cout << "Location " << location.name << " is near a military base with potential interference." << endl;
            cout << "Adjusting network parameters to minimize interference." << endl;
            // Minor adjustments to network parameters
            location.connectivityStatus = 2; // Improving from moderate to good connectivity
        } else {
            cout << "Location " << location.name << " is free from external threats." << endl;
            cout << "Network is operating normally." << endl;
        }

        displayStatus(location.name, location.connectivityStatus);
    }
}






// Function to perform automated testing and updates for resilience strategies
void automatedResilienceTesting(vector<Location>& locations) {
    cout << "Automated resilience testing in progress..." << endl;

    // Test for connectivity issues under various conditions
    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {
            cout << "Testing resilience measures for location " << location.name << " (poor connectivity)." << endl;
            cout << "Initiating backup protocols: rerouting data, deploying additional resources." << endl;
            location.connectivityStatus = 2; // Improve connectivity with backup protocols
        } else if (location.connectivityStatus == 2) {
            cout << "Testing resilience measures for location " << location.name << " (medium connectivity)." << endl;
            cout << "Minor improvements applied." << endl;
            location.connectivityStatus = 1; // Improve connectivity
        } else {
            cout << "Location " << location.name << " is operating optimally." << endl;
        }

        displayStatus(location.name, location.connectivityStatus);
    }
}

// Function to simulate a global disaster event and its impact on the network, followed by recovery efforts
void simulateGlobalDisasterAndRecovery(vector<Location>& locations) {
    cout << "Simulating a global disaster event affecting network infrastructure..." << endl;

    // Simulate the disaster: all locations experience poor connectivity
    for (auto& location : locations) {
        cout << "Disaster impacting location " << location.name << ": Connectivity down." << endl;
        location.connectivityStatus = 3; // Set all locations to poor connectivity during the disaster
        displayStatus(location.name, location.connectivityStatus);
    }

    // Recovery efforts: restore locations based on priority
    cout << "Starting recovery efforts..." << endl;
    for (auto& location : locations) {
        if (location.connectivityStatus == 3) {
            cout << "Restoring connectivity at location " << location.name << "..." << endl;
            location.connectivityStatus = 2; // Gradual restoration to medium connectivity
        }

        displayStatus(location.name, location.connectivityStatus);
    }

    cout << "Disaster recovery complete. System is back to normal operation." << endl;
}

// Function to track performance improvements over time
void trackPerformanceImprovements(const vector<Location>& locations, const vector<vector<int>>& connectivityOverTime) {
    cout << "Tracking performance improvements over time..." << endl;

    for (size_t i = 0; i < locations.size(); ++i) {
        int improvements = 0;

        // Analyze the trend of connectivity improvements over time
        for (size_t j = 1; j < connectivityOverTime[i].size(); ++j) {
            if (connectivityOverTime[i][j] < connectivityOverTime[i][j - 1]) {
                improvements++; // Count when the connectivity status improves
            }
        }

        // Display performance improvement results
        if (improvements > 0) {
            cout << "Location " << locations[i].name << " has shown " << improvements << " performance improvements over time." << endl;
        } else {
            cout << "No significant performance improvements at location " << locations[i].name << "." << endl;
        }
    }
}
// Function to load locations data from a file
void loadLocationsFromFile(vector<Location>& locations) {
    ifstream inFile("locations.txt");
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        Location loc;
        getline(ss, loc.name, ',');
        ss >> loc.connectivityStatus;
        ss.ignore(1);
        ss >> loc.hasJammer;
        ss.ignore(1);
        ss >> loc.nearMilitaryBase;
        ss.ignore(1);
        getline(ss, loc.nearestMetroStation, ',');
        locations.push_back(loc);
    }
    cout << "Locations data loaded from file." << endl;
}

// Function to perform the action based on user input
void performAction(int option, vector<Location>& locations) {
    if (option == 1) {
        string userInput = getLocationInput();
        bool found = locationExists(userInput, locations);
        if (found) {
            int status = getConnectivityStatus(userInput, locations);
            displayStatus(userInput, status);
            displayAdditionalInfo(locations[0]);
        } else {
            cout << "Location not found!" << endl;
        }
    } else if (option == 2) {
        string locationName = getLocationInput();
        int newStatus;
        cout << "Enter new connectivity status (1: Good, 2: Medium, 3: Bad): ";
        cin >> newStatus;
        cin.ignore();
        updateConnectivityStatus(locationName, locations, newStatus);
    } else if (option == 3) {
        string locationName = getLocationInput();
        removeLocation(locationName, locations);
    } else if (option == 4) {
        displayStatistics(locations);
    } else if (option == 5) {
        sortLocationsByName(locations);
    } else if (option == 6) {
        sortLocationsByConnectivity(locations);
    } else if (option == 7) {
        displayAllLocations(locations);
    } else if (option == 8) {
        cout << "Exiting program. Thank you for using the connectivity checker." << endl;
    } else if (option == 9) {
        int status;
        cout << "Enter connectivity status (1, 2, or 3): ";
        cin >> status;
        searchByConnectivityStatus(locations, status);
    } else if (option == 10) {
        bool hasJammer;
        cout << "Enter jammer status (1: Yes, 0: No): ";
        cin >> hasJammer;
        searchByJammer(locations, hasJammer);
    } else if (option == 11) {
        string locationName = getLocationInput();
        int predictedStatus = predictConnectivityStatus(locations, locationName);
        cout << "Predicted connectivity status for " << locationName << " is: ";
        displayStatus(locationName, predictedStatus);
    } else if (option == 12) {
        sortLocationsByMultipleCriteria(locations);
    } else if (option == 13) {
        displayConnectivityHeatmap(locations);
    } else if (option == 14) {
        string currentLocation = getLocationInput();
        suggestBetterLocation(locations, currentLocation);
    } else if (option == 15) {
        simulateLiveConnectivityUpdates(locations);
    } else if (option == 16) {
        saveLocationsToFile(locations);
    } else if (option == 17) {
        loadLocationsFromFile(locations);
    }
    else if (option == 18) {
    getDistanceBetweenLocations(locations);
}

    else {
        cout << "Invalid option! Please try again." << endl;
    }
}
struct LanguageCulture {
    string language;
    string greeting;
    string culturalNorm;
};

// Function to write data to a file
// Function to write data to a file
void writeToFile(const string& filename, const vector<LanguageCulture>& data) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Could not open file for writing." << endl;
        return;
    }

    // Write each entry to the file in CSV format
    for (const auto& entry : data) {
        outFile << entry.language << "," << entry.greeting << "," << entry.culturalNorm << endl;
    }

    outFile.close();
    cout << "Data written to " << filename << " successfully." << endl;
}

// Function to read data from a file
vector<LanguageCulture> readFromFile(const string& filename) {
    ifstream inFile(filename);  // Open the file for reading
    vector<LanguageCulture> data;  // Vector to store the data

    if (!inFile) {  // Check if the file was opened successfully
        cerr << "Error: Could not open file for reading." << endl;
        return data;  // Return an empty vector if the file could not be opened
    }

    string line;
    while (getline(inFile, line)) {  // Read each line from the file
        stringstream ss(line);  // Create a stringstream from the line
        LanguageCulture entry;  // Create a new LanguageCulture entry
        getline(ss, entry.language, ',');  // Read the language from the line
        getline(ss, entry.greeting, ',');  // Read the greeting from the line
        getline(ss, entry.culturalNorm, ',');  // Read the cultural norm from the line
        data.push_back(entry);  // Add the entry to the data vector
    }

    inFile.close();  // Close the file
    return data;  // Return the data vector
}

// Function to display data
/**
 * @brief Displays the dataset.
 *
 * This function iterates through a vector of LanguageCulture objects and prints the details
 * of each entry, including the language, greeting, and cultural norm. If the dataset is empty,
 * it prints a message indicating that no data is available.
 *
 * @param data A vector of LanguageCulture objects to be displayed.
 *
 * The function performs the following steps:
 * 1. Iterates through each entry in the data vector.
 * 2. Prints the language, greeting, and cultural norm of each entry.
 * 3. Prints a separator line for better readability.
 * 4. If the data vector is empty, prints a message indicating that no data is available.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}, {"Japanese", "Konnichiwa", "Collectivism"}};
 * displayData(data);
 * // Output:
 * // Language: English
 * // Greeting: Hello
 * // Cultural Norm: Individualism
 * // ---------------------------
 * // Language: Japanese
 * // Greeting: Konnichiwa
 * // Cultural Norm: Collectivism
 * // ---------------------------
 * @endcode
 */
void displayData(const vector<LanguageCulture>& data) {
    // Iterate through each entry in the data vector
    for (const auto& entry : data) {
        // Print the language of the current entry
        cout << "Language: " << entry.language << endl;
        // Print the greeting of the current entry
        cout << "Greeting: " << entry.greeting << endl;
        // Print the cultural norm of the current entry
        cout << "Cultural Norm: " << entry.culturalNorm << endl;
        // Print a separator line for better readability
        cout << "---------------------------" << endl;
    }
    // If no data is available, print a message indicating that
    if (data.empty()) {
        cout << "No data available to display." << endl;
    }
}

// Function to search for a language in the data
/**
 * @brief Searches for a specific language in the provided data vector and prints its details if found.
 *
 * This function uses the `find_if` algorithm to search for a language within a vector of `LanguageCulture` objects.
 * If the language is found, it prints the language, greeting, and cultural norm associated with it.
 * If the language is not found, it prints a message indicating that the language was not found.
 *
 * @param data A vector of `LanguageCulture` objects containing language data.
 * @param language A string representing the language to search for.
 */
void searchLanguage(const vector<LanguageCulture>& data, const string& language) {
    // Use find_if to search for the language in the data vector
    auto it = find_if(data.begin(), data.end(), [&language](const LanguageCulture& entry) {
        return entry.language == language;  // Return true if the language matches
    });

    // Check if the language was found
    if (it != data.end()) {
        // If found, print the details of the language
        cout << "Language: " << it->language << endl;
        cout << "Greeting: " << it->greeting << endl;
        cout << "Cultural Norm: " << it->culturalNorm << endl;
    } else {
        // If not found, print a message indicating that
        cout << "Language " << language << " not found." << endl;
    }
}

// Function to sort data alphabetically by language
/**
 * @brief Sorts the dataset alphabetically by language.
 *
 * This function sorts a vector of LanguageCulture objects in ascending order
 * based on the language field of each LanguageCulture object.
 *
 * @param data A reference to a vector of LanguageCulture objects to be sorted.
 *
 * The function performs the following steps:
 * 1. Uses the sort algorithm with a lambda function to compare the language fields.
 * 2. Prints a message indicating that the data has been sorted alphabetically by language.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"Japanese", "Konnichiwa", "Collectivism"}, {"English", "Hello", "Individualism"}};
 * sortData(data);
 * // Output:
 * // Data sorted alphabetically by language.
 * @endcode
 */
void sortData(vector<LanguageCulture>& data) {
    // Use the sort algorithm with a lambda function to compare the language fields
    sort(data.begin(), data.end(), [](const LanguageCulture& a, const LanguageCulture& b) {
        return a.language < b.language;
    });
    // Print a message indicating that the data has been sorted alphabetically by language
    cout << "Data sorted alphabetically by language." << endl;
}

// Function to sort data by greeting length
/**
 * @brief Sorts a vector of LanguageCulture objects by the length of their greetings.
 *
 * This function sorts the provided vector of LanguageCulture objects in ascending order
 * based on the length of the greeting string in each LanguageCulture object.
 *
 * @param data A reference to a vector of LanguageCulture objects to be sorted.
 */
void sortByCulturalNormLength(vector<LanguageCulture>& data) {
    sort(data.begin(), data.end(), [](const LanguageCulture& a, const LanguageCulture& b) {
        return a.culturalNorm.size() < b.culturalNorm.size();
    });
    cout << "Data sorted by cultural norm length." << endl;
}

// Function to sort data by a specific field
// Function to sort data by a specific field
/**
 * @brief Sorts the dataset by a specified field.
 *
 * This function sorts a vector of LanguageCulture objects based on the specified field.
 * The field can be "language", "greeting", or "culturalNorm". It prints a message indicating
 * the field by which the data has been sorted.
 *
 * @param data A vector of LanguageCulture objects to be sorted.
 * @param field The field by which to sort the data ("language", "greeting", or "culturalNorm").
 *
 * The function performs the following steps:
 * 1. Checks if the specified field is "language" and sorts the data alphabetically by language.
 * 2. Checks if the specified field is "greeting" and sorts the data alphabetically by greeting.
 * 3. Checks if the specified field is "culturalNorm" and sorts the data alphabetically by cultural norm.
 * 4. Prints a message indicating the field by which the data has been sorted.
 * 5. If an invalid field is specified, prints an error message.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}, {"Japanese", "Konnichiwa", "Collectivism"}};
 * sortDataByField(data, "language");
 * // Output:
 * // Data sorted alphabetically by language.
 * @endcode
 */
void sortDataByField(vector<LanguageCulture>& data, const string& field) {
    if (field == "language") {
        sort(data.begin(), data.end(), [](const LanguageCulture& a, const LanguageCulture& b) {
            return a.language < b.language;
        });
        cout << "Data sorted alphabetically by language." << endl;
    } else if (field == "greeting") {
        sort(data.begin(), data.end(), [](const LanguageCulture& a, const LanguageCulture& b) {
            return a.greeting < b.greeting;
        });
        cout << "Data sorted alphabetically by greeting." << endl;
    } else if (field == "culturalNorm") {
        sort(data.begin(), data.end(), [](const LanguageCulture& a, const LanguageCulture& b) {
            return a.culturalNorm < b.culturalNorm;
        });
        cout << "Data sorted alphabetically by cultural norm." << endl;
    } else {
        cout << "Invalid field specified for sorting." << endl;
    }
}

// Function to filter data by cultural norm keyword
// Function to filter data by cultural norm keyword
/**
 * @brief Filters and displays entries containing a specific keyword in their cultural norm.
 *
 * This function iterates through a vector of LanguageCulture objects and checks if the cultural
 * norm of each entry contains the specified keyword. It prints the details of entries that contain
 * the keyword.
 *
 * @param data A vector of LanguageCulture objects to be filtered.
 * @param keyword The keyword to search for in the cultural norms.
 *
 * The function performs the following steps:
 * 1. Prints a header indicating the keyword being searched for.
 * 2. Iterates through each entry in the data vector.
 * 3. Checks if the cultural norm of the current entry contains the keyword.
 * 4. Prints the details of the entry if the keyword is found in the cultural norm.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}, {"Japanese", "Konnichiwa", "Collectivism"}};
 * filterByCulturalNorm(data, "Individualism");
 * // Output:
 * // Entries containing the keyword 'Individualism':
 * // Language: English
 * // Greeting: Hello
 * // Cultural Norm: Individualism
 * // ---------------------------
 * @endcode
 */
void filterByCulturalNorm(const vector<LanguageCulture>& data, const string& keyword) {
    // Print a header indicating the keyword being searched for
    cout << "Entries containing the keyword '" << keyword << "':" << endl;

    // Iterate through each entry in the data vector
    for (const auto& entry : data) {
        // Check if the cultural norm of the current entry contains the keyword
        if (entry.culturalNorm.find(keyword) != string::npos) {
            // Print the details of the entry if the keyword is found in the cultural norm
            cout << "Language: " << entry.language << endl;
            cout << "Greeting: " << entry.greeting << endl;
            cout << "Cultural Norm: " << entry.culturalNorm << endl;
            cout << "---------------------------" << endl;
        }
    }
}

// Function to add a new entry to the data


// Function to delete an entry by language
/**
 * @brief Deletes an entry from the dataset by language.
 *
 * This function searches for a specific language in the dataset and deletes its entry if found.
 * If the language is not found, it prints a message indicating that.
 *
 * @param data A vector of LanguageCulture objects containing the dataset.
 * @param language The language to search for and delete in the dataset.
 *
 * The function performs the following steps:
 * 1. Uses the remove_if algorithm to find and mark the entry for deletion.
 * 2. Checks if the entry was found and marked for deletion.
 * 3. If found, erases the entry from the data vector and prints a success message.
 * 4. If not found, prints a message indicating that the language was not found.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}, {"Japanese", "Konnichiwa", "Collectivism"}};
 * deleteEntry(data, "English");
 * // Output:
 * // Entry for language 'English' deleted successfully.
 * @endcode
 */
void deleteEntry(vector<LanguageCulture>& data, const string& language) {
    // Use remove_if to find and mark the entry for deletion
    auto it = remove_if(data.begin(), data.end(), [&language](const LanguageCulture& entry) {
        return entry.language == language;  // Return true if the language matches
    });

    // Check if the entry was found and marked for deletion
    if (it != data.end()) {
        // Erase the entry from the data vector
        data.erase(it, data.end());
        // Print a success message
        cout << "Entry for language '" << language << "' deleted successfully." << endl;
    } else {
        // Print a message indicating that the language was not found
        cout << "Language '" << language << "' not found." << endl;
    }
}

// Function to back up data
// Function to back up data
/**
 * @brief Backs up the dataset to a specified file.
 *
 * This function writes the current dataset to a specified backup file. It uses the writeToFile
 * function to perform the actual file writing and prints a message indicating that the data has
 * been backed up successfully.
 *
 * @param data A vector of LanguageCulture objects to be backed up.
 * @param backupFile The name of the file to which the data will be backed up.
 *
 * The function performs the following steps:
 * 1. Calls the writeToFile function to write the data to the backup file.
 * 2. Prints a success message indicating that the data has been backed up.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}};
 * backupData(data, "backup_language_culture.txt");
 * // Output:
 * // Data backed up successfully to backup_language_culture.txt.
 * @endcode
 */
void backupData(const vector<LanguageCulture>& data, const string& backupFile) {
    writeToFile(backupFile, data);  // Write the data to the backup file
    cout << "Data backed up successfully to " << backupFile << "." << endl;  // Print a success message
}

// Function to restore data from backup
// Function to restore data from backup
/**
 * @brief Restores the dataset from a backup file.
 *
 * This function reads data from a specified backup file and replaces the current dataset with
 * the data read from the backup file. It prints a message indicating that the data has been
 * restored successfully.
 *
 * @param data A reference to a vector of LanguageCulture objects to be restored.
 * @param backupFile The name of the backup file from which the data will be read.
 *
 * The function performs the following steps:
 * 1. Calls the readFromFile function to read data from the backup file.
 * 2. Replaces the current dataset with the data read from the backup file.
 * 3. Prints a success message indicating that the data has been restored.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}};
 * restoreData(data, "backup_language_culture.txt");
 * // Output:
 * // Data restored from backup_language_culture.txt.
 * @endcode
 */
void restoreData(vector<LanguageCulture>& data, const string& backupFile) {
    data = readFromFile(backupFile);  // Read data from the backup file and replace the current dataset
    cout << "Data restored from " << backupFile << "." << endl;  // Print a success message
}

// Function to update an existing entry
/**
 * @brief Updates an existing entry in the dataset by language.
 *
 * This function searches for a specific language in the dataset and updates its greeting
 * and cultural norm if found. If the language is not found, it prints a message indicating that.
 *
 * @param data A vector of LanguageCulture objects containing the dataset.
 * @param language The language to search for and update in the dataset.
 *
 * The function performs the following steps:
 * 1. Uses the find_if algorithm to search for the language in the data vector.
 * 2. If the language is found, prompts the user to enter a new greeting and cultural norm.
 * 3. Updates the greeting and cultural norm of the found entry with the new values.
 * 4. Prints a success message indicating that the entry was updated.
 * 5. If the language is not found, prints a message indicating that the language was not found.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}, {"Japanese", "Konnichiwa", "Collectivism"}};
 * updateEntry(data, "English");
 * // Output:
 * // Updating entry for language: English
 * // Enter new greeting: Hi
 * // Enter new cultural norm: Independence
 * // Entry updated successfully.
 * @endcode
 */
void updateEntry(vector<LanguageCulture>& data, const string& language) {
    // Use find_if to search for the language in the data vector
    auto it = find_if(data.begin(), data.end(), [&language](const LanguageCulture& entry) {
        return entry.language == language;  // Return true if the language matches
    });

    // Check if the language was found
    if (it != data.end()) {
        // If found, prompt the user to enter a new greeting
        cout << "Updating entry for language: " << language << endl;
        cout << "Enter new greeting: ";
        getline(cin, it->greeting);  // Read the new greeting from the user
        // Prompt the user to enter a new cultural norm
        cout << "Enter new cultural norm: ";
        getline(cin, it->culturalNorm);  // Read the new cultural norm from the user
        cout << "Entry updated successfully." << endl;  // Confirm the update
    } else {
        // If not found, print a message indicating that
        cout << "Language '" << language << "' not found." << endl;
    }
}

// Function to count occurrences of cultural norm keywords
// Function to count occurrences of cultural norm keywords
/**
 * @brief Counts the occurrences of each keyword in cultural norms.
 *
 * This function iterates through a vector of LanguageCulture objects and counts the occurrences
 * of each keyword in the cultural norms. It uses a map to store the count of each keyword and
 * prints the frequencies of the keywords.
 *
 * @param data A vector of LanguageCulture objects containing cultural norms.
 *
 * The function performs the following steps:
 * 1. Initializes a map to store the count of each keyword.
 * 2. Iterates through each entry in the data vector.
 * 3. Uses a stringstream to split the cultural norm into individual words.
 * 4. Increments the count for each word in the map.
 * 5. Prints the frequencies of the keywords stored in the map.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}, {"Japanese", "Konnichiwa", "Collectivism"}};
 * countKeywordOccurrences(data);
 * // Output:
 * // Cultural Norm Keyword Frequencies:
 * // Individualism: 1
 * // Collectivism: 1
 * @endcode
 */
void countKeywordOccurrences(const vector<LanguageCulture>& data) {
    map<string, int> keywordCount;  // Map to store the count of each keyword

    // Iterate through each entry in the data vector
    for (const auto& entry : data) {
        stringstream ss(entry.culturalNorm);  // Create a stringstream from the cultural norm
        string word;
        // Split the cultural norm into individual words
        while (ss >> word) {
            keywordCount[word]++;  // Increment the count for each word in the map
        }
    }

    // Print the frequencies of the keywords stored in the map
    cout << "Cultural Norm Keyword Frequencies:" << endl;
    for (const auto& pair : keywordCount) {
        cout << pair.first << ": " << pair.second << endl;  // Print each keyword and its count
    }
}

// Function to display dataset statistics
/**
 * @brief Displays statistics about the dataset.
 *
 * This function prints the total number of languages in the dataset and counts the occurrences
 * of each keyword in the cultural norms. It provides an overview of the dataset's size and the
 * frequency of keywords in the cultural norms.
 *
 * @param data A vector of LanguageCulture objects containing the dataset.
 *
 * The function performs the following steps:
 * 1. Prints the total number of languages in the dataset.
 * 2. Calls the countKeywordOccurrences function to count and print the occurrences of each keyword
 *    in the cultural norms.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}, {"Japanese", "Konnichiwa", "Collectivism"}};
 * displayStatistics(data);
 * // Output:
 * // Total number of languages: 2
 * // Cultural Norm Keyword Frequencies:
 * // Individualism: 1
 * // Collectivism: 1
 * @endcode
 */
void displayStatistics(const vector<LanguageCulture>& data) {
    cout << "Total number of languages: " << data.size() << endl;  // Print the total number of languages
    countKeywordOccurrences(data);  // Count and print the occurrences of each keyword in the cultural norms
}

// Function to export data to a formatted report
// Function to export data to a formatted report
/**
 * @brief Exports the dataset to a formatted report file.
 *
 * This function takes a vector of LanguageCulture objects and writes their details to a specified
 * report file in a formatted manner. It includes headers and separators for better readability.
 *
 * @param data A vector of LanguageCulture objects to be exported.
 * @param reportFile The name of the file to which the report will be written.
 *
 * The function performs the following steps:
 * 1. Opens the report file for writing.
 * 2. Checks if the file was opened successfully and prints an error message if not.
 * 3. Writes a header and separator to the report file.
 * 4. Iterates through each entry in the data vector.
 * 5. Writes the language, greeting, and cultural norm of each entry to the report file.
 * 6. Closes the report file and prints a success message.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}, {"Japanese", "Konnichiwa", "Collectivism"}};
 * exportToReport(data, "language_culture_report.txt");
 * // Output:
 * // Report exported to language_culture_report.txt successfully.
 * @endcode
 */
void exportToReport(const vector<LanguageCulture>& data, const string& reportFile) {
    ofstream report(reportFile);  // Open the report file for writing
    if (!report) {  // Check if the file was opened successfully
        cerr << "Error: Could not open file for report export." << endl;
        return;  // Return if the file could not be opened
    }

    // Write a header and separator to the report file
    report << "Language and Cultural Norms Report\n";
    report << "================================\n";

    // Iterate through each entry in the data vector
    for (const auto& entry : data) {
        // Write the language, greeting, and cultural norm of each entry to the report file
        report << "Language: " << entry.language << endl;
        report << "Greeting: " << entry.greeting << endl;
        report << "Cultural Norm: " << entry.culturalNorm << endl;
        report << "--------------------------------\n";  // Write a separator for better readability
    }

    report.close();  // Close the report file
    cout << "Report exported to " << reportFile << " successfully." << endl;  // Print a success message
}

// Function to find the most common cultural norm keyword
// Function to find the most common cultural norm keyword
/**
 * @brief Finds and prints the most common keyword in cultural norms.
 *
 * This function iterates through a vector of LanguageCulture objects and counts the occurrences
 * of each keyword in the cultural norms. It then finds and prints the most common keyword along
 * with its occurrence count. If no keywords are found, it prints a message indicating that.
 *
 * @param data A vector of LanguageCulture objects containing cultural norms.
 *
 * The function performs the following steps:
 * 1. Initializes a map to store the count of each keyword.
 * 2. Iterates through each entry in the data vector.
 * 3. Uses a stringstream to split the cultural norm into individual words.
 * 4. Increments the count for each word in the map.
 * 5. Uses max_element to find the keyword with the highest count.
 * 6. Prints the most common keyword and its count, or a message if no keywords are found.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello", "Individualism"}, {"Japanese", "Konnichiwa", "Collectivism"}};
 * findMostCommonKeyword(data);
 * // Output:
 * // Most common keyword: Individualism with 1 occurrences.
 * @endcode
 */
void findMostCommonKeyword(const vector<LanguageCulture>& data) {
    map<string, int> keywordCount;  // Map to store the count of each keyword

    // Iterate through each entry in the data vector
    for (const auto& entry : data) {
        stringstream ss(entry.culturalNorm);  // Create a stringstream from the cultural norm
        string word;
        // Split the cultural norm into individual words
        while (ss >> word) {
            keywordCount[word]++;  // Increment the count for each word in the map
        }
    }

    // Use max_element to find the keyword with the highest count
    auto maxElement = max_element(keywordCount.begin(), keywordCount.end(),
                                  [](const pair<string, int>& a, const pair<string, int>& b) {
                                      return a.second < b.second;
                                  });

    // Check if any keywords were found
    if (maxElement != keywordCount.end()) {
        // Print the most common keyword and its count
        cout << "Most common keyword: " << maxElement->first << " with " << maxElement->second << " occurrences." << endl;
    } else {
        // Print a message if no keywords were found
        cout << "No keywords found in the data." << endl;
    }
}
// Function to count the number of entries for each language
/**
 * @brief Counts the number of entries for each language in the dataset.
 *
 * This function iterates through a vector of LanguageCulture objects and counts the number of
 * entries for each language. It uses a map to store the count of entries for each language and
 * prints the counts.
 *
 * @param data A vector of LanguageCulture objects to be counted.
 *
 * The function performs the following steps:
 * 1. Initializes a map to store the count of entries for each language.
 * 2. Iterates through each entry in the data vector.
 * 3. Increments the count for the language of the current entry in the map.
 * 4. Prints the count of entries for each language stored in the map.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello"}, {"Spanish", "Hola"}, {"English", "Hi"}};
 * countEntriesByLanguage(data);
 * // Output:
 * // Language Entry Counts:
 * // English: 2 entries.
 * // Spanish: 1 entry.
 * @endcode
 */
// Function to count the number of entries for each language
/**
 * @brief Counts the number of entries for each language in the dataset.
 *
 * This function iterates through a vector of LanguageCulture objects and counts the number of
 * entries for each language. It uses a map to store the count of entries for each language and
 * prints the counts.
 *
 * @param data A vector of LanguageCulture objects to be counted.
 *
 * The function performs the following steps:
 * 1. Initializes a map to store the count of entries for each language.
 * 2. Iterates through each entry in the data vector.
 * 3. Increments the count for the language of the current entry in the map.
 * 4. Prints the count of entries for each language stored in the map.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello"}, {"Spanish", "Hola"}, {"English", "Hi"}};
 * countEntriesByLanguage(data);
 * // Output:
 * // Language Entry Counts:
 * // English: 2 entries.
 * // Spanish: 1 entry.
 * @endcode
 */
void countEntriesByLanguage(const vector<LanguageCulture>& data) {
    map<string, int> languageCount;  // Map to store the count of entries for each language

    // Iterate through each entry in the data vector
    for (const auto& entry : data) {
        languageCount[entry.language]++;  // Increment the count for the language of the current entry
    }

    // Print the count of entries for each language stored in the map
    cout << "Language Entry Counts:\n";
    for (const auto& pair : languageCount) {
        cout << pair.first << ": " << pair.second << " entries." << endl;  // Print the language and its count
    }
}
// Function to find the language with the longest greeting
/**
 * @brief Finds and prints the language with the longest greeting.
 *
 * This function iterates through a vector of LanguageCulture objects and finds the language
 * with the longest greeting. It prints the language and its greeting. If the dataset is empty,
 * it prints a message indicating that no data is available.
 *
 * @param data A vector of LanguageCulture objects to be checked.
 *
 * The function performs the following steps:
 * 1. Checks if the data vector is empty and prints a message if it is.
 * 2. Uses the max_element algorithm to find the entry with the longest greeting.
 * 3. Prints the language and greeting of the entry with the longest greeting.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello"}, {"Spanish", "Hola"}, {"French", "Bonjour"}};
 * findLanguageWithLongestGreeting(data);
 * // Output:
 * // Language with longest greeting:
 * // Language: French
 * // Greeting: Bonjour
 * @endcode
 */
void findLanguageWithLongestGreeting(const vector<LanguageCulture>& data) {
    // Check if the data vector is empty
    if (data.empty()) {
        // Print a message indicating that no data is available
        cout << "No data available." << endl;
        return;
    }

    // Use max_element to find the entry with the longest greeting
    auto longestGreeting = max_element(data.begin(), data.end(), [](const LanguageCulture& a, const LanguageCulture& b) {
        return a.greeting.size() < b.greeting.size();
    });

    // Print the language and greeting of the entry with the longest greeting
    cout << "Language with longest greeting:\n";
    cout << "Language: " << longestGreeting->language << endl;
    cout << "Greeting: " << longestGreeting->greeting << endl;
}
// Function to check if a language exists in the data
/**
 * @brief Checks if a specific language exists in the dataset.
 *
 * This function iterates through a vector of LanguageCulture objects and checks if any of them
 * have a language that matches the specified language. It returns true if the language is found,
 * and false otherwise.
 *
 * @param data A vector of LanguageCulture objects to be checked.
 * @param language The language to search for in the dataset.
 * @return true if the language exists in the dataset, false otherwise.
 *
 * The function performs the following steps:
 * 1. Uses the find_if algorithm to search for the language in the data vector.
 * 2. Returns true if the language is found, and false otherwise.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello"}, {"Spanish", "Hola"}, {"French", "Bonjour"}};
 * bool exists = languageExists(data, "Spanish");
 * // Output: exists will be true
 * @endcode
 */
bool languageExists(const vector<LanguageCulture>& data, const string& language) {
    // Use find_if to search for the language in the data vector
    return find_if(data.begin(), data.end(), [&language](const LanguageCulture& entry) {
        return entry.language == language;  // Return true if the language matches
    }) != data.end();  // Return true if the language is found, false otherwise
}
// Function to find languages with no greeting
/**
 * @brief Finds and prints languages that have no greeting.
 *
 * This function iterates through a vector of LanguageCulture objects and checks if the greeting
 * for each language is empty. If a LanguageCulture object with an empty greeting is found, it
 * prints the language name. If all languages have greetings, it prints a message indicating that
 * all languages have greetings.
 *
 * @param data A vector of LanguageCulture objects to be checked for missing greetings.
 *
 * The function performs the following steps:
 * 1. Prints a header indicating the search for languages with no greeting.
 * 2. Iterates through each entry in the data vector.
 * 3. Checks if the greeting of the current entry is empty.
 * 4. Prints the language name if the greeting is empty.
 * 5. If no languages with empty greetings are found, prints a message indicating that all languages have greetings.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello"}, {"Spanish", ""}, {"French", "Bonjour"}};
 * findLanguagesWithNoGreeting(data);
 * // Output:
 * // Languages with no greeting:
 * // Language: Spanish
 * @endcode
 */
// Function to find languages with no greeting
/**
 * @brief Finds and prints languages that have no greeting.
 *
 * This function iterates through a vector of LanguageCulture objects and checks if the greeting
 * for each language is empty. If a LanguageCulture object with an empty greeting is found, it
 * prints the language name. If all languages have greetings, it prints a message indicating that
 * all languages have greetings.
 *
 * @param data A vector of LanguageCulture objects to be checked for missing greetings.
 *
 * The function performs the following steps:
 * 1. Prints a header indicating the search for languages with no greeting.
 * 2. Iterates through each entry in the data vector.
 * 3. Checks if the greeting of the current entry is empty.
 * 4. Prints the language name if the greeting is empty.
 * 5. If no languages with empty greetings are found, prints a message indicating that all languages have greetings.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello"}, {"Spanish", ""}, {"French", "Bonjour"}};
 * findLanguagesWithNoGreeting(data);
 * // Output:
 * // Languages with no greeting:
 * // Language: Spanish
 * @endcode
 */
void findLanguagesWithNoGreeting(const vector<LanguageCulture>& data) {
    // Print a header indicating the search for languages with no greeting
    cout << "Languages with no greeting:\n";

    // Iterate through each entry in the data vector
    for (const auto& entry : data) {
        // Check if the greeting of the current entry is empty
        if (entry.greeting.empty()) {
            // Print the language name if the greeting is empty
            cout << "Language: " << entry.language << endl;
        }
    }
}


// Function to update greeting of an existing entry by language
// Function to update the greeting of an existing entry by language
void updateGreetingByLanguage(vector<LanguageCulture>& data, const string& language) {
    // Use find_if to search for the language in the data vector
    auto it = find_if(data.begin(), data.end(), [&language](const LanguageCulture& entry) {
        return entry.language == language;  // Return true if the language matches
    });

    // Check if the language was found
    if (it != data.end()) {
        // If found, prompt the user to enter a new greeting
        cout << "Updating greeting for language: " << language << endl;
        cout << "Enter new greeting: ";
        getline(cin, it->greeting);  // Read the new greeting from the user
        cout << "Greeting updated successfully." << endl;  // Confirm the update
    } else {
        // If not found, print a message indicating that
        cout << "Language '" << language << "' not found." << endl;
    }
}
// Function to filter languages by length of greeting
/**
 * @brief Filters languages by the length of their greeting.
 *
 * This function iterates through a vector of LanguageCulture objects and checks if the length
 * of the greeting for each language is at least the specified minimum length. It prints the
 * languages that meet this criterion along with their greetings.
 *
 * @param data A vector of LanguageCulture objects to be filtered.
 * @param minLength The minimum length of the greeting to filter by.
 *
 * The function performs the following steps:
 * 1. Prints a header indicating the minimum greeting length.
 * 2. Iterates through each entry in the data vector.
 * 3. Checks if the length of the greeting of the current entry is at least the specified minimum length.
 * 4. Prints the language and greeting if the length criterion is met.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello"}, {"Spanish", "Hola"}, {"French", "Bonjour"}};
 * filterByGreetingLength(data, 5);
 * // Output:
 * // Languages with a greeting of at least 5 characters:
 * // Language: English, Greeting: Hello
 * // Language: French, Greeting: Bonjour
 * @endcode
 */
// Function to filter languages by length of greeting
/**
 * @brief Filters languages by the length of their greeting.
 *
 * This function iterates through a vector of LanguageCulture objects and checks if the length
 * of the greeting for each language is at least the specified minimum length. It prints the
 * languages that meet this criterion along with their greetings.
 *
 * @param data A vector of LanguageCulture objects to be filtered.
 * @param minLength The minimum length of the greeting to filter by.
 *
 * The function performs the following steps:
 * 1. Prints a header indicating the minimum greeting length.
 * 2. Iterates through each entry in the data vector.
 * 3. Checks if the length of the greeting of the current entry is at least the specified minimum length.
 * 4. Prints the language and greeting if the length criterion is met.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello"}, {"Spanish", "Hola"}, {"French", "Bonjour"}};
 * filterByGreetingLength(data, 5);
 * // Output:
 * // Languages with a greeting of at least 5 characters:
 * // Language: English, Greeting: Hello
 * // Language: French, Greeting: Bonjour
 * @endcode
 */




void filterByGreetingLength(const vector<LanguageCulture>& data, size_t minLength) {
    // Print a header indicating the minimum greeting length
    cout << "Languages with a greeting of at least " << minLength << " characters:\n";

    // Iterate through each entry in the data vector
    for (const auto& entry : data) {
        // Check if the length of the greeting of the current entry is at least the specified minimum length
        if (entry.greeting.length() >= minLength) {
            // Print the language and greeting if the length criterion is met
            cout << "Language: " << entry.language << ", Greeting: " << entry.greeting << endl;
        }
    }
}







// Function to count languages containing a keyword in their greeting
// Function to count languages containing a keyword in their greeting
/**
 * @brief Counts the number of languages that contain a specific keyword in their greeting.
 *
 * This function iterates through a vector of LanguageCulture objects and checks if the greeting
 * of each language contains the specified keyword. It counts the number of languages that contain
 * the keyword in their greeting and prints the count.
 *
 * @param data A vector of LanguageCulture objects to be checked.
 * @param keyword The keyword to search for in the greetings.
 *
 * The function performs the following steps:
 * 1. Initializes a count variable to zero.
 * 2. Iterates through each entry in the data vector.
 * 3. Checks if the greeting of the current entry contains the keyword.
 * 4. Increments the count if the keyword is found in the greeting.
 * 5. Prints the total count of languages with the keyword in their greeting.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Hello"}, {"Spanish", "Hola"}, {"French", "Bonjour"}};
 * countLanguagesWithKeywordInGreeting(data, "lo");
 * // Output:
 * // Number of languages with the keyword 'lo' in their greeting: 1
 * @endcode
 */
void countLanguagesWithKeywordInGreeting(const vector<LanguageCulture>& data, const string& keyword) {
    int count = 0;  // Initialize count variable to zero

    // Iterate through each entry in the data vector
    for (const auto& entry : data) {
        // Check if the greeting of the current entry contains the keyword
        if (entry.greeting.find(keyword) != string::npos) {
            count++;  // Increment the count if the keyword is found in the greeting
        }
    }

    // Print the total count of languages with the keyword in their greeting
    cout << "Number of languages with the keyword '" << keyword << "' in their greeting: " << count << endl;
}

// Function to check if any language lacks a greeting
/**
 * @brief Checks for missing greetings in a list of LanguageCulture objects.
 *
 * This function iterates through a vector of LanguageCulture objects and checks if any of them
 * have an empty greeting. If a LanguageCulture object with an empty greeting is found, it prints
 * a message indicating which language is missing a greeting. If all languages have greetings,
 * it prints a message indicating that all languages have greetings.
 *
 * @param data A vector of LanguageCulture objects to be checked for missing greetings.
 */
void checkForMissingGreetings(const vector<LanguageCulture>& data) {
    bool missingGreeting = false;
    for (const auto& entry : data) {
        if (entry.greeting.empty()) {
            cout << "Language '" << entry.language << "' is missing a greeting." << endl;
            missingGreeting = true;
        }
    }

    if (!missingGreeting) {
        cout << "All languages have greetings." << endl;
    }
}
// Function to print unique cultural norms in the dataset
// Function to print unique cultural norms in the dataset
/**
 * @brief Prints unique cultural norms from a given dataset.
 *
 * This function takes a vector of LanguageCulture objects and extracts unique cultural norms
 * from the dataset. It uses a set to store the unique cultural norms and then prints them.
 *
 * @param data A vector of LanguageCulture objects containing cultural norms.
 *
 * The function performs the following steps:
 * 1. Initializes a set to store unique cultural norms.
 * 2. Iterates through each entry in the data vector.
 * 3. Inserts the cultural norm of the current entry into the set.
 * 4. Prints the unique cultural norms stored in the set.
 *
 * Example usage:
 * @code
 * vector<LanguageCulture> data = {{"English", "Individualism"}, {"Japanese", "Collectivism"}, {"English", "Individualism"}};
 * printUniqueCulturalNorms(data);
 * // Output:
 * // Unique cultural norms:
 * // Individualism
 * // Collectivism
 * @endcode
 */
void printUniqueCulturalNorms(const vector<LanguageCulture>& data) {
    set<string> uniqueNorms;  // Set to store unique cultural norms

    // Iterate through each entry in the data vector
    for (const auto& entry : data) {
        // Insert the cultural norm of the current entry into the set
        uniqueNorms.insert(entry.culturalNorm);
    }

    // Print the unique cultural norms
    cout << "Unique cultural norms:\n";
    for (const auto& norm : uniqueNorms) {
        cout << norm << endl;  // Print each unique cultural norm
    }
}





// Interactive menu
void menu() {
    string filename = "extended_language_culture.txt";
    string backupFile = "backup_language_culture.txt";
    string reportFile = "language_culture_report.txt";
    vector<LanguageCulture> languageData = readFromFile(filename);

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Display Data\n";
        cout << "2. Search Language\n";
        cout << "3. Sort Data\n";
        cout << "4. Filter by Cultural Norm Keyword\n";
        cout << "5. Delete Entry\n";
        cout << "6. Update Entry\n";
        cout << "7. Backup Data\n";
        cout << "8. Restore Data\n";
        cout << "9. Display Statistics\n";
        cout << "10. Export to Report\n";
        cout << "11. Find Most Common Keyword\n";
        cout << "12. Count Entries by Language\n";          // New option
        cout << "13. Find Language with Longest Greeting\n";  // New option
        cout << "14. Check if Language Exists\n";            // New option
        cout << "15. Find Languages with No Greeting\n";
        cout << "16. Update Greeting by Language\n";  // New option
        cout << "17. Filter by Greeting Length\n";  // New option
        cout << "18. Count Languages with Keyword in Greeting\n";  // New option
     // New option
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            displayData(languageData);
            break;
        case 2: {
            string language;
            cout << "Enter language to search: ";
            getline(cin, language);
            searchLanguage(languageData, language);
            break;
        }
        case 3:
            sortData(languageData);
            break;
        case 4: {
            string keyword;
            cout << "Enter keyword to filter by: ";
            getline(cin, keyword);
            filterByCulturalNorm(languageData, keyword);
            break;
        }
        case 5: {
            string language;
            cout << "Enter language to delete: ";
            getline(cin, language);
            deleteEntry(languageData, language);
            break;
        }
        case 6: {
            string language;
            cout << "Enter language to update: ";
            getline(cin, language);
            updateEntry(languageData, language);
            break;
        }
        case 7:
            backupData(languageData, backupFile);
            break;
        case 8:
            restoreData(languageData, backupFile);
            break;
        case 9:
            displayStatistics(languageData);
            break;
        case 10:
            exportToReport(languageData, reportFile);
            break;
        case 11:
            findMostCommonKeyword(languageData);
            break;
        case 12:
            countEntriesByLanguage(languageData);  // New case
            break;
        case 13:
            findLanguageWithLongestGreeting(languageData);  // New case
            break;
        case 14: {
            string language;
            cout << "Enter language to check: ";
            getline(cin, language);
            if (languageExists(languageData, language)) {
                cout << "Language '" << language << "' exists in the data." << endl;
            } else {
                cout << "Language '" << language << "' does not exist in the data." << endl;
            }
            break;
        }
        case 15:
            findLanguagesWithNoGreeting(languageData);  // New case
            break;
        case 16: {
            string language;
            cout << "Enter language to update greeting: ";
            getline(cin, language);
            updateGreetingByLanguage(languageData, language);
            break;
        }
        case 17: {
            size_t minLength;
            cout << "Enter minimum greeting length: ";
            cin >> minLength;
            cin.ignore();
            filterByGreetingLength(languageData, minLength);
            break;
        }
        case 18: {
            string keyword;
            cout << "Enter keyword to search in greeting: ";
            getline(cin, keyword);
            countLanguagesWithKeywordInGreeting(languageData, keyword);
            break;
        }

        case 0:
            cout << "Exiting the program.\n";
            break;
        default:
            cout << "Invalid choice, please try again.\n";
        }
    } while (choice != 0);
}











int main() {
     Graph system(20); // Updated the size to accommodate more cities
    // Add cities
    system.addCity(0, "Delhi");
    system.addCity(1, "Mumbai");
    system.addCity(2, "Chennai");
    system.addCity(3, "Kolkata");
    system.addCity(4, "Bangalore");
    system.addCity(5, "Hyderabad");
    system.addCity(6, "Pune");
    system.addCity(7, "Ahmedabad");
    system.addCity(8, "Jaipur");
    system.addCity(9, "Lucknow");
    system.addCity(10, "Chandigarh");
    system.addCity(11, "Bhopal");
    system.addCity(12, "Indore");
    system.addCity(13, "Nagpur");
    system.addCity(14, "Patna");
    system.addCity(15, "Guwahati");
    system.addCity(16, "Bhubaneswar");
    system.addCity(17, "Thiruvananthapuram");
    system.addCity(18, "Vishakhapatnam");
    system.addCity(19, "Coimbatore");

    // Add routes
    system.addRoute(0, 1, 1400);
    system.addRoute(1, 2, 1300);
    system.addRoute(2, 3, 1650);
    system.addRoute(3, 0, 1500);
    system.addRoute(0, 4, 2100);
    system.addRoute(1, 5, 700);
    system.addRoute(6, 1, 150);
    system.addRoute(7, 1, 530);
    system.addRoute(8, 0, 280);
    system.addRoute(9, 0, 530);
    system.addRoute(10, 0, 250);
    system.addRoute(11, 1, 780);
    system.addRoute(12, 1, 600);
    system.addRoute(13, 1, 790);
    system.addRoute(14, 0, 950);
    system.addRoute(15, 3, 1000);
    system.addRoute(16, 3, 450);
    system.addRoute(17, 2, 760);
    system.addRoute(18, 2, 800);
    system.addRoute(19, 2, 500);

    // Add modes
    system.addMode(1, "Bus", 50, true);
    system.addMode(2, "Train", 300, true);
    system.addMode(3, "Flight", 200, true);
    system.addMode(4, "Car", 4, true);
    system.addMode(5, "Taxi", 4, true);
    system.addMode(6, "E-Bike", 1, true);
    system.addMode(7, "Ship", 1000, true);
    system.addMode(8, "Auto", 100, true);
    Tourist obj;
    obj.addTouristPlaces();
    Graph1 g;
    obj.addEdgesToGraph(g);


int ch;
cout<< "                                                                                                            \n";
cout<<"----------------------------------------------------------------------------\n";
cout << "                   'TOURISM MANAGEMENT'                    \n";
cout<<"----------------------------------------------------------------------------\n";

cout <<"\n                          \n";
do
{

cout<<"\n-------MAIN MENU------\n";
cout<<"                                        \n";
cout<<"1.TOURIST PLACES:\n";
cout<<"2.TOURIST TRANSPORATION:\n";
cout<<"3.TOURIST INTERNET AND DIGITAL CONNECTION :\n";
cout<<"4.TOURIST LANGUAGE AND CULTURAL BARRIES:\n";
cout<<"5.EXIT\n";
cout<<"enter your choice : \n";
cin>>ch;
switch(ch)
{
case 1:
        {

        int choice;
        do{
            cout << "                              \t\t WELCOME TO TOURIST PLACES \n\n\n";
            cout << "                                   \t    TOURIST SOFTWARE\n\n\n";

    if (cin.get() == '\n') {

        int choice;
        Tourist obj;
        obj.addTouristPlaces();
        Graph1 g1;
        obj.addEdgesToGraph(g1);  // Set up the graph with distances between tourist places

        string currentLocation;
        cout << "Enter your current location: ";
        cin.ignore();  // To ignore the leftover newline character from previous input
        getline(cin, currentLocation);

        while (1) {
            cout << "\nOptions:\n";
            cout << "1. Display Tourist Place Names\n";
            cout << "2. Display Cost of Tourist Places\n";
            cout << "3. Display Crowd Status of Tourist Places\n";
            cout << "4. Display Rating of Tourist Places\n";
            cout << "5. Display Locations of Tourist Places\n";
            cout << "6. Select Places to Visit\n";
            cout << "7. Calculate Travel Map\n";
            cout << "8. Calculate Shortest Path from Current Location\n";
            cout << "9. Display Distances from Current Location to All Places\n";
            cout << "10. Display Places by Crowd Status\n";
            cout << "11. Filter Places by Budget\n";
            cout << "12. Display Highest Rated Places\n";
            cout << "13. Add a New Tourist Place\n";
            cout << "14. View Details of a Specific Tourist Place\n";
            cout << "15. Sort Tourist Places by Rating\n";
            cout << "16. Filter Places by Crowd Status\n";

            cout << "17. Sort Places by Cost\n";
            cout << "18. Display Most Expensive Place\n";
            cout << "19. Display Cheapest Place\n";
            cout << "20. Display Places by Rating Range\n";
            cout << "21. Display Places by Location\n";

            cout << "22. Find Places by Name\n";
            cout << "23. Display Places by Popularity\n";
            cout << "24. Plan a Custom Tour\n";
            cout << "25. Display Places by Accessibility\n";
            cout << "26. Display Places by Seasonal Preferences\n";
            cout << "27. Generate Tourist Activities\n";
            cout << "28. Sort Places By Cost\n";
            cout << "29. display Places With Activity\n";

            cout << "Enter any other key for exit\n\n";

            cout << "Enter Your Choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    obj.displayName();
                    break;
                case 2:
                    obj.displayCost();
                    break;
                case 3:
                    obj.displayCrowdStatus();
                    break;
                case 4:
                    obj.displayRating();
                    break;
                case 5:
                    obj.displayLocation();
                    break;
                case 6:
                    obj.selectTouristPlaces();
                    break;
                case 7:
                    obj.travelMap(g1);
                    break;
                case 8:
                    obj.calculateAndDisplayShortestPath(currentLocation, g1);
                    break;
                case 9:
                    obj.displayDistancesFromCurrentLocation(currentLocation, g1);
                    break;

// Add a case for the new functionality

// Add a case for the new functionality
                case 10:
                    obj.displayPlacesByCrowdStatus();
                    break;

// Add a case for the new functionality
                case 11:
                    obj.filterPlacesByBudget();
                    break;

// Add a case for the new functionality
                case 12:
                    obj.displayHighestRatedPlaces();
                    break;

// Add a case for the new functionality
                case 13:
                    obj.addNewPlace();
                    break;
                case 14:
                    obj.viewPlaceDetails();
                    break;

                case 15:
                    obj.sortByRating();
                    break;
                case 16:
                    obj.filterByCrowdStatus();
                    break;
                case 17:
                    obj.sortByCost();
                    break;
                case 18:
                    obj.displayMostExpensivePlace();
                    break;
                case 19:
                    obj.displayCheapestPlace();
                    break;
                case 20:
                    obj.displayPlacesByRatingRange();
                    break;
                case 21:
                    obj.displayPlacesByLocation();
                    break;

                case 22:
                    obj.findPlacesByName();
                    break;
                case 23:
                    obj.displayPlacesByPopularity();
                    break;
                case 24:
                    obj.planCustomTour();
                    break;
                case 25:
                    obj.displayPlacesByAccessibility();
                    break;
                case 26:
                    obj.displayPlacesBySeason();
                    break;
                case 27:
                    obj.generateTouristActivities();
                    break;
                case 28:
                    obj.sortPlacesByCost();
                    break;
                case 29:
                    obj.displayPlacesWithActivity();
                    break;
                case 30:
                    obj.displayPlacesInProximity();
                    break;
                default:
                    exit(0);
            }
        }
    }
    }while (choice != 4);
    }
    break;

case 2:
        {
            int choice;
            do{
                      cout << "                              \t\t WELCOME TO TOURIST TRANSPORT \n\n\n";
                      cout << "                                   \t    TOURIST SOFTWARE\n\n\n";

         int choice;
         do {
        // Display menu options
        cout << "\n--- Transportation Management System ---\n";
        cout << "1. View Cities\n";
        cout << "2. View Transportation Modes\n";
        cout << "3. Add Booking\n";
        cout << "4. View Bookings\n";
        cout << "5. Find Shortest Path\n";
        cout << "6. Update Booking Status\n";
        cout << "7. Delete Booking\n";
        cout << "8. Update Mode Availability\n";
        cout << "9. View Routes\n";
        cout << "10. Display City Routes\n";
        cout << "11. Display Available Modes\n";
        cout << "12. Count Bookings\n";
        cout << "13. Find Longest Path\n";
        cout << "14. Display City Details\n";
        cout << "15. Update City Name\n";
        cout << "16. Update Route Distance\n";
        cout << "17. Delete City\n";
        cout << "18. Display Total Cities\n";
        cout << "19. Display Total Routes\n";
        cout << "20. Display Total Modes\n";
        cout << "21. Display Booking Details\n";
        cout << "22. Add New City\n";
        cout << "23. Add New Mode\n";
        cout << "24. Remove Route\n";
        cout << "25. Display City Names\n";
        cout << "26. Display Mode Details\n";
        cout << "27. Update Mode Capacity\n";
        cout << "28. Display City Count\n";
        cout << "29. Display Mode Count\n";
        cout << "30. Display Booking Count\n";
        cout << "31. Display Route Count\n";
        cout << "32. Display Most Connected City\n";
        cout << "33. Display Least Connected City\n";
        cout << "34. Find All Paths\n";
        cout << "35. Find Cheapest Path\n";
        cout << "36. Display Isolated Cities\n";
        cout << "37. Display Direct Routes\n";
        cout << "38. Display Cities by Mode\n";
        cout << "39. Get City ID by Name\n";
        cout << "40. Display City with Longest Route\n";
        cout << "41. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                system.displayCities();
                break;
            case 2:
                system.displayModes();
                break;
            case 3: {
                string userName, mode;
                int src, dest;
                cout << "Enter your name: ";
                cin >> ws;
                getline(cin, userName);
                cout << "Enter source city ID: ";
                cin >> src;
                cout << "Enter destination city ID: ";
                cin >> dest;
                cout << "Enter mode of transport (e.g., Bus, Train, Flight, Car, Taxi, E-Bike, Ship, Auto): ";
                cin >> ws;
                getline(cin, mode);
                system.bookTicket(userName, src, dest, mode);
                break;
            }
            case 4:
                system.viewBookings();
                break;
            case 5: {
                int src, dest;
                cout << "Enter source city ID: ";
                cin >> src;
                cout << "Enter destination city ID: ";
                cin >> dest;
                system.findShortestPath(src, dest);
                break;
            }
            case 6: {
                int bookingID;
                string newStatus;
                cout << "Enter booking ID: ";
                cin >> bookingID;
                cout << "Enter new status (e.g., Confirmed, Cancelled): ";
                cin >> ws;
                getline(cin, newStatus);
                system.updateBookingStatus(bookingID, newStatus);
                break;
            }
            case 7: {
                int bookingID;
                cout << "Enter booking ID to delete: ";
                cin >> bookingID;
                system.deleteBooking(bookingID);
                break;
            }
            case 8: {
                int modeID;
                bool availability;
                cout << "Enter mode ID: ";
                cin >> modeID;
                cout << "Enter availability (1 for Yes, 0 for No): ";
                cin >> availability;
                system.updateModeAvailability(modeID, availability);
                break;
            }
            case 9:
                system.displayRoutes();
                break;
            case 10: {
                int cityID;
                cout << "Enter city ID to display routes: ";
                cin >> cityID;
                system.displayCityRoutes(cityID);
                break;
            }
            case 11:
                system.displayAvailableModes();
                break;
            case 12:
                system.countBookings();
                break;
            case 13: {
                int src, dest;
                cout << "Enter source city ID: ";
                cin >> src;
                cout << "Enter destination city ID: ";
                cin >> dest;
                system.findLongestPath(src, dest);
                break;
            }
            case 14: {
                int cityID;
                cout << "Enter city ID to display details: ";
                cin >> cityID;
                system.displayCityDetails(cityID);
                break;
            }
            case 15: {
                int cityID;
                string newName;
                cout << "Enter city ID to update name: ";
                cin >> cityID;
                cout << "Enter new city name: ";
                cin >> ws;
                getline(cin, newName);
                system.updateCityName(cityID, newName);
                break;
            }
            case 16: {
                int src, dest, newDistance;
                cout << "Enter source city ID: ";
                cin >> src;
                cout << "Enter destination city ID: ";
                cin >> dest;
                cout << "Enter new route distance: ";
                cin >> newDistance;
                system.updateRouteDistance(src, dest, newDistance);
                break;
            }
            case 17: {
                int cityID;
                cout << "Enter city ID to delete: ";
                cin >> cityID;
                system.deleteCity(cityID);
                break;
            }
            case 18:
                system.displayTotalCities();
                break;
            case 19:
                system.displayTotalRoutes();
                break;
            case 20:
                system.displayTotalModes();
                break;
            case 21: {
                int bookingID;
                cout << "Enter booking ID to display details: ";
                cin >> bookingID;
                system.displayBookingDetails(bookingID);
                break;
            }
            case 22: {
                int cityID;
                string name;
                cout << "Enter new city ID: ";
                cin >> cityID;
                cout << "Enter city name: ";
                cin >> ws;
                getline(cin, name);
                system.addNewCity(cityID, name);
                break;
            }
            case 23: {
                int modeID, capacity;
                string name;
                bool availability;
                cout << "Enter new mode ID: ";
                cin >> modeID;
                cout << "Enter mode name: ";
                cin >> ws;
                getline(cin, name);
                cout << "Enter mode capacity: ";
                cin >> capacity;
                cout << "Enter availability (1 for Yes, 0 for No): ";
                cin >> availability;
                system.addNewMode(modeID, name, capacity, availability);
                break;
            }
            case 24: {
                int src, dest;
                cout << "Enter source city ID: ";
                cin >> src;
                cout << "Enter destination city ID: ";
                cin >> dest;
                system.removeRoute(src, dest);
                break;
            }
            case 25:
                system.displayCityNames();
                break;
            case 26: {
                int modeID;
                cout << "Enter mode ID to display details: ";
                cin >> modeID;
                system.displayModeDetails(modeID);
                break;
            }
            case 27: {
                int modeID, newCapacity;
                cout << "Enter mode ID to update capacity: ";
                cin >> modeID;
                cout << "Enter new capacity: ";
                cin >> newCapacity;
                system.updateModeCapacity(modeID, newCapacity);
                break;
            }
            case 28:
                system.displayCityCount();
                break;
            case 29:
                system.displayModeCount();
                break;
            case 30:
                system.displayBookingCount();
                break;
            case 31:
                system.displayRouteCount();
                break;
            case 32:
                system.displayMostConnectedCity();
                break;
            case 33:
                system.displayLeastConnectedCity();
                break;
            case 34: {
                int src, dest;
                cout << "Enter source city ID: ";
                cin >> src;
                cout << "Enter destination city ID: ";
                cin >> dest;
                system.findAllPaths(src, dest);
                break;
            }
            case 35: {
                int src, dest;
                cout << "Enter source city ID: ";
                cin >> src;
                cout << "Enter destination city ID: ";
                cin >> dest;
                system.findCheapestPath(src, dest);
                break;
            }
            case 36:
                system.displayIsolatedCities();
                break;
            case 37: {
                int cityID;
                cout << "Enter city ID to display direct routes: ";
                cin >> cityID;
                system.displayDirectRoutes(cityID);
                break;
            }
            case 38: {
                string modeName;
                cout << "Enter mode of transport (e.g., Bus, Train): ";
                cin >> ws;
                getline(cin, modeName);
                system.displayCitiesByMode(modeName);
                break;
            }
            case 39: {
                string cityName;
                cout << "Enter city name to get its ID: ";
                cin >> ws;
                getline(cin, cityName);
                system.getCityIDByName(cityName);
                break;
            }
            case 40:
                system.displayCityWithLongestRoute();
                break;
            case 41:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
           }
        } while (choice != 41);

       return 0;






        }while (choice != 4);
     break;
        }
case 3:{
            cout << "                              \t\t WELCOME TO TOURIST INTERNET AND DIGITAL CONNECTIVITY \n\n\n";
            cout << "                                           \t    TOURIST SOFTWARE\n\n\n";

       vector<Location> locations = {

    {"Sampige Road", 3, false, false, "Sampige Road Metro Station"},
    {"Bengaluru North", 1, false, false, "Bengaluru North Metro Station"},
    {"Yelahanka New Town", 2, false, false, "Yelahanka New Town Metro Station"},
    {"Marathahalli", 2, false, false, "Marathahalli Metro Station"},
    {"Bellandur", 1, false, false, "Bellandur Metro Station"},
    {"Bangalore IT Hub", 2, false, false, "Bangalore IT Hub Metro Station"},
    {"Hosur Road", 3, false, false, "Hosur Road Metro Station"},
    {"Indiranagar Extension", 1, false, false, "Indiranagar Extension Metro Station"},
    {"Agrahara", 2, false, false, "Agrahara Metro Station"},
    {"Kadubeesanahalli", 3, false, false, "Kadubeesanahalli Metro Station"},
    {"Koramangala", 3, false, false, "Koramangala Metro Station"},
    {"Electronic City", 2, false, false, "Electronic City Metro Station"},
    {"Whitefield", 3, false, false, "Whitefield Metro Station"},
    {"Hebbal", 1, false, false, "Hebbal Metro Station"},
    {"Jayanagar", 2, false, false, "Jayanagar Metro Station"},
    {"Banashankari", 1, false, false, "Banashankari Metro Station"},
    {"MG Road", 3, false, false, "MG Road Metro Station"},
    {"Lalbagh", 1, false, false, "Lalbagh Metro Station"},
    {"KR Market", 2, false, false, "KR Market Metro Station"},
    {"Peenya", 2, false, false, "Peenya Metro Station"},
    {"Silk Institute", 1, false, false, "Silk Institute Metro Station"},
    {"Vajarahalli", 1, false, false, "Vajarahalli Metro Station"},
    {"Thalaghattapura", 1, false, false, "Thalaghattapura Metro Station"},
    {"National College", 1, false, false, "National College Metro Station"},
    {"Lalbagh", 1, false, false, "Lalbagh Metro Station"},
    {"South End Circle", 1, false, false, "South End Circle Metro Station"},
    {"Jayaprakash Nagar", 1, false, false, "Jayaprakash Nagar Metro Station"},
    {"Yelachenahalli", 1, false, false, "Yelachenahalli Metro Station"},
    {"Konanakunte Cross", 1, false, false, "Konanakunte Cross Metro Station"},
    {"Doddakallasandra", 1, false, false,"Doddakallasandra Metro Station"}};



    bool programRunning = true;
    while (programRunning) {
        displayOptions();
        int option;
        cout << "Enter your choice: ";
        cin >> option;
        cin.ignore();
        performAction(option, locations);

        cout << "Do you want to perform another action? (yes/no): ";
        string continueChoice;
        getline(cin, continueChoice);
        if (continueChoice != "yes" && continueChoice != "Yes") {
            programRunning = false;
        }
    }

    return 0;

    }
case 4:{
            cout << "                              \t\t WELCOME TO TOURIST LANGUAGE AND CULTURAL BARRIERS \n\n\n";
            cout << "                                          \t    TOURIST SOFTWARE\n\n\n";

          menu();
           return 0;

          }

case 5 :{
          exit(0);


       }
          default: cout<<"\n enter valid choice\n";
           break;


  }

 }while (ch!=4);
}


