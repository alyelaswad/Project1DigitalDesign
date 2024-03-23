#include "cir_reader.h"
#include"LogicGates.h"
#include <algorithm>
#include <vector>
#include <map>
#include <set>
bool compare_timestamp(IntermediateValue& a, IntermediateValue& b)
{
    return a.timestamp < b.timestamp;
}
bool gate::outputfc( vector<bool> inputs) {

    if (type == "NAND") {
        if (inputs.size() < 2) {
            cout << "NAND gate requires at least 2 inputs" << endl;
            return false;
        }
        bool result = g.AND(inputs[0], inputs[1]);
        for (int i = 2; i < inputs.size(); ++i) {
            result = g.AND(result, inputs[i]);
        }
        result = g.NOT(result);
        return result;
    }
    else if (type == "NOT") {
        if (inputs.size() != 1) {
            cout << "Invalid number of inputs for NOT gate" << endl;
            return false;
        }
        return g.NOT(inputs[0]);
    }
    else if (type == "AND") {
        if (inputs.size() < 2) {
            cout << "AND gate requires at least 2 inputs" << endl;
            return false;
        }
        bool result = g.AND(inputs[0], inputs[1]);
        for (size_t i = 2; i < inputs.size(); ++i) {
            result = g.AND(result, inputs[i]);
        }
        return result;
    }
    else if (type == "OR") {
        if (inputs.size() < 2) {
            cout << "OR gate requires at least 2 inputs" << endl;
            return false;
        }
        bool result = g.OR(inputs[0], inputs[1]);
        for (size_t i = 2; i < inputs.size(); ++i) {
            result = g.OR(result, inputs[i]);
        }
        return result;
    }
    else if (type == "NOR") {
        if (inputs.size() < 2) {
            cout << "NOR gate requires at least 2 inputs" << endl;
            return false;
        }
        bool result = g.OR(inputs[0], inputs[1]);
        for (size_t i = 2; i < inputs.size(); ++i) {
            result = g.OR(result, inputs[i]);
        }
        result = g.NOT(result);
        return result;
    }
    else {
        cout << "Unknown gate type" << endl;
        return false;
    }
}
void CircuitReader::accessLibFile(const string& pathname)
{
    ifstream inFile(pathname);
    if (!inFile.is_open())
    {
        cout << "Error: Unable to open file" << endl;
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string type, expression, delay, inputnums;
        getline(ss, type, ',');
        for (int i = 0;i < type.length();i++)
        {
            if (!isalpha(type[i])) {
                type.erase(i, 1);

            }
        }
        getline(ss, inputnums, ',');
        getline(ss, expression, ',');
        ss >> delay;
        gate g = { type, stoi(inputnums), expression, stoi(delay) };
        gatesdict.push_back(g);
    }

    inFile.close();
}
void CircuitReader::readvectorgate()
{ // prints the content of the gates in gatesdict
    for (const auto& gate : gatesdict)
    {
        cout << "Gate Type: " << gate.type << endl;
        cout << "Delay: " << gate.delayofgate << endl;
        cout << "Number of Inputs: " << gate.inputnums << endl;
        cout << "Inputs: ";
        for (const auto& input : gate.expression)
        {
            cout << input << " ";
        }
        cout << endl
            << endl;
    }
}

void CircuitReader::accessCirFile(std::string pathname)
{
    ifstream cir_file(pathname); // Open the .cir file
    if (!cir_file.is_open())
    {
        cout << "Error: Unable to open file" << endl;
        return;
    }

    string line;
    while (getline(cir_file, line) && line != "COMPONENTS:")
    {
        if (line == "INPUTS:")
            continue;
        else
        {
            inputs.push_back(line); // Storing the inputs
        }
    }

    while (getline(cir_file, line))
    {
        if (line == "COMPONENTS:")
            continue;

        stringstream ss(line);
        std::string name, type;
        getline(ss, name, ',');
        getline(ss, type, ',');
        for (int i = 0;i < type.length();i++)
        {
            if (!isalpha(type[i])) {
                type.erase(i, 1);

            }
        }

        gate gate_i;
        bool gate_found = false;
        for (int i = 0; i < gatesdict.size(); i++)
        {
            if (type == gatesdict[i].type)
            {
                gate_i = gatesdict[i];
                gate_found = true;
                break;
            }
        }

        if (!gate_found)
        {
            cout << "Gate type '" << type << "' not found in the library" << endl;
            continue;
        }

        string output;
        getline(ss, output, ','); // Reading the outputs
        gate_i.output = output;
        int n = type[type.size() - 1] - '0';
        for (int i = 0; i < n; i++)
        {
            string name1;
            getline(ss, name1, ',');
            gate_i.inputs.push_back(name1); // Reading the inputs
        }
        cir_gates.push_back(gate_i);
    }
    cir_file.close(); // Closing the .cir file
}

bool CircuitReader::getOutput(vector<bool> inputs, string gatename) {
    int foundat = -1;
    for (size_t i = 0; i < gatesdict.size(); ++i) {
        if (gatesdict[i].type == gatename) {
            foundat = i;
            break;
        }
    }
    if (foundat == -1) {
        cout << "Gate not found" << endl;
        return false;
    }
    else {
        return gatesdict[foundat].outputfc(inputs);
    }
}
void CircuitReader::accessStimFile(string pathname)
{
    ifstream inFile;
    inFile.open(pathname);
    // for (int i = 0; i < inputs.size(); i++)
    // {
    //     string c = inputs[i];
    //     dataVector.push_back({0, c, 0});
    // }

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string parcer;

        Data temp;

        getline(ss, parcer, ',');
        temp.timestamp = stoi(parcer);

        getline(ss, parcer, ',');
        temp.variable = parcer;

        getline(ss, parcer, ',');
        temp.value = stoi(parcer);

        dataVector.push_back(temp);
    }
    // for (int i = 0; i < dataVector.size(); i++)
    // {
    //     cout << dataVector[i].timestamp << "\t" << dataVector[i].variable << "\t" << dataVector[i].value << "\n";
    // }
}
void CircuitReader::compute_circuit(int timestamp)
{
    // Map to store intermediate values and their previous values
    map<string, bool> intermediate_values;
    
    // Traverse each gate in the circuit
    for (int i = 0; i < cir_gates.size(); i++)
    {
        gate& current_gate = cir_gates[i];

        // Compute the effective timestamp for this gate including delay
        int gate_timestamp = timestamp + current_gate.delayofgate;

        // Check if this gate's timestamp has already passed
        if (gate_timestamp <= timestamp)
            continue;

        // Vector to store inputs for the current gate
        vector<bool> gate_inputs;

        // Process inputs for the current gate
        for (int j = 0; j < current_gate.inputnums; j++)
        {
            string input_name = current_gate.inputs[j];

            // Check if input value is already computed and stored in intermediate_values
            auto it = intermediate_values.find(input_name);
            bool input_value;
            if (it != intermediate_values.end())
            {
                input_value = it->second;
            }
            else
            {
                // Find the corresponding value from current_values
                char input_variable = input_name[0];
                int input_index = input_variable - 'A'; // Assuming inputs are labeled A, B, C, ...
                input_value = current_values[input_index];
            }

            // Store the input value for the gate
            gate_inputs.push_back(input_value);
        }

        // Compute the output of the gate based on its inputs
        bool output_value = getOutput(gate_inputs, current_gate.type);

        // Record the event since the value has changed
        intermediateValues.push_back(IntermediateValue(gate_timestamp, current_gate.output, output_value));
        intermediate_values[current_gate.output] = output_value;
        previous_values[current_gate.output] = output_value;
    }
}

void CircuitReader::SimulateProgram()
{
    string input("");
    for (int i = 0; i < inputs.size(); i++)
    {
        input += inputs[i][0];
        intermediateValues.push_back(IntermediateValue(0, input, 0));
        input = "";
    }

    for (int i = 0; i < dataVector.size(); i++)
        intermediateValues.push_back(IntermediateValue(dataVector[i].timestamp, dataVector[i].variable, dataVector[i].value));

    vector<Data> outputs;
    for (int i = 0; i < inputs.size(); i++)
    {
        current_values.push_back(0);
    }
    compute_circuit(0);
    for (int i = 0; i < dataVector.size(); i++)
    {
        char c;
        c = dataVector[i].variable[0];
        current_values[int(c) - 65] = dataVector[i].value;
        compute_circuit(dataVector[i].timestamp);
        // cout << "End of Event" << i + 1 << endl
        //      << endl;
    } 
for (int k = 0; k < intermediateValues.size(); k++) {
    bool added = false;
    if (intermediateValues[k].variable.at(0) == 'w' || intermediateValues[k].variable.at(0) == 'Y') {
        for (int i = 0; i < cir_gates.size(); i++) {
            if(added) break;
            if (cir_gates[i].output == intermediateValues[k].variable) {
                for (int j = 0; j < cir_gates[i].inputs.size(); j++) {
                    if(added) break;
                    if (!cir_gates[i].inputs[j].empty() && cir_gates[i].inputs[j][0] == 'w') {
                        for (int m = 0; m < cir_gates.size(); m++) {
                            if (cir_gates[m].output == cir_gates[i].inputs[j]) {
                                if (!added) {
                                    intermediateValues[k].timestamp += cir_gates[m].delayofgate;
                                    added = true;
                                    break;
                                }
                            }
                        } 
                    }
                }
            }
        }
    }
}


    std::sort(intermediateValues.begin(), intermediateValues.end(), compare_timestamp);

    for (int k = 0; k < intermediateValues.size(); k++) {
        cout << intermediateValues[k].timestamp << "," << intermediateValues[k].variable << ","
             << (intermediateValues[k].value ? "1" : "0") << endl;    
    }
}