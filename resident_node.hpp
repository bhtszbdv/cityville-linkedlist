#pragma once
#include <string>

// represents one person/resident in the city
struct Node {
    std::string residentID;     
    int         age;
    std::string mode;           
    double      distance;       
    double      emissionFactor; 
    int         days;          
    std::string ageGroup;      
    double      totalEmission;  

    Node* next; 

    Node() : age(0), distance(0.0), emissionFactor(0.0), days(0), ageGroup(""), totalEmission(0.0), next(nullptr) {}
};