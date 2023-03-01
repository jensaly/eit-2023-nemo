//
// Created by Tormod Mork Muller on 22/02/2023.
//

#include "fetchVehicleData.h"
#include <curl/curl.h>
#include "iostream"

// This function will be called by curl to handle the response
static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* stream) {
    size_t bytes = size * nmemb;
    stream->append(ptr, bytes);
    return bytes;
}

void Make_curl_request() {
    CURL* curl = curl_easy_init();

    if (curl) {
        // Set the URL to request
        curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts");

        // Set the callback function to handle the response
        std::string response_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Print the response
            std::cout << response_string << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
    }
}
