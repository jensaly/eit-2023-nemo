#include "queue.h"

Queue::Queue(std::string name, float size, float width, float y, std::initializer_list<VehicleFlags> reserved_flags, std::initializer_list<VehicleFlags> priority_flags)
        : name{std::move(name)}, available_size{size}, total_size{size}, width{width}, y{y} {
    reserved = std::vector<bool>(100, false);
    priority = std::vector<bool>(100, false);
    for (auto& i : reserved_flags) {
        SetReservedFlag(i);
    }
    for (auto& i : priority_flags) {
        SetPriorityFlag(i);
    }
}

void Queue::clear() {
    vehicles.clear();
    total_vehicles = 0;
    available_size = total_size;
}