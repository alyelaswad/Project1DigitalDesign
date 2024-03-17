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

class CircuitReader
{
private:
    vector<gate> gatesdict;
    vector<gate> cir_gates;
    vector<bool> inputs;

public:
    void accessLibFile(const string &pathname);             // Function to read the .lib file
    void accessCirFile(string pathname);                    // Function to read the .cir file
    bool getOutput(bool A, bool B, const string &gatetype); // Function to get output of gates with 2 inputs
    bool getOutput(bool A, const string &gatetype);         // Function to get output of gates with 1 input
    void readvectorgate();
};
