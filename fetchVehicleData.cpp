#include "fetchVehicleData.h"
#include <curl/curl.h>
#include "iostream"
#include <fstream>
#include "filesystem"
#include "json.hpp"

using json = nlohmann::json;

/**
 * This function will be called by curl to handle the response
 * @param ptr Pointer to a buffer that contains the data that was received from the HTTP request.
 * @param size The size of each data element in the buffer (char in this case).
 * @param nmemb The number of data elements in the buffer (the number of char elements in the buffer).
 * @param stream Pointer to a string object that will be used to store the received data. The callback function appends the received data to this string
 * @return
 */
static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* stream) {
    size_t bytes = size * nmemb;
    stream->append(ptr, bytes);
    return bytes;
}

/**
 * Method that makes the API call to fetch vehicle data about the vehicle with licence plate number car_ID
 * @param car_ID The licence plate number of the vehicle of interest
 * @return The vehicle object with the needed vehicle data
 */
API_Vehicle Make_curl_request(const std::string& car_ID) {
    // Vehicle object that will be used to store vehicle data
    API_Vehicle vehicle;

    // Reads the API key
    std::ifstream credentialsFile("../config/api_key.txt");
    std::cout << std::filesystem::current_path() << std::endl;
    std::string api_key;
    if (credentialsFile.is_open()) {
        std::getline(credentialsFile, api_key);
        credentialsFile.close();
    } else {
        std::cout << "Unable to open credentials file\n";
    }

    CURL* curl = curl_easy_init();
    curl_slist* headers = nullptr;
    // URL to the Statens Vegvesen API (without the licence plate of interest)
    std::string base_url = "https://www.vegvesen.no/ws/no/vegvesen/kjoretoy/felles/datautlevering/enkeltoppslag/kjoretoydata?kjennemerke=";

    if (curl) {
        // Set the URL to request
        std::string url = base_url + car_ID;
        std::cout << url << "\n";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to handle the response
        std::string response_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        // Add the SVV-Authorization header with the API key
        std::string authorization_header = "SVV-Authorization: " + api_key;
        headers = curl_slist_append(headers, authorization_header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            json j = json::parse(response_string);

            // Saves the response data of interest into the vehicle object
            vehicle.kjennemerke = j["kjoretoydataListe"][0]["kjoretoyId"]["kjennemerke"].get<std::string>();
            vehicle.lengde = j["kjoretoydataListe"][0]["godkjenning"]["tekniskGodkjenning"]["tekniskeData"]["dimensjoner"]["lengde"].get<int>();
            vehicle.bredde = j["kjoretoydataListe"][0]["godkjenning"]["tekniskGodkjenning"]["tekniskeData"]["dimensjoner"]["bredde"].get<int>();
            vehicle.vekt = j["kjoretoydataListe"][0]["godkjenning"]["tekniskGodkjenning"]["tekniskeData"]["vekter"]["egenvekt"].get<int>();
            vehicle.motor = j["kjoretoydataListe"][0]["godkjenning"]["tekniskGodkjenning"]["tekniskeData"]["motorOgDrivverk"]["motor"][0]["drivstoff"][0]["drivstoffKode"]["kodeNavn"].get<std::string>();

            /*
            // Print the data in the Vehicle object
            std::cout << "Kjennemerke: " << vehicle.kjennemerke << std::endl;
            std::cout << "Lengde: " << vehicle.lengde << " cm" << std::endl;
            std::cout << "Bredde: " << vehicle.bredde << " cm" << std::endl;
            std::cout << "Vekt: " << vehicle.vekt << " kg" << std::endl;
            std::cout << "Motor: " << vehicle.motor << std::endl;

            // Print the response
            std::cout << response_string << std::endl;*/
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    return vehicle;
}