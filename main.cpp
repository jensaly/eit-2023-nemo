#include "yard.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include "fetchVehicleData.h"
#include "randomRegistrationNumber.h"

int main() {
    srand(time(NULL));
    Yard y = Yard(BasicRules(), BasicRules(), 5, 60, 4);
    y.queues[0].SetReservedFlag(VehicleFlags::Ambulance);
    y.queues[0].SetReservedFlag(VehicleFlags::HC);
    y.queues[2].SetPriorityFlag(VehicleFlags::Heavy);
    Ferry ferry{5, 17.8, 1.9, 5 * 1.9, 18.0};
    static std::vector<std::pair<size_t, std::vector<size_t>>> y_f_parallel;
    if (y_f_parallel.empty()) {
        for (size_t i = 0; i < ferry.queues.size(); i++) {
            y_f_parallel.push_back({i,{i}});
        }
        y_f_parallel[1].second.push_back(0);
    }
    y.SimulateQueueArrival(std::gamma_distribution(1.4, 1.5), 60);
    y.Embark(ferry, y_f_parallel);

    // Get a random registration number and store it in a variable
    string car_ID = GetRandomRegistrationNumber("licence_plates.txt");

    // Method for calling API for vehicle data
    API_Vehicle apiVehicle = Make_curl_request(car_ID);

    // Prints the data of the vehicle from the API
    cout << "Kjennemerke: " << apiVehicle.kjennemerke << "\n";
    cout << "Lengde: " << apiVehicle.lengde << "\n";
    cout << "Bredde: " << apiVehicle.bredde << "\n";
    cout << "Vekt: " << apiVehicle.vekt << "\n";
    cout << "Motor: " << apiVehicle.motor << "\n";

}