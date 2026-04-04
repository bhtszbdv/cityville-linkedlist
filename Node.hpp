#pragma once
#include <string>

struct Node {
    std::string residentID;   // e.g. "A001", "B001"
    int         age;
    std::string mode;         // Car, Bus, Bicycle, Walking, School Bus, Carpool
    double      distance;     // Daily Distance (km)
    double      emissionFactor; // Carbon Emission Factor (kg CO2/km)
    int         days;         // Average Days per Month
    double      totalEmission; // = distance * emissionFactor * days  (computed on load)

    Node* next;

    Node() : age(0), distance(0.0), emissionFactor(0.0), days(0), totalEmission(0.0), next(nullptr) {}
};