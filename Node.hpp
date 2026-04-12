#pragma once
#include <string>

// one person in the city
struct Node {
    std::string residentID;     // like A001 or B023
    int         age;
    std::string mode;           // how they get around
    double      distance;       // km per day
    double      emissionFactor; // how much co2 per km
    int         days;           // days they travel per month
    double      totalEmission;  // distance * factor * days, calculated on load

    Node* next; // points to the next person in the list

    Node() : age(0), distance(0.0), emissionFactor(0.0), days(0), totalEmission(0.0), next(nullptr) {}
};