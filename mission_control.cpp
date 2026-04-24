// mission_control.cpp - handles all the menus and user input stuff
#include <iostream>
#include <string>
#include <chrono>
#include <limits>
#include <vector>
#include "city_linkedlist.hpp"
using namespace std;

// forward declarations so functions can call each other
void cityAMenu(LinkedList& cityA);
void cityBMenu(LinkedList& cityB);
void cityCMenu(LinkedList& cityC);
void compareCities(const LinkedList& cityA, const LinkedList& cityB, const LinkedList& cityC);

// only allows valid transport modes for each city, rejects random input
string getValidMode(char cityPrefix) {
    vector<string> modes;
    
        // different cities have different transport options
    if (cityPrefix == 'A') {
        modes = { "Car", "Bus", "Bicycle", "Walking" };
    } else if (cityPrefix == 'B') {
        modes = { "Carpool", "Walking", "Bicycle", "School Bus" };
    } else {
        // city C can use everything
        modes = { "Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool" };
    }
    int modeCount = modes.size();

    while (true) {
        cout << "  Select Mode of Transport:\n";
        for (int i = 0; i < modeCount; ++i)
            cout << "    [" << (i + 1) << "] " << modes[i] << "\n";
        cout << "  Your choice (number or name): ";

        string input;
        getline(cin, input);

        // trim whitespace from input
        size_t s = input.find_first_not_of(" \t");
        size_t e = input.find_last_not_of(" \t");
        if (s == string::npos) { cout << "  [ERROR] Input cannot be empty.\n"; continue; }
        input = input.substr(s, e - s + 1);

        // see if they typed a number or the actual name
        bool isNumber = !input.empty();
        for (char c : input) if (!isdigit(c)) { isNumber = false; break; }
        if (isNumber) {
            int idx = stoi(input);
            if (idx >= 1 && idx <= modeCount)
                return modes[idx - 1];
            cout << "  [ERROR] Please enter a number between 1 and "
                 << modeCount << ".\n";
            continue;
        }

        // also accept the name directly, case doesnt matter
        string lower = input;
        for (char& c : lower) c = (char)tolower(c);

        for (int i = 0; i < modeCount; ++i) {
            string cmp = modes[i];
            for (char& c : cmp) c = (char)tolower(c);
            if (lower == cmp)
                return modes[i]; // return the properly cased version
        }

        cout << "  [ERROR] \"" << input << "\" is not a valid mode for City " << cityPrefix << ".\n";
    }
}

// prevents crash if someone types letters when a number is expected
int getValidIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush newline
            return value;
        } else {
            cout << "  [ERROR] Invalid input. Please enter numbers only.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// same but for doubles
double getValidDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "  [ERROR] Invalid input. Please enter numbers only.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// collects all info for a new resident from the user
// age group is auto detected from age - no manual selection needed
void collectResidentData(char cityPrefix, string& id, int& age, string& mode,
                         double& dist, double& emFactor, int& days,
                         string& ageGroup, double& totalEmission) {
    while (true) {
        cout << "  Resident ID            : "; getline(cin, id);

        // strip leading/trailing whitespace
        size_t s = id.find_first_not_of(" \t\r\n");
        size_t e = id.find_last_not_of(" \t\r\n");
        if (s == string::npos) { cout << "  [ERROR] ID cannot be empty.\n"; continue; }
        id = id.substr(s, e - s + 1);

        if (id.length() != 4 ||
            toupper(id[0]) != cityPrefix ||
            !isdigit(id[1]) || !isdigit(id[2]) || !isdigit(id[3])) {
            cout << "  [ERROR] Invalid ID format. It must be '" << cityPrefix
                 << "' followed by exactly 3 numbers (e.g. " << cityPrefix << "001).\n";
            continue;
        }
        id[0] = toupper(id[0]);
        break;
    }

    age      = getValidIntInput("  Age                    : ");

    // figure out which age group they belong to based on city + age
    ageGroup = LinkedList::ageGroupLabel(age, cityPrefix);
    cout << "  Age Group (auto)       : " << ageGroup << "\n";

    mode     = getValidMode(cityPrefix);
    dist     = getValidDoubleInput("  Daily Distance (km)    : ");
    emFactor = getValidDoubleInput("  Emission Factor (kg/km): ");
    days     = getValidIntInput("  Days per Month         : ");

    totalEmission = dist * emFactor * days;
}

// sort menu for city A and B only
// first pick the algorithm, then pick what to sort by
// [0] goes back at either step
void sortMenuAB(LinkedList& city) {
    cout << "\n  Select Sorting Algorithm:\n";
    cout << "  [1] Bubble Sort    (O(n^2), stable)\n";
    cout << "  [2] Insertion Sort (O(n^2) worst, O(n) best)\n";
    cout << "  [0] Back\n";
    int algo = getValidIntInput("  Your choice: ");

    if (algo == 0) { cout << "  Returning to city menu.\n"; return; }
    if (algo < 1 || algo > 2) { cout << "  Invalid choice.\n"; return; }

    cout << "\n  Sort by:\n";
    cout << "  [1] Age\n";
    cout << "  [2] Daily Distance\n";
    cout << "  [3] Total Carbon Emission\n";
    cout << "  [0] Back\n";
    int field = getValidIntInput("  Your choice: ");

    if (field == 0) { cout << "  Returning to city menu.\n"; return; }

    if (algo == 1) {  // bubble sort
        if      (field == 1) city.sortByAge();
        else if (field == 2) city.sortByDistance();
        else if (field == 3) city.sortByEmission();
        else { cout << "  Invalid field choice.\n"; return; }
    } else {           // insertion sort
        if      (field == 1) city.insertionSortByAge();
        else if (field == 2) city.insertionSortByDistance();
        else if (field == 3) city.insertionSortByEmission();
        else { cout << "  Invalid field choice.\n"; return; }
    }
    city.display();
}

// sort menu for city C - my teammate handles this part, bubble sort only
void sortMenu(LinkedList& city) {
    cout << "\n  Sort by:\n";
    cout << "  [1] Age\n";
    cout << "  [2] Daily Distance\n";
    cout << "  [3] Total Carbon Emission\n";
    int choice = getValidIntInput("  Enter choice: ");

    switch (choice) {
        case 1: city.sortByAge();       city.display(); break;
        case 2: city.sortByDistance();  city.display(); break;
        case 3: city.sortByEmission();  city.display(); break;
        default: cout << "  Invalid choice.\n";
    }
}

// search menu for city A and B - has a back option at each step
void searchMenuAB(char cityPrefix, LinkedList& city) {
    cout << "\n  Search by:\n";
    cout << "  [1] Age Group (range)\n";
    cout << "  [2] Mode of Transport\n";
    cout << "  [3] Distance Threshold (> X km)\n";
    cout << "  [4] Exact Age - Binary Search\n";
    cout << "  [0] Back\n";
    int choice = getValidIntInput("  Enter choice: ");

    if (choice == 0) { cout << "  Returning to city menu.\n"; return; }

    if (choice == 1) {
        int minA = getValidIntInput("  Enter min age (0 = Back): ");
        if (minA == 0) { cout << "  Returning to city menu.\n"; return; }
        int maxA = getValidIntInput("  Enter max age: ");
        city.linearSearchByAgeGroup(minA, maxA);

    } else if (choice == 2) {
        string mode = getValidMode(cityPrefix);
        city.linearSearchByMode(mode);

    } else if (choice == 3) {
        double km;
        cout << "  Enter distance threshold (km): "; cin >> km;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        city.linearSearchByDistanceThreshold(km);

    } else if (choice == 4) {
        int age;
        cout << "  Enter exact age: "; cin >> age;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        city.binarySearchByAge(age);

    } else {
        cout << "  Invalid choice.\n";
    }
}

// search menu for city C - teammate's version, no back option
void searchMenu(char cityPrefix, LinkedList& city) {
    cout << "\n  Search by:\n";
    cout << "  [1] Age Group (range)\n";
    cout << "  [2] Mode of Transport\n";
    cout << "  [3] Distance Threshold (> X km)\n";
    cout << "  [4] Exact Age - Binary Search\n";
    int choice = getValidIntInput("  Enter choice: ");

    if (choice == 1) {
        int minA, maxA;
        cout << "  Enter min age: "; cin >> minA;
        cout << "  Enter max age: "; cin >> maxA;
        city.linearSearchByAgeGroup(minA, maxA);

    } else if (choice == 2) {
        string mode = getValidMode(cityPrefix);
        city.linearSearchByMode(mode);

    } else if (choice == 3) {
        double km;
        cout << "  Enter distance threshold (km): "; cin >> km;
        city.linearSearchByDistanceThreshold(km);

    } else if (choice == 4) {
        int age;
        cout << "  Enter exact age: "; cin >> age;
        city.binarySearchByAge(age);

    } else {
        cout << "  Invalid choice.\n";
    }
}

// prints the city menu - reused for A, B and C
void printCityMenu(const string& cityName) {
    cout << "\n==================================================\n";
    cout << "                CITY " << cityName << " MENU\n";
    cout << "==================================================\n";
    cout << "\n--- Data Viewing ---\n";
    cout << "[1]  View Original Data\n";
    cout << "\n--- Linked List Operations ---\n";
    cout << "[2]  Insert at Beginning\n";
    cout << "[3]  Insert at End\n";
    cout << "[4]  Insert at Specific Position\n";
    cout << "[5]  Delete a Resident\n";
    cout << "[6]  Reverse Linked List\n";
    cout << "\n--- Data Processing ---\n";
    if (cityName == "A" || cityName == "B")
        cout << "[7]  Sort Data  (Age / Distance / Emission) [Bubble & Insertion Sort]\n";
    else
        cout << "[7]  Sort Data  (Age / Distance / Emission)\n";
    cout << "[8]  Search Data  (Age / Mode / Distance)\n";
    cout << "\n--- Analysis ---\n";
    cout << "[9]  Carbon Emission Analysis\n";
    cout << "[10] Age Group Analysis\n";
    cout << "\n--------------------------------------------------\n";
    cout << "\n[11] Back to Main Menu\n";
    cout << "[0]  Exit Program\n";
    cout << "\n==================================================\n";
}

// city A menu
void cityAMenu(LinkedList& cityA) {
    do {
        printCityMenu("A");
        int choice = getValidIntInput("Enter your choice (0-11): ");

        string id, mode, ageGroup;
        int age, days, pos, oldSize;
        double dist, emFactor, totalEmission;

        switch (choice) {
            case 1: // show data sorted by ID (original order)
                cityA.sortByResidentID();
                cityA.display();
                cityA.saveToCSV();
                break;

            case 2: // insert at head
                collectResidentData('A', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityA.insertAtBeginning(id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityA.saveToCSV();
                break;

            case 3: // append to end
                oldSize = cityA.size();
                collectResidentData('A', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityA.insert(id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                if (cityA.size() > oldSize) {
                    cout << "  Resident " << id << " inserted at end (position "
                         << cityA.size() << ").\n";
                    cityA.saveToCSV();
                }
                break;

            case 4: // insert at a position the user specifies
                cout << "  Current list size: " << cityA.size() << "\n";
                pos = getValidIntInput("  Enter position (1 = head): ");
                collectResidentData('A', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityA.insertAtPosition(pos, id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityA.saveToCSV();
                break;

            case 5: // delete a resident
                cout << "  Enter Resident ID to delete: "; getline(cin, id);
                if (cityA.deleteResident(id)) cityA.saveToCSV();
                break;

            case 6: // reverse the list
                cityA.reverse();
                cityA.display();
                cityA.saveToCSV();
                break;

            case 7: // sort
                sortMenuAB(cityA);
                cityA.saveToCSV();
                break;

            case 8: // search
                searchMenuAB('A', cityA);
                break;

            case 9: // carbon analysis
                cityA.carbonAnalysis();
                break;

            case 10: // age group breakdown
                cityA.ageGroupAnalysis();
                break;

            case 11: // back to main menu
                return;

            case 0: // exit
                cout << "\nGoodbye!\n";
                exit(0);

            default:
                cout << "  Invalid choice! Please enter 0-11.\n";
        }
    } while (true);
}

// city B menu - basically same as A
void cityBMenu(LinkedList& cityB) {
    do {
        printCityMenu("B");
        int choice = getValidIntInput("Enter your choice (0-11): ");

        string id, mode, ageGroup;
        int age, days, pos, oldSize;
        double dist, emFactor, totalEmission;

        switch (choice) {
            case 1:
                cityB.sortByResidentID();
                cityB.display();
                cityB.saveToCSV();
                break;

            case 2:
                collectResidentData('B', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityB.insertAtBeginning(id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityB.saveToCSV();
                break;

            case 3:
                oldSize = cityB.size();
                collectResidentData('B', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityB.insert(id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                if (cityB.size() > oldSize) {
                    cout << "  Resident " << id << " inserted at end (position "
                         << cityB.size() << ").\n";
                    cityB.saveToCSV();
                }
                break;

            case 4:
                cout << "  Current list size: " << cityB.size() << "\n";
                pos = getValidIntInput("  Enter position (1 = head): ");
                collectResidentData('B', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityB.insertAtPosition(pos, id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityB.saveToCSV();
                break;

            case 5:
                cout << "  Enter Resident ID to delete: "; getline(cin, id);
                if (cityB.deleteResident(id)) cityB.saveToCSV();
                break;

            case 6:
                cityB.reverse();
                cityB.display();
                cityB.saveToCSV();
                break;

            case 7:
                sortMenuAB(cityB);
                cityB.saveToCSV();
                break;

            case 8:
                searchMenuAB('B', cityB);
                break;

            case 9:
                cityB.carbonAnalysis();
                break;

            case 10:
                cityB.ageGroupAnalysis();
                break;

            case 11:
                return;

            case 0:
                cout << "\nGoodbye!\n";
                exit(0);

            default:
                cout << "  Invalid choice! Please enter 0-11.\n";
        }
    } while (true);
}

// city C menu - my teammate's section, same layout
void cityCMenu(LinkedList& cityC) {
    do {
        printCityMenu("C");
        int choice = getValidIntInput("Enter your choice (0-11): ");

        string id, mode, ageGroup;
        int age, days, pos, oldSize;
        double dist, emFactor, totalEmission;

        switch (choice) {
            case 1:
                cityC.sortByResidentID();
                cityC.display();
                cityC.saveToCSV();
                break;

            case 2:
                collectResidentData('C', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityC.insertAtBeginning(id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityC.saveToCSV();
                break;

            case 3:
                oldSize = cityC.size();
                collectResidentData('C', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityC.insert(id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                if (cityC.size() > oldSize) {
                    cout << "  Resident " << id << " inserted at end (position "
                         << cityC.size() << ").\n";
                    cityC.saveToCSV();
                }
                break;

            case 4:
                cout << "  Current list size: " << cityC.size() << "\n";
                pos = getValidIntInput("  Enter position (1 = head): ");
                collectResidentData('C', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityC.insertAtPosition(pos, id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityC.saveToCSV();
                break;

            case 5:
                cout << "  Enter Resident ID to delete: "; getline(cin, id);
                if (cityC.deleteResident(id)) cityC.saveToCSV();
                break;

            case 6:
                cityC.reverse();
                cityC.display();
                cityC.saveToCSV();
                break;

            case 7:
                sortMenu(cityC);
                cityC.saveToCSV();
                break;

            case 8:
                searchMenu('C', cityC);
                break;

            case 9:
                cityC.carbonAnalysis();
                break;

            case 10:
                cityC.ageGroupAnalysis();
                break;

            case 11:
                return;

            case 0:
                cout << "\nGoodbye!\n";
                exit(0);

            default:
                cout << "  Invalid choice! Please enter 0-11.\n";
        }
    } while (true);
}

// compare all 3 cities - shows memory, traversal time and emissions
void compareCities(const LinkedList& cityA, const LinkedList& cityB, const LinkedList& cityC) {
    cout << "\n==================================================\n";
    cout << "     COMPARISON: CITY A vs CITY B vs CITY C\n";
    cout << "==================================================\n";

    // rough memory comparison - linked list has extra overhead from the next pointer
    // array stores just the data, linked list stores data + pointer per node
    struct PlainResident { string id; int age; string mode; double dist; double emFactor; int days; double total; };
    struct LinkedNode    { PlainResident data; void* next; };
    size_t arrayNodeSize  = sizeof(PlainResident);
    size_t linkedNodeSize = sizeof(LinkedNode);

    cout << "\n--- Memory Usage ---\n";
    cout << "  (Array node size: " << arrayNodeSize << " bytes  |  Linked list node size: " << linkedNodeSize << " bytes)\n";
    cout << "  City A: " << cityA.size() << " residents -> array ~" << cityA.size() * arrayNodeSize
         << " bytes  |  linked list ~" << cityA.size() * linkedNodeSize << " bytes\n";
    cout << "  City B: " << cityB.size() << " residents -> array ~" << cityB.size() * arrayNodeSize
         << " bytes  |  linked list ~" << cityB.size() * linkedNodeSize << " bytes\n";
    cout << "  City C: " << cityC.size() << " residents -> array ~" << cityC.size() * arrayNodeSize
         << " bytes  |  linked list ~" << cityC.size() * linkedNodeSize << " bytes\n";
    cout << "  * Linked list uses extra memory per node due to the next pointer overhead.\n";

    // time how long it takes to traverse each city
    cout << "\n--- Traversal Time (summing emissions) ---\n";

    auto t1 = chrono::high_resolution_clock::now();
    cityA.carbonAnalysis();
    auto t2 = chrono::high_resolution_clock::now();
    cityB.carbonAnalysis();
    auto t3 = chrono::high_resolution_clock::now();
    cityC.carbonAnalysis();
    auto t4 = chrono::high_resolution_clock::now();

    cout << "\n  City A traversal: "
         << chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << " us\n";
    cout << "  City B traversal: "
         << chrono::duration_cast<chrono::microseconds>(t3 - t2).count() << " us\n";
    cout << "  City C traversal: "
         << chrono::duration_cast<chrono::microseconds>(t4 - t3).count() << " us\n";

    // age group breakdown for each city
    cout << "\n--- Age Group Analysis ---\n";
    cout << "\n>>> CITY A <<<\n";
    cityA.ageGroupAnalysis();
    cout << "\n>>> CITY B <<<\n";
    cityB.ageGroupAnalysis();
    cout << "\n>>> CITY C <<<\n";
    cityC.ageGroupAnalysis();

    cout << "\n==================================================\n";
}

// main menu screen
void displayMainMenu() {
    cout << "\n==================================================\n";
    cout << "       CITYVILLE CARBON ANALYSIS SYSTEM\n";
    cout << "             (LINKED LIST VERSION)\n";
    cout << "==================================================\n";
    cout << "\n[1] City A  (Metropolitan City)\n";
    cout << "[2] City B  (University Town)\n";
    cout << "[3] City C  (Suburban / Rural Area)\n";
    cout << "\n[4] Compare All Cities\n";
    cout << "[0] Exit\n";
    cout << "\n--------------------------------------------------\n";
}

// entry point - load data then run the menu loop
int main() {
    LinkedList cityA, cityB, cityC;

    cout << "\nLoading datasets...\n";
    cityA.loadFromCSV("datasetA.csv");
    cityB.loadFromCSV("datasetB.csv");
    cityC.loadFromCSV("datasetC.csv");

    int choice;
    do {
        displayMainMenu();
        choice = getValidIntInput("");

        switch (choice) {
            case 1: cityAMenu(cityA);                        break;
            case 2: cityBMenu(cityB);                        break;
            case 3: cityCMenu(cityC);                        break;
            case 4: compareCities(cityA, cityB, cityC);      break;
            case 0: cout << "\nGoodbye!\n";                  break;
            default: cout << "\nInvalid choice. Please enter 0-4.\n";
        }
    } while (choice != 0);

    return 0;
}