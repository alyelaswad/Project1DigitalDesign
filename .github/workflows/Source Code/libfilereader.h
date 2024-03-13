#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "LogicGates.h" 
using namespace std;
struct gate {
    string name;
    int inputnums;
    string expression;
    int delayofgate;
};

class libfilereader {
private:
    vector<gate> gatesdict;

public:
   void accesslibfile(string);
 /* void readvectorgate();*/
};

