#include <iostream>
#include <string>
#include <vector>
#include <vector>
#include "libfile.h" 
using namespace std;
class circuit
{
private:
vector <char> inputs; // A vector that contains inputs as characters
vector <gate*> lib_gates; // A vector that stores pointers to gates of the .lib file
vector <gate*> cir_gates; // A vector that stores pointers to gates of the .cir file
public:
circuit(libfilereader lib_file); // A constructor that recieves a lib_file object to store the vector of lib_gates
void access_cir_file(std::string pathname); // A function that reads the .cir file and stores the gates used in the circuit in the cir_gates 
                                                vector

};
