#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "libfilereader.cpp"
#include "cir_reader.h"

circuit::circuit(libfilereader lib_file)
{
    gate* gate_i;
    for(int i=0;i<lib_gates.size();i++)
    {
    lib_gates.push_back(&(lib_file.get_gatesdict()[i]));
    }
}

void circuit::access_cir_file(std::string pathname)
{
    ifstream cir_file(pathname);
    string line;
    while (getline(cir_file, line) && line!="COMPONENTS:") 
    {
        if(line == "INPUTS:")
            continue;
        else
        {
        inputs.push_back(0);
        }
    }
    while(getline(cir_file, line))
    { 
        if(line == "COMPONENTS:")
            continue;
        std::string name;
        std::stringstream ss(line);       
        getline(ss, name ,',');
        gate* gate_i;
        for(int i=0;i<lib_gates.size();i++)
        {
            if(name==lib_gates[i]->type)
            gate_i= lib_gates[i];
        }
        int n = name[name.size()-1]-'0';
        for(int i=0;i<n;i++)
        {
            string name1;
            getline(ss,name1,',');
            gate_i->inputs[i]=name1;
            cout <<name1;
        }
        string output;
        getline(ss,output, ',');
        gate_i->output=output;
    cir_file.close();
    // for(int i=0;i<inputs.size();i++)
    // {
    // cout << inputs[i] << "ASJD";
    // }
}
 for(int i=0;i<inputs.size();i++)
     {
     cout << inputs[i] << "ASJD";
     }
}