#include "algorithms.h"
#include "ferry.h"
#include "filehandler.h"
#include "yard.h"

//typedef std::ranges::drop_view<std::ranges::ref_view<std::vector<Queue>>> vecrange;

void WorstFit::operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel) {
    Yard yard_cpy{yard};
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
        f_min_q->AddVehicleToQueue(std::move(*v), ferry);
        min_q->EraseVehicleFromQueue(v, yard);
    }
    ferry.FindCOM();
    fh.Write(ferry, yard_cpy);
}
// Coarse-sorting version of WorstFit algorithm, individual car arrival into queues
bool WorstFit::operator()(Yard& yard, Vehicle& vehicle) {
    auto& queues = yard.queues;
    auto& min_queue = *std::max_element(queues.begin(), queues.end(), [&](auto& a, auto& b){ return a.available_size < b.available_size;});
    if (min_queue.available_size < vehicle.length) {
        return false;
    }
    min_queue.AddVehicleToQueue(vehicle, yard);
    return true;
}

void BestFit::operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel) {
    Yard yard_cpy{yard};
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
        f_min_q->AddVehicleToQueue(std::move(*v), ferry);
        min_q->EraseVehicleFromQueue(v, yard);
    }
    ferry.FindCOM();
    fh.Write(ferry, yard_cpy);
}
// Coarse-sorting version of WorstFit algorithm, individual car arrival into queues
bool BestFit::operator()(Yard& yard, Vehicle& vehicle) {
    auto& queues = yard.queues;
    auto& min_queue = *std::max_element(queues.begin(), queues.end(), [&](auto& a, auto& b){ return a.available_size < b.available_size;});
    if (min_queue.available_size < vehicle.length) {
        return false;
    }
    min_queue.AddVehicleToQueue(vehicle, yard);
    return true;
}

/*
 * Ambulances have the highest priority, will all be boarded first
 */
void BasicRules::operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel) {
    Yard yard_cpy{yard};
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
                queue_weight.push({vehicle_weight, i});
            }
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
        f_min_q->AddVehicleToQueue(std::move(*v), ferry);
        min_q->EraseVehicleFromQueue(v, yard);
    }
    ferry.FindCOM();
    fh.Write(ferry, yard_cpy);
}

/*
 * Coarse sorting for BasicRules.
 * Queues are given 2 vector<bool>'s, reserved and priority
 * Reserved means the queue is only for one or more types of vehicle
 * Priority means the queue prefers certain types of vehicles
 * Vehicle characteristics are checked against these vector<bool>'s
 * Vehicles are prioritized for their reserved queues, and down-prioritized for non-reserved
 * Weight is a function of point score and available space in every queue.
 * A queue with 0 weight is open for all
 */
bool BasicRules::operator()(Yard& yard, Vehicle& vehicle) {
    auto& queues = yard.queues;
    std::priority_queue<std::pair<double, size_t>> queue_weight;
    for (int i = 0; i < queues.size(); i++) {
        auto& q = queues[i];
        double vehicle_weight = 1;
        /*
         * We check if a vehicle has reserved status in a queue
         * If it does, we give it a weighted sum.
         * If it does not, we deduct a sum.
         */
        int reserved_matches = 0; // we count the number of reserved matches
        int priority_matches = 0; // and priority matches
        for (int j = 0; j < q.reserved.size(); j++) {
            if (vehicle.characteristics[j] && q.reserved[j]) { // if the queue's reserved status matches the vehicle
                reserved_matches++;
            }
        }
        for (int j = 0; j < q.reserved.size(); j++) {
            if (vehicle.characteristics[j] && q.priority[j]) { // if the queue's priority status matches the vehicle
                priority_matches++;
            }
        }
        if (q.has_reserved) {
            vehicle_weight = (reserved_matches > 0) ? reserved_matches * 10000 : -10000; // if we have 1 or more reserved matches, that queue gets a large bonus proportional to number of matches, otherwise a large penalty
        }
        if (q.has_priority) {
            vehicle_weight = (priority_matches > 0) ? priority_matches : -1; // if we have 1 or more reserved matches, that queue gets a large bonus proportional to number of matches, otherwise a small penalty
        }
        vehicle_weight *= q.available_size / q.total_size;
        queue_weight.push(std::pair<double, size_t>(vehicle_weight, i));
    }
    auto best = queue_weight.top();
    auto best_queue = &queues[best.second];
    // Finds
    while (!best_queue->IsAvailableSpace(vehicle)) {
        if (queue_weight.empty()) {
            return false;
        }
        queue_weight.pop();
        best = queue_weight.top();
        best_queue = &queues[best.second];
    }
    best_queue->AddVehicleToQueue(vehicle, yard);
    return true;
}

struct FerryYardCombo {
    Ferry f;
    Yard y;
    FerryYardCombo(Ferry f, Yard y) : f{f}, y{y} {}
};

void LoadReservedVehicles(Ferry& f, Yard& y) {
    auto& queues = y.queues;
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
                queue_weight.push({vehicle_weight, i});
            }
        }
        if (queues[0].vehicles.empty()) {
            break;
        }
        auto min_q = queues.begin() + queue_weight.top().second;
        auto v = min_q->vehicles.begin();
        auto first_queue = std::find_if(f.queues.begin(), f.queues.end(), [&](const Queue& q1) { return q1.available_size > v->length; });
        if (first_queue == f.queues.end()) {
            // error, return
        }
        auto f_min_q = std::max_element(first_queue, f.queues.end(), [](const Queue& q1, const Queue& q2) { return q1.available_size < q2.available_size; });
        f_min_q->AddVehicleToQueue(std::move(*v), f);
        min_q->EraseVehicleFromQueue(v, y);
    }
}

double FerryObjectiveFunction(Ferry& ferry) {
    double x = ferry.car_com.first, y = ferry.car_com.second, x0 = ferry.com.first, y0 = ferry.com.second;
    return sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0));
}

void FindPresentBest(std::vector<FerryYardCombo>& fyc) {
    std::for_each(fyc.begin(), fyc.end(), [](auto& combo){combo.f.FindCOM();});
    auto it = std::max_element(fyc.begin(), fyc.end(), [](auto& combo1, auto& combo2){ return FerryObjectiveFunction(combo1.f) > FerryObjectiveFunction(combo2.f); });
    auto copy = *it;
    fyc.clear();
    fyc.push_back(std::move(copy));
}

/*
 * Fine-sorting for optimizing COM. This solution is basic at O(q^n), but I've implemented some
 * restrictions. First, it handles ambulances and other priority vehicles. Then, once every row is
 * filled, the best solution is picked and new solutions spring from it.
 */
void OptimizeCOM::operator()(Ferry& f, Yard& y, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel) {
    Yard yard_cpy{y};
    // Handling priority vehicles. This is similar to the solution presented in BasicRules
    LoadReservedVehicles(f, y);
    std::vector<FerryYardCombo> solutions_vector;
    solutions_vector.emplace_back(f, y);
    int i = 0;
    uint64_t bound = pow(y.queues.size(), f.queues.size());
    while (i < solutions_vector.size()) {
        auto fyc = solutions_vector[i]; // solution under consideration
        const auto yard = fyc.y; // yard under consideration
        const auto ferry = fyc.f; // ferry under consideration
        for (int j = 0; j < yard.queues.size(); j++) {
            for (int k = 0; k < ferry.queues.size(); k++) {
                if (yard.queues[j].vehicles.empty()) continue; // if there are no vehicles in this yard queue, move to next
                Yard y_copy = yard; //
                Ferry f_copy = ferry;
                auto yard_queue = y_copy.queues.begin() + j;
                auto v = yard_queue->vehicles.begin();
                auto y_c = yard_queue;
                auto f_c = f_copy.queues.begin() + k;
                f_c->AddVehicleToQueue(std::move(*v), f_copy);
                y_c->EraseVehicleFromQueue(v, y_copy);
                solutions_vector.push_back(std::move(FerryYardCombo{f_copy, y_copy}));
            }
        }
        i++;
        if (solutions_vector.size() > bound) {
            FindPresentBest(solutions_vector);
            i = 0;
            std::cout << solutions_vector.back().f.total_vehicles << "\n";
        }
    }
    FindPresentBest(solutions_vector);
    f = std::move(solutions_vector.back().f);
    y = std::move(solutions_vector.back().y);
    std::cout << "Finished" << "\n";
}

bool OptimizeCOM::operator()(Yard& yard, Vehicle& vehicle) {
    return false;
}

void LoadHighPriorityEvenly(Queue& q, Ferry& f, Yard& y) {
    while (!q.vehicles.empty()) {
        auto veh_it = q.vehicles.begin();
        auto min_q = std::max_element(f.queues.begin(), f.queues.end(), [&](const Queue& q1, const Queue q2){ return q1.available_size < q2.available_size; });
        auto& f_min_q = *min_q;
        q.EraseVehicleFromQueue(veh_it, y);
        min_q->AddVehicleToQueue(std::move(*veh_it), f);
    }
}

void ShiftCOMByShiftingCars(Ferry& f) {
    auto min_q = std::min_element(f.queues.begin(), f.queues.end(), [&](const Queue& q1, const Queue q2){ return q1.available_size < q2.available_size; });
    double min_q_dist = min_q->available_size;
    double com_x_dist = f.car_com.first - f.com.first;
    if (com_x_dist > 0) {
        return;
    }
    else {
        double x_shift = abs((min_q_dist < abs(com_x_dist)) ? min_q_dist : com_x_dist);
        for (auto &q: f.queues) {
            for (auto &v: q.vehicles) {
                v.x += x_shift;
            }
        }
        f.FindCOM();
    }
}

void WorstFitParallel::operator()(Ferry& ferry, Yard& yard, FileHandler& fh, std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel) {
    Yard yard_cpy{yard};
    // Ambulances override existing rules and load first, alone, and immediately to the first row, if possible.
    auto it = std::find_if(yard.queues.begin(), yard.queues.end(), [](const Queue& q){ return q.GetReservedFlag(VehicleFlags::Ambulance); });
    if (it != yard.queues.end()) {
        LoadHighPriorityEvenly(*it, ferry, yard);
    }
    if (y_f_parallel.empty()) {
        for (size_t i = 0; i < ferry.queues.size(); i++) {
            y_f_parallel.push_back({i,{i}});
        }
    }
    for (auto& y_f_combo : y_f_parallel) {
        if (y_f_combo.second.size() == 1) {
            auto &fq = ferry.queues[y_f_combo.second[0]];
            auto &yq = yard.queues[y_f_combo.first];
            //auto& fq_vehicles = fq.vehicles;
            auto &yq_vehicles = yq.vehicles;
            while (!yq_vehicles.empty()) {
                auto v_first = yq_vehicles.begin();
                fq.AddVehicleToQueue(std::move(*v_first), ferry);
                yq.EraseVehicleFromQueue(v_first, yard);
            }
        } else {
            auto &yq = yard.queues[y_f_combo.first];
            auto &yq_vehicles = yq.vehicles;
            while (!yq_vehicles.empty()) {
                std::vector<std::vector<Queue>::iterator> it_vec;
                for (size_t index : y_f_combo.second) {
                    it_vec.push_back(ferry.queues.begin() + index);
                }
                auto& fq = **std::max_element(it_vec.begin(), it_vec.end(), [&](const std::vector<Queue>::iterator& q1, const std::vector<Queue>::iterator q2){ return q1->available_size < q2->available_size; });
                auto v_first = yq_vehicles.begin();
                fq.AddVehicleToQueue(std::move(*v_first), ferry);
                yq.EraseVehicleFromQueue(v_first, yard);
            }
        }
    }
    ferry.FindCOM();
    ShiftCOMByShiftingCars(ferry);
    fh.Write(ferry, yard_cpy);
}
// Coarse-sorting version of WorstFit algorithm, individual car arrival into queues
bool WorstFitParallel::operator()(Yard& yard, Vehicle& vehicle) {
    return worstfit(yard, vehicle);
}