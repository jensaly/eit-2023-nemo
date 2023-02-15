#ifndef EIT_CAR_H
#define EIT_CAR_H

#include "utility.h"

struct Vehicle {
    std::string type = "B";
    std::string plate_nr;
    float length; // m
    float width; // m
    float mass; // kg
    float arrival_time = 0; // time of simulated arrival
    float x = 0; // bottom left corner placement
    float y = 0;
    float vehicle_half_buff = 0.15; // lengthwise clearance between vehicles
    float col[4]; // car color, used in part by the graphical side

    Vehicle(std::string type, std::string plate_nr, float length, float width, float mass, float arrival_time);
    Vehicle() : Vehicle("B", "dbg007", 4.18, 1.765, 1265, -1) {}
};

#endif