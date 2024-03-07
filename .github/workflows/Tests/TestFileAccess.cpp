// This file is to test that we can access the text input files (lib,circ,stim)
#include <iostream>
#include <fstream>
using namespace std;
void AccessFile(string Pathname)
{
    ifstream inFile;
    inFile.open(Pathname);
    string line;
    while (getline(inFile, line))
    {
        cout << line << endl;
    }
}

int main()
{
    string InputFile = "circ_4Var.stim";
    cout << "\n\nThe output of " << InputFile << " is : \n";
    AccessFile(InputFile);
    InputFile = "circ1.cir";
    cout << "\n\nThe output of " << InputFile << " is : \n";
    AccessFile(InputFile);
    InputFile = "DD1.lib";
    cout << "\n\nThe output of " << InputFile << " is : \n";
    AccessFile(InputFile);

    return 0;
}
