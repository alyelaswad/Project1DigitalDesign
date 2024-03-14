#include "simulator.h"
using namespace std;

void simulator::accesslibfile( string pathname) {
    ifstream inFile;
    inFile.open(pathname);
    int k = 0;
    string line;
    while (getline(inFile, line))
    {
        for (int i = 0;i < line.size();i++) {
            if (line[i] == ' ')
            {
                line.erase(i,1);
            }
    }
         stringstream ss(line);
        string name, expression, delay, inputnums;
        getline(ss, name, ',');
        getline(ss, inputnums, ',');
        getline(ss, expression, ',');
        ss >> delay;
       
        gate g = { name, stoi(inputnums),expression,stoi(delay) };
        gatesdict.push_back(g);
    }
}
bool simulator::getouput(bool A, bool B, string gatename) {
    int foundat = -9999;
    for (int i = 0;i < gatesdict.size();i++)
    {
        if (gatesdict[i].name == gatename) {
            foundat = i;
        }
    }
    if (foundat == -9999) {
        cout << "Gate not found";
        return 0;
    }
    else return gatesdict[foundat].output(A, B);
}
bool simulator::getouput(bool A, string gatename) {
    int foundat = -9999;
    for (int i = 0;i < gatesdict.size();i++)
    {
        if (gatesdict[i].name == gatename) {
            foundat = i;
        }
    }
    if (foundat == -9999) {
        cout << "Gate not found";
        return 0;
    }
    else return gatesdict[foundat].output(A, A);
}
void simulator::readvectorgate() {
    for (const auto& gate : gatesdict) {
        cout << "Gate Name: " << gate.name << endl;
        cout << "Delay: " << gate.delayofgate << endl;
        cout << "Number of Inputs: " << gate.inputnums << endl;
        cout << "Expression: " << gate.expression << endl;
    }
}

