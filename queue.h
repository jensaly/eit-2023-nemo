#ifndef EIT_QUEUE_H
#define EIT_QUEUE_H

#include "vehicle.h"

struct Queue {
    std::string name;
    uint64_t flags = 0;
    float available_size;
    float total_size;
    float width;
    uint16_t total_vehicles = 0;
    float y;
    std::vector<bool> reserved; // lane is reserved exclusively for certain types of vehicle
    std::vector<int64_t> reserved_score;
    std::vector<bool> priority; // lane has priority for a specific types of vehicles
    std::vector<int64_t> priority_score;
    bool has_reserved = false;
    bool has_priority = false;
    bool superpriority = false;

    std::vector<Vehicle> vehicles;

    Queue(std::string name, float size, float width, float y, std::initializer_list<VehicleFlags> reserved_flags = {}, std::initializer_list<VehicleFlags> priority_flags = {});
    template<typename T> void AddVehicleToQueue(Vehicle vehicle, T& t) {
        vehicle.y = y + (width - vehicle.width) / 2;
        float real_x = 0;
        if (vehicles.empty()) {
            vehicle.x = real_x;
        }
        else {
            auto& veh = vehicles.back();
            real_x = veh.x + veh.length + veh.buf;
            vehicle.x = real_x;
        }
        available_size -= vehicle.length + vehicle.buf;
        vehicles.push_back(vehicle);
        total_vehicles++;
        t.total_vehicles++;
    }
    template<typename T> void EraseVehicleFromQueue(std::vector<Vehicle>::iterator& vehicle, T& t)  {
        available_size += vehicle->length;
        vehicles.erase(vehicle);
        total_vehicles--;
        t.total_vehicles++;
    }
    void SetReservedFlag(VehicleFlags flag) { reserved[(int)flag] = true; reserved_score[int(flag)] = 10000; has_reserved = true; }
    void UnsetReservedFlag(VehicleFlags flag) { reserved[(int)flag] = false; if (std::all_of(reserved.begin(), reserved.end(), [](const bool& b){ return !b; })) { has_reserved = false; } }
    bool GetReservedFlag(VehicleFlags flag) const { return reserved[(int)flag]; }
    void SetReservedPoints(VehicleFlags flag, int64_t score) { reserved[(int)flag] = true; reserved_score[int(flag)] = score; has_reserved = true; }

    void SetPriorityFlag(VehicleFlags flag) { priority[(int)flag] = true; priority_score[int(flag)] = 1;  has_priority = true; }
    void UnsetPriorityFlag(VehicleFlags flag) { priority[(int)flag] = false; if (std::all_of(priority.begin(), priority.end(), [](const bool& b){ return !b; })) { has_priority = false; } }
    bool GetPriorityFlag(VehicleFlags flag) const { return priority[(int)flag]; }
    void SetPriorityPoints(VehicleFlags flag, int64_t score) { reserved[(int)flag] = true; priority_score[int(flag)] = score; has_reserved = true; }

    void SetSuperPriority(bool set) { superpriority = set; }
    bool GetSuperPriority() const { return superpriority; }

    bool IsAvailableSpace(const Vehicle& v) const { return available_size > v.length; }
    void clear();
};

#endif //EIT_QUEUE_H
