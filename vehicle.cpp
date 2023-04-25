#include "vehicle.h"

std::string vfstr(VehicleFlags e) {
    switch(e) {
        case VehicleFlags::Ambulance: {
            return "Ambulance";
        }
        case VehicleFlags::HC: {
            return "HC";
        }
        case VehicleFlags::Heavy: {
            return "Heavy";
        }
        case VehicleFlags::Electric: {
            return "Electric";
        }
        default: {
            return "DFLT";
        }
    }
}

VehicleFlags& operator ++ (VehicleFlags& e) {
    if (e == VehicleFlags::End) {
        throw std::out_of_range("for E& operator ++ (E&)");
    }
    e = VehicleFlags(static_cast<std::underlying_type<VehicleFlags>::type>(e) + 1);
    return e;
}

Vehicle::Vehicle(std::string type, std::string plate_nr, float length, float width, float mass, float buf, float arrival_time, std::mt19937 generator) : type{type}, plate_nr{plate_nr},
    length{length}, width{width}, mass{mass}, buf{buf}, arrival_time{arrival_time}, col{1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f} {
    characteristics = std::vector<bool>(100, false);
    std::uniform_int_distribution rand;
    if (mass > 1350) {
        this->type = "H";
        SetFlag(VehicleFlags::Heavy);
        this->length *= 3;
    } else if (rand(generator)%3 == 0) {
        this->type = "B_EL";
        SetFlag(VehicleFlags::Electric);
        col[0] = 0; col[1] = 1.0f; col[2] = 0;
    }
    else {
        if (rand(generator) % 9 == 0) {
            this->type = "AMB";
            SetFlag(VehicleFlags::Ambulance);
            col[0] = 1.0f; col[1] = 0; col[2] = 0;
        }
    }
}