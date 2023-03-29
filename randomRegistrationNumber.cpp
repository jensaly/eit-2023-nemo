//
// Created by Tormod Mork Muller on 29/03/2023.
//

#include "randomRegistrationNumber.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

/**
 * Retrieves a random licence plate number from the list of licence plates.
 * List is found in cmake-build-debug inside licence_plates.txt
 * @param filename The name of the licence plate txt file
 * @return Returns the randomly selected licence plate number as a string
 */
string GetRandomRegistrationNumber(const string& filename)
{
    // Open the file in read mode
    ifstream file(filename);

    // Check if the file is opened successfully
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    // Count the number of lines in the file
    int count = 0;
    string line;
    while (getline(file, line)) {
        count++;
    }

    // Check if the file is empty
    if (count == 0) {
        cerr << "Error: File " << filename << " is empty" << endl;
        exit(1);
    }

    // Generate a random number
    srand(time(0));
    int random_number = rand() % count;

    /*
    // Debug print statement
    cout << "Number of lines in file: " << count << endl;
    cout << "Random number generated: " << random_number << endl;
    */

    // Iterate through the file until you reach the line corresponding to the random number
    file.clear();
    file.seekg(0, ios::beg);
    for (int i = 0; i < random_number; i++)
    {
        getline(file, line);
    }

    // Extract and return the registration number from that line
    string registration_number = line;

    // Close the file
    file.close();

    return registration_number;
}