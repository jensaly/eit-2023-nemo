#include "preloadingqueue.h"

LoadingQueue::LoadingQueue(std::string name, double size, float width) : name{std::move(name)}, available_size{size}, total_size{size}, width{width} {

}

Yard::Yard(int number_of_queues, double queue_length, double queue_width) {
    for (int i = 0; i < number_of_queues; i++) {
        queues.emplace_back(std::to_string(i), queue_length, queue_width);
    }
}

bool Yard::Arrival(Vehicle vehicle) {
    // TODO: Replace the code here with a scoring system
    auto& min_queue = *std::max_element(queues.begin(), queues.end(), [&](auto& a, auto& b){ return a.available_size < b.available_size;});
    if (min_queue.available_size < vehicle.length) {
        pre_yard.push_back(vehicle);
        return false;
    }
    min_queue.available_size -= vehicle.length;
    min_queue.vehicles.push_back(vehicle);
    min_queue.total_vehicles++;
    return true;
}

void Yard::GenerateSetOfVehicles(uint16_t number) {
    for (uint16_t i = 0; i < number; i++) {
        Arrival(Vehicle());
    }
}