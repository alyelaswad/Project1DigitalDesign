#include <iostream>
#include <string>
#include <vector>
#include <vector>
#include "libfile.h" 
using namespace std;
class circuit
{
private:
vector <char> inputs;
vector <gate*> lib_gates;
public:
circuit(libfilereader lib_file);
void access_cir_file(std::string pathname);

};