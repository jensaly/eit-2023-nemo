#include "preloadingqueue.h"

Yard::Yard(int number_of_queues, double queue_length, double queue_width) : ferry{5, 17.8, 4.5, 30.0, 30.0} {
    for (int i = 0; i < number_of_queues; i++) {
        queues.emplace_back(std::to_string(i), queue_length, queue_width);
    }
}

// Function takes a vehicle and puts it into a Yard queue depending on a set of rules
// At present, that rule is "whichever queue has the most size available)
// If the arrival cannot be slotted into the yard, it is instead put in a single pre-queue
bool Yard::Arrival(Vehicle vehicle) {
    // TODO: Replace the code here with a scoring system
    auto& min_queue = *std::max_element(queues.begin(), queues.end(), [&](auto& a, auto& b){ return a.available_size < b.available_size;});
    if (min_queue.available_size < vehicle.length) {
        pre_yard.push_back(vehicle);
        return false;
    }
    min_queue.AddVehicleToQueue(vehicle);
    return true;
}

// Current simulating arrivals with a hard-coded gamma distribution for arrival times
// Subject to change, better distribution needed
void Yard::SimulteQueueArrival(double time) {
    std::default_random_engine generator;
    std::gamma_distribution<double> distribution(4, 3.4);
    double t = 0;
    while (t < time) {
        double dt = distribution(generator);
        t += dt;
        Arrival(Vehicle(4.18 * (1 + (0.2 * rand() / RAND_MAX - 0.1)), 1.765 * (1 + (0.2 * rand() / RAND_MAX - 0.1)), 1265 * (1 + (0.2 * rand() / RAND_MAX - 0.1)), t)); // defaulted to a Yaris Cross because no better data
        vehicles_in_yard++;
    }
}

void Yard::GenerateSetOfVehicles(uint16_t number) {
    for (uint16_t i = 0; i < number; i++) {
        Arrival(Vehicle());
    }
}

void Yard::Embark() {
    WorstFitAlgorithm();
}

// This algorithm is basic and does not have any memory, merely responding to the current state of the queue
void Yard::WorstFitAlgorithm() {
    // Find first available car
    while (true) {
        auto q = find_if(queues.begin(), queues.end(), [&](auto &queue) { return queue.total_vehicles > 0; });
        if (q == queues.end()) {
            break;
        }
        auto v = std::min_element(q->vehicles.begin(), q->vehicles.end(), [&](auto &veh1, auto &veh2) { return veh1.length < veh2.length; });
        auto min_q = q;
        for (; q != queues.end(); q++) {
            auto loc_min_v = std::min_element(q->vehicles.begin(), q->vehicles.end(),[&](auto &a, auto &b) { return a.length < b.length; });
            if (loc_min_v != q->vehicles.end() && loc_min_v->length < v->length) {
                v = loc_min_v;
                min_q = q;
            }
        }
        auto f_min_q = std::max_element(ferry.queues.begin(), ferry.queues.end(), [&](auto& q1, auto& q2){ return q1.available_size < q2.available_size; });
        f_min_q->AddVehicleToQueue(std::move(*v));
        min_q->EraseVehicleFromQueue(v);
    }
}

void Yard::Test4Cars() {
    Arrival({3, 1.5, 700, 0});
    Arrival({3.2, 1.5, 700, 0});
    Arrival({3.1, 1.5, 700, 0});
    Arrival({2.9, 1.5, 700, 0});
    Arrival({3.05, 1.5, 700, 0});
    Arrival({3, 1.5, 700, 0});
    Arrival({3, 1.5, 700, 0});
}

