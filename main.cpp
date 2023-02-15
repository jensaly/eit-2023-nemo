#include "algorithms.h"

int main() {
    Yard y = Yard<WorstFit>(5, 60, 4);
    Ferry ferry{5, 17.8, 1.9, 5 * 1.9, 18.0};
    y.SimulteQueueArrival(std::gamma_distribution<double>(1.4, 1.5), 30);
    y.Embark(ferry);
}
