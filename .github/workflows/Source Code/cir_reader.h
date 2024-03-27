#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "LogicGates.h"

using namespace std;

struct gate
{
    std::string type;       // Type of the gate ; for example: NAND2,AND2,NOT,.....
    int inputnums;          // the number of inputs the gate receives
    std::string expression; // The expression that defines the function of the gate
    int delayofgate;        // Delay of the gate
    vector<string> inputs;  // Vector of the names of the inputs
    LogicGates g;
    string output;

    bool outputfc(vector<bool> inputss, string type, vector<gate> gatesdict);
};
struct Data // stores data taken from stim file
{
    int timestamp;
    string variable;
    bool value;
};

struct IntermediateValue
{
    int timestamp;
    string variable;
    bool value;

    IntermediateValue(int ts, const string &var, bool val) : timestamp(ts), variable(var), value(val) {}
};

class CircuitReader
{
private:
    vector<gate> gatesdict;
    vector<string> inputs;
    vector<bool> current_values;
    vector<IntermediateValue> intermediateValues;
    map<string, bool> previous_values;

public:
    vector<Data> dataVector;
    vector<gate> cir_gates;
    void accessLibFile(const string &pathname);           // Function to read the .lib file
    void accessCirFile(string pathname);                  // Function to read the .cir file
    void accessStimFile(string pathname);                 // Function to read the .stim file
    bool getOutput(vector<bool> inputs, string gatename); // Function to get output of gates with n number inputs
    void SimulateProgram(string pathname);                // Function to Sim Program and Output in File
    void compute_circuit(int timestamp);                  // Function to compute circuit depending on each timestamp
    void clearIntermediateValues()
    {
        intermediateValues.clear(); // Clearing the intermediate values vector to prevent memory corruption
    }

    void clearPreviousValues()
    {
        previous_values.clear(); // Clearing the previous values map to prevent memory corruption
    }

    void clearCurrentValues()
    {
        current_values.clear(); // Clearing the current values vector to prevent memory corruption
    }

    void clearCircuitGates()
    {
        cir_gates.clear(); // Clearing the circuit gates vector to prevent memory corruption
    }

    void clearGatesDict()
    {
        gatesdict.clear(); // Clearing the gates dictionary vector to prevent memory corruption
    }
};
