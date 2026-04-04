#pragma once
#include <string>
#include "Node.hpp"

class LinkedList {
private:
    Node* head;
    int   count;

public:
    // ── Lifecycle ──────────────────────────────────────────────
    LinkedList();
    ~LinkedList();

    // ── Basic operations ───────────────────────────────────────
    void insert(const std::string& id, int age, const std::string& mode,
                double distance, double emFactor, int days);
    void display() const;
    int  size()    const { return count; }
    void clear();

    // ── File I/O ───────────────────────────────────────────────
    bool loadFromCSV(const std::string& filename);

    // ── Sorting (Bubble Sort – O(n²)) ─────────────────────────
    void sortByAge();
    void sortByDistance();
    void sortByEmission();

    // ── Searching ─────────────────────────────────────────────
    // Linear search on any list
    void linearSearchByAgeGroup(int minAge, int maxAge) const;
    void linearSearchByMode(const std::string& mode)     const;
    void linearSearchByDistanceThreshold(double minKm)   const;

    // Binary search – list MUST be sorted by age first
    void binarySearchByAge(int targetAge) const;

    // ── Analysis ──────────────────────────────────────────────
    void carbonAnalysis()   const;  // total + per mode
    void ageGroupAnalysis() const;  // per group: preferred mode, totals, avg

private:
    // Helper: get age-group label
    static std::string ageGroupLabel(int age);
    // Helper: swap data between two nodes (keeps pointers intact)
    static void swapData(Node* a, Node* b);
};