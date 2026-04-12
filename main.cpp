// main.cpp - this is where everything starts i guess
#include <iostream>
#include <string>
#include <chrono>
#include <limits>
#include <vector>
#include "LinkedList.hpp"
using namespace std;

// need these so the functions below can find each other
void cityAMenu(LinkedList& cityA);
void cityBMenu(LinkedList& cityB);
void cityCMenu(LinkedList& cityC);
void compareCities(const LinkedList& cityA, const LinkedList& cityB, const LinkedList& cityC);

// only accept valid transport modes, no random text or numbers
string getValidMode(char cityPrefix) {
    vector<string> modes;
    
    // set allowed modes based on the city
    if (cityPrefix == 'A') {
        modes = { "Car", "Bus", "Bicycle", "Walking" };
    } else if (cityPrefix == 'B') {
        modes = { "Carpool", "Walking", "Bicycle", "School Bus" };
    } else {
        // city C has all 6 options
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

        // remove spaces at front and back
        size_t s = input.find_first_not_of(" \t");
        size_t e = input.find_last_not_of(" \t");
        if (s == string::npos) { cout << "  [ERROR] Input cannot be empty.\n"; continue; }
        input = input.substr(s, e - s + 1);

        // check if they typed a number
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

        // check if they typed the name instead, ignore uppercase/lowercase
        string lower = input;
        for (char& c : lower) c = (char)tolower(c);

        for (int i = 0; i < modeCount; ++i) {
            string cmp = modes[i];
            for (char& c : cmp) c = (char)tolower(c);
            if (lower == cmp)
                return modes[i]; // give back the properly capitalised version
        }

        cout << "  [ERROR] \"" << input << "\" is not a valid mode for City " << cityPrefix << ".\n";
    }
}

// stops the programme crashing if someone types letters instead of a number
int getValidIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear newline
            return value;
        } else {
            cout << "  [ERROR] Invalid input. Please enter numbers only.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// same as above but for decimals
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

// ask user to fill in all the resident info
void collectResidentData(char cityPrefix, string& id, int& age, string& mode,
                         double& dist, double& emFactor, int& days) {
    while (true) {
        cout << "  Resident ID            : "; getline(cin, id);

        // clean out spaces
        size_t s = id.find_first_not_of(" \t\r\n");
        size_t e = id.find_last_not_of(" \t\r\n");
        if (s == string::npos) { cout << "  [ERROR] ID cannot be empty.\n"; continue; }
        id = id.substr(s, e - s + 1);

        if (id.length() != 4 || 
            toupper(id[0]) != cityPrefix || 
            !isdigit(id[1]) || !isdigit(id[2]) || !isdigit(id[3])) {
            cout << "  [ERROR] Invalid ID format. It must be '" << cityPrefix << "' followed by exactly 3 numbers (e.g. " << cityPrefix << "001).\n";
            continue;
        }
        // ensure the letter is always capitalized before saving
        id[0] = toupper(id[0]);
        break;
    }

    age = getValidIntInput("  Age                    : ");
    mode = getValidMode(cityPrefix);
    dist = getValidDoubleInput("  Daily Distance (km)    : ");
    emFactor = getValidDoubleInput("  Emission Factor (kg/km): ");
    days = getValidIntInput("  Days per Month         : ");
}

// sort submenu, used by all cities
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

// search submenu, shared between all cities
void searchMenu(LinkedList& city) {
    cout << "\n  Search by:\n";
    cout << "  [1] Age Group (range)\n";
    cout << "  [2] Mode of Transport\n";
    cout << "  [3] Distance Threshold (> X km)\n";
    cout << "  [4] Exact Age - Binary Search (sort by age first!)\n";
    int choice = getValidIntInput("  Enter choice: ");

    if (choice == 1) {
        int minA, maxA;
        cout << "  Enter min age: "; cin >> minA;
        cout << "  Enter max age: "; cin >> maxA;
        city.linearSearchByAgeGroup(minA, maxA);

    } else if (choice == 2) {
        string mode;
        cin.ignore();
        cout << "  Enter mode (Car / Bus / Bicycle / Walking / School Bus / Carpool): ";
        getline(cin, mode);
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

// prints the menu box for whichever city we pass in
void printCityMenu(const string& cityName) {
    cout << "\n==================================================\n";
    cout << "                CITY " << cityName << " MENU\n";
    cout << "==================================================\n";
    cout << "\n--- Data Viewing ---\n";
    cout << "[1]  View All Residents\n";
    cout << "\n--- Linked List Operations ---\n";
    cout << "[2]  Insert at Beginning\n";
    cout << "[3]  Insert at End\n";
    cout << "[4]  Insert at Specific Position\n";
    cout << "[5]  Delete a Resident\n";
    cout << "[6]  Reverse Linked List\n";
    cout << "\n--- Data Processing ---\n";
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

// city A menu loop
void cityAMenu(LinkedList& cityA) {
    do {
        printCityMenu("A");
        int choice = getValidIntInput("Enter your choice (0-11): ");

        string id, mode;
        int age, days, pos, oldSize;
        double dist, emFactor;

        switch (choice) {
            case 1: // show all
                cityA.display();
                break;

            case 2: // add to front
                collectResidentData('A', id, age, mode, dist, emFactor, days);
                cityA.insertAtBeginning(id, age, mode, dist, emFactor, days);
                break;

            case 3: // add to back
                oldSize = cityA.size();
                collectResidentData('A', id, age, mode, dist, emFactor, days);
                cityA.insert(id, age, mode, dist, emFactor, days);
                if (cityA.size() > oldSize) {
                    cout << "  Resident " << id << " inserted at end (position "
                         << cityA.size() << ").\n";
                }
                break;

            case 4: // add at a specific spot
                cout << "  Current list size: " << cityA.size() << "\n";
                pos = getValidIntInput("  Enter position (1 = head): ");
                collectResidentData('A', id, age, mode, dist, emFactor, days);
                cityA.insertAtPosition(pos, id, age, mode, dist, emFactor, days);
                break;

            case 5: // remove someone
                cin.ignore();
                cout << "  Enter Resident ID to delete: "; getline(cin, id);
                cityA.deleteResident(id);
                break;

            case 6: // flip the list
                cityA.reverse();
                cityA.display();
                break;

            case 7: // sort stuff
                sortMenu(cityA);
                break;

            case 8: // search stuff
                searchMenu(cityA);
                break;

            case 9: // carbon numbers
                cityA.carbonAnalysis();
                break;

            case 10: // age breakdown
                cityA.ageGroupAnalysis();
                break;

            case 11: // go back
                return;

            case 0: // quit
                cout << "\nGoodbye!\n";
                exit(0);

            default:
                cout << "  Invalid choice! Please enter 0-11.\n";
        }
    } while (true);
}

// city B menu loop, basically the same as A
void cityBMenu(LinkedList& cityB) {
    do {
        printCityMenu("B");
        int choice = getValidIntInput("Enter your choice (0-11): ");

        string id, mode;
        int age, days, pos, oldSize;
        double dist, emFactor;

        switch (choice) {
            case 1:
                cityB.display();
                break;

            case 2:
                collectResidentData('B', id, age, mode, dist, emFactor, days);
                cityB.insertAtBeginning(id, age, mode, dist, emFactor, days);
                break;

            case 3:
                oldSize = cityB.size();
                collectResidentData('B', id, age, mode, dist, emFactor, days);
                cityB.insert(id, age, mode, dist, emFactor, days);
                if (cityB.size() > oldSize) {
                    cout << "  Resident " << id << " inserted at end (position "
                         << cityB.size() << ").\n";
                }
                break;

            case 4:
                cout << "  Current list size: " << cityB.size() << "\n";
                pos = getValidIntInput("  Enter position (1 = head): ");
                collectResidentData('B', id, age, mode, dist, emFactor, days);
                cityB.insertAtPosition(pos, id, age, mode, dist, emFactor, days);
                break;

            case 5:
                cin.ignore();
                cout << "  Enter Resident ID to delete: "; getline(cin, id);
                cityB.deleteResident(id);
                break;

            case 6:
                cityB.reverse();
                cityB.display();
                break;

            case 7:
                sortMenu(cityB);
                break;

            case 8:
                searchMenu(cityB);
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

// city C menu loop, same structure as A and B
void cityCMenu(LinkedList& cityC) {
    do {
        printCityMenu("C");
        int choice = getValidIntInput("Enter your choice (0-11): ");

        string id, mode;
        int age, days, pos, oldSize;
        double dist, emFactor;

        switch (choice) {
            case 1:
                cityC.display();
                break;

            case 2:
                collectResidentData('C', id, age, mode, dist, emFactor, days);
                cityC.insertAtBeginning(id, age, mode, dist, emFactor, days);
                break;

            case 3:
                oldSize = cityC.size();
                collectResidentData('C', id, age, mode, dist, emFactor, days);
                cityC.insert(id, age, mode, dist, emFactor, days);
                if (cityC.size() > oldSize) {
                    cout << "  Resident " << id << " inserted at end (position "
                         << cityC.size() << ").\n";
                }
                break;

            case 4:
                cout << "  Current list size: " << cityC.size() << "\n";
                pos = getValidIntInput("  Enter position (1 = head): ");
                collectResidentData('C', id, age, mode, dist, emFactor, days);
                cityC.insertAtPosition(pos, id, age, mode, dist, emFactor, days);
                break;

            case 5:
                cin.ignore();
                cout << "  Enter Resident ID to delete: "; getline(cin, id);
                cityC.deleteResident(id);
                break;

            case 6:
                cityC.reverse();
                cityC.display();
                break;

            case 7:
                sortMenu(cityC);
                break;

            case 8:
                searchMenu(cityC);
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

// compares all three cities - memory usage, traversal time, and emissions side by side
void compareCities(const LinkedList& cityA, const LinkedList& cityB, const LinkedList& cityC) {
    cout << "\n==================================================\n";
    cout << "     COMPARISON: CITY A vs CITY B vs CITY C\n";
    cout << "==================================================\n";

    // memory comparison using real struct sizes from the validated city C file
    // each linked list node = the data fields + one pointer for next
    // a plain array element would just be the data fields without the pointer
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

    // traversal time for each city
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

    // age group breakdown for all three
    cout << "\n--- Age Group Analysis ---\n";
    cout << "\n>>> CITY A <<<\n";
    cityA.ageGroupAnalysis();
    cout << "\n>>> CITY B <<<\n";
    cityB.ageGroupAnalysis();
    cout << "\n>>> CITY C <<<\n";
    cityC.ageGroupAnalysis();

    cout << "\n==================================================\n";
}

// the main menu that shows first
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

// entry point
int main() {
    LinkedList cityA, cityB, cityC;

    cout << "\nLoading datasets...\n";
    cityA.loadFromCSV("dataset1-cityA.csv");
    cityB.loadFromCSV("dataset2-cityB.csv");
    cityC.loadFromCSV("dataset3-cityC.csv");

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