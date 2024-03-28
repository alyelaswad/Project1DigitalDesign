#pragma once
#include <vector>
#include <map>
#include <unordered_map>
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

struct IntermediateValue
{
    int timestamp;
    string variable;
    bool value;
    bool operator<(const IntermediateValue &other) const
    {
        if (timestamp != other.timestamp)
        {
            return timestamp < other.timestamp;
        }
        if (variable != other.variable)
        {
            return variable < other.variable;
        }
        return value < other.value;
    }
    IntermediateValue() {}
    IntermediateValue(int ts, const string &var, bool val) : timestamp(ts), variable(var), value(val) {}
};

class CircuitReader
{
private:
    vector<gate> gatesdict;
    vector<string> inputs;
    vector<bool> current_values;
    vector<IntermediateValue> intermediateValues;
    std::unordered_map<std::string, bool> previousValues;

public:
    vector<IntermediateValue> dataVector;
    vector<gate> cir_gates;
    void accessLibFile(const string &pathname);           // Function to read the .lib file
    void accessCirFile(string pathname);                  // Function to read the .cir file
    void accessStimFile(string pathname);                 // Function to read the .stim file
    bool getOutput(vector<bool> inputs, string gatename); // Function to get output of gates with n number inputs
    void SimulateProgram(string pathname);                // Function to Sim Program and Output in File
    void compute_circuit(int timestamp);                  // Function to compute circuit depending on each timestamp
    bool getPreviousValue(const std::string &variable)
    {
        if (previousValues.find(variable) != previousValues.end())
        {  return previousValues[variable];}
          else
        { // If previous value not found, return default value (false)
            return false; }
    }

    // Function to update previous value of an output variable
    void updatePreviousValue(const std::string &variable, bool value){previousValues[variable] = value;}
};
