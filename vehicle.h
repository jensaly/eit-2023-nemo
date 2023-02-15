#ifndef EIT_CAR_H
#define EIT_CAR_H

#include "utility.h"

struct Vehicle {
    float length; // m
    float width; // m
    float mass; // kg
    float arrival_time = 0; // time of simulated arrival
    float x = 0; // bottom left corner placement
    float y = 0;
    float vehicle_half_buff = 0.15; // lengthwise clearance between vehicles
    float col[4]; // car color, used in part by the graphical side

    Vehicle(float length, float width, float mass, float arrival_time) : length{length}, width{width}, mass{mass}, arrival_time{arrival_time}, col{1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 255} {}
    Vehicle() : Vehicle(4.18, 1.765, 1265, -1) {}
};

#endif