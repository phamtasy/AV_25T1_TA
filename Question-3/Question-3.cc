#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// Question 3: This is an extension task that requires you to decode sensor data from a CAN log file.
// CAN (Controller Area Network) is a communication standard used in automotive applications (including Redback cars)
// to allow communication between sensors and controllers.
//
// Your Task: Using the definition in the Sensors.dbc file, extract the "WheelSpeedRR" values
// from the candump.log file. Parse these values correctly and store them in an output.txt file with the following format:
// (<UNIX_TIME>): <DECODED_VALUE>
// eg:
// (1705638753.913408): 1234.5
// (1705638754.915609): 6789.0
// ...
// The above values are not real numbers; they are only there to show the expected data output format.
// You do not need to use any external libraries. Use the resources below to understand how to extract sensor data.
// Hint: Think about manual bit masking and shifting, data types required,
// what formats are used to represent values, etc.
// Resources:
// https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial
// https://www.csselectronics.com/pages/can-dbc-file-database-intro

// WheelSpeedRR value
// SG_ WheelSpeedRR : 32|16@0+ (0.1,0) [0|0] "km/h" Vector__XXX
// Bit start at 32, length 16 (bits 32 - 47), scale 0.1, offset 0, min 0, max 0, unit km/h
// Data is in big-endian format
// BO_ 1797 ECU_WheelSpeed: 8 Vector__XXX
// can ID is 1797

int main() {
    // Open files
    ofstream output_file("../Question-3/output.txt");
    fstream input_file("../Question-3/candump.log");
    string line;

    // Loop through the input file
    while (getline(input_file, line)) {
        // Extract timestamp, interface, can_id, and data from the line
        istringstream iss(line);
        string timestamp, interface, can_id, data;
        iss >> timestamp >> interface >> data;

        // Split can_id and data
        size_t hash_pos = data.find("#");
        if (hash_pos == string::npos) {
            continue;
        }

        can_id = data.substr(0, hash_pos);
        data = data.substr(hash_pos + 1);

        // Convert can_id to decimal and filter out for correct can_id (1797)
        if (stoul(can_id, nullptr, 16) != 1797) {
            continue;
        }

        // Convert data to decimal and extract WheelSpeedRR value (data is in big-endian format)
        unsigned int wheelSpeedRR = std::stoul(data.substr(8, 4), nullptr, 16);

        output_file << timestamp << ": " << wheelSpeedRR * 0.1 << endl;
    }

    // Close files
    input_file.close();
    output_file.close();

    return 0;
}