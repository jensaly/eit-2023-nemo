#ifndef EIT_CAR_H
#define EIT_CAR_H

#include "utility.h"

struct Vehicle {
    float length; // m
    float width; // m
    float mass; // kg

    Vehicle(float length, float width, float mass) : length{length}, width{width}, mass{mass} {}
    Vehicle() : Vehicle(3.0, 2.0, 800) {}
};

struct Ferry {

};

#endif