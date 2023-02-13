#ifndef EIT_FERRY_H
#define EIT_FERRY_H

#include "vehicle.h"

struct Queue {
    std::string name;
    uint64_t flags = 0;
    double available_size;
    const double total_size;
    const float width;
    uint16_t total_vehicles = 0;

    std::vector<Vehicle> vehicles;

    Queue(std::string name, double size, float width);
    void AddVehicleToQueue(Vehicle vehicle);
    void EraseVehicleFromQueue(std::vector<Vehicle>::iterator& vehicle);
};

struct Ferry {
    std::vector<Queue> queues;
    double width;
    double height;

    Ferry(int queues, double q_height, double q_width, double width, double height);
};

#endif //EIT_FERRY_H
