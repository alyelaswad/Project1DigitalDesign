#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
// #include "LogicGates.h" 
struct gate {
    std::string type; // Type of the gate ; for example: NAND2,AND2,NOT1,..... 
    int delayofgate; // Delay of the gate
    std::string expression; // The expression that defines the function of the gate
    int inputnums; // the number of inputs the gate receives
    vector <string> inputs; // Vector of the names of the inputs
    std::string output; // The name of the output 
    std::string name; // name of the gate ; for example G0,G1,G2,..... 
};

class libfilereader {
private:
    vector<gate> gatesdict; // Vector of the gates  

public:
   void accesslibfile(std::string pathname); // Function that reads the .lib file
   void readvectorgate(); // A function that prints the gates vector
   vector<gate> get_gatesdict(); // A getter for the gatesdict
};
