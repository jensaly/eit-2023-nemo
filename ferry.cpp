#include "ferry.h"

// Calculates COM as the middle of any given vehicle
// Not a good approximation for trucks, prudent to update
// if able
void Ferry::FindCOM() {
    float x_cm = 0, y_cm = 0, total_mass = 0;
    for (auto& q : queues) {
        for (auto& v : q.vehicles) {
            x_cm += v.mass * (v.x + v.length / 2);
            y_cm += v.mass * (v.y + v.width / 2);
            total_mass += v.mass;
        }
    }
    car_com = {x_cm / total_mass, y_cm / total_mass};
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
    com = {length / 2, width / 2};
}

void Ferry::clear() {
    for (auto& q : queues) q.clear();
    car_com = {0,0};
}