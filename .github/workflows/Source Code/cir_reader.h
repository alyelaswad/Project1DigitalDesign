#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "LogicGates.h"

using namespace std;

struct gate
{
    std::string type;       // Type of the gate ; for example: NAND2,AND2,NOT1,.....
    int inputnums;          // the number of inputs the gate receives
    std::string expression; // The expression that defines the function of the gate
    int delayofgate;        // Delay of the gate
    vector<string> inputs;  // Vector of the names of the inputs
    LogicGates g;
    string output;

    bool outputfc(bool A, bool B);
};
struct Data // stores data taken from stim file
{
    int timestamp;
    string variable;
    bool value;
};

class CircuitReader
{
private:
    vector<gate> gatesdict;
    vector<gate> cir_gates;
    vector<Data> dataVector;
    vector<string> inputs;
    vector <bool> current_values;

public:
    void accessLibFile(const string &pathname);             // Function to read the .lib file
    void accessCirFile(string pathname);                    // Function to read the .cir file
    void accessStimFile(string pathname);                   // Function to read the .stim file
    bool getOutput(bool A, bool B, const string &gatetype); // Function to get output of gates with 2 inputs
    bool getOutput(bool A, const string &gatetype);         // Function to get output of gates with 1 input
    void SimulateProgram(string OutputPath);
    void compute_circuit();
    void readvectorgate();
};
