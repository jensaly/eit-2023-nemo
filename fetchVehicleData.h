//
// Created by Tormod Mork Muller on 22/02/2023.
//

#ifndef EIT_FETCHVEHICLEDATA_H
#define EIT_FETCHVEHICLEDATA_H

#include <curl/curl.h>
#include <iostream>
#include <string>

/**
 * A vehicle struct for the returned vehicle data from the API
 * Can be swapped with the Vehicles struct if needed
 */
struct API_Vehicle {
    std::string kjennemerke;
    int lengde{};
    int bredde{};
    int vekt{};
    std::string motor;
};

/**
 * Method that makes the API call to fetch vehicle data about the vehicle with licence plate number car_ID
 * @param car_ID The licence plate number of the vehicle of interest
 * @return The vehicle object with the needed vehicle data
 */
API_Vehicle Make_curl_request(const std::string& car_ID);

#endif //EIT_FETCHVEHICLEDATA_H
