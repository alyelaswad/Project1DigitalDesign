#include "libfile.h"
#include <fstream>
#include <string>
#ifndef LIFEREADER_CPP
#define LIFEREADER_CPP
using  namespace std;

vector<gate> libfilereader::get_gatesdict() // getter for the gatesdict
{
    return gatesdict;
}
void libfilereader::accesslibfile(std::string pathname) // Reads the .lib file
{
    ifstream inFile(pathname);
    if(!inFile.is_open())
    cout <<"File not found";
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string name, expression, delay, inputnums;
        getline(ss, name, ',');
       
        getline(ss, inputnums,',');
        getline(ss, expression, ',');
        
        ss >> delay;
        gate g = { name, stoi(inputnums), expression, stoi(delay) };
        gatesdict.push_back(g);
    }

    inFile.close();
}
void libfilereader::readvectorgate() { // prints the content of the gates in gatesdict
    for (const auto& gate : gatesdict) {
        cout << "Gate Name: " << gate.name << endl;
        cout << "Delay: " << gate.delayofgate << endl;
        cout << "Number of Inputs: " << gate.inputnums << endl;
        cout << "Inputs: ";
        for (const auto& input : gate.expression) {
            cout << input << " ";
        }
        cout << endl << endl;
    }
}
#endif
