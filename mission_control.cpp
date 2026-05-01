//handles all the menus and user input stuff
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>
#include <limits>
#include "city_linkedlist.hpp"
using namespace std;

// ==============================================================
//   City C - array bits   (Ahmad)
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

// ── border constants ──────────────────────────────────────────────────────
// Stats box: Label(20) | Value(55)
static const char* STAT_BD =
    "+----------------------+---------------------------------------------------------+";
// Array/search table: ID(6) Age(4) Group(35) Mode(12) Dist(9) Total(14)
static const char* CARY_BD =
    "+--------+------+-------------------------------------+--------------+-----------+----------------+";
// Menu box top/divider (52 chars between +)
static const char* MNU_HDR = "+====================================================+";
static const char* MNU_MID = "+----------------------------------------------------+";
//   Comparison table - by   Ahmad   
//    (yeah, Ahmad wrote this part; notes are a bit messy)

static const char* CMP_BD =
    "+--------------+----------+--------------+--------------+--------------+";

// ── output helpers ────────────────────────────────────────────────────────
static string mc_fit(const string& s, int w) {
    if ((int)s.size() <= w) return s;
    return s.substr(0, w - 1) + "~";
}
static string mc_fmtD(double v, int prec) {
    ostringstream oss; oss << fixed << setprecision(prec) << v; return oss.str();
}
static void mc_statRow(const string& lbl, const string& val) {
    cout << "| " << left << setw(20) << mc_fit(lbl, 20)
         << " | " << left << setw(55) << mc_fit(val, 55) << " |\n";
}
// bordered stats box for a sort operation (linked list)
static void mc_printSortStats(const string& algo, double us,
                               const string& tc, const string& sc, int total) {
    struct PlainRes { string a; int b; string c; double d, e; int f; string g; double h; };
    struct LLNode   { PlainRes data; void* next; };
    string mem = to_string(total) + " nodes x " + to_string((long long)sizeof(LLNode))
               + " B = ~" + to_string((long long)total * (long long)sizeof(LLNode))
               + " B  (arr ~" + to_string((long long)total * (long long)sizeof(PlainRes)) + " B)";
    cout << "\n" << STAT_BD << "\n";
    mc_statRow("Algorithm",        algo);
    mc_statRow("Time",             mc_fmtD(us, 4) + " microseconds");
    mc_statRow("Time Complexity",  tc);
    mc_statRow("Space Complexity", sc);
    mc_statRow("Records Sorted",   to_string(total));
    mc_statRow("Memory Usage",     mem);
    cout << STAT_BD << "\n";
}
// bordered stats box for an array search operation
static void mc_printArraySearchStats(const string& label, double us,
                                      int found, int total) {
    long long sz  = (long long)sizeof(CityCResident);
    string mem = to_string(total) + " x " + to_string(sz)
               + " B = ~" + to_string((long long)total * sz) + " B (array)";
    cout << "\n" << STAT_BD << "\n";
    mc_statRow("Algorithm",        label);
    mc_statRow("Time",             mc_fmtD(us, 4) + " microseconds");
    mc_statRow("Time Complexity",  "O(n)");
    mc_statRow("Space Complexity", "O(1)");
    mc_statRow("Records Found",    to_string(found) + " / " + to_string(total));
    mc_statRow("Memory (Array)",   mem);
    cout << STAT_BD << "\n";
}
// one menu content line, left-justified, 50-char wide
static void menuLine(const string& s = "") {
    cout << "| " << left << setw(50) << mc_fit(s, 50) << " |\n";
}

// ── City C array display helpers ──────────────────────────────────────────
static void caryHeader() {
    cout << CARY_BD << "\n";
    cout << "| " << left
         << setw(6)  << "ID"        << " | "
         << setw(4)  << "Age"       << " | "
         << setw(35) << "Age Group" << " | "
         << setw(12) << "Mode"      << " | "
         << setw(9)  << "Dist(km)"  << " | "
         << setw(14) << "Total(kg)" << " |\n";
    cout << CARY_BD << "\n";
}
static void caryRow(const CityCResident& r) {
    cout << "| " << left
         << setw(6)  << mc_fit(r.residentID, 6)   << " | "
         << right    << setw(4) << r.age            << " | "
         << left     << setw(35) << mc_fit(r.ageGroup, 35) << " | "
         << setw(12) << mc_fit(r.mode, 12)          << " | "
         << right    << setw(9)  << fixed << setprecision(1) << r.distance      << " | "
         << right    << setw(14) << fixed << setprecision(2) << r.totalEmission << " |\n";
}

// ── City C array load ─────────────────────────────────────────────────────
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
            r.residentID     = id;
            r.age            = stoi(ageStr);
            r.mode           = mode;
            r.distance       = stod(distStr);
            r.emissionFactor = stod(emStr);
            r.days           = stoi(daysStr);
            r.ageGroup       = ageGroup;
            r.totalEmission  = totalStr.empty()
                               ? r.distance * r.emissionFactor * r.days
                               : stod(totalStr);
            cityCArr[cityCArr_size++] = r;
        } catch (...) {}
    }
    file.close();
}

// display City C array as a bordered table
void displayCityCArray() {
    if (cityCArr_size == 0) { cout << "  (array is empty)\n"; return; }
    cout << "\n";
    caryHeader();
    for (int i = 0; i < cityCArr_size; i++) caryRow(cityCArr[i]);
    cout << CARY_BD << "\n";
    cout << "  Total records: " << cityCArr_size << "\n";
}

// bubble sort on a copy of the City C array (does not modify the original)
double bubbleSortCityCArr(int field) {
    CityCResident tmp[MAX_CITY_C_ARR];
    for (int i = 0; i < cityCArr_size; i++) tmp[i] = cityCArr[i];

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < cityCArr_size - 1; i++) {
        for (int j = 0; j < cityCArr_size - i - 1; j++) {
            bool doSwap = false;
            if      (field == 1 && tmp[j].age           > tmp[j+1].age)           doSwap = true;
            else if (field == 2 && tmp[j].distance      > tmp[j+1].distance)      doSwap = true;
            else if (field == 3 && tmp[j].totalEmission > tmp[j+1].totalEmission) doSwap = true;
            if (doSwap) { CityCResident t = tmp[j]; tmp[j] = tmp[j+1]; tmp[j+1] = t; }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, micro>(end - start).count();
}

// linear search on City C array by mode
void linearSearchCityCArr_mode(const string& mode) {
    string modeLow = mode;
    for (char& c : modeLow) c = (char)tolower(c);

    cout << "\n";
    caryHeader();
    auto start = chrono::high_resolution_clock::now();
    int found = 0;
    for (int i = 0; i < cityCArr_size; i++) {
        string cur = cityCArr[i].mode;
        for (char& c : cur) c = (char)tolower(c);
        if (cur == modeLow) { caryRow(cityCArr[i]); found++; }
    }
    auto end = chrono::high_resolution_clock::now();
    double us = chrono::duration<double, micro>(end - start).count();
    cout << CARY_BD << "\n";
    mc_printArraySearchStats("Linear Search (Array) by Mode", us, found, cityCArr_size);
}

// linear search on City C array by age range
void linearSearchCityCArr_age(int minAge, int maxAge) {
    cout << "\n";
    caryHeader();
    auto start = chrono::high_resolution_clock::now();
    int found = 0;
    for (int i = 0; i < cityCArr_size; i++) {
        if (cityCArr[i].age >= minAge && cityCArr[i].age <= maxAge) {
            caryRow(cityCArr[i]); found++;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    double us = chrono::duration<double, micro>(end - start).count();
    cout << CARY_BD << "\n";
    mc_printArraySearchStats("Linear Search (Array) by Age", us, found, cityCArr_size);
}

// compare City C array vs linked list
void compareArrayVsLinkedListCityC(const LinkedList& cityC) {
    struct PlainNode { string a; int b; string c; double d, e; int f; string g; double h; };
    struct LLNode    { PlainNode data; void* next; };
    size_t arrSz      = sizeof(CityCResident);
    size_t llSz       = sizeof(LLNode);
    size_t arrTotal   = (size_t)cityCArr_size * arrSz;
    size_t llTotal    = (size_t)cityC.size()  * llSz;

    cout << "\n" << MNU_HDR << "\n";
    menuLine("      CITY C: ARRAY vs LINKED LIST");
    cout << MNU_HDR << "\n";

    // memory section
    cout << "\n" << STAT_BD << "\n";
    mc_statRow("Section",         "Memory Usage");
    mc_statRow("Array node size", to_string((long long)arrSz) + " B");
    mc_statRow("LL node size",    to_string((long long)llSz)
                                  + " B (+" + to_string((long long)(llSz - arrSz)) + " ptr)");
    mc_statRow("Array total",     to_string((long long)arrTotal)
                                  + " B  (" + to_string(cityCArr_size) + " records)");
    mc_statRow("LL total",        to_string((long long)llTotal)
                                  + " B  (" + to_string(cityC.size()) + " records)");
    mc_statRow("LL overhead",     to_string((long long)(llTotal - arrTotal))
                                  + " B extra ("
                                  + mc_fmtD((double)(llTotal - arrTotal) / arrTotal * 100.0, 1)
                                  + "% more)");
    cout << STAT_BD << "\n";

    // traversal time
    auto t1 = chrono::high_resolution_clock::now();
    double arrSum = 0;
    for (int i = 0; i < cityCArr_size; i++) arrSum += cityCArr[i].totalEmission;
    auto t2 = chrono::high_resolution_clock::now();
    cityC.carbonAnalysis();
    auto t3 = chrono::high_resolution_clock::now();
    long long arrUs = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    long long llUs  = chrono::duration_cast<chrono::microseconds>(t3 - t2).count();

    cout << "\n" << STAT_BD << "\n";
    mc_statRow("Section",         "Traversal Time (sum emissions)");
    mc_statRow("Array traversal", to_string(arrUs)
                                  + " us  (sum = " + mc_fmtD(arrSum, 2) + " kg CO2)");
    mc_statRow("LL traversal",    to_string(llUs) + " us");
    mc_statRow("Faster",          arrUs <= llUs ? "Array" : "Linked List");
    cout << STAT_BD << "\n";

    // sort time comparison
    double arrSortMs = bubbleSortCityCArr(3);
    LinkedList& cityC_mut = const_cast<LinkedList&>(cityC);
    cityC_mut.sortByEmission();
    double llSortMs = cityC_mut.sortTime();
    cityC_mut.sortByResidentID();

    cout << "\n" << STAT_BD << "\n";
    mc_statRow("Section",     "Bubble Sort by Emission");
    mc_statRow("Array sort",  mc_fmtD(arrSortMs, 4) + " ms");
    mc_statRow("LL sort",     mc_fmtD(llSortMs,  4) + " ms");
    mc_statRow("Faster",      arrSortMs <= llSortMs ? "Array" : "Linked List");
    cout << STAT_BD << "\n";

    // key differences
    cout << "\n" << STAT_BD << "\n";
    mc_statRow("Section",     "Key Structural Differences");
    mc_statRow("Array",       "Fixed size  O(1) random access  cache-friendly");
    mc_statRow("Linked List", "Dynamic size  O(n) access  extra pointer/node");
    cout << STAT_BD << "\n\n";
}

// ── forward declarations ──────────────────────────────────────────────────
void cityAMenu(LinkedList& cityA);
void cityBMenu(LinkedList& cityB);
void cityCMenu(LinkedList& cityC);
void compareCities(const LinkedList& cityA, const LinkedList& cityB, const LinkedList& cityC);
void multiConditionSearchMenuAB(char cityPrefix, LinkedList& city);

// only allows valid transport modes for each city
string getValidMode(char cityPrefix) {
    const int MAX_MODES = 6;
    string modes[MAX_MODES];
    int modeCount = 0;

    if (cityPrefix == 'A') {
        modes[0] = "Car"; modes[1] = "Bus"; modes[2] = "Bicycle"; modes[3] = "Walking";
        modeCount = 4;
    } else if (cityPrefix == 'B') {
        modes[0] = "Carpool"; modes[1] = "Walking"; modes[2] = "Bicycle"; modes[3] = "School Bus";
        modeCount = 4;
    } else {
        modes[0] = "Car";  modes[1] = "Bus";        modes[2] = "Bicycle";
        modes[3] = "Walking"; modes[4] = "School Bus"; modes[5] = "Carpool";
        modeCount = 6;
    }

    while (true) {
        cout << "  Select Mode of Transport:\n";
        for (int i = 0; i < modeCount; ++i)
            cout << "    [" << (i + 1) << "] " << modes[i] << "\n";
        cout << "  Your choice (number or name): ";

        string input;
        getline(cin, input);

        size_t s = input.find_first_not_of(" \t");
        size_t e = input.find_last_not_of(" \t");
        if (s == string::npos) { cout << "  [ERROR] Input cannot be empty.\n"; continue; }
        input = input.substr(s, e - s + 1);

        bool isNumber = !input.empty();
        for (char c : input) if (!isdigit(c)) { isNumber = false; break; }
        if (isNumber) {
            int idx = stoi(input);
            if (idx >= 1 && idx <= modeCount) return modes[idx - 1];
            cout << "  [ERROR] Please enter a number between 1 and " << modeCount << ".\n";
            continue;
        }

        string lower = input;
        for (char& c : lower) c = (char)tolower(c);
        for (int i = 0; i < modeCount; ++i) {
            string cmp = modes[i];
            for (char& c : cmp) c = (char)tolower(c);
            if (lower == cmp) return modes[i];
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
void collectResidentData(char cityPrefix, string& id, int& age, string& mode,
                         double& dist, double& emFactor, int& days,
                         string& ageGroup, double& totalEmission) {
    while (true) {
        cout << "  Resident ID            : "; getline(cin, id);
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
    ageGroup = LinkedList::ageGroupLabel(age, cityPrefix);
    cout << "  Age Group (auto)       : " << ageGroup << "\n";
    mode         = getValidMode(cityPrefix);
    dist         = getValidDoubleInput("  Daily Distance (km)    : ");
    emFactor     = getValidDoubleInput("  Emission Factor (kg/km): ");
    days         = getValidIntInput("  Days per Month         : ");
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

    string algoName, fieldName, tc, sc;
    if (algo == 1) {
        algoName = "Bubble Sort"; tc = "O(n^2)"; sc = "O(1)";
        if      (field == 1) { city.sortByAge();      fieldName = "Age"; }
        else if (field == 2) { city.sortByDistance(); fieldName = "Daily Distance"; }
        else if (field == 3) { city.sortByEmission(); fieldName = "Total Emission"; }
        else { cout << "  Invalid field.\n"; return; }
    } else {
        algoName = "Insertion Sort"; tc = "O(n^2) worst / O(n) best"; sc = "O(1)";
        if      (field == 1) { city.insertionSortByAge();      fieldName = "Age"; }
        else if (field == 2) { city.insertionSortByDistance(); fieldName = "Daily Distance"; }
        else if (field == 3) { city.insertionSortByEmission(); fieldName = "Total Emission"; }
        else { cout << "  Invalid field.\n"; return; }
    }
    city.display();
    mc_printSortStats(algoName + " by " + fieldName, city.sortTime(), tc, sc, city.size());
}

// sort menu for city C
void sortMenu(LinkedList& city) {
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
    cout << "  [3] Total Carbon Emission\n";
    cout << "  [0] Back\n";
    int field = getValidIntInput("  Your choice: ");
    if (field == 0) { cout << "  Returning to city menu.\n"; return; }

    string algoName, fieldName, tc, sc;
    if (algo == 1) {
        algoName = "Bubble Sort"; tc = "O(n^2)"; sc = "O(1)";
        if      (field == 1) { city.sortByAge();      fieldName = "Age"; }
        else if (field == 2) { city.sortByDistance(); fieldName = "Daily Distance"; }
        else if (field == 3) { city.sortByEmission(); fieldName = "Total Carbon Emission"; }
        else { cout << "  Invalid field.\n"; return; }
    } else {
        algoName = "Insertion Sort"; tc = "O(n^2) worst / O(n) best"; sc = "O(1)";
        if      (field == 1) { city.insertionSortByAge();      fieldName = "Age"; }
        else if (field == 2) { city.insertionSortByDistance(); fieldName = "Daily Distance"; }
        else if (field == 3) { city.insertionSortByEmission(); fieldName = "Total Carbon Emission"; }
        else { cout << "  Invalid field.\n"; return; }
    }
    city.display();
    mc_printSortStats(algoName + " by " + fieldName, city.sortTime(), tc, sc, city.size());
}

// search menu for city A and B
void searchMenuAB(char cityPrefix, LinkedList& city) {
    cout << "\n  Search by:\n";
    cout << "  [1] Age Group (range)\n";
    cout << "  [2] Mode of Transport\n";
    cout << "  [3] Distance Threshold (> X km)\n";
    cout << "  [4] Exact Age - Binary Search\n";
    cout << "  [5] Multi-Condition Search\n";
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

// search menu for city C
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

// prints the city menu with bordered box design
void printCityMenu(const string& cityName) {
    cout << "\n" << MNU_HDR << "\n";
    menuLine("              CITY " + cityName + " MENU");
    cout << MNU_HDR << "\n";
    menuLine();
    menuLine("  Data Viewing");
    menuLine("  [1]  View Sorted Result");
    menuLine("  [2]  View Original Dataset");
    menuLine();
    menuLine("  Linked List Operations");
    menuLine("  [3]  Reverse Linked List");
    menuLine();
    menuLine("  Data Processing");
    menuLine("  [4]  Sort Data");
    menuLine("  [5]  Search Data");
    menuLine();
    menuLine("  Analysis");
    if (cityName == "A" || cityName == "B") {
        menuLine("  [6]  Total Emissions Summary");
        menuLine("  [7]  Emissions by Transport Mode");
        menuLine("  [8]  Transport Percentage Distribution");
        menuLine("  [9]  Age Group Analysis");
        menuLine("  [10] Top / Lowest Emitters");
        menuLine();
        cout << MNU_MID << "\n";
        menuLine("  [11] Back to Main Menu");
        menuLine("  [0]  Exit Program");
    } else {
        menuLine("  [6]  Carbon Emission Analysis");
        menuLine("  [7]  Age Group Analysis");
        menuLine("  [8]  Array vs Linked List Comparison");
        menuLine();
        cout << MNU_MID << "\n";
        menuLine("  [9]  Back to Main Menu");
        menuLine("  [0]  Exit Program");
    }
    cout << MNU_HDR << "\n";
}

// city A menu
void cityAMenu(LinkedList& cityA) {
    do {
        printCityMenu("A");
        int choice = getValidIntInput("  Your choice (0-11): ");

        string id, mode, ageGroup;
        int age, days, pos, oldSize;
        double dist, emFactor, totalEmission;

        switch (choice) {
            case 1:
                cityA.display();
                cityA.saveToCSV();
                break;
            case 2:
                cityA.sortByResidentID();
                cityA.display();
                cityA.saveToCSV();
                break;
            case 3:
                cityA.reverse();
                cityA.display();
                cityA.saveToCSV();
                break;
            case 4:
                sortMenuAB(cityA);
                cityA.saveToCSV();
                break;
            case 5:
                searchMenuAB('A', cityA);
                break;
            case 6:
                cityA.totalEmissionsReport('A');
                break;
            case 7:
                cityA.emissionsByMode('A');
                break;
            case 8:
                cityA.transportPercentageDistribution('A');
                break;
            case 9:
                cityA.ageGroupAnalysisExtended('A');
                break;
            case 10: {
                int n = getValidIntInput("  Show top/bottom N emitters (e.g. 5): ");
                cityA.topBottomEmitters(n);
                break;
            }
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

// city B menu - same structure as A
void cityBMenu(LinkedList& cityB) {
    do {
        printCityMenu("B");
        int choice = getValidIntInput("  Your choice (0-11): ");

        string id, mode, ageGroup;
        int age, days, pos, oldSize;
        double dist, emFactor, totalEmission;

        switch (choice) {
            case 1:
                cityB.display();
                cityB.saveToCSV();
                break;
            case 2:
                cityB.sortByResidentID();
                cityB.display();
                cityB.saveToCSV();
                break;
            case 3:
                cityB.reverse();
                cityB.display();
                cityB.saveToCSV();
                break;
            case 4:
                sortMenuAB(cityB);
                cityB.saveToCSV();
                break;
            case 5:
                searchMenuAB('B', cityB);
                break;
            case 6:
                cityB.totalEmissionsReport('B');
                break;
            case 7:
                cityB.emissionsByMode('B');
                break;
            case 8:
                cityB.transportPercentageDistribution('B');
                break;
            case 9:
                cityB.ageGroupAnalysisExtended('B');
                break;
            case 10: {
                int n = getValidIntInput("  Show top/bottom N emitters (e.g. 5): ");
                cityB.topBottomEmitters(n);
                break;
            }
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
        int choice = getValidIntInput("  Your choice (0-9): ");

        string id, mode, ageGroup;
        int age, days, pos, oldSize;
        double dist, emFactor, totalEmission;

        switch (choice) {
            case 1:
                cityC.display();
                cityC.saveToCSV();
                break;
            case 2:
                cityC.sortByResidentID();
                cityC.display();
                cityC.saveToCSV();
                break;
            case 3:
                cityC.reverse();
                cityC.display();
                cityC.saveToCSV();
                break;
            case 4:
                sortMenu(cityC);
                cityC.saveToCSV();
                break;
            case 5:
                searchMenu('C', cityC);
                break;
            case 6:
                cityC.carbonAnalysis();
                break;
            case 7:
                cityC.ageGroupAnalysis();
                break;
            case 8:
                compareArrayVsLinkedListCityC(cityC);
                break;
            case 9:
                return;
            case 0:
                cout << "\nGoodbye!\n";
                exit(0);
            default:
                cout << "  Invalid choice! Please enter 0-9.\n";
        }
    } while (true);
}

// compare all 3 cities
void compareCities(const LinkedList& cityA, const LinkedList& cityB, const LinkedList& cityC) {
    cout << "\n" << MNU_HDR << "\n";
    menuLine("     COMPARISON: CITY A vs CITY B vs CITY C");
    cout << MNU_HDR << "\n";

    struct PlainResident { string id; int age; string mode; double dist, emFactor;
                           int days; string ageGroup; double total; };
    struct LinkedNode    { PlainResident data; void* next; };
    size_t arrSz = sizeof(PlainResident);
    size_t llSz  = sizeof(LinkedNode);

    // memory comparison table
    cout << "\n" << CMP_BD << "\n";
    cout << "| " << left  << setw(12) << "City"       << " | "
                 << right << setw(8)  << "Records"    << " | "
                 << right << setw(12) << "Array (B)"  << " | "
                 << right << setw(12) << "LL (B)"     << " | "
                 << right << setw(12) << "Extra (B)"  << " |\n";
    cout << CMP_BD << "\n";

    auto cmpRow = [&](const string& name, int cnt) {
        long long aB = (long long)cnt * (long long)arrSz;
        long long lB = (long long)cnt * (long long)llSz;
        cout << "| " << left  << setw(12) << name << " | "
                     << right << setw(8)  << cnt  << " | "
                     << right << setw(12) << aB   << " | "
                     << right << setw(12) << lB   << " | "
                     << right << setw(12) << (lB - aB) << " |\n";
    };
    cmpRow("City A", cityA.size());
    cmpRow("City B", cityB.size());
    cmpRow("City C", cityC.size());
    cout << CMP_BD << "\n";
    cout << "  * LL uses extra " << (long long)(llSz - arrSz)
         << " B/node for the next pointer.\n";

    // traversal time
    auto t1 = chrono::high_resolution_clock::now();
    cityA.carbonAnalysis();
    auto t2 = chrono::high_resolution_clock::now();
    cityB.carbonAnalysis();
    auto t3 = chrono::high_resolution_clock::now();
    cityC.carbonAnalysis();
    auto t4 = chrono::high_resolution_clock::now();

    cout << "\n" << STAT_BD << "\n";
    mc_statRow("Section",          "Traversal Time (sum emissions)");
    mc_statRow("City A traversal",
               to_string(chrono::duration_cast<chrono::microseconds>(t2-t1).count()) + " us");
    mc_statRow("City B traversal",
               to_string(chrono::duration_cast<chrono::microseconds>(t3-t2).count()) + " us");
    mc_statRow("City C traversal",
               to_string(chrono::duration_cast<chrono::microseconds>(t4-t3).count()) + " us");
    cout << STAT_BD << "\n";

    // age group analysis
    cout << "\n>>> CITY A <<<\n";
    cityA.ageGroupAnalysis();
    cout << "\n>>> CITY B <<<\n";
    cityB.ageGroupAnalysis();
    cout << "\n>>> CITY C <<<\n";
    cityC.ageGroupAnalysis();

    cout << "\n" << MNU_HDR << "\n\n";
}

// main menu screen
void displayMainMenu() {
    cout << "\n" << MNU_HDR << "\n";
    menuLine("      CITYVILLE CARBON ANALYSIS SYSTEM");
    menuLine("            (LINKED LIST VERSION)");
    cout << MNU_HDR << "\n";
    menuLine();
    menuLine("  [1] City A  (Metropolitan City)");
    menuLine("  [2] City B  (University Town)");
    menuLine("  [3] City C  (Suburban / Rural Area)");
    menuLine();
    menuLine("  [4] Compare All Cities");
    menuLine("  [0] Exit");
    menuLine();
    cout << MNU_HDR << "\n";
}

// entry point
int main() {
    LinkedList cityA, cityB, cityC;

    cout << "\nLoading datasets...\n";
    cityA.loadFromCSV("datasetA.csv");
    cityB.loadFromCSV("datasetB.csv");
    cityC.loadFromCSV("datasetC.csv");
    loadCityCArray("datasetC.csv");

    int choice;
    do {
        displayMainMenu();
        choice = getValidIntInput("  Your choice: ");

        switch (choice) {
            case 1: cityAMenu(cityA);                   break;
            case 2: cityBMenu(cityB);                   break;
            case 3: cityCMenu(cityC);                   break;
            case 4: compareCities(cityA, cityB, cityC); break;
            case 0: cout << "\nGoodbye!\n";             break;
            default: cout << "\nInvalid choice. Please enter 0-4.\n";
        }
    } while (choice != 0);

    return 0;
}
