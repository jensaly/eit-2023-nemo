#include "vehicle.h"

Vehicle::Vehicle(std::string type, std::string plate_nr, float length, float width, float mass, float arrival_time) : type{type}, plate_nr{plate_nr},
    length{length}, width{width}, mass{mass}, arrival_time{arrival_time}, col{1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 255} {}