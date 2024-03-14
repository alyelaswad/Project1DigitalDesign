#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "simulator.h"
using namespace std;

int main()
{
    simulator reader;
    string filename = "source.txt";
    cout << "Are they similar?: " << reader.accandcompsimfile(filename, "source1.txt") << endl;
    reader.accesslibfile(filename);
    cout << reader.getouput(1,0,"NAND2");
    reader.readvectorgate();
    return 0;
}
