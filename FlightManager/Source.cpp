#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;

bool input_done = false;
int globalFlightID = 0; // Global flight ID counter

struct flight {
    string from;
    string to;
    int hour;
    int min;
    int price;
    int ID;

    flight* next;
    flight* prev;

    //TO DO: implement constructor
    //Hint: You may need to implement multiple constructors

    // Default constructor
    flight() : from(""), to(""), hour(0), min(0), price(0), next(nullptr), prev(nullptr) {}

    // Constructor with so many parameters
    flight(string f, string t, int h, int m, int p) : from(f), to(t), hour(h), min(m), price(p), next(nullptr), prev(nullptr) {}

    void setID(int ID) {
        this->ID = ID;
    }
};

struct airline {
    string name;
    int ID;
    airline* next;
    flight* flights;

    //TO DO: implement constructor
    //Hint: You may need to implement multiple constructors

    // Default constructor
    airline() : name(""), ID(0), next(nullptr), flights(nullptr) {}

    // Constructor with not so many parameters
    airline(string n, int id) : name(n), ID(id), next(nullptr), flights(nullptr) {}

};


pair<vector<string>, vector<vector<flight>>> read_files(bool& input_done) {
    vector<string> airlines;
    vector<vector<flight>> flights(100); // Start with a sufficiently sized vector

    while (!input_done) {
        string airline_name;
        string filename;

        cout << "Enter the name of airline: ";
        cin >> airline_name;
        cout << "Enter the name of airline file: ";
        cin >> filename;

        // Handle "done" immediately
        if (filename == "done") {
            input_done = true; // Mark input as done
            //cout << "Done entered, stopping file reading." << endl; // Debug
            break;             // Exit the loop immediately
            
        }

        ifstream input(filename);
        if (input.fail()) {
            cout << "Could not open file: " << filename << endl;
            continue; // Skip to the next iteration for valid input
        }

        airlines.push_back(airline_name); // Add the airline name to the vector
        cout << "Processing input file: " << filename << endl;

        int line_no = 0;
        vector<string> lines;
        string line;
        int vector_location = airlines.size() - 1;
        
        while (getline(input, line)) {
            lines.push_back(line);
            line_no++;

            if (line_no % 5 == 0) {
                flight a_flight(lines[line_no - 5], lines[line_no - 4], stoi(lines[line_no - 3]), stoi(lines[line_no - 2]), stoi(lines[line_no - 1]));
                a_flight.setID(globalFlightID++); // Assign global ID
                //DEBUG
                //cout << "globalFlightID updated: " << globalFlightID << " (Debug: after line 100)" << endl;

                flights[vector_location].push_back(a_flight);
            }
        }
    }

    if (input_done) {
        cout << "Input files are already read, however you can do manual changes.." << endl;
    }

    return make_pair(airlines, flights);
}






airline* make_linked_list_structure(vector<string> airlines, vector<vector<flight>> flights) {
    airline* head = nullptr; // Head of the airline linked list

    for (int i = 0; i < airlines.size(); i++) {
        // Create a new airline node
        airline* newAirline = new airline(airlines[i], i);

        if (head == nullptr) {
            head = newAirline; // Set head if the list is empty
        }
        else {
            airline* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next; // Traverse to the end of the list
            }
            temp->next = newAirline; // Append the new airline
        }

        for (int j = 0; j < flights[i].size(); j++) {
            // Create a new flight node
            flight* newFlight = new flight(flights[i][j].from, flights[i][j].to, flights[i][j].hour, flights[i][j].min, flights[i][j].price);
            newFlight->setID(flights[i][j].ID); // Use the ID assigned in read_files
            //DEBUG
            //cout << "Using existing globalFlightID: " << flights[i][j].ID << " (Debug: after line 142)" << endl;

            // Insert the flight in sorted order by time
            if (newAirline->flights == nullptr) {
                newAirline->flights = newFlight; // First flight in the list
            }
            else {
                flight* tempFlight = newAirline->flights;
                flight* prevFlight = nullptr;

                // Find the correct position based on time
                while (tempFlight != nullptr && (newFlight->hour > tempFlight->hour ||
                    (newFlight->hour == tempFlight->hour && newFlight->min > tempFlight->min))) {
                    prevFlight = tempFlight;
                    tempFlight = tempFlight->next;
                }

                if (prevFlight == nullptr) {
                    // Insert at the head of the flight list
                    newFlight->next = newAirline->flights;
                    newAirline->flights->prev = newFlight;
                    newAirline->flights = newFlight;
                }
                else if (tempFlight == nullptr) {
                    // Insert at the end of the flight list
                    prevFlight->next = newFlight;
                    newFlight->prev = prevFlight;
                }
                else {
                    // Insert in the middle of the list
                    prevFlight->next = newFlight;
                    newFlight->prev = prevFlight;
                    newFlight->next = tempFlight;
                    tempFlight->prev = newFlight;
                }
            }
        }

        
    }
    return head;
}

void print_all(airline* head) {
    for (airline* currentAirline = head; currentAirline != nullptr; currentAirline = currentAirline->next) { //Iterate over airlines
        cout << "###################################" << endl;
        cout << "### AIRLINE ID: " << currentAirline->ID << " ###" << endl;
        cout << "NAME: " << currentAirline->name << endl;
        cout << "FLIGHTS: ";
        for (flight* currentFlight = currentAirline->flights; currentFlight != nullptr; currentFlight = currentFlight->next) { //Iterate over flights
            cout << "#[" << currentFlight->ID << "|" << currentFlight->from << "->" << currentFlight->to << "|" << currentFlight->hour << ":" << currentFlight->min << "|" << currentFlight->price << "TRY]#" << endl;
        }
    }
}

void add_flight_with_input(airline*& head) {
    string airLineToAddName;
    string from, to;
    int hour, min, price;
    //Get the info from user
    cout << "Adding manual entry:" << endl;
    cout << "AIRLINE: "; cin >> airLineToAddName; cout << endl;
    cout << "FROM: "; cin >> from; cout << endl;
    cout << "TO: "; cin >> to; cout << endl;
    cout << "HOUR: "; cin >> hour; cout << endl;
    cout << "MIN: "; cin >> min; cout << endl;
    cout << "PRICE: "; cin >> price; cout << endl;

    //Find the biggest airline ID number to set the airline ID
    airline* airlineToLookBiggestID = head;
    int biggestAirLineID = 0;

    int biggestFligthID = 0;

    while (airlineToLookBiggestID != nullptr) {
        if (airlineToLookBiggestID->ID > biggestAirLineID) {
            biggestAirLineID = airlineToLookBiggestID->ID;
        }
        flight* flightToLookBiggestID = airlineToLookBiggestID->flights;
        while (flightToLookBiggestID != nullptr) {
            if (flightToLookBiggestID->ID > biggestFligthID) {
                biggestFligthID = flightToLookBiggestID->ID;
            }
            flightToLookBiggestID = flightToLookBiggestID->next;
        }
        airlineToLookBiggestID = airlineToLookBiggestID->next;
    }
    int airLineAddID = biggestAirLineID + 1;
    extern int globalFlightID; // Use the global flight ID
    int flightAddID = globalFlightID++;
    //DEBUG
    //cout << "globalFlightID updated: " << globalFlightID << " (Debug: after line 231)" << endl;



    // Create a new flight
    flight* newFlight = new flight(from, to, hour, min, price);

    // Set the id for the flight
    newFlight->setID(flightAddID);


    // Check if the airline list is empty
    if (head == nullptr) {
        // Create a new airline and add the flight
        head = new airline(airLineToAddName, airLineAddID);
        head->flights = newFlight;
    }
    else {
        // Search for the airline in the list
        airline* temp = head;
        while (temp != nullptr && temp->name != airLineToAddName) {
            temp = temp->next;
        }

        // If the airline was found in the list
        if (temp != nullptr) {
            // Add the flight to the existing airline
            flight* current = temp->flights;
            flight* prev = nullptr;
            while (current != nullptr && (hour > current->hour || (hour == current->hour && min > current->min))) {
                prev = current;
                current = current->next;
            }
            if (prev == nullptr) { // Insert at head
                newFlight->next = temp->flights;
                temp->flights = newFlight;
            }
            else { // Insert at middle or end
                newFlight->next = current;
                prev->next = newFlight;
            }
        }
        else {
            // Create a new airline and add the flight
            airline* newAirline = new airline(airLineToAddName, airLineAddID);
            newAirline->flights = newFlight;

            // Add the new airline to the end of the list
            temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newAirline;
        }
    }
    cout << "Flight ID " << newFlight->ID << " is added to the list.." << endl;
}

void remove_flight_with_input(airline*& head) {
    int flightIDToRemove;
    cout << "Flight id to remove: "; cin >> flightIDToRemove; cout << endl;

    airline* tempAirline = head;
    airline* prevAirline = nullptr;
    while (tempAirline != nullptr) { //Iterate over airlines
        flight* tempFlight = tempAirline->flights;
        flight* prevFlight = nullptr;
        while (tempFlight != nullptr && tempFlight->ID != flightIDToRemove) { //Iterate over flights
            prevFlight = tempFlight;
            tempFlight = tempFlight->next;
        }
        if (tempFlight != nullptr) { // Flight found
            if (prevFlight == nullptr) { // Flight is at head of list
                tempAirline->flights = tempFlight->next;
            }
            else { // Flight is in middle or end of list
                prevFlight->next = tempFlight->next;
                if (tempFlight->next != nullptr) { // Not at end of list
                    tempFlight->next->prev = prevFlight;
                }
            }
            delete tempFlight;

            if (tempAirline->flights == nullptr) { // No flights left in this airline
                if (prevAirline == nullptr) { // Airline is at head of list
                    head = tempAirline->next;
                    delete tempAirline;
                    tempAirline = head; // Continue with next airline
                }
                else { // Airline is in middle or end of list
                    prevAirline->next = tempAirline->next;
                    delete tempAirline;
                    tempAirline = prevAirline->next; // Continue with next airline
                }
            }
            else { // Some flights still left in this airline
                prevAirline = tempAirline;
                tempAirline = tempAirline->next; // Continue with next airline
            }
        }
        else { // Flight not found in this airline
            prevAirline = tempAirline;
            tempAirline = tempAirline->next; // Continue with next airline
        }
    }
    cout << "Flight ID " << flightIDToRemove << " is removed from the list.." << endl;
}


//Function to find the cheapest route
pair<int, vector<flight*>> findCheapestRoute(
    airline* head,
    string currentPoint,
    string destination,
    int maxTransfer,
    int currentTransfer,
    int totalCost,
    vector<flight*> currentRoute
) {
    // Base case: Exceeded maximum number of transfers
    if (currentTransfer > maxTransfer) {
        return make_pair(INT_MAX, vector<flight*>()); // Return no route
    }

    // Base case: Reached destination
    if (currentPoint == destination) {
        return make_pair(totalCost, currentRoute); // Return the current route and cost
    }

    // Initialize minimum cost and route
    int minCost = INT_MAX;
    vector<flight*> minRoute;

    airline* tempAirline = head;
    while (tempAirline != nullptr) {
        flight* tempFlight = tempAirline->flights;
        while (tempFlight != nullptr) {
            if (tempFlight->from == currentPoint) { // Only consider flights starting from the current point
                //DEBUG
                /*cout << "Checking flight from " << tempFlight->from << " to " << tempFlight->to
                    << " with current transfers: " << currentTransfer
                    << " and max allowed: " << maxTransfer << endl;*/
                

                // Prepare the updated route
                vector<flight*> newRoute = currentRoute;
                newRoute.push_back(tempFlight);

                // Increment transfer count only if it's not the first flight
                int nextTransfer = currentRoute.empty() ? currentTransfer : currentTransfer + 1;

                // Recursively find the route
                pair<int, vector<flight*>> result = findCheapestRoute(
                    head,
                    tempFlight->to,      // Move to the flight's destination
                    destination,
                    maxTransfer,
                    nextTransfer,        // Update transfer count
                    totalCost + tempFlight->price,
                    newRoute
                );

                // Update minimum cost and route if a better path is found
                if (result.first < minCost) {
                    minCost = result.first;
                    minRoute = result.second;
                }
            }
            tempFlight = tempFlight->next; // Move to the next flight
        }
        tempAirline = tempAirline->next; // Move to the next airline
    }

    return make_pair(minCost, minRoute); // Return the cheapest route found
}




int pathfinder(airline* head) {
    string startPoint, destination;
    int maxTransfer;
    cout << "Where are you now? "; cin >> startPoint; cout << endl;
    cout << "Where do you want to go "; cin >> destination; cout << endl;
    cout << "Maximum number of transfers: "; cin >> maxTransfer; cout << endl;

    //Call the function to find the route
    pair<int, vector<flight*>> result = findCheapestRoute(head, startPoint, destination, maxTransfer, 0, 0, vector<flight*>());

    if (result.first == INT_MAX) { //No valid route found
        cout << "No path found between" << startPoint << " and " << destination;
        return -1;
    }
    else { //Valid route found
        cout << "##Best price path##" << endl;
        cout << "Cheapest route: ";
        for (flight* f : result.second) { //For all the flights in this route
            cout << "[" << f->ID << "|" << f->from << "->" << f->to << "|" << f->hour << ":" << f->min << "|" << f->price << "]";
            if (f->next != nullptr) { //Print "->" if there is another flight after this flight
                cout << "->";
            }
        }
        cout << " $TOTAL PRICE: " << result.first << endl;

        return result.first; //Return the cost
    }
}




void delete_linked_list(airline*& head) {
    if (head) {
        // First, delete the next airlines
        delete_linked_list(head->next);

        // Then, delete the flights of this airline
        flight* currentFlight = head->flights;
        while (currentFlight) {
            flight* nextFlight = currentFlight->next;
            delete currentFlight;
            currentFlight = nextFlight;
        }

        // Finally, delete this airline
        delete head;
    }
    head = nullptr;
    globalFlightID = 0;
}




//
void printMainMenu() {
    cout << endl;
    cout << "I***********************************************I" << endl
        << "I               0 - DELETE DATA                 I" << endl
        << "I               1 - READ FILES                  I" << endl
        << "I               2 - PRINT ALL FLIGHTS           I" << endl
        << "I               3 - ADD FLIGHT                  I" << endl
        << "I               4 - REMOVE FLIGHT               I" << endl
        << "I               5 - PATH FINDER                 I" << endl
        << "I               6 - EXIT                        I" << endl
        << "I***********************************************I" << endl
        << ">>";
    cout << endl;
}

void processMainMenu() {

    pair<vector<string>, vector<vector<flight>>> lines_flights;
    airline* head = nullptr;

    char input;

    do {
        printMainMenu();
        cout << "Please enter your option " << endl;
        cin >> input;
        switch (input) {
        case '0':
            cout << "Commented out functionalities are going to be implemented" << endl;
            delete_linked_list(head);
            cout << "Data is deleted!" << endl;
            input_done = false;

            break;
        case '1':
            if (not input_done) {
                lines_flights = read_files(input_done);
                head = make_linked_list_structure(lines_flights.first, lines_flights.second);
                cout << "Files are read.." << endl;
            }
            else {
                cout << "Files are already read.." << endl;
            }
            input_done = true;

            break;
        case '2':
            cout << "Commented out functionalities are going to be implemented" << endl;
            if (input_done == false) {
                cout << "List is empty.." << endl;
            }
            else {
                print_all(head);
            }

            break;
        case '3':
            cout << "Commented out functionalities are going to be implemented" << endl;
            add_flight_with_input(head);
            break;
        case '4':
            cout << "Commented out functionalities are going to be implemented" << endl;
            remove_flight_with_input(head);
            break;
        case '5':
            cout << "Commented out functionalities are going to be implemented" << endl;
            pathfinder(head);
            break;
        case '6':
            cout << "Exiting.." << endl;
            exit(0);
        default:
            cout << "Invalid option: please enter again" << endl;
        }
    } while (true);
}
//


int main() {

    processMainMenu();
    //Comply with the provided structs and specified linked list structure for a seamless grading

    return 0;

}




