#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#ifndef EIT_RANDOMREGISTRATIONNUMBER_H
#define EIT_RANDOMREGISTRATIONNUMBER_H

using namespace std;

/**
 * Retrieves a random licence plate number from the list of licence plates.
 * List is found in cmake-build-debug inside licence_plates.txt
 * @param filename The name of the licence plate txt file
 * @return Returns the randomly selected licence plate number as a string
 */
string GetRandomRegistrationNumber(const string& filename);

#endif //EIT_RANDOMREGISTRATIONNUMBER_H