#ifndef EIT_QUEUE_H
#define EIT_QUEUE_H

#include "vehicle.h"

struct Queue {
    std::string name;
    uint64_t flags = 0;
    double available_size;
    const double total_size;
    const float width;
    uint16_t total_vehicles = 0;
    float y;

    std::vector<Vehicle> vehicles;

    Queue(std::string name, double size, float width, float y);
    void AddVehicleToQueue(Vehicle vehicle);
    void EraseVehicleFromQueue(std::vector<Vehicle>::iterator& vehicle);
};

#endif //EIT_QUEUE_H
