// mission_control.cpp - handles all the menus and user input stuff
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>
#include <limits>
#include <vector>
#include "city_linkedlist.hpp"
using namespace std;

// ==============================================================
//  CITY C - ARRAY-BASED STRUCTURE
//  (teammate's implementation, integrated for comparison purposes)
// ==============================================================
struct CityCResident {
    string residentID;
    int    age;
    string mode;
    double distance;
    double emissionFactor;
    int    days;
    string ageGroup;
    double totalEmission;
};

const int MAX_CITY_C_ARR = 1000;
CityCResident cityCArr[MAX_CITY_C_ARR];
int           cityCArr_size = 0;

// load City C CSV into the plain array
void loadCityCArray(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "  [ERROR] Cannot open " << filename << " for array load.\n";
        return;
    }
    string line;
    getline(file, line); // skip header
    cityCArr_size = 0;
    while (getline(file, line) && cityCArr_size < MAX_CITY_C_ARR) {
        if (line.empty() || line == "\r") continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        stringstream ss(line);
        string id, ageStr, mode, distStr, emStr, daysStr, ageGroup, totalStr;
        getline(ss, id,       ','); getline(ss, ageStr,   ',');
        getline(ss, mode,     ','); getline(ss, distStr,  ',');
        getline(ss, emStr,    ','); getline(ss, daysStr,  ',');
        getline(ss, ageGroup, ','); getline(ss, totalStr, ',');
        // trim whitespace
        auto trim = [](string& s) {
            size_t a = s.find_first_not_of(" \t\r\n");
            size_t b = s.find_last_not_of(" \t\r\n");
            s = (a == string::npos) ? "" : s.substr(a, b - a + 1);
        };
        trim(id); trim(ageStr); trim(mode); trim(distStr);
        trim(emStr); trim(daysStr); trim(ageGroup); trim(totalStr);
        if (id.empty()) continue;
        try {
            CityCResident r;
            r.residentID    = id;
            r.age           = stoi(ageStr);
            r.mode          = mode;
            r.distance      = stod(distStr);
            r.emissionFactor= stod(emStr);
            r.days          = stoi(daysStr);
            r.ageGroup      = ageGroup;
            r.totalEmission = totalStr.empty()
                              ? r.distance * r.emissionFactor * r.days
                              : stod(totalStr);
            cityCArr[cityCArr_size++] = r;
        } catch (...) {}
    }
    file.close();
    cout << "  Loaded " << cityCArr_size << " records into City C array.\n";
}

// display City C array as a table
void displayCityCArray() {
    if (cityCArr_size == 0) { cout << "  (array is empty)\n"; return; }
    cout << "\n";
    cout << left
         << setw(8)  << "ID"
         << setw(6)  << "Age"
         << setw(36) << "Age Group"
         << setw(14) << "Mode"
         << setw(10) << "Dist(km)"
         << setw(14) << "TotalEmis(kg)"
         << "\n";
    cout << string(88, '-') << "\n";
    for (int i = 0; i < cityCArr_size; i++) {
        cout << left
             << setw(8)  << cityCArr[i].residentID
             << setw(6)  << cityCArr[i].age
             << setw(36) << cityCArr[i].ageGroup
             << setw(14) << cityCArr[i].mode
             << setw(10) << fixed << setprecision(1) << cityCArr[i].distance
             << setw(14) << fixed << setprecision(2) << cityCArr[i].totalEmission
             << "\n";
    }
    cout << string(88, '-') << "\n";
    cout << "  Total records: " << cityCArr_size << "\n";
}

// bubble sort on a copy of the City C array (does not modify the original)
double bubbleSortCityCArr(int field) {
    // sort a temporary copy
    CityCResident tmp[MAX_CITY_C_ARR];
    for (int i = 0; i < cityCArr_size; i++) tmp[i] = cityCArr[i];

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < cityCArr_size - 1; i++) {
        for (int j = 0; j < cityCArr_size - i - 1; j++) {
            bool doSwap = false;
            if      (field == 1 && tmp[j].age            > tmp[j+1].age)           doSwap = true;
            else if (field == 2 && tmp[j].distance       > tmp[j+1].distance)      doSwap = true;
            else if (field == 3 && tmp[j].totalEmission  > tmp[j+1].totalEmission) doSwap = true;
            if (doSwap) {
                CityCResident t = tmp[j];
                tmp[j] = tmp[j+1];
                tmp[j+1] = t;
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

// linear search on City C array by mode
void linearSearchCityCArr_mode(const string& mode) {
    string modeLow = mode;
    for (char& c : modeLow) c = (char)tolower(c);

    auto start = chrono::high_resolution_clock::now();
    cout << "\n  [Array Linear Search] Mode = \"" << mode << "\"\n";
    cout << left << setw(8) << "ID" << setw(6) << "Age" << setw(14) << "Mode"
         << setw(10) << "Dist(km)" << setw(14) << "TotalEmis(kg)" << "\n";
    cout << string(52, '-') << "\n";
    int found = 0;
    for (int i = 0; i < cityCArr_size; i++) {
        string cur = cityCArr[i].mode;
        for (char& c : cur) c = (char)tolower(c);
        if (cur == modeLow) {
            cout << left
                 << setw(8)  << cityCArr[i].residentID
                 << setw(6)  << cityCArr[i].age
                 << setw(14) << cityCArr[i].mode
                 << setw(10) << fixed << setprecision(1) << cityCArr[i].distance
                 << setw(14) << fixed << setprecision(2) << cityCArr[i].totalEmission
                 << "\n";
            found++;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << string(52, '-') << "\n";
    cout << "  Found " << found << " residents  |  Time: "
         << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n)  |  Space: O(1)\n";
}

// linear search on City C array by age range
void linearSearchCityCArr_age(int minAge, int maxAge) {
    auto start = chrono::high_resolution_clock::now();
    cout << "\n  [Array Linear Search] Age " << minAge << " - " << maxAge << "\n";
    cout << left << setw(8) << "ID" << setw(6) << "Age" << setw(14) << "Mode"
         << setw(10) << "Dist(km)" << setw(14) << "TotalEmis(kg)" << "\n";
    cout << string(52, '-') << "\n";
    int found = 0;
    for (int i = 0; i < cityCArr_size; i++) {
        if (cityCArr[i].age >= minAge && cityCArr[i].age <= maxAge) {
            cout << left
                 << setw(8)  << cityCArr[i].residentID
                 << setw(6)  << cityCArr[i].age
                 << setw(14) << cityCArr[i].mode
                 << setw(10) << fixed << setprecision(1) << cityCArr[i].distance
                 << setw(14) << fixed << setprecision(2) << cityCArr[i].totalEmission
                 << "\n";
            found++;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << string(52, '-') << "\n";
    cout << "  Found " << found << " residents  |  Time: "
         << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n)  |  Space: O(1)\n";
}

// compare City C array vs linked list
void compareArrayVsLinkedListCityC(const LinkedList& cityC) {
    cout << "\n  ========== CITY C: ARRAY vs LINKED LIST ==========\n";

    // --- Memory ---
    struct PlainNode { string a; int b; string c; double d, e; int f; string g; double h; };
    struct LLNode    { PlainNode data; void* next; };
    size_t arrNodeSz = sizeof(CityCResident);
    size_t llNodeSz  = sizeof(LLNode);
    size_t arrTotalMem  = cityCArr_size * arrNodeSz;
    size_t llTotalMem   = cityC.size()  * llNodeSz;

    cout << "\n  --- Memory Usage ---\n";
    cout << "  Per-element size  : Array = " << arrNodeSz << " bytes  |  Linked List = "
         << llNodeSz << " bytes (+" << (llNodeSz - arrNodeSz) << " pointer overhead)\n";
    cout << "  Total (" << cityCArr_size << " records) : Array ~" << arrTotalMem
         << " bytes  |  Linked List ~" << llTotalMem << " bytes\n";
    cout << "  Extra LL overhead : " << (llTotalMem - arrTotalMem) << " bytes ("
         << fixed << setprecision(1)
         << ((double)(llTotalMem - arrTotalMem) / arrTotalMem * 100.0) << "% more)\n";

    // --- Traversal time (sum all emissions) ---
    cout << "\n  --- Traversal Time (summing all emissions) ---\n";

    auto t1 = chrono::high_resolution_clock::now();
    double arrSum = 0;
    for (int i = 0; i < cityCArr_size; i++) arrSum += cityCArr[i].totalEmission;
    auto t2 = chrono::high_resolution_clock::now();

    cityC.carbonAnalysis();   // traverses the linked list internally
    auto t3 = chrono::high_resolution_clock::now();

    long long arrUs  = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    long long llUs   = chrono::duration_cast<chrono::microseconds>(t3 - t2).count();

    cout << "  Array traversal       : " << arrUs << " us  (sum = "
         << fixed << setprecision(2) << arrSum << " kg CO2)\n";
    cout << "  Linked list traversal : " << llUs  << " us\n";
    cout << "  Faster                : "
         << (arrUs <= llUs ? "Array" : "Linked List") << "\n";

    // --- Sort time comparison (bubble sort by emission) ---
    cout << "\n  --- Sort Time Comparison (Bubble Sort by Total Emission) ---\n";

    double arrSortMs = bubbleSortCityCArr(3);   // 3 = totalEmission, sorts a copy

    // Sort the linked list and read time via sortTime()
    LinkedList& cityC_mut = const_cast<LinkedList&>(cityC);
    cityC_mut.sortByEmission();
    double llSortMs = cityC_mut.sortTime();

    cout << "  Array bubble sort       : " << fixed << setprecision(4) << arrSortMs << " ms\n";
    cout << "  Linked list bubble sort : " << fixed << setprecision(4) << llSortMs  << " ms\n";
    cout << "  Faster                  : "
         << (arrSortMs <= llSortMs ? "Array" : "Linked List") << "\n";

    // Restore linked list original order
    cityC_mut.sortByResidentID();

    cout << "\n  --- Key Differences ---\n";
    cout << "  Array       : Fixed size, O(1) random access, cache-friendly traversal.\n";
    cout << "  Linked List : Dynamic size, O(n) access, extra pointer memory per node.\n";
    cout << "  ==================================================\n";
}

// forward declarations so functions can call each other

void cityAMenu(LinkedList& cityA);
void cityBMenu(LinkedList& cityB);
void cityCMenu(LinkedList& cityC);
void compareCities(const LinkedList& cityA, const LinkedList& cityB, const LinkedList& cityC);
void multiConditionSearchMenuAB(char cityPrefix, LinkedList& city);

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

// sort menu for city A and B
void sortMenuAB(LinkedList& city) {
    cout << "\n  Select Algorithm:\n";
    cout << "  [1] Bubble Sort\n";
    cout << "  [2] Insertion Sort\n";
    cout << "  [0] Back\n";
    int algo = getValidIntInput("  Your choice: ");

    if (algo == 0) { cout << "  Returning to city menu.\n"; return; }
    if (algo < 1 || algo > 2) { cout << "  Invalid choice.\n"; return; }

    cout << "\n  Sort by:\n";
    cout << "  [1] Age\n";
    cout << "  [2] Daily Distance\n";
    cout << "  [3] Total Emission\n";
    cout << "  [0] Back\n";
    int field = getValidIntInput("  Your choice: ");

    if (field == 0) { cout << "  Returning to city menu.\n"; return; }

    string algoName, fieldName, complexity;
    if (algo == 1) {
        if      (field == 1) { city.sortByAge();              algoName = "Bubble Sort"; fieldName = "Age";            complexity = "O(n^2) | O(1)"; }
        else if (field == 2) { city.sortByDistance();         algoName = "Bubble Sort"; fieldName = "Daily Distance"; complexity = "O(n^2) | O(1)"; }
        else if (field == 3) { city.sortByEmission();         algoName = "Bubble Sort"; fieldName = "Total Emission"; complexity = "O(n^2) | O(1)"; }
        else { cout << "  Invalid field.\n"; return; }
    } else {
        if      (field == 1) { city.insertionSortByAge();      algoName = "Insertion Sort"; fieldName = "Age";            complexity = "O(n^2) worst / O(n) best | O(1)"; }
        else if (field == 2) { city.insertionSortByDistance(); algoName = "Insertion Sort"; fieldName = "Daily Distance"; complexity = "O(n^2) worst / O(n) best | O(1)"; }
        else if (field == 3) { city.insertionSortByEmission(); algoName = "Insertion Sort"; fieldName = "Total Emission"; complexity = "O(n^2) worst / O(n) best | O(1)"; }
        else { cout << "  Invalid field.\n"; return; }
    }
    city.display();
    cout << "  --------------------------------------------------\n";
    cout << "  Algorithm     : " << algoName << " by " << fieldName << "\n";
    cout << "  Execution Time: " << fixed << setprecision(4) << city.sortTime() << " ms\n";
    cout << "  Complexity    : " << complexity << "\n";
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
    cout << "  [5] Multi-Condition Search (Age+Mode / Distance+Mode)\n";
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

    } else if (choice == 5) {
        multiConditionSearchMenuAB(cityPrefix, city);

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
    cout << "[7]  Sort Data\n";
    cout << "[8]  Search Data\n";
    if (cityName == "A" || cityName == "B") {
        cout << "\n--- Analysis ---\n";
        cout << "[9]  Total Emissions Summary\n";
        cout << "[10] Emissions by Transport Mode\n";
        cout << "[11] Transport Percentage Distribution\n";
        cout << "[12] Age Group Analysis\n";
        cout << "[13] Top / Lowest Emitters\n";
        cout << "\n--------------------------------------------------\n";
        cout << "[14] Back to Main Menu\n";
        cout << "[0]  Exit Program\n";
    } else {
        cout << "\n--- Analysis ---\n";
        cout << "[9]  Carbon Emission Analysis\n";
        cout << "[10] Age Group Analysis\n";
        cout << "[11] Array vs Linked List Comparison\n";
        cout << "\n--------------------------------------------------\n";
        cout << "[12] Back to Main Menu\n";
        cout << "[0]  Exit Program\n";
    }
    cout << "==================================================\n";
}

// city A menu
void cityAMenu(LinkedList& cityA) {
    do {
        printCityMenu("A");
        int choice = getValidIntInput("Enter your choice (0-14): ");

        string id, mode, ageGroup;
        int age, days, pos, oldSize;
        double dist, emFactor, totalEmission;

        switch (choice) {
            case 1:
                cityA.sortByResidentID();
                cityA.display();
                cityA.saveToCSV();
                break;

            case 2:
                collectResidentData('A', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityA.insertAtBeginning(id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityA.saveToCSV();
                break;

            case 3:
                oldSize = cityA.size();
                collectResidentData('A', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityA.insert(id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                if (cityA.size() > oldSize) {
                    cout << "  Resident " << id << " inserted at end (position "
                         << cityA.size() << ").\n";
                    cityA.saveToCSV();
                }
                break;

            case 4:
                cout << "  Current list size: " << cityA.size() << "\n";
                pos = getValidIntInput("  Enter position (1 = head): ");
                collectResidentData('A', id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityA.insertAtPosition(pos, id, age, mode, dist, emFactor, days, ageGroup, totalEmission);
                cityA.saveToCSV();
                break;

            case 5:
                cout << "  Enter Resident ID to delete: "; getline(cin, id);
                if (cityA.deleteResident(id)) cityA.saveToCSV();
                break;

            case 6:
                cityA.reverse();
                cityA.display();
                cityA.saveToCSV();
                break;

            case 7:
                sortMenuAB(cityA);
                cityA.saveToCSV();
                break;

            case 8:
                searchMenuAB('A', cityA);
                break;

            case 9:
                cityA.totalEmissionsReport('A');
                break;

            case 10:
                cityA.emissionsByMode('A');
                break;

            case 11:
                cityA.transportPercentageDistribution('A');
                break;

            case 12:
                cityA.ageGroupAnalysisExtended('A');
                break;

            case 13: {
                int n = getValidIntInput("  Show top/bottom N emitters (e.g. 5): ");
                cityA.topBottomEmitters(n);
                break;
            }

            case 14:
                return;

            case 0:
                cout << "\nGoodbye!\n";
                exit(0);

            default:
                cout << "  Invalid choice! Please enter 0-14.\n";
        }
    } while (true);
}

// city B menu - same as A
void cityBMenu(LinkedList& cityB) {
    do {
        printCityMenu("B");
        int choice = getValidIntInput("Enter your choice (0-14): ");

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
                cityB.totalEmissionsReport('B');
                break;

            case 10:
                cityB.emissionsByMode('B');
                break;

            case 11:
                cityB.transportPercentageDistribution('B');
                break;

            case 12:
                cityB.ageGroupAnalysisExtended('B');
                break;

            case 13: {
                int n = getValidIntInput("  Show top/bottom N emitters (e.g. 5): ");
                cityB.topBottomEmitters(n);
                break;
            }

            case 14:
                return;

            case 0:
                cout << "\nGoodbye!\n";
                exit(0);

            default:
                cout << "  Invalid choice! Please enter 0-14.\n";
        }
    } while (true);
}

// multi-condition search sub-menu (accessed from Search menu option 5)
void multiConditionSearchMenuAB(char cityPrefix, LinkedList& city) {
    cout << "\n  ===== MULTI-CONDITION SEARCH =====\n";
    cout << "  [1] Age Group (range) AND Mode of Transport\n";
    cout << "  [2] Distance Threshold (> X km) AND Transport Type\n";
    cout << "  [0] Back\n";
    int choice = getValidIntInput("  Enter choice: ");

    if (choice == 0) { cout << "  Returning to search menu.\n"; return; }

    if (choice == 1) {
        int minA = getValidIntInput("  Enter min age (0 = Back): ");
        if (minA == 0) { cout << "  Returning.\n"; return; }
        int maxA = getValidIntInput("  Enter max age: ");
        string mode = getValidMode(cityPrefix);
        city.multiConditionSearchAgeMode(minA, maxA, mode);

    } else if (choice == 2) {
        double km = getValidDoubleInput("  Enter distance threshold (km, 0 = Back): ");
        if (km == 0.0) { cout << "  Returning.\n"; return; }
        string mode = getValidMode(cityPrefix);
        city.multiConditionSearchDistMode(km, mode);

    } else {
        cout << "  Invalid choice.\n";
    }
}


void cityCMenu(LinkedList& cityC) {
    do {
        printCityMenu("C");
        int choice = getValidIntInput("Enter your choice (0-12): ");

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
                compareArrayVsLinkedListCityC(cityC);
                break;

            case 12:
                return;

            case 0:
                cout << "\nGoodbye!\n";
                exit(0);

            default:
                cout << "  Invalid choice! Please enter 0-12.\n";
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
    loadCityCArray("datasetC.csv");  // also load City C into array for comparison

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