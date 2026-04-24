#pragma once
#include <string>
#include "resident_node.hpp"

      // linked list class - stores all resident nodes for a city
class LinkedList {
private:
    Node* head;    // start of the list
    int   count; // total nodes
    std::string sourceFilename;    // so we know where to save back
    std::string csvHeader;      // keep the original header line

public:
       // constructor/destructor
    LinkedList();
    ~LinkedList();

        // append to end (mainly used when loading csv)
    void insert(const std::string& id, int age, const std::string& mode,
                double distance, double emFactor, int days,
                const std::string& ageGroup, double totalEmission);
       // add to front of list
    void insertAtBeginning(const std::string& id, int age, const std::string& mode,
                           double distance, double emFactor, int days,
                           const std::string& ageGroup, double totalEmission);
                          // insert at a given position (1 = head)
    void insertAtPosition(int pos, const std::string& id, int age, const std::string& mode,
                          double distance, double emFactor, int days,
                          const std::string& ageGroup, double totalEmission);
            // delete by resident id
    bool deleteResident(const std::string& id);
       // reverse the whole list
    void reverse();

    void display() const;
    int  size()    const { return count; }
    void clear();

     // load from csv file
    bool loadFromCSV(const std::string& filename);
          // write current list back to csv
    bool saveToCSV() const;

          // bubble sort - O(n^2)
    void sortByAge();
    void sortByDistance();
    void sortByEmission();

             // insertion sort - faster when nearly sorted
    void insertionSortByAge();
    void insertionSortByDistance();
    void insertionSortByEmission();

     // go back to original order (sort by resident ID)
    void sortByResidentID();

       // linear search
    void linearSearchByAgeGroup(int minAge, int maxAge) const;
    void linearSearchByMode(const std::string& mode)     const;
    void linearSearchByDistanceThreshold(double minKm)   const;

    // binary search - needs list sorted by age first
    void binarySearchByAge(int targetAge) const;

    // analysis
    void carbonAnalysis()   const;
    void ageGroupAnalysis() const;

        // get the right age        group label based on city
    static std::string ageGroupLabel(int age, char cityPrefix);
private:
            // swap node       data without moving pointers
    static void swapData(Node* a, Node* b);
     // check if id already in list
    bool idExists(const std::string& id) const;
};