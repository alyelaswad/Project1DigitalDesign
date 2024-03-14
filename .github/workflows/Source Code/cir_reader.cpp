#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "libfilereader.cpp"
#include "cir_reader.h"

circuit::circuit(libfilereader lib_file) // Passing the .lib vector of gates to the circuit
{
    gate* gate_i;
    for(int i=0;i<lib_gates.size();i++)
    {
    lib_gates.push_back(&(lib_file.get_gatesdict()[i]));
    }
}

void circuit::access_cir_file(std::string pathname) // Reading the file and storing the information in a vector of gates 
{
    ifstream cir_file(pathname); // openining the .cir file 
    string line;
    while (getline(cir_file, line) && line!="COMPONENTS:") //storing the inputs
    {
        if(line == "INPUTS:")
            continue;
        else
        {
        inputs.push_back(0);
        }
    }
    while(getline(cir_file, line)) // Storing the name, type, inputs and outputs of the circuit gates
    { 
        if(line == "COMPONENTS:")
            continue;
        std::string name;
        std::stringstream ss(line);       
        getline(ss, name ,',');
        gate* gate_i;
        for(int i=0;i<lib_gates.size();i++)
        {
            if(name==lib_gates[i]->type) // checking the type of the gate
            gate_i= lib_gates[i];
        }
        int n = name[name.size()-1]-'0';
        for(int i=0;i<n;i++) // Reading the inputs 
        {
            string name1;
            getline(ss,name1,',');
            gate_i->inputs[i]=name1;
           // cout <<name1;
        }
        string output;
        getline(ss,output, ','); // Reading the outputs
        gate_i->output=output;
        cir_gates.push_back(*gate_i);
        cir_file.close(); //closing the .cir file 
        delete gate_i; // deleting the pointer
}
 for(int i=0;i<inputs.size();i++)
     {
     cout << inputs[i] << endl;
     }
}
