#include "ferry.h"


Queue::Queue(std::string name, double size, float width, float y) : name{std::move(name)}, available_size{size}, total_size{size}, width{width}, y{y} {

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

Ferry::Ferry(int num_queues, double q_length, double q_width, double width, double length) : width{width}, length{length} {
    if (width < num_queues * q_width) {
        std::cerr << "Ferry to narrow";
    }
    if (q_length > length) {
        std::cerr << "Ferry to short";
    }
    for (int i = 0; i < num_queues; i++) {
        queues.emplace_back(std::to_string(i), q_length, q_width, i * q_width);
    }
}