#include "fetchVehicleData.h"
#include <curl/curl.h>
#include "iostream"
#include <fstream>
#include "filesystem"
#include "json.hpp"

using json = nlohmann::json;

struct Vehicle {
    std::string kjennemerke;
    int lengde{};
    int bredde{};
    int vekt{};
    std::string motor;
    std::string drivstofftype;
};

// This function will be called by curl to handle the response
static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* stream) {
    size_t bytes = size * nmemb;
    stream->append(ptr, bytes);
    return bytes;
}

void Make_curl_request(const std::string& car_ID) {
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
    //std::string api_key = "2791f35c-81dc-4c2a-b21c-21f7b26e861e";
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

            Vehicle vehicle;
            vehicle.kjennemerke = j["kjoretoydataListe"][0]["kjoretoyId"]["kjennemerke"].get<std::string>();
            vehicle.lengde = j["kjoretoydataListe"][0]["godkjenning"]["tekniskGodkjenning"]["tekniskeData"]["dimensjoner"]["lengde"].get<int>();
            vehicle.bredde = j["kjoretoydataListe"][0]["godkjenning"]["tekniskGodkjenning"]["tekniskeData"]["dimensjoner"]["bredde"].get<int>();
            vehicle.vekt = j["kjoretoydataListe"][0]["godkjenning"]["tekniskGodkjenning"]["tekniskeData"]["vekter"]["egenvekt"].get<int>();
            vehicle.motor = j["kjoretoydataListe"][0]["godkjenning"]["tekniskGodkjenning"]["tekniskeData"]["motorOgDrivverk"]["motor"][0]["drivstoff"][0]["drivstoffKode"]["kodeNavn"].get<std::string>();

            // Print the data in the Vehicle object
            std::cout << "Kjennemerke: " << vehicle.kjennemerke << std::endl;
            std::cout << "Lengde: " << vehicle.lengde << " cm" << std::endl;
            std::cout << "Bredde: " << vehicle.bredde << " cm" << std::endl;
            std::cout << "Vekt: " << vehicle.vekt << " kg" << std::endl;
            std::cout << "Motor: " << vehicle.motor << std::endl;

            // Print the response
            std::cout << response_string << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}