#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "cir_reader.cpp"
using namespace std;

int main()
{
    libfilereader L;
    L.accesslibfile("DD1.lib");
    circuit mycirc(L);
    mycirc.access_cir_file("circ2.cir");
}
