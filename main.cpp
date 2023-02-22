#include "yard.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <curl/curl.h>


int main() {
    srand(time(NULL));
    Yard y = Yard(DoNothing(), DoNothing(), 5, 60, 4);
    y.queues[0].SetReservedFlag(VehicleFlags::Ambulance);
    y.queues[0].SetReservedFlag(VehicleFlags::HC);
    y.queues[2].SetPriorityFlag(VehicleFlags::Heavy);
    Ferry ferry{5, 17.8, 1.9, 5 * 1.9, 18.0};

    CURL *curl;
    CURLcode res;
    curl_global_cleanup();
}
