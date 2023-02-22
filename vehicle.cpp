#include "vehicle.h"

Vehicle::Vehicle(std::string type, std::string plate_nr, float length, float width, float mass, float arrival_time) : type{type}, plate_nr{plate_nr},
    length{length}, width{width}, mass{mass}, arrival_time{arrival_time}, col{1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f} {
    characteristics = std::vector<bool>(100, false);
    if (mass > 1300) {
        SetFlag(VehicleFlags::Heavy);
    }
    else {
        if (rand() % 5 == 0) {
            SetFlag(VehicleFlags::Ambulance);
            col[0] = 1.0f; col[1] = 0; col[2] = 0;
        }
    }
}