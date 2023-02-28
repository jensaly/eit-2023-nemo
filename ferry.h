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

    Ferry(int queues, double q_height, double q_width, double width, double height);
    void FindCOM();
    void clear();
};

#endif //EIT_FERRY_H
