// main.cpp - this is where everything starts i guess
#include <iostream>
#include <string>
#include "LinkedList.hpp"
using namespace std;

// need these so the functions below can find each other
void cityAMenu(LinkedList& cityA);
void cityBMenu(LinkedList& cityB);
void cityCMenu();
void compareCities(const LinkedList& cityA, const LinkedList& cityB);

// only accept valid transport modes, no random text or numbers
string getValidMode() {
    // the 6 valid options, properly spelled
    const string MODES[] = {
        "Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"
    };
    const int MODE_COUNT = 6;

    while (true) {
        cout << "  Select Mode of Transport:\n";
        for (int i = 0; i < MODE_COUNT; ++i)
            cout << "    [" << (i + 1) << "] " << MODES[i] << "\n";
        cout << "  Your choice (number or name): ";

        string input;
        getline(cin, input);

        // remove spaces at front and back
        size_t s = input.find_first_not_of(" \t");
        size_t e = input.find_last_not_of(" \t");
        if (s == string::npos) { cout << "  [ERROR] Input cannot be empty.\n"; continue; }
        input = input.substr(s, e - s + 1);

        // check if they typed a number like 1-6
        bool isNumber = !input.empty();
        for (char c : input) if (!isdigit(c)) { isNumber = false; break; }
        if (isNumber) {
            int idx = stoi(input);
            if (idx >= 1 && idx <= MODE_COUNT)
                return MODES[idx - 1];
            cout << "  [ERROR] Please enter a number between 1 and "
                 << MODE_COUNT << ".\n";
            continue;
        }

        // check if they typed the name instead, ignore uppercase/lowercase
        string lower = input;
        for (char& c : lower) c = (char)tolower(c);

        for (int i = 0; i < MODE_COUNT; ++i) {
            string cmp = MODES[i];
            for (char& c : cmp) c = (char)tolower(c);
            if (lower == cmp)
                return MODES[i]; // give back the properly capitalised version
        }

        cout << "  [ERROR] \"" << input << "\" is not a valid mode.\n"
             << "  Valid options: Car, Bus, Bicycle, Walking, School Bus, Carpool\n";
    }
}

// ask user to fill in all the resident info
void collectResidentData(string& id, int& age, string& mode,
                         double& dist, double& emFactor, int& days) {
    cin.ignore();
    cout << "  Resident ID            : "; getline(cin, id);
    cout << "  Age                    : "; cin >> age;
    cin.ignore();
    mode = getValidMode();
    cout << "  Daily Distance (km)    : "; cin >> dist;
    cout << "  Emission Factor (kg/km): "; cin >> emFactor;
    cout << "  Days per Month         : "; cin >> days;
}

// sort submenu, used by both city A and B
void sortMenu(LinkedList& city) {
    int choice;
    cout << "\n  Sort by:\n";
    cout << "  [1] Age\n";
    cout << "  [2] Daily Distance\n";
    cout << "  [3] Total Carbon Emission\n";
    cout << "  Enter choice: ";
    cin >> choice;

    switch (choice) {
        case 1: city.sortByAge();       city.display(); break;
        case 2: city.sortByDistance();  city.display(); break;
        case 3: city.sortByEmission();  city.display(); break;
        default: cout << "  Invalid choice.\n";
    }
}

// search submenu, also shared between cities
void searchMenu(LinkedList& city) {
    int choice;
    cout << "\n  Search by:\n";
    cout << "  [1] Age Group (range)\n";
    cout << "  [2] Mode of Transport\n";
    cout << "  [3] Distance Threshold (> X km)\n";
    cout << "  [4] Exact Age - Binary Search (sort by age first!)\n";
    cout << "  Enter choice: ";
    cin >> choice;

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
    cout << "Enter your choice (0-11): ";
}

// city A menu loop
void cityAMenu(LinkedList& cityA) {
    int choice;
    do {
        printCityMenu("A");
        cin >> choice;

        string id, mode;
        int age, days, pos;
        double dist, emFactor;

        switch (choice) {
            case 1: // show all
                cityA.display();
                break;

            case 2: // add to front
                collectResidentData(id, age, mode, dist, emFactor, days);
                cityA.insertAtBeginning(id, age, mode, dist, emFactor, days);
                break;

            case 3: // add to back
                collectResidentData(id, age, mode, dist, emFactor, days);
                cityA.insert(id, age, mode, dist, emFactor, days);
                cout << "  Resident " << id << " inserted at end (position "
                     << cityA.size() << ").\n";
                break;

            case 4: // add at a specific spot
                cout << "  Current list size: " << cityA.size() << "\n";
                cout << "  Enter position (1 = head): "; cin >> pos;
                collectResidentData(id, age, mode, dist, emFactor, days);
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
    int choice;
    do {
        printCityMenu("B");
        cin >> choice;

        string id, mode;
        int age, days, pos;
        double dist, emFactor;

        switch (choice) {
            case 1:
                cityB.display();
                break;

            case 2:
                collectResidentData(id, age, mode, dist, emFactor, days);
                cityB.insertAtBeginning(id, age, mode, dist, emFactor, days);
                break;

            case 3:
                collectResidentData(id, age, mode, dist, emFactor, days);
                cityB.insert(id, age, mode, dist, emFactor, days);
                cout << "  Resident " << id << " inserted at end (position "
                     << cityB.size() << ").\n";
                break;

            case 4:
                cout << "  Current list size: " << cityB.size() << "\n";
                cout << "  Enter position (1 = head): "; cin >> pos;
                collectResidentData(id, age, mode, dist, emFactor, days);
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

// city C is not our job
void cityCMenu() {
    cout << "\n  City C is handled by another team member.\n";
}

// show both cities side by side
void compareCities(const LinkedList& cityA, const LinkedList& cityB) {
    cout << "\n====================================================\n";
    cout << "  COMPARISON: CITY A vs CITY B\n";
    cout << "====================================================\n";

    cout << "\n>>> CITY A <<<\n";
    cityA.carbonAnalysis();
    cityA.ageGroupAnalysis();

    cout << "\n>>> CITY B <<<\n";
    cityB.carbonAnalysis();
    cityB.ageGroupAnalysis();

    cout << "\n====================================================\n";
    cout << "  (Full comparison including City C to be added by\n";
    cout << "   the team member responsible for City C)\n";
    cout << "====================================================\n";
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
    cout << "\n[4] Compare Cities A & B\n";
    cout << "[0] Exit\n";
    cout << "\n--------------------------------------------------\n";
    cout << "Enter your choice: ";
}

// entry point
int main() {
    LinkedList cityA, cityB;

    cout << "\nLoading datasets...\n";
    cityA.loadFromCSV("dataset1-cityA.csv");
    cityB.loadFromCSV("dataset2-cityB.csv");

    int choice;
    do {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1: cityAMenu(cityA);               break;
            case 2: cityBMenu(cityB);               break;
            case 3: cityCMenu();                    break;
            case 4: compareCities(cityA, cityB);    break;
            case 0: cout << "\nGoodbye!\n";         break;
            default: cout << "\nInvalid choice. Please enter 0-4.\n";
        }
    } while (choice != 0);

    return 0;
}