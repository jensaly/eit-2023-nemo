#ifndef EIT_FERRY_H
#define EIT_FERRY_H

#include "queue.h"

struct Ferry {
    std::vector<Queue> queues;
    double width;
    double length;
    std::pair<float, float> car_com{0,0};
    std::pair<float, float> com{0,0};
    int total_vehicles = 0;

    Ferry(int num_queues, double q_length, double q_width, double width, double length);
    void FindCOM();
    void clear();
};

#endif //EIT_FERRY_H
