//
// Created by Tormod Mork Muller on 22/02/2023.
//

#ifndef EIT_FETCHVEHICLEDATA_H
#define EIT_FETCHVEHICLEDATA_H

#include <curl/curl.h>
#include <iostream>
#include <string>

void Make_curl_request(const std::string& car_ID);

#endif //EIT_FETCHVEHICLEDATA_H
