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
    LogicGates g;
    bool output(bool A, bool B) {
        if (name == "NAND2") {
            return g.NAND(A, B);
        }
        else if (name == "NOT") {
            return g.NOT(A);
        }
        else if (name == "AND2") {
            return g.AND(A, B);
        }
        else if (name == "OR2") {
            return g.OR(A, B);
        }
        else if (name == "NOR2") {
            return g.NOR(A, B);
        }

    }
   
    vector <string> inputs;
};


class simulator {
private:
    vector<gate> gatesdict;

public:
  void accesslibfile(string);
  void readvectorgate();
  bool getouput(bool , bool , string);
  bool getouput(bool, string);
};

