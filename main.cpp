// ============================================================
//  main.cpp  –  CityVille Carbon Analysis System
//               LINKED LIST VERSION
//  Datasets: City A (dataset1-cityA.csv)
//            City B (dataset2-cityB.csv)
//            City C (dataset3-cityC.csv)  [handled by teammate]
// ============================================================
#include <iostream>
#include "LinkedList.hpp"
using namespace std;

// Forward declarations
void cityAMenu(LinkedList& cityA);
void cityBMenu(LinkedList& cityB);
void cityCMenu();
void compareCities(const LinkedList& cityA, const LinkedList& cityB);

// ─────────────────────────────────────────────────────────────
//  Main Menu
// ─────────────────────────────────────────────────────────────
void displayMainMenu() {
    cout << "\n============================================\n";
    cout << "     CITYVILLE CARBON ANALYSIS SYSTEM\n";
    cout << "           (LINKED LIST VERSION)\n";
    cout << "============================================\n";
    cout << "\nSelect an option:\n";
    cout << "1. City A (Metropolitan City)\n";
    cout << "2. City B (University Town)\n";
    cout << "3. City C (Suburban / Rural Area)\n";
    cout << "\n4. Compare Cities A & B\n";
    cout << "0. Exit\n";
    cout << "\nEnter your choice: ";
}

// ─────────────────────────────────────────────────────────────
//  Sort Sub-Menu  (reused by both cities)
// ─────────────────────────────────────────────────────────────
void sortMenu(LinkedList& city) {
    int choice;
    cout << "\n  --- Sort Menu ---\n";
    cout << "  1. Sort by Age\n";
    cout << "  2. Sort by Daily Distance\n";
    cout << "  3. Sort by Total Carbon Emission\n";
    cout << "  Enter choice: ";
    cin  >> choice;

    switch (choice) {
        case 1: city.sortByAge();       city.display(); break;
        case 2: city.sortByDistance();  city.display(); break;
        case 3: city.sortByEmission();  city.display(); break;
        default: cout << "  Invalid.\n";
    }
}

// ─────────────────────────────────────────────────────────────
//  Search Sub-Menu  (reused by both cities)
// ─────────────────────────────────────────────────────────────
void searchMenu(LinkedList& city) {
    int choice;
    cout << "\n  --- Search Menu ---\n";
    cout << "  1. Linear Search by Age Group\n";
    cout << "  2. Linear Search by Mode of Transport\n";
    cout << "  3. Linear Search by Distance Threshold (> X km)\n";
    cout << "  4. Binary Search by Exact Age  (sort by age first!)\n";
    cout << "  Enter choice: ";
    cin  >> choice;

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
        cout << "  Enter exact age to search: "; cin >> age;
        city.binarySearchByAge(age);

    } else {
        cout << "  Invalid choice.\n";
    }
}

// ─────────────────────────────────────────────────────────────
//  Add Resident helper
// ─────────────────────────────────────────────────────────────
void addResident(LinkedList& city) {
    string id, mode;
    int age, days;
    double dist, emFactor;

    cin.ignore();
    cout << "  Resident ID     : "; getline(cin, id);
    cout << "  Age             : "; cin >> age;
    cin.ignore();
    cout << "  Mode of Transport: "; getline(cin, mode);
    cout << "  Daily Distance (km): "; cin >> dist;
    cout << "  Emission Factor (kg CO2/km): "; cin >> emFactor;
    cout << "  Days per Month  : "; cin >> days;

    city.insert(id, age, mode, dist, emFactor, days);
    cout << "  Resident " << id << " added successfully.\n";
}

// ─────────────────────────────────────────────────────────────
//  City A Menu
// ─────────────────────────────────────────────────────────────
void cityAMenu(LinkedList& cityA) {
    int choice;
    do {
        cout << "\n============================================\n";
        cout << "              CITY A MENU\n";
        cout << "============================================\n";
        cout << "1. View Data\n";
        cout << "2. Add Resident\n";
        cout << "3. Sort Data\n";
        cout << "4. Search Data\n";
        cout << "5. Carbon Analysis\n";
        cout << "6. Age Group Analysis\n";
        cout << "9. Back to Main Menu\n";
        cout << "0. Exit Program\n";
        cout << "\nEnter your choice: ";
        cin  >> choice;

        switch (choice) {
            case 1: cityA.display();          break;
            case 2: addResident(cityA);       break;
            case 3: sortMenu(cityA);          break;
            case 4: searchMenu(cityA);        break;
            case 5: cityA.carbonAnalysis();   break;
            case 6: cityA.ageGroupAnalysis(); break;
            case 9: return;
            case 0: exit(0);
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 9);
}

// ─────────────────────────────────────────────────────────────
//  City B Menu
// ─────────────────────────────────────────────────────────────
void cityBMenu(LinkedList& cityB) {
    int choice;
    do {
        cout << "\n============================================\n";
        cout << "              CITY B MENU\n";
        cout << "============================================\n";
        cout << "1. View Data\n";
        cout << "2. Add Resident\n";
        cout << "3. Sort Data\n";
        cout << "4. Search Data\n";
        cout << "5. Carbon Analysis\n";
        cout << "6. Age Group Analysis\n";
        cout << "9. Back to Main Menu\n";
        cout << "0. Exit Program\n";
        cout << "\nEnter your choice: ";
        cin  >> choice;

        switch (choice) {
            case 1: cityB.display();          break;
            case 2: addResident(cityB);       break;
            case 3: sortMenu(cityB);          break;
            case 4: searchMenu(cityB);        break;
            case 5: cityB.carbonAnalysis();   break;
            case 6: cityB.ageGroupAnalysis(); break;
            case 9: return;
            case 0: exit(0);
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 9);
}

// ─────────────────────────────────────────────────────────────
//  City C Menu  (stub – for teammate)
// ─────────────────────────────────────────────────────────────
void cityCMenu() {
    cout << "\n  City C is handled by another team member.\n";
}

// ─────────────────────────────────────────────────────────────
//  Compare Cities A & B
// ─────────────────────────────────────────────────────────────
void compareCities(const LinkedList& cityA, const LinkedList& cityB) {
    // Call each city's analysis so the user can see the tables side by side
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

// ─────────────────────────────────────────────────────────────
//  main()
// ─────────────────────────────────────────────────────────────
int main() {
    // Load datasets once at startup
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
            default: cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}