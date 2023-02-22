#include "queue.h"

Queue::Queue(std::string name, double size, float width, float y, std::initializer_list<VehicleFlags> reserved_flags, std::initializer_list<VehicleFlags> priority_flags)
        : name{std::move(name)}, available_size{size}, total_size{size}, width{width}, y{y} {
    reserved = std::vector<bool>(100, false);
    priority = std::vector<bool>(100, false);
    for (auto& i : reserved_flags) {
        SetReservedFlag(i);
    }
    for (auto& i : reserved_flags) {
        SetPriorityFlag(i);
    }
}

void Queue::AddVehicleToQueue(Vehicle vehicle) {
    vehicle.y = y + (width - vehicle.width) / 2;
    vehicle.x = total_size - available_size;
    available_size -= vehicle.length;
    vehicles.push_back(vehicle);
    total_vehicles++;
}

void Queue::EraseVehicleFromQueue(std::vector<Vehicle>::iterator& vehicle) {
    available_size += vehicle->length;
    vehicles.erase(vehicle);
    total_vehicles--;
}