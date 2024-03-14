#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
// #include "LogicGates.h" 
struct gate {
    std::string name;
    int delayofgate;
    std::string expression;
    int inputnums;
    vector <string> inputs;
    std::string output;
    std::string type; 
};

class libfilereader {
private:
    vector<gate> gatesdict;

public:
   void accesslibfile(std::string pathname);
   void readvectorgate();
   vector<gate> get_gatesdict();
};