#include "libfilereader.h"


void libfilereader::accesslibfile(string pathname) {
    ifstream inFile;
    inFile.open(pathname);
    string line;
    while (getline(inFile, line))
    {   
        
    }
}