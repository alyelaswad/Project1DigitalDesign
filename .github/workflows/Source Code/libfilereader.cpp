#include "libfilereader.h"
using namespace std;

void libfilereader::accesslibfile( string pathname) {
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
       
        gate g = { name, stoi(inputnums),stoi(delay) };
        gatesdict.push_back(g);
    }
}
void libfilereader::readvectorgate() {
    for (const auto& gate : gatesdict) {
        cout << "Gate Name: " << gate.name << endl;
        cout << "Delay: " << gate.delayofgate << endl;
        cout << "Number of Inputs: " << gate.inputnums << endl;
    }
}

