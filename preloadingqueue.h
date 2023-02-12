#ifndef EIT_PRELOADINGQUEUE_H
#define EIT_PRELOADINGQUEUE_H

#include "vehicle.h"

struct LoadingQueue {
    std::string name;
    uint64_t flags = 0;
    double available_size;
    const double total_size;
    const float width;
    uint16_t total_vehicles = 0;

    std::deque<Vehicle> vehicles;

    LoadingQueue(std::string name, double size, float width);
};

struct Yard {
    std::vector<LoadingQueue> queues;
    std::deque<Vehicle> pre_yard;

    Yard(int number_of_queues, double queue_length, double queue_width);
    bool Arrival(Vehicle vehicle);
    void GenerateSetOfVehicles(uint16_t number);
    void Embark();
};

#endif //EIT_PRELOADINGQUEUE_H
