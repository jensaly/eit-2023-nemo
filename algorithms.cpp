#include "algorithms.h"
#include "ferry.h"
#include "filehandler.h"
#include "yard.h"

void WorstFit::operator()(Ferry& ferry, Yard& yard, FileHandler& fh) {
    auto& queues = yard.queues;
    while (true) {
        // Find first available car, if no cars available all are sorted
        auto q = find_if(queues.begin(), queues.end(), [&](auto &queue) { return queue.total_vehicles > 0; });
        if (q == queues.end()) {
            break;
        }
        auto v = std::min_element(q->vehicles.begin(), q->vehicles.end(), [&](auto &veh1, auto &veh2) { return veh1.length < veh2.length; });
        auto min_q = q;
        // Find the absolute smallest car in the whole yard
        for (; q != queues.end(); q++) {
            auto loc_min_v = std::min_element(q->vehicles.begin(), q->vehicles.end(),[&](auto &a, auto &b) { return a.length < b.length; });
            if (loc_min_v != q->vehicles.end() && loc_min_v->length < v->length) {
                v = loc_min_v;
                min_q = q;
            }
        }
        // Find queue with most available space, add vehicle to that queue
        auto first_queue = std::find_if(ferry.queues.begin(), ferry.queues.end(), [&](const Queue& q1) { return q1.available_size > v->length; });
        if (first_queue == ferry.queues.end()) {
            // error, return
        }
        auto f_min_q = std::max_element(first_queue, ferry.queues.end(), [](const Queue& q1, const Queue& q2) { return q1.available_size < q2.available_size; });
        f_min_q->AddVehicleToQueue(std::move(*v));
        min_q->EraseVehicleFromQueue(v);
    }
    ferry.FindCOM();
    fh.Write(ferry);
}
// Coarse-sorting version of WorstFit algorithm, individual car arrival into queues
bool WorstFit::operator()(Yard& yard, Vehicle& vehicle) {
    auto& queues = yard.queues;
    auto& min_queue = *std::max_element(queues.begin(), queues.end(), [&](auto& a, auto& b){ return a.available_size < b.available_size;});
    if (min_queue.available_size < vehicle.length) {
        return false;
    }
    min_queue.AddVehicleToQueue(vehicle);
    return true;
}

void BestFit::operator()(Ferry& ferry, Yard& yard, FileHandler& fh) {
    auto& queues = yard.queues;
    while (true) {
        // Find first available car, if no cars available all are sorted
        auto q = find_if(queues.begin(), queues.end(), [&](auto &queue) { return queue.total_vehicles > 0; });
        if (q == queues.end()) {
            break;
        }
        auto v = std::min_element(q->vehicles.begin(), q->vehicles.end(), [&](auto &veh1, auto &veh2) { return veh1.length < veh2.length; });
        auto min_q = q;
        // Find the absolute smallest car in the whole yard
        for (; q != queues.end(); q++) {
            auto loc_min_v = std::min_element(q->vehicles.begin(), q->vehicles.end(),[&](auto &a, auto &b) { return a.length < b.length; });
            if (loc_min_v != q->vehicles.end() && loc_min_v->length < v->length) {
                v = loc_min_v;
                min_q = q;
            }
        }
        // Find queue with most available space, add vehicle to that queue
        auto first_queue = std::find_if(ferry.queues.begin(), ferry.queues.end(), [&](const Queue& q1) { return q1.available_size > v->length; });
        if (first_queue == ferry.queues.end()) {
            // error, return
        }
        auto f_min_q = std::min_element(first_queue, ferry.queues.end(), [](const Queue& q1, const Queue& q2) { return q1.available_size < q2.available_size; });
        f_min_q->AddVehicleToQueue(std::move(*v));
        min_q->EraseVehicleFromQueue(v);
    }
    ferry.FindCOM();
    fh.Write(ferry);
}
// Coarse-sorting version of WorstFit algorithm, individual car arrival into queues
bool BestFit::operator()(Yard& yard, Vehicle& vehicle) {
    auto& queues = yard.queues;
    auto& min_queue = *std::max_element(queues.begin(), queues.end(), [&](auto& a, auto& b){ return a.available_size < b.available_size;});
    if (min_queue.available_size < vehicle.length) {
        return false;
    }
    min_queue.AddVehicleToQueue(vehicle);
    return true;
}

/*
 * Ambulances have highest priority, will all be boarded first
 */
void BasicRules::operator()(Ferry& ferry, Yard& yard, FileHandler& fh) {
    auto& queues = yard.queues;
    while (true) {
        std::priority_queue<std::pair<int64_t, size_t>> queue_weight;
        for (int i = 0; i < queues.size(); i++) {
            int64_t vehicle_weight = 0;
            auto& q = queues[i];
            if (!q.vehicles.empty()) {
                auto& v = q.vehicles.front();
                if (v.GetFlag(VehicleFlags::Ambulance)) {
                    vehicle_weight += 10000;
                }
            }
            queue_weight.push({vehicle_weight, i});
        }
        if (queue_weight.empty()) {
            break;
        }
        auto min_q = queues.begin() + queue_weight.top().second;
        auto v = min_q->vehicles.begin();
        auto first_queue = std::find_if(ferry.queues.begin(), ferry.queues.end(), [&](const Queue& q1) { return q1.available_size > v->length; });
        if (first_queue == ferry.queues.end()) {
            // error, return
        }
        auto f_min_q = std::max_element(first_queue, ferry.queues.end(), [](const Queue& q1, const Queue& q2) { return q1.available_size < q2.available_size; });
        f_min_q->AddVehicleToQueue(std::move(*v));
        min_q->EraseVehicleFromQueue(v);
    }
}

/*
 * Coarse sorting for BasicRules.
 * Queues are given 2 vector<bool>'s, reserved and priority
 * Reserved means the queue is only for one or more types of vehicle
 * Priority means the queue prefers certain types of vehicles
 * Vehicle characteristics are checked against these vector<bool>'s
 * Vehicles are prioritized for their reserved queues, and down-prioritized for non-reserved
 * A queue with 0 weight is open for all
 */
bool BasicRules::operator()(Yard& yard, Vehicle& vehicle) {
    auto& queues = yard.queues;
    std::priority_queue<std::pair<int64_t, size_t>> queue_weight;
    for (int i = 0; i < queues.size(); i++) {
        auto& q = queues[i];
        int64_t vehicle_weight = 2;
        for (int j = 0; j < q.reserved.size(); j++) {
            if (vehicle.characteristics[j] && q.reserved[j]) {
                vehicle_weight += 10000;
            }
            else if (!vehicle.characteristics[j] && q.reserved[j]) {
                vehicle_weight -= 10000;
            }

            if (vehicle.characteristics[j] && q.priority[j]) {
                vehicle_weight += 3;
            }
            else if (!vehicle.characteristics[j] && q.priority[j]) {
                vehicle_weight += 1;
            }
        }
        queue_weight.push(std::pair<uint64_t, size_t>(vehicle_weight, i));
    }
    auto best = queue_weight.top();
    auto best_queue = &queues[best.second];
    // Finds 
    while (!best_queue->IsAvailableSpace(vehicle)) {
        queue_weight.pop();
        best = queue_weight.top();
        best_queue = &queues[best.second];
    }
    // If the best queue has a negative value, it is reserved and thus we cannot enter it.
    if (best.first < 0) {
        return false;
    }
    best_queue->AddVehicleToQueue(vehicle);
    return false;
}