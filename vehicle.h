#ifndef EIT_CAR_H
#define EIT_CAR_H

#include "utility.h"

struct Vehicle {
    float length; // m
    float width; // m
    float mass; // kg
    float arrival_time = 0;

    Vehicle(float length, float width, float mass, float arrival_time) : length{length}, width{width}, mass{mass}, arrival_time{arrival_time} {}
    Vehicle() : Vehicle(4.18, 1.765, 1265, -1) {}
};

#endif