
#include "cir_reader.h"

void CircuitReader::accessLibFile(const string& pathname) {
    ifstream inFile(pathname);
    if (!inFile.is_open()) {
        cout << "Error: Unable to open file" << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string type, expression, delay, inputnums;
        getline(ss, type, ',');
        getline(ss, inputnums, ',');
        getline(ss, expression, ',');
        ss >> delay;
        gate g = { type, stoi(inputnums), expression, stoi(delay) };
        gatesdict.push_back(g);
    }

    inFile.close();
}
void CircuitReader::readvectorgate() { // prints the content of the gates in gatesdict
    for (const auto& gate : gatesdict) {
        cout << "Gate Type: " << gate.type << endl;
        cout << "Delay: " << gate.delayofgate << endl;
        cout << "Number of Inputs: " << gate.inputnums << endl;
        cout << "Inputs: ";
        for (const auto& input : gate.expression) {
            cout << input << " ";
        }
        cout << endl << endl;
    }
}

void CircuitReader::accessCirFile(std::string pathname) {
    ifstream cir_file(pathname); // Open the .cir file
    if (!cir_file.is_open()) {
        cout << "Error: Unable to open file" << endl;
        return;
    }

    string line;
    while (getline(cir_file, line) && line != "COMPONENTS:") {
        if (line == "INPUTS:") continue;
        else inputs.push_back(0); // Storing the inputs
    }

    while (getline(cir_file, line)) {
        if (line == "COMPONENTS:") continue;

        stringstream ss(line);
        std::string name,type;
        getline(ss,name, ',');
        getline(ss, type, ',');
     
        gate gate_i; 
        bool gate_found = false;
        for (int i = 0; i < gatesdict.size(); i++) {
            if (type== gatesdict[i].type) {
                gate_i = gatesdict[i]; 
                gate_found = true;
                break;
            }
        }

        if (!gate_found) {
            cout << "Gate type '" << type << "' not found in the library" << endl;
            continue;
        }

       
        string output;
        getline(ss, output, ','); // Reading the outputs
        gate_i.output = output;
       
        for (int i = 0; i < 7; i++) {
            string name1;
            getline(ss, name1, ',');
            cout << name1<<endl;
            gate_i.inputs[i] = name1; // Reading the inputs

        }
        cir_gates.push_back(gate_i);
    }

    cir_file.close(); // Closing the .cir file
}


bool CircuitReader::getOutput(bool A, bool B, const string& gatetype) {
    auto found_gate = find_if(gatesdict.begin(), gatesdict.end(), [&](const gate& g) { return g.type == gatetype; });
    if (found_gate == gatesdict.end()) {
        cout << "Gate not found" << endl;
        return false;
    }

    return found_gate->outputfc(A, B);
}

bool CircuitReader::getOutput(bool A, const string& gatetype) {
    auto found_gate = find_if(gatesdict.begin(), gatesdict.end(), [&](const gate& g) { return g.type == gatetype; });
    if (found_gate == gatesdict.end()) {
        cout << "Gate not found" << endl;
        return false;
    }

    return found_gate->outputfc(A, A);
}
