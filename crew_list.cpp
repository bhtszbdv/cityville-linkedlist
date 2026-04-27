  // crew_list.cpp - all the linked list logic is in here
#include "city_linkedlist.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <chrono>
using namespace std;

    // nothing special in constructor/destructor just initialise head and count
LinkedList::LinkedList() : head(nullptr), count(0) {}

LinkedList::~LinkedList() { clear(); }

void LinkedList::clear() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur->next;
        delete cur;
        cur = tmp;
    }
    head  = nullptr;
    count = 0;
}

    // scan the list and check if the same id already exists
bool LinkedList::idExists(const string& id) const {
    Node* cur = head;
    while (cur) {
        if (cur->residentID == id) return true;
        cur = cur->next;
    }
    return false;
}

       // append new node at the end
void LinkedList::insert(const string& id, int age, const string& mode,
                        double distance, double emFactor, int days,
                        const string& ageGroup, double totalEmission) {
    if (idExists(id)) {
        cout << "  [ERROR] Resident ID \"" << id << "\" already exists. Insert cancelled.\n";
        return;
    }
    Node* newNode = new Node();
    newNode->residentID     = id;
    newNode->age            = age;
    newNode->mode           = mode;
    newNode->distance       = distance;
    newNode->emissionFactor = emFactor;
    newNode->days           = days;
    newNode->ageGroup       = ageGroup;
    newNode->totalEmission  = totalEmission;
    newNode->next           = nullptr;

    if (!head) {
        head = newNode;
    } else {
        Node* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = newNode;
    }
    ++count;
}

      // put the new node at the very beginning
void LinkedList::insertAtBeginning(const string& id, int age, const string& mode,
                                   double distance, double emFactor, int days,
                                   const string& ageGroup, double totalEmission) {
    if (idExists(id)) {
        cout << "  [ERROR] Resident ID \"" << id << "\" already exists. Insert cancelled.\n";
        return;
    }
    Node* newNode = new Node();
    newNode->residentID     = id;
    newNode->age            = age;
    newNode->mode           = mode;
    newNode->distance       = distance;
    newNode->emissionFactor = emFactor;
    newNode->days           = days;
    newNode->ageGroup       = ageGroup;
    newNode->totalEmission  = totalEmission;
    newNode->next           = head;
    head = newNode;
    ++count;
    cout << "  Resident " << id << " inserted at the beginning.\n";
}

    // insert at a user-given position, 1 = head
void LinkedList::insertAtPosition(int pos, const string& id, int age, const string& mode,
                                  double distance, double emFactor, int days,
                                  const string& ageGroup, double totalEmission) {
    if (idExists(id)) {
        cout << "  [ERROR] Resident ID \"" << id << "\" already exists. Insert cancelled.\n";
        return;
    }
    if (pos <= 1) {
        insertAtBeginning(id, age, mode, distance, emFactor, days, ageGroup, totalEmission);
        return;
    }
    if (pos > count + 1) {
        cout << "  [WARN] Position " << pos << " is beyond list size (" << count
             << "). Inserting at end.\n";
        insert(id, age, mode, distance, emFactor, days, ageGroup, totalEmission);
        cout << "  Resident " << id << " inserted at end (position " << count << ").\n";
        return;
    }

    Node* newNode = new Node();
    newNode->residentID     = id;
    newNode->age            = age;
    newNode->mode           = mode;
    newNode->distance       = distance;
    newNode->emissionFactor = emFactor;
    newNode->days           = days;
    newNode->ageGroup       = ageGroup;
    newNode->totalEmission  = totalEmission;

    Node* cur = head;
    for (int i = 1; i < pos - 1; ++i) cur = cur->next;
    newNode->next = cur->next;
    cur->next     = newNode;
    ++count;
    cout << "  Resident " << id << " inserted at position " << pos << ".\n";
}

// find and remove a resident by their id
bool LinkedList::deleteResident(const string& id) {
    if (!head) {
        cout << "  List is empty. Nothing to delete.\n";
        return false;
    }

    // check if its the first one
    if (head->residentID == id) {
        Node* tmp = head;
        head = head->next;
        delete tmp;
        --count;
        cout << "  Resident " << id << " deleted successfully.\n";
        return true;
    }

    // otherwise keep looking
    Node* prev = head;
    Node* cur  = head->next;
    while (cur) {
        if (cur->residentID == id) {
            prev->next = cur->next;
            delete cur;
            --count;
            cout << "  Resident " << id << " deleted successfully.\n";
            return true;
        }
        prev = cur;
        cur  = cur->next;
    }

    cout << "  Resident ID \"" << id << "\" not found.\n";
    return false;
}

// flip the whole list around
void LinkedList::reverse() {
    if (!head || !head->next) {
        cout << "  List has fewer than 2 nodes - nothing to reverse.\n";
        return;
    }
    Node* prev = nullptr;
    Node* cur  = head;
    while (cur) {
        Node* nxt  = cur->next;
        cur->next  = prev;
        prev       = cur;
        cur        = nxt;
    }
    head = prev;
    cout << "  Linked list reversed successfully (" << count << " nodes).\n";
}

// read data from the csv file and load into the list
bool LinkedList::loadFromCSV(const string& filename) {
    sourceFilename = filename;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "  [ERROR] Cannot open file: " << filename << "\n";
        return false;
    }

    string line;
    getline(file, csvHeader); // save the header row so we can restore it when saving

    int loaded = 0;
    while (getline(file, line)) {
        if (line.empty() || line == "\r") continue;

        // windows files sometimes have \r at the end
        if (!line.empty() && line.back() == '\r') line.pop_back();

        stringstream ss(line);
        string id, ageStr, mode, distStr, emStr, daysStr, ageGroup, totalEmStr;

        getline(ss, id,         ',');
        getline(ss, ageStr,     ',');
        getline(ss, mode,       ',');
        getline(ss, distStr,    ',');
        getline(ss, emStr,      ',');
        getline(ss, daysStr,    ',');
        getline(ss, ageGroup,   ',');  // column 7 - AgeGroup
        getline(ss, totalEmStr, ',');  // column 8 - TotalMonthlyEmission

        // clean up any extra spaces
        auto trim = [](string& s) {
            size_t start = s.find_first_not_of(" \t\r\n");
            size_t end   = s.find_last_not_of(" \t\r\n");
            s = (start == string::npos) ? "" : s.substr(start, end - start + 1);
        };
        trim(id); trim(ageStr); trim(mode); trim(distStr);
        trim(emStr); trim(daysStr); trim(ageGroup); trim(totalEmStr);

        if (id.empty()) continue;

        try {
            int    age      = stoi(ageStr);
            double dist     = stod(distStr);
            double em       = stod(emStr);
            int    days     = stoi(daysStr);
            // use TotalMonthlyEmission from CSV if present, otherwise compute it
            double totalEm  = totalEmStr.empty() ? (dist * em * days) : stod(totalEmStr);
            // use AgeGroup from CSV if present, otherwise derive it from age + city
            if (ageGroup.empty()) {
                // infer city prefix from the resident ID prefix (first char of id)
                char cityPfx = (!id.empty()) ? (char)toupper(id[0]) : '\0';
                ageGroup = ageGroupLabel(age, cityPfx);
            }
            insert(id, age, mode, dist, em, days, ageGroup, totalEm);
            ++loaded;
        } catch (...) {
            cout << "  [WARN] Skipping bad row: " << line << "\n";
        }
    }
    file.close();
    cout << "  Loaded " << loaded << " records from " << filename << "\n";
    return true;
}

// push the current linked list completely back into the csv file
bool LinkedList::saveToCSV() const {
    if (sourceFilename.empty()) return false;
    ofstream file(sourceFilename);
    if (!file.is_open()) {
        cout << "  [ERROR] Could not open file for writing: " << sourceFilename << "\n";
        return false;
    }

    // write the original header block we saved
    if (!csvHeader.empty()) file << csvHeader << "\n";
    else file << "ResidentID,Age,ModeOfTransport,DailyDistance,CarbonEmissionFactor,AverageDayPerMonth,AgeGroup,TotalMonthlyEmission\n";

    int saved = 0;
    Node* cur = head;
    while (cur) {
        file << cur->residentID    << ","
             << cur->age           << ","
             << cur->mode          << ","
             << cur->distance      << ","
             << cur->emissionFactor << ","
             << cur->days          << ","
             << cur->ageGroup      << ","
             << fixed << setprecision(2) << cur->totalEmission << "\n";
        cur = cur->next;
        saved++;
    }

    file.close();
    // No print here, we will let mission_control print the success message
    return true;
}

// print all the data in a table
void LinkedList::display() const {
    if (!head) { cout << "  (no data)\n"; return; }

    cout << "\n";
    cout << left
         << setw(8)  << "ID"
         << setw(6)  << "Age"
         << setw(36) << "Age Group"
         << setw(14) << "Mode"
         << setw(10) << "Dist(km)"
         << setw(12) << "EmFactor"
         << setw(6)  << "Days"
         << setw(14) << "TotalEmis(kg)"
         << "\n";
    cout << string(106, '-') << "\n";

    Node* cur = head;
    while (cur) {
        cout << left
             << setw(8)  << cur->residentID
             << setw(6)  << cur->age
             << setw(36) << cur->ageGroup
             << setw(14) << cur->mode
             << setw(10) << fixed << setprecision(1) << cur->distance
             << setw(12) << fixed << setprecision(2) << cur->emissionFactor
             << setw(6)  << cur->days
             << setw(14) << fixed << setprecision(2) << cur->totalEmission
             << "\n";
        cur = cur->next;
    }
    cout << string(106, '-') << "\n";
    cout << "  Total records: " << count << "\n";
}

// swap all the data inside two nodes, dont touch the pointers
void LinkedList::swapData(Node* a, Node* b) {
    swap(a->residentID,     b->residentID);
    swap(a->age,            b->age);
    swap(a->mode,           b->mode);
    swap(a->distance,       b->distance);
    swap(a->emissionFactor, b->emissionFactor);
    swap(a->days,           b->days);
    swap(a->ageGroup,       b->ageGroup);
    swap(a->totalEmission,  b->totalEmission);
}

// bubble sort by age
void LinkedList::sortByAge() {
    if (!head || !head->next) return;

    auto start = chrono::high_resolution_clock::now();

    bool swapped;
    do {
        swapped = false;
        Node* cur = head;
        while (cur->next) {
            if (cur->age > cur->next->age) {
                swapData(cur, cur->next);
                swapped = true;
            }
            cur = cur->next;
        }
    } while (swapped);

    auto end = chrono::high_resolution_clock::now();
    lastSortMs = chrono::duration<double, milli>(end - start).count();
}

// bubble sort by distance
void LinkedList::sortByDistance() {
    if (!head || !head->next) return;

    auto start = chrono::high_resolution_clock::now();

    bool swapped;
    do {
        swapped = false;
        Node* cur = head;
        while (cur->next) {
            if (cur->distance > cur->next->distance) {
                swapData(cur, cur->next);
                swapped = true;
            }
            cur = cur->next;
        }
    } while (swapped);

    auto end = chrono::high_resolution_clock::now();
    lastSortMs = chrono::duration<double, milli>(end - start).count();
}

// bubble sort by total carbon emission
void LinkedList::sortByEmission() {
    if (!head || !head->next) return;

    auto start = chrono::high_resolution_clock::now();

    bool swapped;
    do {
        swapped = false;
        Node* cur = head;
        while (cur->next) {
            if (cur->totalEmission > cur->next->totalEmission) {
                swapData(cur, cur->next);
                swapped = true;
            }
            cur = cur->next;
        }
    } while (swapped);

    auto end = chrono::high_resolution_clock::now();
    lastSortMs = chrono::duration<double, milli>(end - start).count();
}

// insertion sort by age
void LinkedList::insertionSortByAge() {
    if (!head || !head->next) return;

    auto start = chrono::high_resolution_clock::now();

    // only the sorted portion ends at 'sortedEnd'; everything after is unsorted
    Node* sortedEnd = head;
    while (sortedEnd->next) {
        Node* key = sortedEnd->next; // the element we are about to place
        if (key->age >= sortedEnd->age) {
            // already in the right place, just extend the sorted section
            sortedEnd = sortedEnd->next;
        } else {
            // detach key from its current position
            sortedEnd->next = key->next;

            // find where to insert it inside the sorted section
            if (key->age < head->age) {
                // goes before head
                key->next = head;
                head = key;
            } else {
                Node* prev = head;
                while (prev->next != key && prev->next->age <= key->age)
                    prev = prev->next;
                key->next  = prev->next;
                prev->next = key;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    lastSortMs = chrono::duration<double, milli>(end - start).count();
}

// insertion sort by daily distance
void LinkedList::insertionSortByDistance() {
    if (!head || !head->next) return;

    auto start = chrono::high_resolution_clock::now();

    Node* sortedEnd = head;
    while (sortedEnd->next) {
        Node* key = sortedEnd->next;
        if (key->distance >= sortedEnd->distance) {
            sortedEnd = sortedEnd->next;
        } else {
            sortedEnd->next = key->next;
            if (key->distance < head->distance) {
                key->next = head;
                head = key;
            } else {
                Node* prev = head;
                while (prev->next != key && prev->next->distance <= key->distance)
                    prev = prev->next;
                key->next  = prev->next;
                prev->next = key;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    lastSortMs = chrono::duration<double, milli>(end - start).count();
}

// insertion sort by total carbon emission
void LinkedList::insertionSortByEmission() {
    if (!head || !head->next) return;

    auto start = chrono::high_resolution_clock::now();

    Node* sortedEnd = head;
    while (sortedEnd->next) {
        Node* key = sortedEnd->next;
        if (key->totalEmission >= sortedEnd->totalEmission) {
            sortedEnd = sortedEnd->next;
        } else {
            sortedEnd->next = key->next;
            if (key->totalEmission < head->totalEmission) {
                key->next = head;
                head = key;
            } else {
                Node* prev = head;
                while (prev->next != key && prev->next->totalEmission <= key->totalEmission)
                    prev = prev->next;
                key->next  = prev->next;
                prev->next = key;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    lastSortMs = chrono::duration<double, milli>(end - start).count();
}

// restore the original dataset order by sorting lexicographically on the Resident ID string
// e.g. A001 < A002 < ... < A200, B001 < B002, etc.
void LinkedList::sortByResidentID() {
    if (!head || !head->next) return;

    auto start = chrono::high_resolution_clock::now();

    Node* sortedEnd = head;
    while (sortedEnd->next) {
        Node* key = sortedEnd->next;
        if (key->residentID >= sortedEnd->residentID) {
            sortedEnd = sortedEnd->next;
        } else {
            sortedEnd->next = key->next;
            if (key->residentID < head->residentID) {
                key->next = head;
                head = key;
            } else {
                Node* prev = head;
                while (prev->next != key && prev->next->residentID <= key->residentID)
                    prev = prev->next;
                key->next  = prev->next;
                prev->next = key;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    lastSortMs = chrono::duration<double, milli>(end - start).count();
}

// search for residents in an age range
void LinkedList::linearSearchByAgeGroup(int minAge, int maxAge) const {
    cout << "\n  [Linear Search] Age range " << minAge << " - " << maxAge << "\n";
    auto start = chrono::high_resolution_clock::now();

    cout << "\n  " << left
         << setw(8)  << "ID"
         << setw(6)  << "Age"
         << setw(14) << "Mode"
         << setw(12) << "Dist(km)"
         << setw(14) << "TotalEmis(kg)"
         << "\n";
    cout << "  " << string(54, '-') << "\n";

    int found = 0;
    Node* cur = head;
    while (cur) {
        if (cur->age >= minAge && cur->age <= maxAge) {
            cout << "  " << left
                 << setw(8)  << cur->residentID
                 << setw(6)  << cur->age
                 << setw(14) << cur->mode
                 << setw(12) << fixed << setprecision(1) << cur->distance
                 << setw(14) << fixed << setprecision(2) << cur->totalEmission
                 << "\n";
            ++found;
        }
        cur = cur->next;
    }

    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "  " << string(54, '-') << "\n";
    cout << "  Found " << found << " residents  |  Time: "
         << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n)  |  Space: O(1)\n";
}

// search by transport mode
void LinkedList::linearSearchByMode(const string& mode) const {
    cout << "\n  [Linear Search] Mode = \"" << mode << "\"\n";
    auto start = chrono::high_resolution_clock::now();

    cout << "\n  " << left
         << setw(8)  << "ID"
         << setw(6)  << "Age"
         << setw(14) << "Mode"
         << setw(12) << "Dist(km)"
         << setw(14) << "TotalEmis(kg)"
         << "\n";
    cout << "  " << string(54, '-') << "\n";

    int found = 0;
    Node* cur = head;
    while (cur) {
        // lowercase both so it doesnt matter how they typed it
        string a = cur->mode, b = mode;
        for (char& c : a) c = (char)tolower(c);
        for (char& c : b) c = (char)tolower(c);
        if (a == b) {
            cout << "  " << left
                 << setw(8)  << cur->residentID
                 << setw(6)  << cur->age
                 << setw(14) << cur->mode
                 << setw(12) << fixed << setprecision(1) << cur->distance
                 << setw(14) << fixed << setprecision(2) << cur->totalEmission
                 << "\n";
            ++found;
        }
        cur = cur->next;
    }

    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "  " << string(54, '-') << "\n";
    cout << "  Found " << found << " residents  |  Time: "
         << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n)  |  Space: O(1)\n";
}

// find everyone who travels more than X km per day
void LinkedList::linearSearchByDistanceThreshold(double minKm) const {
    cout << "\n  [Linear Search] Daily Distance > " << minKm << " km\n";
    auto start = chrono::high_resolution_clock::now();

    cout << "\n  " << left
         << setw(8)  << "ID"
         << setw(6)  << "Age"
         << setw(14) << "Mode"
         << setw(12) << "Dist(km)"
         << setw(14) << "TotalEmis(kg)"
         << "\n";
    cout << "  " << string(54, '-') << "\n";

    int found = 0;
    Node* cur = head;
    while (cur) {
        if (cur->distance > minKm) {
            cout << "  " << left
                 << setw(8)  << cur->residentID
                 << setw(6)  << cur->age
                 << setw(14) << cur->mode
                 << setw(12) << fixed << setprecision(1) << cur->distance
                 << setw(14) << fixed << setprecision(2) << cur->totalEmission
                 << "\n";
            ++found;
        }
        cur = cur->next;
    }

    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "  " << string(54, '-') << "\n";
    cout << "  Found " << found << " residents  |  Time: "
         << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n)  |  Space: O(1)\n";
}

// binary search by age, but the list has to be sorted first
void LinkedList::binarySearchByAge(int targetAge) const {
    cout << "\n  [Binary Search] Age = " << targetAge
         << "  (requires list sorted by age!)\n";
    auto start = chrono::high_resolution_clock::now();

    // linked lists cant do random access so we make a temporary array of pointers
    Node** arr = new Node*[count];
    Node* cur = head;
    for (int i = 0; i < count; ++i, cur = cur->next) arr[i] = cur;

    int lo = 0, hi = count - 1, found = 0;
    cout << "\n  Binary Search Steps:\n";
    int step = 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        cout << "    Step " << step++ << ": lo=" << lo << " hi=" << hi
             << " mid=" << mid << " (age=" << arr[mid]->age << ")\n";
        if (arr[mid]->age == targetAge) {
            // spread out from the middle to catch everyone with that age
            int L = mid, R = mid;
            while (L > 0    && arr[L-1]->age == targetAge) --L;
            while (R < count-1 && arr[R+1]->age == targetAge) ++R;

            cout << "\n  " << left
                 << setw(8)  << "ID"
                 << setw(6)  << "Age"
                 << setw(14) << "Mode"
                 << setw(12) << "Dist(km)"
                 << setw(14) << "TotalEmis(kg)"
                 << "\n";
            cout << "  " << string(54, '-') << "\n";
            for (int i = L; i <= R; ++i) {
                cout << "  " << left
                     << setw(8)  << arr[i]->residentID
                     << setw(6)  << arr[i]->age
                     << setw(14) << arr[i]->mode
                     << setw(12) << fixed << setprecision(1) << arr[i]->distance
                     << setw(14) << fixed << setprecision(2) << arr[i]->totalEmission
                     << "\n";
                ++found;
            }
            break;
        } else if (arr[mid]->age < targetAge) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    delete[] arr;

    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    if (found == 0) cout << "  No residents found with age " << targetAge << "\n";
    else            cout << "  " << string(54, '-') << "\n";
    cout << "  Found " << found << " residents  |  Time: "
         << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(log n) [on index array]  |  Space: O(n) [index]\n";
    cout << "  Note: Pure linked list binary search is O(n) because random access\n"
         << "        requires traversal. An index array is used here for demonstration.\n";
}

// figure out which age group a person belongs to, using the exact labels from each city's dataset
string LinkedList::ageGroupLabel(int age, char cityPrefix) {
    if (cityPrefix == 'A') {
        if (age >= 26 && age <= 45) return "Working Adults (Early Career)";
        if (age >= 46 && age <= 60) return "Working Adults (Late Career)";
        return "Unknown";
    }
    if (cityPrefix == 'B') {
        if (age >=  6 && age <= 17) return "Children & Teenagers";
        if (age >= 18 && age <= 25) return "University Students / Young Adults";
        return "Unknown";
    }
    // City C and fallback - mix of both young and adult groups
    if (age >=  6 && age <= 17) return "Children & Teenagers";
    if (age >= 18 && age <= 25) return "University Students / Young Adults";
    if (age >= 26 && age <= 45) return "Working Adults (Early Career)";
    if (age >= 46 && age <= 60) return "Working Adults (Late Career)";
    if (age >= 61)              return "Senior Citizens / Retirees";
    return "Unknown";
}

// shows total emissions and a breakdown by transport mode
void LinkedList::carbonAnalysis() const {
    if (!head) { cout << "  (no data)\n"; return; }

    map<string, double> modeEmission;
    map<string, int>    modeCount;
    double totalEmission = 0.0;

    Node* cur = head;
    while (cur) {
        modeEmission[cur->mode] += cur->totalEmission;
        modeCount[cur->mode]++;
        totalEmission += cur->totalEmission;
        cur = cur->next;
    }

    cout << "\n  ========== CARBON EMISSION ANALYSIS ==========\n";
    cout << "  Total residents: " << count << "\n";
    cout << fixed << setprecision(2);
    cout << "  Total carbon emissions: " << totalEmission << " kg CO2/month\n";
    cout << "  Average per resident:   " << totalEmission / count << " kg CO2/month\n";

    cout << "\n  " << left
         << setw(22) << "Mode of Transport"
         << setw(10) << "Count"
         << setw(18) << "Total Emis (kg)"
         << setw(18) << "Avg per Resident"
         << setw(8)  << "Share %"
         << "\n";
    cout << "  " << string(76, '-') << "\n";

    for (auto& [mode, em] : modeEmission) {
        int    cnt = modeCount[mode];
        double avg = em / cnt;
        double pct = (totalEmission > 0) ? (em / totalEmission * 100.0) : 0.0;
        cout << "  " << left
             << setw(22) << mode
             << setw(10) << cnt
             << setw(18) << fixed << setprecision(2) << em
             << setw(18) << fixed << setprecision(2) << avg
             << setw(8)  << fixed << setprecision(1) << pct
             << "\n";
    }
    cout << "  " << string(76, '-') << "\n";
}

// breaks down emissions and preferred transport by age group
void LinkedList::ageGroupAnalysis() const {
    if (!head) { cout << "  (no data)\n"; return; }

    // all possible labels that appear across the datasets, in display order
    const string GROUPS[] = {
        "Children & Teenagers",
        "University Students / Young Adults",
        "Working Adults (Early Career)",
        "Working Adults (Late Career)",
        "Senior Citizens / Retirees",
        "Unknown"
    };

    map<string, double>             groupEmission;
    map<string, int>                groupCount;
    map<string, map<string,int>>    groupModeCount;

    Node* cur = head;
    while (cur) {
        // use the stored ageGroup directly - no re-computation needed
        const string& grp = cur->ageGroup;
        groupEmission[grp]          += cur->totalEmission;
        groupCount[grp]++;
        groupModeCount[grp][cur->mode]++;
        cur = cur->next;
    }

    cout << "\n  ========== AGE GROUP ANALYSIS ==========\n";
    cout << "\n  " << left
         << setw(38) << "Age Group"
         << setw(8)  << "Count"
         << setw(20) << "Total Emis (kg)"
         << setw(20) << "Avg Emis/Resident"
         << setw(18) << "Preferred Mode"
         << "\n";
    cout << "  " << string(104, '-') << "\n";

    for (const string& grp : GROUPS) {
        if (groupCount.find(grp) == groupCount.end()) continue;
        int    cnt = groupCount[grp];
        double em  = groupEmission[grp];
        double avg = em / cnt;

        // whichever mode has the most people in this group
        string prefMode = "-";
        int    maxCnt   = 0;
        for (auto& [m, c] : groupModeCount[grp]) {
            if (c > maxCnt) { maxCnt = c; prefMode = m; }
        }

        cout << "  " << left
             << setw(38) << grp
             << setw(8)  << cnt
             << setw(20) << fixed << setprecision(2) << em
             << setw(20) << fixed << setprecision(2) << avg
             << setw(18) << prefMode
             << "\n";
    }
    cout << "  " << string(104, '-') << "\n";

    // show mode counts for each group
    cout << "\n  Detailed mode breakdown per age group:\n\n";
    for (const string& grp : GROUPS) {
        if (groupModeCount.find(grp) == groupModeCount.end()) continue;
        cout << "  >> " << grp << "\n";
        cout << "     " << left << setw(20) << "Mode" << setw(8) << "Count" << "\n";
        for (auto& [m, c] : groupModeCount[grp]) {
            cout << "     " << left << setw(20) << m << setw(8) << c << "\n";
        }
        cout << "\n";
    }
}

// ==============================================================
//  EXTENDED ANALYSIS METHODS (City A and B only)
// ==============================================================

// 1. Total emissions summary with per-mode breakdown and insight
void LinkedList::totalEmissionsReport(char city) const {
    if (!head) { cout << "  (no data)\n"; return; }

    map<string, double> modeEm;
    map<string, int>    modeCnt;
    double total = 0.0;

    Node* cur = head;
    while (cur) {
        modeEm[cur->mode]  += cur->totalEmission;
        modeCnt[cur->mode]++;
        total += cur->totalEmission;
        cur = cur->next;
    }

    cout << "\n  ========== TOTAL EMISSIONS SUMMARY - CITY " << city << " ==========\n";
    cout << fixed << setprecision(2);
    cout << "  Total Residents     : " << count << "\n";
    cout << "  Total CO2 Emitted   : " << total << " kg CO2/month\n";
    cout << "  Average per Resident: " << (count > 0 ? total / count : 0.0) << " kg CO2/month\n";

    cout << "\n  " << left
         << setw(22) << "Mode"
         << setw(10) << "Residents"
         << setw(20) << "Total Emis (kg)"
         << setw(20) << "Avg/Resident (kg)"
         << setw(10) << "Share %"
         << "\n";
    cout << "  " << string(82, '-') << "\n";

    string topMode; double topEm = -1.0;
    for (auto& [mode, em] : modeEm) {
        int    cnt = modeCnt[mode];
        double avg = em / cnt;
        double pct = (total > 0) ? (em / total * 100.0) : 0.0;
        cout << "  " << left
             << setw(22) << mode
             << setw(10) << cnt
             << setw(20) << fixed << setprecision(2) << em
             << setw(20) << fixed << setprecision(2) << avg
             << setw(10) << fixed << setprecision(1) << pct << "\n";
        if (em > topEm) { topEm = em; topMode = mode; }
    }
    cout << "  " << string(82, '-') << "\n";

    double topPct = (total > 0) ? (topEm / total * 100.0) : 0.0;
    cout << "\n  >> Insight: " << topMode << " usage contributes the majority of emissions"
         << " in City " << city << " (" << fixed << setprecision(1) << topPct << "%).\n";
}

// 2. Emissions by transport mode, sorted descending with insights
void LinkedList::emissionsByMode(char city) const {
    if (!head) { cout << "  (no data)\n"; return; }

    map<string, double> modeEm;
    map<string, int>    modeCnt;
    double total = 0.0;

    Node* cur = head;
    while (cur) {
        modeEm[cur->mode]  += cur->totalEmission;
        modeCnt[cur->mode]++;
        total += cur->totalEmission;
        cur = cur->next;
    }

    // collect and sort modes descending by total emission (simple bubble)
    vector<pair<double, string>> sorted;
    for (auto& [mode, em] : modeEm) sorted.push_back({em, mode});
    for (int i = 0; i < (int)sorted.size() - 1; i++)
        for (int j = i+1; j < (int)sorted.size(); j++)
            if (sorted[j].first > sorted[i].first) swap(sorted[i], sorted[j]);

    cout << "\n  ========== EMISSIONS BY TRANSPORT MODE - CITY " << city << " ==========\n";
    cout << "\n  " << left
         << setw(22) << "Mode"
         << setw(10) << "Residents"
         << setw(20) << "Total Emis (kg)"
         << setw(20) << "Avg/Resident (kg)"
         << setw(10) << "Share %"
         << "\n";
    cout << "  " << string(82, '-') << "\n";

    for (auto& [em, mode] : sorted) {
        int    cnt = modeCnt[mode];
        double avg = (cnt > 0) ? em / cnt : 0.0;
        double pct = (total > 0) ? (em / total * 100.0) : 0.0;
        cout << "  " << left
             << setw(22) << mode
             << setw(10) << cnt
             << setw(20) << fixed << setprecision(2) << em
             << setw(20) << fixed << setprecision(2) << avg
             << setw(10) << fixed << setprecision(1) << pct << "\n";
    }
    cout << "  " << string(82, '-') << "\n";

    string highMode = sorted.front().second;
    string lowMode  = sorted.back().second;
    double highPct  = (total > 0) ? (sorted.front().first / total * 100.0) : 0.0;
    double lowPct   = (total > 0) ? (sorted.back().first  / total * 100.0) : 0.0;
    cout << "\n  >> Insight: " << highMode << " is the highest emitting transport mode in City "
         << city << " (" << fixed << setprecision(1) << highPct << "%).\n";
    cout << "  >> Insight: " << lowMode  << " produces the fewest emissions in City "
         << city << " (" << fixed << setprecision(1) << lowPct  << "%).\n";
}

// 3. Transport percentage distribution (count-based) with ASCII bar chart
void LinkedList::transportPercentageDistribution(char city) const {
    if (!head) { cout << "  (no data)\n"; return; }

    map<string, int> modeCnt;
    Node* cur = head;
    while (cur) { modeCnt[cur->mode]++; cur = cur->next; }

    // sort descending by count
    vector<pair<int, string>> sorted;
    for (auto& [mode, cnt] : modeCnt) sorted.push_back({cnt, mode});
    for (int i = 0; i < (int)sorted.size() - 1; i++)
        for (int j = i+1; j < (int)sorted.size(); j++)
            if (sorted[j].first > sorted[i].first) swap(sorted[i], sorted[j]);

    cout << "\n  ========== TRANSPORT PERCENTAGE DISTRIBUTION - CITY " << city << " ==========\n";
    cout << "\n  " << left
         << setw(22) << "Mode"
         << setw(12) << "Residents"
         << setw(10) << "Share %"
         << "  Distribution Bar\n";
    cout << "  " << string(74, '-') << "\n";

    const int BAR_WIDTH = 30;
    string topMode; int topCnt = 0;
    for (auto& [cnt, mode] : sorted) {
        double pct  = (count > 0) ? (cnt * 100.0 / count) : 0.0;
        int    bars = (int)(pct / 100.0 * BAR_WIDTH);
        string bar  = string(bars, '#') + string(BAR_WIDTH - bars, '.');
        cout << "  " << left
             << setw(22) << mode
             << setw(12) << cnt
             << setw(10) << fixed << setprecision(1) << pct
             << "  [" << bar << "]\n";
        if (cnt > topCnt) { topCnt = cnt; topMode = mode; }
    }
    cout << "  " << string(74, '-') << "\n";

    double topPct = (count > 0) ? (topCnt * 100.0 / count) : 0.0;
    cout << "\n  >> Insight: " << topMode << " is the most commonly used transport mode in City "
         << city << " (" << fixed << setprecision(1) << topPct << "% of residents).\n";
}

// 4. Extended age group analysis with top-group insight
void LinkedList::ageGroupAnalysisExtended(char city) const {
    if (!head) { cout << "  (no data)\n"; return; }

    const string GROUPS[] = {
        "Children & Teenagers",
        "University Students / Young Adults",
        "Working Adults (Early Career)",
        "Working Adults (Late Career)",
        "Senior Citizens / Retirees",
        "Unknown"
    };

    map<string, double>           groupEm;
    map<string, int>              groupCnt;
    map<string, map<string,int>>  groupMode;

    Node* cur = head;
    while (cur) {
        const string& g = cur->ageGroup;
        groupEm[g]  += cur->totalEmission;
        groupCnt[g]++;
        groupMode[g][cur->mode]++;
        cur = cur->next;
    }

    cout << "\n  ========== AGE GROUP ANALYSIS - CITY " << city << " ==========\n";
    cout << "\n  " << left
         << setw(38) << "Age Group"
         << setw(8)  << "Count"
         << setw(20) << "Total Emis (kg)"
         << setw(20) << "Avg/Resident (kg)"
         << setw(18) << "Preferred Mode"
         << "\n";
    cout << "  " << string(104, '-') << "\n";

    string topGroup; double topGroupEm = -1.0;
    for (const string& g : GROUPS) {
        if (groupCnt.find(g) == groupCnt.end()) continue;
        int    cnt      = groupCnt[g];
        double em       = groupEm[g];
        double avg      = em / cnt;
        string prefMode = "-"; int maxC = 0;
        for (auto& [m, c] : groupMode[g]) if (c > maxC) { maxC = c; prefMode = m; }

        cout << "  " << left
             << setw(38) << g
             << setw(8)  << cnt
             << setw(20) << fixed << setprecision(2) << em
             << setw(20) << fixed << setprecision(2) << avg
             << setw(18) << prefMode << "\n";
        if (em > topGroupEm) { topGroupEm = em; topGroup = g; }
    }
    cout << "  " << string(104, '-') << "\n";

    // detailed mode breakdown per group
    cout << "\n  Detailed mode breakdown per age group:\n\n";
    for (const string& g : GROUPS) {
        if (groupMode.find(g) == groupMode.end()) continue;
        cout << "  >> " << g << "\n";
        cout << "     " << left << setw(20) << "Mode" << setw(8) << "Count" << "\n";
        for (auto& [m, c] : groupMode[g])
            cout << "     " << left << setw(20) << m << setw(8) << c << "\n";
        cout << "\n";
    }

    // insight
    string prefOfTop = "-"; int maxC = 0;
    for (auto& [m, c] : groupMode[topGroup]) if (c > maxC) { maxC = c; prefOfTop = m; }
    cout << "  >> Insight: " << topGroup
         << " is the biggest emitting age group in City " << city
         << " with " << fixed << setprecision(2) << topGroupEm << " kg CO2/month.\n";
    cout << "  >> Insight: Their preferred mode of transport is " << prefOfTop << ".\n";
}

// 5. Top N and Bottom N emitters by total emission
void LinkedList::topBottomEmitters(int n) const {
    if (!head) { cout << "  (no data)\n"; return; }
    if (n < 1) n = 5;
    if (n > count) n = count;

    // build a local pointer array and selection-sort descending
    Node** arr = new Node*[count];
    Node* cur = head;
    for (int i = 0; i < count; ++i, cur = cur->next) arr[i] = cur;
    for (int i = 0; i < count - 1; ++i) {
        int hi = i;
        for (int j = i+1; j < count; ++j)
            if (arr[j]->totalEmission > arr[hi]->totalEmission) hi = j;
        if (hi != i) swap(arr[i], arr[hi]);
    }

    // top N
    cout << "\n  ===== TOP " << n << " HIGHEST EMITTERS =====\n";
    cout << "\n  " << left
         << setw(7)  << "Rank"
         << setw(8)  << "ID"
         << setw(5)  << "Age"
         << setw(36) << "Age Group"
         << setw(14) << "Mode"
         << setw(16) << "Total Emis (kg)"
         << "\n";
    cout << "  " << string(86, '-') << "\n";
    for (int i = 0; i < n; ++i) {
        cout << "  " << left
             << setw(7)  << ("#" + to_string(i + 1))
             << setw(8)  << arr[i]->residentID
             << setw(5)  << arr[i]->age
             << setw(36) << arr[i]->ageGroup
             << setw(14) << arr[i]->mode
             << setw(16) << fixed << setprecision(2) << arr[i]->totalEmission << "\n";
    }
    cout << "  " << string(86, '-') << "\n";

    // bottom N (display lowest first)
    cout << "\n  ===== BOTTOM " << n << " LOWEST EMITTERS =====\n";
    cout << "\n  " << left
         << setw(7)  << "Rank"
         << setw(8)  << "ID"
         << setw(5)  << "Age"
         << setw(36) << "Age Group"
         << setw(14) << "Mode"
         << setw(16) << "Total Emis (kg)"
         << "\n";
    cout << "  " << string(86, '-') << "\n";
    for (int i = count - 1; i >= count - n; --i) {
        int rank = count - i;
        cout << "  " << left
             << setw(7)  << ("#" + to_string(rank))
             << setw(8)  << arr[i]->residentID
             << setw(5)  << arr[i]->age
             << setw(36) << arr[i]->ageGroup
             << setw(14) << arr[i]->mode
             << setw(16) << fixed << setprecision(2) << arr[i]->totalEmission << "\n";
    }
    cout << "  " << string(86, '-') << "\n";

    double gap = arr[0]->totalEmission - arr[count - 1]->totalEmission;
    cout << "\n  >> Insight: The emission gap between the highest and lowest emitter is "
         << fixed << setprecision(2) << gap << " kg CO2/month.\n";
    cout << "  >> Insight: Top emitter " << arr[0]->residentID << " uses "
         << arr[0]->mode << " (" << fixed << setprecision(2)
         << arr[0]->totalEmission << " kg CO2/month).\n";

    delete[] arr;
}

// ==============================================================
//  MULTI-CONDITION SEARCH METHODS (City A and B only)
// ==============================================================

// Search by age range AND mode of transport (both conditions must match)
void LinkedList::multiConditionSearchAgeMode(int minAge, int maxAge, const string& mode) const {
    auto start = chrono::high_resolution_clock::now();

    // normalise input mode to lowercase for comparison
    string modeLow = mode;
    for (char& c : modeLow) c = (char)tolower(c);

    cout << "\n  [Multi-Condition Search] Age " << minAge << "-" << maxAge
         << "  AND  Mode = \"" << mode << "\"\n";
    cout << "\n  " << left
         << setw(8)  << "ID"
         << setw(6)  << "Age"
         << setw(36) << "Age Group"
         << setw(14) << "Mode"
         << setw(12) << "Dist(km)"
         << setw(16) << "Total Emis (kg)"
         << "\n";
    cout << "  " << string(92, '-') << "\n";

    int found = 0;
    Node* cur = head;
    while (cur) {
        string curModeLow = cur->mode;
        for (char& c : curModeLow) c = (char)tolower(c);
        if (cur->age >= minAge && cur->age <= maxAge && curModeLow == modeLow) {
            cout << "  " << left
                 << setw(8)  << cur->residentID
                 << setw(6)  << cur->age
                 << setw(36) << cur->ageGroup
                 << setw(14) << cur->mode
                 << setw(12) << fixed << setprecision(1) << cur->distance
                 << setw(16) << fixed << setprecision(2) << cur->totalEmission << "\n";
            ++found;
        }
        cur = cur->next;
    }

    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "  " << string(92, '-') << "\n";
    cout << "  Found " << found << " residents  |  Time: "
         << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n)  |  Space: O(1)\n";
}

// Search by distance threshold AND mode of transport (both conditions must match)
void LinkedList::multiConditionSearchDistMode(double minKm, const string& mode) const {
    auto start = chrono::high_resolution_clock::now();

    string modeLow = mode;
    for (char& c : modeLow) c = (char)tolower(c);

    cout << "\n  [Multi-Condition Search] Daily Distance > " << minKm
         << " km  AND  Mode = \"" << mode << "\"\n";
    cout << "\n  " << left
         << setw(8)  << "ID"
         << setw(6)  << "Age"
         << setw(36) << "Age Group"
         << setw(14) << "Mode"
         << setw(12) << "Dist(km)"
         << setw(16) << "Total Emis (kg)"
         << "\n";
    cout << "  " << string(92, '-') << "\n";

    int found = 0;
    Node* cur = head;
    while (cur) {
        string curModeLow = cur->mode;
        for (char& c : curModeLow) c = (char)tolower(c);
        if (cur->distance > minKm && curModeLow == modeLow) {
            cout << "  " << left
                 << setw(8)  << cur->residentID
                 << setw(6)  << cur->age
                 << setw(36) << cur->ageGroup
                 << setw(14) << cur->mode
                 << setw(12) << fixed << setprecision(1) << cur->distance
                 << setw(16) << fixed << setprecision(2) << cur->totalEmission << "\n";
            ++found;
        }
        cur = cur->next;
    }

    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "  " << string(92, '-') << "\n";
    cout << "  Found " << found << " residents  |  Time: "
         << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n)  |  Space: O(1)\n";
}
