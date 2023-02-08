#ifndef EIT_CAR_H
#define EIT_CAR_H

#include "utility.h"

struct Vehicle {
    uint16_t length; // m
    uint16_t width; // m
    uint16_t mass; // kg

    Vehicle(uint16_t length, uint16_t width, uint16_t mass) : length{length}, width{width}, mass{mass} {}
};

#endif