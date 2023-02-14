#ifndef EIT_ALGORITHMS_H
#define EIT_ALGORITHMS_H

#include "preloadingqueue.h"

/* Worst fit algoritm: Finds the bins with most available space and placed item there
 * Bad algorithm, should not be used, but illustrative
 */
struct WorstFit {
    void operator()(Ferry& ferry, Yard<WorstFit>& yard) {
        // Find first available car
        while (true) {
            auto q = find_if(yard.queues.begin(), yard.queues.end(), [&](auto &queue) { return queue.total_vehicles > 0; });
            if (q == yard.queues.end()) {
                break;
            }
            auto v = std::min_element(q->vehicles.begin(), q->vehicles.end(), [&](auto &veh1, auto &veh2) { return veh1.length < veh2.length; });
            auto min_q = q;
            for (; q != yard.queues.end(); q++) {
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
};

#endif //EIT_ALGORITHMS_H
