  // crew_list.cpp - all the linked list logic is in here
#include "city_linkedlist.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
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
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "\n  [Bubble Sort by Age] Sorted " << count
         << " nodes in " << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n^2)  |  Space Complexity: O(1)  (in-place data swap)\n";
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
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "\n  [Bubble Sort by Distance] Sorted " << count
         << " nodes in " << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n^2)  |  Space Complexity: O(1)  (in-place data swap)\n";
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
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "\n  [Bubble Sort by Emission] Sorted " << count
         << " nodes in " << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n^2)  |  Space Complexity: O(1)  (in-place data swap)\n";
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
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "\n  [Insertion Sort by Age] Sorted " << count
         << " nodes in " << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n^2) worst / O(n) best  |  Space Complexity: O(1)  (in-place pointer re-linking)\n";
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
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "\n  [Insertion Sort by Distance] Sorted " << count
         << " nodes in " << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n^2) worst / O(n) best  |  Space Complexity: O(1)  (in-place pointer re-linking)\n";
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
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "\n  [Insertion Sort by Emission] Sorted " << count
         << " nodes in " << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n^2) worst / O(n) best  |  Space Complexity: O(1)  (in-place pointer re-linking)\n";
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
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << "\n  [Restored Original Order] Sorted by Resident ID (" << count
         << " residents) in " << fixed << setprecision(4) << ms << " ms\n";
    cout << "  Time Complexity: O(n^2) worst / O(n) best  |  Space Complexity: O(1)\n";
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
