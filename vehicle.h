#ifndef EIT_CAR_H
#define EIT_CAR_H

#include "utility.h"

enum class VehicleFlags {
    Begin = -1,
    Ambulance = 0,
    HC = 1,
    Heavy = 2,
    Electric = 3,
    End
};

std::string vfstr(VehicleFlags e);

VehicleFlags& operator ++ (VehicleFlags& e);

struct Vehicle {
    std::string type = "B";
    std::string plate_nr;
    float length; // m
    float width; // m
    float mass; // kg
    float arrival_time = 0; // time of simulated arrival
    float x = 0; // bottom left corner placement
    float y = 0;
    float buf;
    float col[4]; // car color, used in part by the graphical side
    std::vector<bool> characteristics; // characteristics of a vehicle
    std::string y_q_loaded = "DFLT";

    Vehicle(std::string type, std::string plate_nr, float length, float width, float mass, float buf, float arrival_time, std::mt19937 geneator);
    Vehicle() : Vehicle("B", "dbg007", 4.18, 1.765, 1265, 0.3, -1, std::mt19937{std::random_device{}()}) {}
    void SetFlag(VehicleFlags flag) { characteristics[(int)flag] = true; }
    void UnsetFlag(VehicleFlags flag) { characteristics[(int)flag] = false; }
    bool GetFlag(VehicleFlags flag) { return characteristics[(int)flag]; }
};

#endif