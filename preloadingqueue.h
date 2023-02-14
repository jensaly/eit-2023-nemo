#ifndef EIT_PRELOADINGQUEUE_H
#define EIT_PRELOADINGQUEUE_H

#include "ferry.h"

template<class Algorithm>
struct Yard {
    std::vector<Queue> queues; // main yard area
    std::deque<Vehicle> pre_yard; // road leading up to yard
    int vehicles_in_yard = 0;
    Algorithm algorithm; // selected algorithm for fine sorting

    Yard(int number_of_queues, double queue_length, double queue_width, Algorithm algorithm) : algorithm{algorithm} {
        for (int i = 0; i < number_of_queues; i++) {
            queues.emplace_back(std::to_string(i), queue_length, queue_width, queue_width * i);
        }
    }

    // Function takes a vehicle and puts it into a Yard queue depending on a set of rules
    // At present, that rule is "whichever queue has the most size available)
    // If the arrival cannot be slotted into the yard, it is instead put in a single pre-queue
    bool Arrival(Vehicle vehicle) {
        // TODO: Replace the code here with a scoring system
        auto& min_queue = *std::max_element(queues.begin(), queues.end(), [&](auto& a, auto& b){ return a.available_size < b.available_size;});
        auto x = 0;
        if (min_queue.available_size < vehicle.length) {
            pre_yard.push_back(vehicle);
            return false;
        }
        min_queue.AddVehicleToQueue(vehicle);
        return true;
    }

    // Current simulating arrivals with a generic, pre-initialized distribution
    // Uses a mersenne twister for generating random numbers from the distribution
    template<typename T> void SimulteQueueArrival(T distribution, double time) {
        static std::mt19937 generator{std::random_device{}()};
        double t = 0;
        while (t < time) {
            double dt = distribution(generator);
            t += dt;
            Arrival(Vehicle(4.18 * (1 + (0.2f * rand() / RAND_MAX - 0.1)), 1.72f * (1 + (0.2f * rand() / RAND_MAX - 0.1)), 1265 * (1 + (0.2 * rand() / RAND_MAX - 0.1)), t)); // defaulted to a Yaris Cross because no better data
            vehicles_in_yard++;
        }
    }

    // Runs the embarking by calling operator() on the selected algorithm
    void Embark(Ferry& f) {
        algorithm(f, *this);
    }

    //void Test4Cars();
};

//void WorstFitAlgorithm(Ferry& ferry, Yard& yard);

#endif //EIT_PRELOADINGQUEUE_H
