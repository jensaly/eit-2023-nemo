#ifndef EIT_ALGORITHMS_H
#define EIT_ALGORITHMS_H

#include "yard.h"

/*
 * General structure of these algorithms:
 * void operator()(Ferry& ferry, std::vector<Queue>& queues, FileHandler& fh) -> fine-sorting algorithm
 * bool operator()(std::vector<Queue>& queues, Vehicle& vehicle) -> coarse-sorting algorithm
 */

/* Worst fit algoritm: Finds the bins with most available space and placed item there
 * Bad algorithm, should not be used, but illustrative
 */
struct WorstFit {
    // Fine-sorting version of WorstFit algorithm, from Yard queues to Ferry queues
    void operator()(Ferry& ferry, std::vector<Queue>& queues, FileHandler& fh) {
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
        ferry.FindCOM();
        fh.Write(ferry);
    }
    // Coarse-sorting version of WorstFit algorithm, individual car arrival into queues
    bool operator()(std::vector<Queue>& queues, Vehicle& vehicle) {
        auto& min_queue = *std::max_element(queues.begin(), queues.end(), [&](auto& a, auto& b){ return a.available_size < b.available_size;});
        if (min_queue.available_size < vehicle.length) {
            return false;
        }
        min_queue.AddVehicleToQueue(vehicle);
        return true;
    }
};

// Algorithm that does nothing, just for testing how the operator() overloading works
struct DoNothing {
    void operator()(Ferry& ferry, std::vector<Queue>& queues, FileHandler& fh) {

    }
    bool operator()(std::vector<Queue>& queues, Vehicle& vehicle) {
        return false;
    }
};


#endif //EIT_ALGORITHMS_H
