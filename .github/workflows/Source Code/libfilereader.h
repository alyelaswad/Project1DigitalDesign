#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stack>
#include <map>
#include <cstdarg>
#include "LogicGates.h" 
using namespace std;
struct gate {
    string name;
    int inputnums;
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


class libfilereader {
private:
    vector<gate> gatesdict;

public:
   void accesslibfile(string);
  void readvectorgate();
  bool getouput(bool A, bool B) {

      return gatesdict[2].output(A, B);
  }
  bool getouput(bool A) {

      return gatesdict[0].output(A,A);
  }
};

