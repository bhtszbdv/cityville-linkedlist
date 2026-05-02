#pragma once
#include <string>
#include "resident_node.hpp"

      // linked list class - stores all resident nodes for a city
class LinkedList {
private:
    Node* head;    // start of the list
    int   count;   // total nodes
    std::string sourceFilename;   // path of the file loaded from
    std::string csvHeader;        // original header line
    double lastSortUs = 0.0;   // elapsed time from the last sort call

public:
       // constructor/destructor
    LinkedList();
    ~LinkedList();

    void display() const;
    int    size()        const { return count; }
    double sortTime()    const { return lastSortUs; }  // microseconds from last sort
    void clear();
    void reverse();

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

    // multi-condition search (City A and B only)
    void multiConditionSearchAgeMode(int minAge, int maxAge, const std::string& mode) const;
    void multiConditionSearchDistMode(double minKm, const std::string& mode) const;

    // analysis
    void carbonAnalysis()   const;
    void ageGroupAnalysis() const;

    // extended analysis (City A and B only)
    void totalEmissionsReport(char city)           const;
    void emissionsByMode(char city)                const;
    void transportPercentageDistribution(char city) const;
    void ageGroupAnalysisExtended(char city)       const;
    void topBottomEmitters(int n)                  const;

        // get the right age        group label based on city
    static std::string ageGroupLabel(int age, char cityPrefix);
private:
            // swap node       data without moving pointers
    static void swapData(Node* a, Node* b);
};