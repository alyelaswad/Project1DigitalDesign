#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "libfilereader.h"
using namespace std;

int main()
{
    libfilereader reader;
    string filename = "source.txt";
    reader.accesslibfile(filename);
    cout << reader.getouput(0);
  /* reader.readvectorgate();*/
    return 0;
}
