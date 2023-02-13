#ifndef EIT_PRELOADINGQUEUE_H
#define EIT_PRELOADINGQUEUE_H

#include "ferry.h"

struct Yard {
    Ferry ferry;
    std::vector<Queue> queues;
    std::deque<Vehicle> pre_yard;
    int vehicles_in_yard = 0;
    double mat = 60; // mean arrival time (seconds) for the pre_yard queue

    Yard(int number_of_queues, double queue_length, double queue_width);
    bool Arrival(Vehicle vehicle);
    void GenerateSetOfVehicles(uint16_t number);
    void SimulteQueueArrival(double time);

    void Embark();
    void WorstFitAlgorithm();

    void Test4Cars();
};

#endif //EIT_PRELOADINGQUEUE_H
