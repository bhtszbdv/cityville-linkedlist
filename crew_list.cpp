//all the linked list logic is in here
#include "city_linkedlist.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <limits>
#include <chrono>
using namespace std;

//  City A & B - Sirin    


// ── border constants ──────────────────────────────────────────────────────

static const char* RES_BORDER  =
    "+--------+------+-------------------------------------+--------------+-----------+----------+------+----------------+";

static const char* SRCH_BORDER =
    "+--------+------+-------------------------------------+--------------+-----------+----------------+";

static const char* STAT_BORDER =
    "+----------------------+---------------------------------------------------------+";

static const char* ANAL_BORDER =
    "+-----------------------+----------+--------------------+--------------------+----------+";

static const char* AGRP_BORDER =
    "+-----------------------------------------------+---------+---------------------+---------------------+------------------+";

static const char* RANK_BORDER =
    "+--------+--------+-------+-------------------------------------+--------------+-----------------+";

static const char* TRAN_BORDER =
    "+-----------------------+------------+----------+----------------------------------+";

static const char* SUB_BORDER  =
    "+----------------------+----------+";

// ── output helpers ────────────────────────────────────────────────────────
static string fit(const string& s, int w) {
    if ((int)s.size() <= w) return s;
    return s.substr(0, w - 1) + "~";
}

static string fmtD(double v, int prec) {
    ostringstream oss;
    oss << fixed << setprecision(prec) << v;
    return oss.str();
}

static void statRow(const string& lbl, const string& val) {
    cout << "| " << left << setw(20) << fit(lbl, 20)
         << " | " << left << setw(55) << fit(val, 55) << " |\n";
}

static void printSearchStats(const string& algo, double us,
                             const string& tc, const string& sc,
                             int found, int total) {
    struct PlainRes { string a; int b; string c; double d, e; int f; string g; double h; };
    struct LLNode   { PlainRes data; void* next; };
    long long llSz  = (long long)sizeof(LLNode);
    long long arrSz = (long long)sizeof(PlainRes);
    string mem = to_string(total) + " nodes x " + to_string(llSz)
               + " B = ~" + to_string((long long)total * llSz)
               + " B  (arr ~" + to_string((long long)total * arrSz) + " B)";
    cout << "\n" << STAT_BORDER << "\n";
    statRow("Algorithm",        algo);
    statRow("Time",             fmtD(us, 2) + " microseconds");
    statRow("Time Complexity",  tc);
    statRow("Space Complexity", sc);
    statRow("Records Found",    to_string(found) + " / " + to_string(total));
    statRow("Memory Usage",     mem);
    cout << STAT_BORDER << "\n";
}

static void srchHeader() {
    cout << SRCH_BORDER << "\n";
    cout << "| " << left
         << setw(6)  << "ID"             << " | "
         << setw(4)  << "Age"            << " | "
         << setw(35) << "Age Group"      << " | "
         << setw(12) << "Mode"           << " | "
         << setw(9)  << "Dist(km)"       << " | "
         << setw(14) << "Total Emis(kg)" << " |\n";
    cout << SRCH_BORDER << "\n";
}

static void srchRow(const Node* n) {
    cout << "| " << left  << setw(6)  << fit(n->residentID, 6)  << " | "
         << right << setw(4)  << n->age                           << " | "
         << left  << setw(35) << fit(n->ageGroup, 35)             << " | "
         << left  << setw(12) << fit(n->mode, 12)                 << " | "
         << right << setw(9)  << fixed << setprecision(1) << n->distance      << " | "
         << right << setw(14) << fixed << setprecision(2) << n->totalEmission << " |\n";
}

// ── constructor / destructor ──────────────────────────────────────────────
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

bool LinkedList::idExists(const string& id) const {
    Node* cur = head;
    while (cur) {
        if (cur->residentID == id) return true;
        cur = cur->next;
    }
    return false;
}

// ── insert / delete / reverse ─────────────────────────────────────────────
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

// ── CSV I/O ───────────────────────────────────────────────────────────────
bool LinkedList::loadFromCSV(const string& filename) {
    sourceFilename = filename;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "  [ERROR] Cannot open file: " << filename << "\n";
        return false;
    }

    string line;
    getline(file, csvHeader);

    int loaded = 0;
    while (getline(file, line)) {
        if (line.empty() || line == "\r") continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();

        stringstream ss(line);
        string id, ageStr, mode, distStr, emStr, daysStr, ageGroup, totalEmStr;
        getline(ss, id,         ',');
        getline(ss, ageStr,     ',');
        getline(ss, mode,       ',');
        getline(ss, distStr,    ',');
        getline(ss, emStr,      ',');
        getline(ss, daysStr,    ',');
        getline(ss, ageGroup,   ',');
        getline(ss, totalEmStr, ',');

        auto trim = [](string& s) {
            size_t start = s.find_first_not_of(" \t\r\n");
            size_t end   = s.find_last_not_of(" \t\r\n");
            s = (start == string::npos) ? "" : s.substr(start, end - start + 1);
        };
        trim(id); trim(ageStr); trim(mode); trim(distStr);
        trim(emStr); trim(daysStr); trim(ageGroup); trim(totalEmStr);

        if (id.empty()) continue;

        try {
            int    age     = stoi(ageStr);
            double dist    = stod(distStr);
            double em      = stod(emStr);
            int    days    = stoi(daysStr);
            double totalEm = totalEmStr.empty() ? (dist * em * days) : stod(totalEmStr);
            if (ageGroup.empty()) {
                char cityPfx = (!id.empty()) ? (char)toupper(id[0]) : '\0';
                ageGroup = ageGroupLabel(age, cityPfx);
            }
            if (idExists(id)) {
                cout << "  [ERROR] Resident ID \"" << id << "\" already exists. Skipping.\n";
                continue;
            }
            Node* newNode = new Node();
            newNode->residentID     = id;
            newNode->age            = age;
            newNode->mode           = mode;
            newNode->distance       = dist;
            newNode->emissionFactor = em;
            newNode->days           = days;
            newNode->ageGroup       = ageGroup;
            newNode->totalEmission  = totalEm;
            newNode->next           = nullptr;

            if (!head) {
                head = newNode;
            } else {
                Node* cur = head;
                while (cur->next) cur = cur->next;
                cur->next = newNode;
            }
            ++count;
            ++loaded;
        } catch (...) {
            cout << "  [WARN] Skipping bad row: " << line << "\n";
        }
    }
    file.close();
    cout << "  Loaded " << loaded << " records from " << filename << "\n";
    return true;
}

bool LinkedList::saveToCSV() const {
    if (sourceFilename.empty()) return false;
    ofstream file(sourceFilename);
    if (!file.is_open()) {
        cout << "  [ERROR] Could not open file for writing: " << sourceFilename << "\n";
        return false;
    }
    if (!csvHeader.empty()) file << csvHeader << "\n";
    else file << "ResidentID,Age,ModeOfTransport,DailyDistance,CarbonEmissionFactor,AverageDayPerMonth,AgeGroup,TotalMonthlyEmission\n";

    Node* cur = head;
    while (cur) {
        file << cur->residentID     << ","
             << cur->age            << ","
             << cur->mode           << ","
             << cur->distance       << ","
             << cur->emissionFactor << ","
             << cur->days           << ","
             << cur->ageGroup       << ","
             << fixed << setprecision(2) << cur->totalEmission << "\n";
        cur = cur->next;
    }
    file.close();
    return true;
}

// ── display ───────────────────────────────────────────────────────────────
void LinkedList::display() const {
    if (!head) {
        cout << "\n" << RES_BORDER << "\n";
        cout << "| " << left << setw(113) << " (no data)" << " |\n";
        cout << RES_BORDER << "\n";
        return;
    }
    cout << "\n" << RES_BORDER << "\n";
    cout << "| " << left
         << setw(6)  << "ID"             << " | "
         << setw(4)  << "Age"            << " | "
         << setw(35) << "Age Group"      << " | "
         << setw(12) << "Mode"           << " | "
         << setw(9)  << "Dist(km)"       << " | "
         << setw(8)  << "EmFac"          << " | "
         << setw(4)  << "Days"           << " | "
         << setw(14) << "Total Emis(kg)" << " |\n";
    cout << RES_BORDER << "\n";

    Node* cur = head;
    while (cur) {
        cout << "| " << left  << setw(6)  << fit(cur->residentID, 6)   << " | "
             << right << setw(4)  << cur->age                            << " | "
             << left  << setw(35) << fit(cur->ageGroup, 35)              << " | "
             << left  << setw(12) << fit(cur->mode, 12)                  << " | "
             << right << setw(9)  << fixed << setprecision(1) << cur->distance        << " | "
             << right << setw(8)  << fixed << setprecision(4) << cur->emissionFactor  << " | "
             << right << setw(4)  << cur->days                                         << " | "
             << right << setw(14) << fixed << setprecision(2) << cur->totalEmission   << " |\n";
        cur = cur->next;
    }
    cout << RES_BORDER << "\n";
    cout << "  Total records: " << count << "\n";
}

// ── swap helper ───────────────────────────────────────────────────────────
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

// ── bubble sorts ──────────────────────────────────────────────────────────
void LinkedList::sortByAge() {
    if (!head || !head->next) return;
    auto start = chrono::high_resolution_clock::now();
    bool swapped;
    do {
        swapped = false;
        Node* cur = head;
        while (cur->next) {
            if (cur->age > cur->next->age) { swapData(cur, cur->next); swapped = true; }
            cur = cur->next;
        }
    } while (swapped);
    lastSortUs = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
}

void LinkedList::sortByDistance() {
    if (!head || !head->next) return;
    auto start = chrono::high_resolution_clock::now();
    bool swapped;
    do {
        swapped = false;
        Node* cur = head;
        while (cur->next) {
            if (cur->distance > cur->next->distance) { swapData(cur, cur->next); swapped = true; }
            cur = cur->next;
        }
    } while (swapped);
    lastSortUs = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
}

void LinkedList::sortByEmission() {
    if (!head || !head->next) return;
    auto start = chrono::high_resolution_clock::now();
    bool swapped;
    do {
        swapped = false;
        Node* cur = head;
        while (cur->next) {
            if (cur->totalEmission > cur->next->totalEmission) { swapData(cur, cur->next); swapped = true; }
            cur = cur->next;
        }
    } while (swapped);
    lastSortUs = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
}

// ── insertion sorts ───────────────────────────────────────────────────────
void LinkedList::insertionSortByAge() {
    if (!head || !head->next) return;
    auto start = chrono::high_resolution_clock::now();
    Node* sortedEnd = head;
    while (sortedEnd->next) {
        Node* key = sortedEnd->next;
        if (key->age >= sortedEnd->age) {
            sortedEnd = sortedEnd->next;
        } else {
            sortedEnd->next = key->next;
            if (key->age < head->age) { key->next = head; head = key; }
            else {
                Node* prev = head;
                while (prev->next != key && prev->next->age <= key->age) prev = prev->next;
                key->next  = prev->next;
                prev->next = key;
            }
        }
    }
    lastSortUs = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
}

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
            if (key->distance < head->distance) { key->next = head; head = key; }
            else {
                Node* prev = head;
                while (prev->next != key && prev->next->distance <= key->distance) prev = prev->next;
                key->next  = prev->next;
                prev->next = key;
            }
        }
    }
    lastSortUs = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
}

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
            if (key->totalEmission < head->totalEmission) { key->next = head; head = key; }
            else {
                Node* prev = head;
                while (prev->next != key && prev->next->totalEmission <= key->totalEmission) prev = prev->next;
                key->next  = prev->next;
                prev->next = key;
            }
        }
    }
    lastSortUs = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
}

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
            if (key->residentID < head->residentID) { key->next = head; head = key; }
            else {
                Node* prev = head;
                while (prev->next != key && prev->next->residentID <= key->residentID) prev = prev->next;
                key->next  = prev->next;
                prev->next = key;
            }
        }
    }
    lastSortUs = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
}

// ── linear searches ───────────────────────────────────────────────────────
void LinkedList::linearSearchByAgeGroup(int minAge, int maxAge) const {
    cout << "\n  [Linear Search]  Age range: " << minAge << " - " << maxAge << "\n";
    auto start = chrono::high_resolution_clock::now();
    srchHeader();
    int found = 0;
    Node* cur = head;
    while (cur) {
        if (cur->age >= minAge && cur->age <= maxAge) { srchRow(cur); ++found; }
        cur = cur->next;
    }
    cout << SRCH_BORDER << "\n";
    double us = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
    printSearchStats("Linear Search by Age", us, "O(n)", "O(1)", found, count);
}

void LinkedList::linearSearchByMode(const string& mode) const {
    cout << "\n  [Linear Search]  Mode: \"" << mode << "\"\n";
    auto start = chrono::high_resolution_clock::now();
    srchHeader();
    int found = 0;
    Node* cur = head;
    while (cur) {
        string a = cur->mode, b = mode;
        for (char& c : a) c = (char)tolower(c);
        for (char& c : b) c = (char)tolower(c);
        if (a == b) { srchRow(cur); ++found; }
        cur = cur->next;
    }
    cout << SRCH_BORDER << "\n";
    double us = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
    printSearchStats("Linear Search by Mode", us, "O(n)", "O(1)", found, count);
}

void LinkedList::linearSearchByDistanceThreshold(double minKm) const {
    cout << "\n  [Linear Search]  Daily Distance > " << minKm << " km\n";
    auto start = chrono::high_resolution_clock::now();
    srchHeader();
    int found = 0;
    Node* cur = head;
    while (cur) {
        if (cur->distance > minKm) { srchRow(cur); ++found; }
        cur = cur->next;
    }
    cout << SRCH_BORDER << "\n";
    double us = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
    printSearchStats("Linear Search by Distance", us, "O(n)", "O(1)", found, count);
}

// ── binary search ─────────────────────────────────────────────────────────
void LinkedList::binarySearchByAge(int targetAge) const {
    cout << "\n  [Binary Search]  Age = " << targetAge
         << "  (list must be sorted by age first)\n";
    auto start = chrono::high_resolution_clock::now();

    Node** arr = new Node*[count];
    Node* cur = head;
    for (int i = 0; i < count; ++i, cur = cur->next) arr[i] = cur;

    int lo = 0, hi = count - 1, found = 0;
    cout << "\n  Steps:\n";
    int step = 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        cout << "    [" << step++ << "] lo=" << lo << "  hi=" << hi
             << "  mid=" << mid << "  (age=" << arr[mid]->age << ")\n";
        if (arr[mid]->age == targetAge) {
            int L = mid, R = mid;
            while (L > 0       && arr[L-1]->age == targetAge) --L;
            while (R < count-1 && arr[R+1]->age == targetAge) ++R;
            cout << "\n";
            srchHeader();
            for (int i = L; i <= R; ++i) { srchRow(arr[i]); ++found; }
            cout << SRCH_BORDER << "\n";
            break;
        } else if (arr[mid]->age < targetAge) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    delete[] arr;

    double us = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
    if (found == 0)
        cout << "\n  No residents found with age " << targetAge << "\n";
    printSearchStats("Binary Search by Age", us,
                     "O(log n) [on index array]", "O(n) [index array]", found, count);
    if (found == 0)
        cout << "  Note: sort by age first for accurate binary search results.\n";
}

// ── age group label ───────────────────────────────────────────────────────
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
    if (age >=  6 && age <= 17) return "Children & Teenagers";
    if (age >= 18 && age <= 25) return "University Students / Young Adults";
    if (age >= 26 && age <= 45) return "Working Adults (Early Career)";
    if (age >= 46 && age <= 60) return "Working Adults (Late Career)";
    if (age >= 61)              return "Senior Citizens / Retirees";
    return "Unknown";
}

// ── carbon analysis ───────────────────────────────────────────────────────
void LinkedList::carbonAnalysis() const {
    if (!head) { cout << "  (no data)\n"; return; }

    struct ModeData { string mode; double emission; int count; };
    ModeData modes[100];
    int modeCount_arr = 0;
    double totalEmission = 0.0;

    Node* cur = head;
    while (cur) {
        int idx = -1;
        for (int i = 0; i < modeCount_arr; i++) {
            if (modes[i].mode == cur->mode) { idx = i; break; }
        }
        if (idx == -1) {
            modes[modeCount_arr].mode = cur->mode;
            modes[modeCount_arr].emission = cur->totalEmission;
            modes[modeCount_arr].count = 1;
            modeCount_arr++;
        } else {
            modes[idx].emission += cur->totalEmission;
            modes[idx].count++;
        }
        totalEmission += cur->totalEmission;
        cur = cur->next;
    }

    char city = (!head->residentID.empty()) ? (char)toupper(head->residentID[0]) : '?';
    cout << "\n  ===== CARBON EMISSION ANALYSIS - CITY " << city << " =====\n";
    cout << "  Residents: " << count
         << "   |   Total: " << fixed << setprecision(2) << totalEmission << " kg CO2/month"
         << "   |   Avg: " << (count ? totalEmission / count : 0.0) << " kg CO2/month\n\n";

    cout << ANAL_BORDER << "\n";
    cout << "| " << left
         << setw(21) << "Mode of Transport" << " | "
         << setw(8)  << "Count"             << " | "
         << setw(18) << "Total Emis (kg)"   << " | "
         << setw(18) << "Avg/Resident (kg)" << " | "
         << setw(8)  << "Share %"           << " |\n";
    cout << ANAL_BORDER << "\n";

    for (int i = 0; i < modeCount_arr; i++) {
        double avg = modes[i].emission / modes[i].count;
        double pct = (totalEmission > 0) ? (modes[i].emission / totalEmission * 100.0) : 0.0;
        cout << "| " << left  << setw(21) << fit(modes[i].mode, 21) << " | "
             << right << setw(8)  << modes[i].count                    << " | "
             << right << setw(18) << fixed << setprecision(2) << modes[i].emission  << " | "
             << right << setw(18) << fixed << setprecision(2) << avg << " | "
             << right << setw(8)  << fixed << setprecision(1) << pct << " |\n";
    }
    cout << ANAL_BORDER << "\n";
}

// ── age group analysis ────────────────────────────────────────────────────
void LinkedList::ageGroupAnalysis() const {
    if (!head) { cout << "  (no data)\n"; return; }

    const string GROUPS[] = {
        "Children & Teenagers",
        "University Students / Young Adults",
        "Working Adults (Early Career)",
        "Working Adults (Late Career)",
        "Senior Citizens / Retirees",
        "Unknown"
    };

    struct GroupData { string name; double emission; int count; int minAge; int maxAge; };
    struct ModeInfo { string mode; int count; };
    struct GroupDetail { string groupName; ModeInfo modes[20]; int modeCount; };
    
    GroupData groups[6];
    GroupDetail groupDetails[6];
    int groupCount_arr = 0;

    for (int i = 0; i < 6; i++) {
        groups[i].name = GROUPS[i];
        groups[i].emission = 0.0;
        groups[i].count = 0;
        groups[i].minAge = 9999;
        groups[i].maxAge = -1;
        groupDetails[i].groupName = GROUPS[i];
        groupDetails[i].modeCount = 0;
    }

    Node* cur = head;
    while (cur) {
        int gIdx = -1;
        for (int i = 0; i < 6; i++) {
            if (groups[i].name == cur->ageGroup) { gIdx = i; break; }
        }
        if (gIdx >= 0) {
            groups[gIdx].emission += cur->totalEmission;
            groups[gIdx].count++;
            if (cur->age < groups[gIdx].minAge) groups[gIdx].minAge = cur->age;
            if (cur->age > groups[gIdx].maxAge) groups[gIdx].maxAge = cur->age;
            
            int mIdx = -1;
            for (int j = 0; j < groupDetails[gIdx].modeCount; j++) {
                if (groupDetails[gIdx].modes[j].mode == cur->mode) { mIdx = j; break; }
            }
            if (mIdx == -1) {
                groupDetails[gIdx].modes[groupDetails[gIdx].modeCount].mode = cur->mode;
                groupDetails[gIdx].modes[groupDetails[gIdx].modeCount].count = 1;
                groupDetails[gIdx].modeCount++;
            } else {
                groupDetails[gIdx].modes[mIdx].count++;
            }
        }
        cur = cur->next;
    }

    cout << "\n  ===== AGE GROUP ANALYSIS =====\n\n";
    cout << AGRP_BORDER << "\n";
    cout << "| " << left
         << setw(45) << "Age Group (min-max)"         << " | "
         << setw(7)  << "Count"             << " | "
         << setw(19) << "Total Emis (kg)"   << " | "
         << setw(19) << "Avg/Resident (kg)" << " | "
         << setw(16) << "Preferred Mode"    << " |\n";
    cout << AGRP_BORDER << "\n";

    for (int i = 0; i < 6; i++) {
        if (groups[i].count == 0) continue;
        double avg = groups[i].emission / groups[i].count;
        string prefMode = "-"; int maxCnt = 0;
        for (int j = 0; j < groupDetails[i].modeCount; j++)
            if (groupDetails[i].modes[j].count > maxCnt) { 
                maxCnt = groupDetails[i].modes[j].count; 
                prefMode = groupDetails[i].modes[j].mode; 
            }
        string grpLabel = groups[i].name + " (" + to_string(groups[i].minAge) + "-" + to_string(groups[i].maxAge) + ")";
        cout << "| " << left  << setw(45) << fit(grpLabel, 45)      << " | "
             << right << setw(7)  << groups[i].count                         << " | "
             << right << setw(19) << fixed << setprecision(2) << groups[i].emission  << " | "
             << right << setw(19) << fixed << setprecision(2) << avg << " | "
             << left  << setw(16) << fit(prefMode, 16)           << " |\n";
    }
    cout << AGRP_BORDER << "\n";

    cout << "\n  Mode breakdown per group:\n";
    for (int i = 0; i < 6; i++) {
        if (groupDetails[i].modeCount == 0) continue;
        cout << "\n  >> " << groupDetails[i].groupName << "\n";
        cout << "  " << SUB_BORDER << "\n";
        cout << "  | " << left << setw(20) << "Mode" << " | " << setw(8) << "Count" << " |\n";
        cout << "  " << SUB_BORDER << "\n";
        for (int j = 0; j < groupDetails[i].modeCount; j++)
            cout << "  | " << left << setw(20) << fit(groupDetails[i].modes[j].mode, 20) << " | " << right << setw(8) << groupDetails[i].modes[j].count << " |\n";
        cout << "  " << SUB_BORDER << "\n";
    }
}

// ── extended analysis (City A and B) ─────────────────────────────────────

void LinkedList::totalEmissionsReport(char city) const {
    if (!head) { cout << "  (no data)\n"; return; }

    struct ModeData { string mode; double emission; int count; };
    ModeData modes[100];
    int modeCount_arr = 0;
    double total = 0.0;

    Node* cur = head;
    while (cur) {
        int idx = -1;
        for (int i = 0; i < modeCount_arr; i++) {
            if (modes[i].mode == cur->mode) { idx = i; break; }
        }
        if (idx == -1) {
            modes[modeCount_arr].mode = cur->mode;
            modes[modeCount_arr].emission = cur->totalEmission;
            modes[modeCount_arr].count = 1;
            modeCount_arr++;
        } else {
            modes[idx].emission += cur->totalEmission;
            modes[idx].count++;
        }
        total += cur->totalEmission;
        cur = cur->next;
    }

    cout << "\n  ===== TOTAL EMISSIONS SUMMARY - CITY " << city << " =====\n";
    cout << "  Residents: " << count
         << "   |   Total: " << fixed << setprecision(2) << total << " kg CO2/month"
         << "   |   Avg: " << (count ? total / count : 0.0) << " kg CO2/month\n\n";

    cout << ANAL_BORDER << "\n";
    cout << "| " << left
         << setw(21) << "Mode"              << " | "
         << setw(8)  << "Residents"         << " | "
         << setw(18) << "Total Emis (kg)"   << " | "
         << setw(18) << "Avg/Resident (kg)" << " | "
         << setw(8)  << "Share %"           << " |\n";
    cout << ANAL_BORDER << "\n";

    string topMode; double topEm = -1.0;
    for (int i = 0; i < modeCount_arr; i++) {
        double avg = modes[i].emission / modes[i].count;
        double pct = (total > 0) ? (modes[i].emission / total * 100.0) : 0.0;
        cout << "| " << left  << setw(21) << fit(modes[i].mode, 21) << " | "
             << right << setw(8)  << modes[i].count                    << " | "
             << right << setw(18) << fixed << setprecision(2) << modes[i].emission  << " | "
             << right << setw(18) << fixed << setprecision(2) << avg << " | "
             << right << setw(8)  << fixed << setprecision(1) << pct << " |\n";
        if (modes[i].emission > topEm) { topEm = modes[i].emission; topMode = modes[i].mode; }
    }
    cout << ANAL_BORDER << "\n";

    double topPct = (total > 0) ? (topEm / total * 100.0) : 0.0;
    cout << "\n  >> Insight: " << topMode << " contributes the most emissions in City "
         << city << " (" << fixed << setprecision(1) << topPct << "%).\n";
}

void LinkedList::emissionsByMode(char city) const {
    if (!head) { cout << "  (no data)\n"; return; }

    struct ModeData { double emission; string mode; int count; };
    ModeData modes[100];
    int modeCount_arr = 0;
    double total = 0.0;

    Node* cur = head;
    while (cur) {
        int idx = -1;
        for (int i = 0; i < modeCount_arr; i++) {
            if (modes[i].mode == cur->mode) { idx = i; break; }
        }
        if (idx == -1) {
            modes[modeCount_arr].mode = cur->mode;
            modes[modeCount_arr].emission = cur->totalEmission;
            modes[modeCount_arr].count = 1;
            modeCount_arr++;
        } else {
            modes[idx].emission += cur->totalEmission;
            modes[idx].count++;
        }
        total += cur->totalEmission;
        cur = cur->next;
    }

    for (int i = 0; i < modeCount_arr - 1; i++)
        for (int j = i+1; j < modeCount_arr; j++)
            if (modes[j].emission > modes[i].emission) {
                ModeData tmp = modes[i];
                modes[i] = modes[j];
                modes[j] = tmp;
            }

    cout << "\n  ===== EMISSIONS BY TRANSPORT MODE - CITY " << city << " =====\n\n";
    cout << ANAL_BORDER << "\n";
    cout << "| " << left
         << setw(21) << "Mode"              << " | "
         << setw(8)  << "Residents"         << " | "
         << setw(18) << "Total Emis (kg)"   << " | "
         << setw(18) << "Avg/Resident (kg)" << " | "
         << setw(8)  << "Share %"           << " |\n";
    cout << ANAL_BORDER << "\n";

    for (int i = 0; i < modeCount_arr; i++) {
        double avg = (modes[i].count > 0) ? modes[i].emission / modes[i].count : 0.0;
        double pct = (total > 0) ? (modes[i].emission / total * 100.0) : 0.0;
        cout << "| " << left  << setw(21) << fit(modes[i].mode, 21) << " | "
             << right << setw(8)  << modes[i].count                    << " | "
             << right << setw(18) << fixed << setprecision(2) << modes[i].emission  << " | "
             << right << setw(18) << fixed << setprecision(2) << avg << " | "
             << right << setw(8)  << fixed << setprecision(1) << pct << " |\n";
    }
    cout << ANAL_BORDER << "\n";

    if (modeCount_arr > 0) {
        cout << "\n  >> Insight: " << modes[0].mode
             << " is the highest emitting mode in City " << city << " ("
             << fixed << setprecision(1) << (modes[0].emission / total * 100.0) << "%).\n";
        cout << "  >> Insight: " << modes[modeCount_arr - 1].mode
             << " produces the fewest emissions in City " << city << " ("
             << fixed << setprecision(1) << (modes[modeCount_arr - 1].emission / total * 100.0) << "%).\n";
    }
}

void LinkedList::transportPercentageDistribution(char city) const {
    if (!head) { cout << "  (no data)\n"; return; }

    struct ModeCount { int count; string mode; };
    ModeCount modes[100];
    int modeCount_arr = 0;
    Node* cur = head;
    while (cur) {
        int idx = -1;
        for (int i = 0; i < modeCount_arr; i++) {
            if (modes[i].mode == cur->mode) { idx = i; break; }
        }
        if (idx == -1) {
            modes[modeCount_arr].mode = cur->mode;
            modes[modeCount_arr].count = 1;
            modeCount_arr++;
        } else {
            modes[idx].count++;
        }
        cur = cur->next;
    }

    for (int i = 0; i < modeCount_arr - 1; i++)
        for (int j = i+1; j < modeCount_arr; j++)
            if (modes[j].count > modes[i].count) {
                ModeCount tmp = modes[i];
                modes[i] = modes[j];
                modes[j] = tmp;
            }

    cout << "\n  ===== TRANSPORT PERCENTAGE DISTRIBUTION - CITY " << city << " =====\n\n";
    cout << TRAN_BORDER << "\n";
    cout << "| " << left
         << setw(21) << "Mode"       << " | "
         << setw(10) << "Residents"  << " | "
         << setw(8)  << "Share %"    << " | "
         << setw(32) << "Distribution" << " |\n";
    cout << TRAN_BORDER << "\n";

    const int BAR_WIDTH = 30;
    string topMode; int topCnt = 0;
    for (int i = 0; i < modeCount_arr; i++) {
        double pct  = (count > 0) ? (modes[i].count * 100.0 / count) : 0.0;
        int    bars = (int)(pct / 100.0 * BAR_WIDTH);
        string bar  = "[" + string(bars, '#') + string(BAR_WIDTH - bars, '.') + "]";
        cout << "| " << left  << setw(21) << fit(modes[i].mode, 21) << " | "
             << right << setw(10) << modes[i].count                    << " | "
             << right << setw(8)  << fixed << setprecision(1) << pct << " | "
             << left  << setw(32) << bar                    << " |\n";
        if (modes[i].count > topCnt) { topCnt = modes[i].count; topMode = modes[i].mode; }
    }
    cout << TRAN_BORDER << "\n";

    double topPct = (count > 0) ? (topCnt * 100.0 / count) : 0.0;
    cout << "\n  >> Insight: " << topMode << " is the most-used mode in City "
         << city << " (" << fixed << setprecision(1) << topPct << "% of residents).\n";
}

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

    struct GroupData { string name; double emission; int count; int minAge; int maxAge; };
    struct ModeInfo { string mode; int count; };
    struct GroupDetail { string groupName; ModeInfo modes[20]; int modeCount; };
    
    GroupData groups[6];
    GroupDetail groupDetails[6];

    for (int i = 0; i < 6; i++) {
        groups[i].name = GROUPS[i];
        groups[i].emission = 0.0;
        groups[i].count = 0;
        groups[i].minAge = 9999;
        groups[i].maxAge = -1;
        groupDetails[i].groupName = GROUPS[i];
        groupDetails[i].modeCount = 0;
    }

    Node* cur = head;
    while (cur) {
        int gIdx = -1;
        for (int i = 0; i < 6; i++) {
            if (groups[i].name == cur->ageGroup) { gIdx = i; break; }
        }
        if (gIdx >= 0) {
            groups[gIdx].emission += cur->totalEmission;
            groups[gIdx].count++;
            if (cur->age < groups[gIdx].minAge) groups[gIdx].minAge = cur->age;
            if (cur->age > groups[gIdx].maxAge) groups[gIdx].maxAge = cur->age;
            
            int mIdx = -1;
            for (int j = 0; j < groupDetails[gIdx].modeCount; j++) {
                if (groupDetails[gIdx].modes[j].mode == cur->mode) { mIdx = j; break; }
            }
            if (mIdx == -1) {
                groupDetails[gIdx].modes[groupDetails[gIdx].modeCount].mode = cur->mode;
                groupDetails[gIdx].modes[groupDetails[gIdx].modeCount].count = 1;
                groupDetails[gIdx].modeCount++;
            } else {
                groupDetails[gIdx].modes[mIdx].count++;
            }
        }
        cur = cur->next;
    }

    cout << "\n  ===== AGE GROUP ANALYSIS - CITY " << city << " =====\n\n";
    cout << AGRP_BORDER << "\n";
    cout << "| " << left
         << setw(45) << "Age Group (min-max)"         << " | "
         << setw(7)  << "Count"             << " | "
         << setw(19) << "Total Emis (kg)"   << " | "
         << setw(19) << "Avg/Resident (kg)" << " | "
         << setw(16) << "Preferred Mode"    << " |\n";
    cout << AGRP_BORDER << "\n";

    string topGroup; double topGroupEm = -1.0; int topGroupIdx = -1;
    for (int i = 0; i < 6; i++) {
        if (groups[i].count == 0) continue;
        double avg = groups[i].emission / groups[i].count;
        string prefMode = "-"; int maxC = 0;
        for (int j = 0; j < groupDetails[i].modeCount; j++)
            if (groupDetails[i].modes[j].count > maxC) { 
                maxC = groupDetails[i].modes[j].count; 
                prefMode = groupDetails[i].modes[j].mode; 
            }
        string grpLabel = groups[i].name + " (" + to_string(groups[i].minAge) + "-" + to_string(groups[i].maxAge) + ")";
        cout << "| " << left  << setw(45) << fit(grpLabel, 45)         << " | "
             << right << setw(7)  << groups[i].count                          << " | "
             << right << setw(19) << fixed << setprecision(2) << groups[i].emission  << " | "
             << right << setw(19) << fixed << setprecision(2) << avg << " | "
             << left  << setw(16) << fit(prefMode, 16)            << " |\n";
        if (groups[i].emission > topGroupEm) { topGroupEm = groups[i].emission; topGroup = groups[i].name; topGroupIdx = i; }
    }
    cout << AGRP_BORDER << "\n";

    cout << "\n  Mode breakdown per group:\n";
    for (int i = 0; i < 6; i++) {
        if (groupDetails[i].modeCount == 0) continue;
        cout << "\n  >> " << groupDetails[i].groupName << "\n";
        cout << "  " << SUB_BORDER << "\n";
        cout << "  | " << left << setw(20) << "Mode" << " | " << setw(8) << "Count" << " |\n";
        cout << "  " << SUB_BORDER << "\n";
        for (int j = 0; j < groupDetails[i].modeCount; j++)
            cout << "  | " << left << setw(20) << fit(groupDetails[i].modes[j].mode, 20) << " | " << right << setw(8) << groupDetails[i].modes[j].count << " |\n";
        cout << "  " << SUB_BORDER << "\n";
    }

    string prefOfTop = "-"; int maxC = 0;
    if (topGroupIdx >= 0) {
        for (int j = 0; j < groupDetails[topGroupIdx].modeCount; j++)
            if (groupDetails[topGroupIdx].modes[j].count > maxC) { 
                maxC = groupDetails[topGroupIdx].modes[j].count; 
                prefOfTop = groupDetails[topGroupIdx].modes[j].mode; 
            }
    }
    cout << "\n  >> Insight: " << topGroup
         << " is the biggest emitting age group in City " << city
         << " with " << fixed << setprecision(2) << topGroupEm << " kg CO2/month.\n";
    cout << "  >> Insight: Their preferred transport mode is " << prefOfTop << ".\n";
}

void LinkedList::topBottomEmitters(int n) const {
    if (!head) { cout << "  (no data)\n"; return; }
    if (n < 1) n = 5;
    if (n > count) n = count;

    Node** arr = new Node*[count];
    Node* cur = head;
    for (int i = 0; i < count; ++i, cur = cur->next) arr[i] = cur;
    for (int i = 0; i < count - 1; ++i) {
        int hi = i;
        for (int j = i+1; j < count; ++j)
            if (arr[j]->totalEmission > arr[hi]->totalEmission) hi = j;
        if (hi != i) swap(arr[i], arr[hi]);
    }

    auto printRankTable = [&](const string& title, int from, int to, bool asc) {
        cout << "\n  " << title << "\n";
        cout << RANK_BORDER << "\n";
        cout << "| " << left
             << setw(6)  << "Rank"  << " | "
             << setw(6)  << "ID"    << " | "
             << setw(5)  << "Age"   << " | "
             << setw(35) << "Age Group" << " | "
             << setw(12) << "Mode"  << " | "
             << setw(15) << "Total Emis(kg)" << " |\n";
        cout << RANK_BORDER << "\n";
        if (asc) {
            for (int i = from; i <= to; ++i) {
                int rank = count - i;
                cout << "| " << left  << setw(6)  << ("#" + to_string(rank))         << " | "
                     << left  << setw(6)  << fit(arr[i]->residentID, 6)               << " | "
                     << right << setw(5)  << arr[i]->age                               << " | "
                     << left  << setw(35) << fit(arr[i]->ageGroup, 35)                 << " | "
                     << left  << setw(12) << fit(arr[i]->mode, 12)                     << " | "
                     << right << setw(15) << fixed << setprecision(2) << arr[i]->totalEmission << " |\n";
            }
        } else {
            for (int i = from; i <= to; ++i) {
                cout << "| " << left  << setw(6)  << ("#" + to_string(i + 1))         << " | "
                     << left  << setw(6)  << fit(arr[i]->residentID, 6)               << " | "
                     << right << setw(5)  << arr[i]->age                               << " | "
                     << left  << setw(35) << fit(arr[i]->ageGroup, 35)                 << " | "
                     << left  << setw(12) << fit(arr[i]->mode, 12)                     << " | "
                     << right << setw(15) << fixed << setprecision(2) << arr[i]->totalEmission << " |\n";
            }
        }
        cout << RANK_BORDER << "\n";
    };

    printRankTable("TOP " + to_string(n) + " HIGHEST EMITTERS", 0, n - 1, false);
    printRankTable("BOTTOM " + to_string(n) + " LOWEST EMITTERS", count - n, count - 1, true);

    double gap = arr[0]->totalEmission - arr[count - 1]->totalEmission;
    cout << "\n  >> Insight: Emission gap (highest - lowest) = "
         << fixed << setprecision(2) << gap << " kg CO2/month.\n";
    cout << "  >> Insight: Top emitter " << arr[0]->residentID << " uses "
         << arr[0]->mode << " (" << fixed << setprecision(2)
         << arr[0]->totalEmission << " kg CO2/month).\n";

    delete[] arr;
}

// ── multi-condition search ────────────────────────────────────────────────
void LinkedList::multiConditionSearchAgeMode(int minAge, int maxAge, const string& mode) const {
    cout << "\n  [Multi-Condition Search]  Age " << minAge << "-" << maxAge
         << "  AND  Mode = \"" << mode << "\"\n";
    auto start = chrono::high_resolution_clock::now();

    string modeLow = mode;
    for (char& c : modeLow) c = (char)tolower(c);

    srchHeader();
    int found = 0;
    Node* cur = head;
    while (cur) {
        string curLow = cur->mode;
        for (char& c : curLow) c = (char)tolower(c);
        if (cur->age >= minAge && cur->age <= maxAge && curLow == modeLow)
            { srchRow(cur); ++found; }
        cur = cur->next;
    }
    cout << SRCH_BORDER << "\n";
    double us = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
    printSearchStats("Multi-Cond: Age+Mode", us, "O(n)", "O(1)", found, count);
}

void LinkedList::multiConditionSearchDistMode(double minKm, const string& mode) const {
    cout << "\n  [Multi-Condition Search]  Dist > " << minKm
         << " km  AND  Mode = \"" << mode << "\"\n";
    auto start = chrono::high_resolution_clock::now();

    string modeLow = mode;
    for (char& c : modeLow) c = (char)tolower(c);

    srchHeader();
    int found = 0;
    Node* cur = head;
    while (cur) {
        string curLow = cur->mode;
        for (char& c : curLow) c = (char)tolower(c);
        if (cur->distance > minKm && curLow == modeLow)
            { srchRow(cur); ++found; }
        cur = cur->next;
    }
    cout << SRCH_BORDER << "\n";
    double us = chrono::duration<double, micro>(chrono::high_resolution_clock::now() - start).count();
    printSearchStats("Multi-Cond: Dist+Mode", us, "O(n)", "O(1)", found, count);
}
