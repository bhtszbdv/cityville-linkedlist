#pragma once
#include <string>

// represents one person/resident in the city
struct Node {
    std::string residentID;     // e.g A001, B023 etc
    int         age;
    std::string mode;           // transport mode (car, bus..)
    double      distance;       // daily km
    double      emissionFactor; // co2 per km
    int         days;           // how many days per month they travel
    std::string ageGroup;       // "Working Adults (Early Career)" etc
    double      totalEmission;  // either from csv or calculated when user adds manually

    Node* next; // pointer to next node

    Node() : age(0), distance(0.0), emissionFactor(0.0), days(0), ageGroup(""), totalEmission(0.0), next(nullptr) {}
};