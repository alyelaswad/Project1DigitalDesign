#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;


class libfilereader
{ private:
	vector<gate> gatesdict;
public:
	void accesslibfile(string pathname);
};
struct gate {
    string name;
    int delayofgate;
    int inputnums;
    vector<bool> inputs;
    bool output;
    bool AND(bool A, bool B) {
        return A && B;
    }
    bool OR(bool A, bool B) {
        return A || B;
    }
    bool NOT(bool A) {
        return !A;
    }
    bool NAND(bool A, bool B) {
        return NOT(AND(A, B));
    }
    void calculateOutput() {
        if (name == "AND" && inputnums == 2) {
            output = AND(inputs[0], inputs[1]);
        }
        else if (name == "OR" && inputnums == 2) {
            output = OR(inputs[0], inputs[1]);
        }
        else if (name == "NOT" && inputnums == 1) {
            output = NOT(inputs[0]);
        }
        else if (name == "NAND" && inputnums == 2) {
            output = NAND(inputs[0], inputs[1]);
        }
        else {
           
            cout<< "Unsupported gate or invalid input for gate: " << name << endl;
        }
    }
};

