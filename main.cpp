#include "preloadingqueue.h"

int main() {
    std::cout << "Ferry goes brrrrr";
    Vehicle(12, 12, 12);
    std::vector<Vehicle> vehicles;
    vehicles.emplace_back(12, 12, 12);
    vehicles.emplace_back(12, 12, 12);
    vehicles.emplace_back(12, 12, 12);

    auto y = Yard(7, 17.0, 4.0);
    y.GenerateSetOfVehicles(100);
}
