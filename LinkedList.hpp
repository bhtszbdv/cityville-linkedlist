#pragma once
#include <string>
#include "Node.hpp"

// the linked list class, holds all the resident nodes
class LinkedList {
private:
    Node* head;  // first node
    int   count; // how many nodes are in here

public:
    // setup and cleanup
    LinkedList();
    ~LinkedList();

    // insert at the end (used when loading from csv)
    void insert(const std::string& id, int age, const std::string& mode,
                double distance, double emFactor, int days);
    // insert at the very front
    void insertAtBeginning(const std::string& id, int age, const std::string& mode,
                           double distance, double emFactor, int days);
    // insert somewhere in the middle, position starts at 1
    void insertAtPosition(int pos, const std::string& id, int age, const std::string& mode,
                          double distance, double emFactor, int days);
    // remove a node by resident id
    bool deleteResident(const std::string& id);
    // flip the whole list
    void reverse();

    void display() const;
    int  size()    const { return count; }
    void clear();

    // reads the csv and loads everything in
    bool loadFromCSV(const std::string& filename);

    // sorting, all bubble sort so O(n^2)
    void sortByAge();
    void sortByDistance();
    void sortByEmission();

    // linear search options
    void linearSearchByAgeGroup(int minAge, int maxAge) const;
    void linearSearchByMode(const std::string& mode)     const;
    void linearSearchByDistanceThreshold(double minKm)   const;

    // binary search, list needs to be sorted by age first
    void binarySearchByAge(int targetAge) const;

    // analysis functions
    void carbonAnalysis()   const;
    void ageGroupAnalysis() const;

private:
    // returns the age group label for a given age
    static std::string ageGroupLabel(int age);
    // swaps data between two nodes without touching the pointers
    static void swapData(Node* a, Node* b);
    // checks if an id is already in the list
    bool idExists(const std::string& id) const;
};