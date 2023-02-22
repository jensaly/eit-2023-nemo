#ifndef EIT_PRELOADINGQUEUE_H
#define EIT_PRELOADINGQUEUE_H

#include "filehandler.h"
#include "algorithms.h"

struct BaseAlgorithm;

struct Yard {
    std::vector<Queue> queues; // main yard area
    std::deque<Vehicle> pre_yard; // road leading up to yard
    int vehicles_in_yard = 0;
    std::unique_ptr<BaseAlgorithm> f_algorithm; // selected algorithm for fine sorting
    std::unique_ptr<BaseAlgorithm> c_algorithm; // selected algorithm for sorting arriving cars
    FileHandler fh{};

    template<class CoarseAlgorithm, class FineAlgorithm>
    Yard(CoarseAlgorithm, FineAlgorithm, int number_of_queues, double queue_length, double queue_width) : f_algorithm{std::make_unique<FineAlgorithm>()}, c_algorithm{std::make_unique<CoarseAlgorithm>()} {
        for (int i = 0; i < number_of_queues; i++) {
            queues.emplace_back(std::to_string(i), queue_length, queue_width, queue_width * i);
        }
    }

    // Function takes a vehicle and puts it into a Yard queue depending on a set of rules
    // At present, that rule is "whichever queue has the most size available)
    // If the arrival cannot be slotted into the yard, it is instead put in a single pre-queue
    void Arrival(Vehicle vehicle) {
        // TODO: Replace the code here with a scoring system
        if (!(*c_algorithm)(queues, vehicle)) {
            pre_yard.push_back(vehicle);
        }
    }

    // Current simulating arrivals with a generic, pre-initialized distribution
    // Uses a mersenne twister for generating random numbers from the distribution
    // Also uses a random generator for plate_nr at this time
    template<typename T> void SimulteQueueArrival(T distribution, double time)  {
        static std::mt19937 generator{std::random_device{}()};
        double t = 0;
        while (t < time) {
            double dt = distribution(generator);
            t += dt;
            std::string plate_nr = std::string(1, ('A' + rand()%26)) + std::string(1,('A' + rand()%26));
            plate_nr += std::to_string(rand() % 10) + std::to_string(rand() % 10) + std::to_string(rand() % 10) + std::to_string(rand() % 10) + std::to_string(rand() % 10);
            Arrival(Vehicle("B", plate_nr, 4.18 * (1 + (0.2f * rand() / RAND_MAX - 0.1)), 1.72f * (1 + (0.2f * rand() / RAND_MAX - 0.1)), 1265 * (1 + (0.2 * rand() / RAND_MAX - 0.1)), t)); // defaulted to a Yaris Cross because no better data
            vehicles_in_yard++;
        }
    }

    // Runs the embarking by calling operator() on the selected algorithm
    void Embark(Ferry& f) {
        (*f_algorithm)(f, queues, fh);
    }

    template<typename CoarseAlgorithm> void SetCoarseAlgorithm() {
        c_algorithm = std::make_unique<CoarseAlgorithm>();
    }
    template<typename FineAlgorithm> void SetFineAlgorithm() {
        f_algorithm = std::make_unique<FineAlgorithm>();
    }
};

#endif //EIT_PRELOADINGQUEUE_H
