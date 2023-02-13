#include "ferry.h"


Queue::Queue(std::string name, double size, float width) : name{std::move(name)}, available_size{size}, total_size{size}, width{width} {

}

void Queue::AddVehicleToQueue(Vehicle vehicle) {
    available_size -= vehicle.length;
    vehicles.push_back(vehicle);
    total_vehicles++;
}

void Queue::EraseVehicleFromQueue(std::vector<Vehicle>::iterator& vehicle) {
    available_size += vehicle->length;
    vehicles.erase(vehicle);
    total_vehicles--;
}

Ferry::Ferry(int num_queues, double q_height, double q_width, double width, double height) : width{width}, height{height} {
    if (width < num_queues * q_width) {
        std::cerr << "Ferry to narrow";
    }
    if (q_height > height) {
        std::cerr << "Ferry to short";
    }
    for (int i = 0; i < num_queues; i++) {
        queues.emplace_back(std::to_string(i), q_height, q_width);
    }
}