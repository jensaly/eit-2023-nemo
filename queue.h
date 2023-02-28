#ifndef EIT_QUEUE_H
#define EIT_QUEUE_H

#include "vehicle.h"

struct Queue {
    std::string name;
    uint64_t flags = 0;
    float available_size;
    const float total_size;
    const float width;
    uint16_t total_vehicles = 0;
    float y;
    std::vector<bool> reserved; // lane is reserved exclusively for certain types of vehicle
    std::vector<bool> priority; // lane has priority for a specific types of vehicles
    bool has_reserved = false;
    bool has_priority = false;

    std::vector<Vehicle> vehicles;

    Queue(std::string name, float size, float width, float y, std::initializer_list<VehicleFlags> reserved_flags = {}, std::initializer_list<VehicleFlags> priority_flags = {});
    void AddVehicleToQueue(Vehicle vehicle);
    void EraseVehicleFromQueue(std::vector<Vehicle>::iterator& vehicle);
    void SetReservedFlag(VehicleFlags flag) { reserved[(int)flag] = true; has_reserved = true; }
    void UnsetReservedFlag(VehicleFlags flag) { reserved[(int)flag] = false; }
    bool GetReservedFlag(VehicleFlags flag) { return reserved[(int)flag]; }
    void SetPriorityFlag(VehicleFlags flag) { priority[(int)flag] = true; has_priority = true; }
    void UnsetPriorityFlag(VehicleFlags flag) { priority[(int)flag] = false; }
    bool GetPriorityFlag(VehicleFlags flag) const { return priority[(int)flag]; }

    bool IsAvailableSpace(const Vehicle& v) const { return available_size > v.length; }
    void clear();
};

#endif //EIT_QUEUE_H
