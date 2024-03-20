#include "cir_reader.h"
#include <algorithm>
#include <vector>
#include <map>
#include <set>
bool compare_timestamp(IntermediateValue &a, IntermediateValue &b)
{
    return a.timestamp < b.timestamp;
}
bool gate::outputfc(bool A, bool B)
{
    // function to return the output gate
    if (type == "NAND2")
    {
        return g.NAND(A, B);
    }
    else if (type == "NOT")
    {
        return g.NOT(A);
    }
    else if (type == "AND2")
    {
        return g.AND(A, B);
    }
    else if (type == "OR2")
    {
        return g.OR(A, B);
    }
    else if (type == "NOR2")
    {
        return g.NOR(A, B);
    }
    return 0;
}
void CircuitReader::accessLibFile(const string &pathname)
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
        getline(ss, inputnums, ',');
        getline(ss, expression, ',');
        ss >> delay;
        gate g = {type, stoi(inputnums), expression, stoi(delay)};
        gatesdict.push_back(g);
    }

    inFile.close();
}
void CircuitReader::readvectorgate()
{ // prints the content of the gates in gatesdict
    for (const auto &gate : gatesdict)
    {
        cout << "Gate Type: " << gate.type << endl;
        cout << "Delay: " << gate.delayofgate << endl;
        cout << "Number of Inputs: " << gate.inputnums << endl;
        cout << "Inputs: ";
        for (const auto &input : gate.expression)
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

bool CircuitReader::getOutput(bool A, bool B, const string &gatetype)
{
    int foundat = -9999;
    for (int i = 0; i < gatesdict.size(); i++)
    {
        if (gatesdict[i].type == gatetype)
        {
            foundat = i;
        }
    }
    if (foundat == -9999)
    {
        cout << "Gate not found";
        return 0;
    }
    else
        return gatesdict[foundat].outputfc(A, B);
    ;
}

bool CircuitReader::getOutput(bool A, const string &gatetype)
{
    int foundat = -9999;
    for (int i = 0; i < gatesdict.size(); i++)
    {
        if (gatesdict[i].type == gatetype)
        {
            foundat = i;
        }
    }
    if (foundat == -9999)
    {
        cout << "Gate not found";
        return 0;
    }
    else
        return gatesdict[foundat].outputfc(A, A);
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
        bool output_value;
        gate &current_gate = cir_gates[i];

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
        output_value = (current_gate.inputnums == 1) ? getOutput(gate_inputs[0], current_gate.type)
                                                     : getOutput(gate_inputs[0], gate_inputs[1], current_gate.type);

        auto it = previous_values.find(current_gate.output);
        if (it != previous_values.end() && it->second == output_value)
        {
            continue;
        }

        // Record the event since the value has changed
        intermediateValues.push_back(IntermediateValue(timestamp + current_gate.delayofgate, current_gate.output, output_value));
        intermediate_values[current_gate.output] = output_value;
        previous_values[current_gate.output] = output_value;
    }
}
void CircuitReader::SimulateProgram()
{
    // ofstream outputfile(OutputPath);
    // if (!outputfile.is_open())
    // {
    //     cout << "Could not open output file" << endl;
    //     return;
    // }
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
    std::sort(intermediateValues.begin(), intermediateValues.end(), compare_timestamp);
    for (int i = 0; i < intermediateValues.size(); i++)
        cout << intermediateValues[i].timestamp << "," << intermediateValues[i].variable << ","
             << (intermediateValues[i].value ? "1" : "0") << endl;
    // for (int i = 0; i < cir_gates.size(); i++)
    // {
    //     if (cir_gates[i].type == "NOT")
    //         outputfc(, cir_gates[i].type);
    // }
}
