#include "cir_reader.h"                                            // including the header file
#include "LogicGates.h"                                            // this header file has the logic gates functionalities
#include <algorithm>                                               //
#include <vector>                                                  // the main data structure used was vectors, this allowed us to use its predefined functions such as size, find, sort, etc..
#include <map>                                                     // this allowed us to map each variable to its boolean value
bool compare_timestamp(IntermediateValue &a, IntermediateValue &b) // compares the values based on their timestamp
{
    return a.timestamp < b.timestamp;
}
bool gate::outputfc(vector<bool> inputs)
{
    if (type == "NAND")
    { // if it is a NAND gate
        if (inputs.size() < 2)
        {
            cout << "NAND gate requires at least 2 inputs" << endl;
            return false;
        }
        bool result = g.AND(inputs[0], inputs[1]); // call AND
        for (int i = 2; i < inputs.size(); ++i)
        {
            result = g.AND(result, inputs[i]);
        }
        result = g.NOT(result); // invert the AND gate to achieve NAND functionality
        return result;
    }
    else if (type == "NOT")
    {
        if (inputs.size() != 1) // NOT gates can only work on a single input
        {
            cout << "Invalid number of inputs for NOT gate" << endl;
            return false;
        }
        return g.NOT(inputs[0]);
    }
    else if (type == "AND")
    {
        if (inputs.size() < 2)
        {
            cout << "AND gate requires at least 2 inputs" << endl;
            return false;
        }
        bool result = g.AND(inputs[0], inputs[1]);
        for (size_t i = 2; i < inputs.size(); ++i) // this function allows us to take multiple inputs to AND
        {
            result = g.AND(result, inputs[i]);
        }
        return result;
    }
    else if (type == "OR")
    {
        if (inputs.size() < 2)
        {
            cout << "OR gate requires at least 2 inputs" << endl;
            return false;
        }
        bool result = g.OR(inputs[0], inputs[1]);
        for (size_t i = 2; i < inputs.size(); ++i) // this function allows us to take multiple inputs to OR
        {
            result = g.OR(result, inputs[i]);
        }
        return result;
    }
    else if (type == "NOR")
    {
        if (inputs.size() < 2)
        {
            cout << "NOR gate requires at least 2 inputs" << endl;
            return false;
        }
        bool result = g.OR(inputs[0], inputs[1]);  // call OR
        for (size_t i = 2; i < inputs.size(); ++i) // we OR each input with the other until we check all inputs
        {
            result = g.OR(result, inputs[i]);
        }
        result = g.NOT(result); // invert OR to achieve NOR functionality
        return result;
    }
    else
    {
        cout << "Unknown gate type" << endl;
        return false;
    }
}
void CircuitReader::accessLibFile(const string &pathname)
{
    ifstream inFile(pathname); // Reads from the .lib file
    if (!inFile.is_open())     // If we can not open the file, an error message shows up
    {
        cout << "Error: Unable to open file" << endl;
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string type, expression, delay, inputnums;
        getline(ss, type, ','); // partitioning the line, into the gate name,type, inputs, and expression
        for (int i = 0; i < type.length(); i++)
        {
            if (!isalpha(type[i]))
            {
                type.erase(i, 1); // to erase the spaces between the lines
            }
        }
        getline(ss, inputnums, ',');
        getline(ss, expression, ',');
        ss >> delay;
        gate g = {type, stoi(inputnums), expression, stoi(delay)}; // calling the intiliazer to create the gate
        // from the read data.
        gatesdict.push_back(g); // pushing the gate into the dictionary of gates
    }

    inFile.close(); // closing the file after reading its data.
}
void CircuitReader::readvectorgate() // Debugging function to test that the .lib file was read correctly
{                                    // prints the content of the gates in gatesdict
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
    while (getline(cir_file, line) && line != "COMPONENTS:") // until all the inputs are read
    {
        if (line == "INPUTS:") // we ignore the lines were INPUTS is written
                               // as it does not add any value to the data
            continue;          // goes to the next iteration, ignoring the loop body
        else
        {
            inputs.push_back(line); // Storing the inputs
        }
    }

    while (getline(cir_file, line)) // reading the gate description
    {
        if (line == "COMPONENTS:") // we ignore the lines were COMPONENTS is written
                                   // as it does not add any value to the data
            continue;

        stringstream ss(line); // string stream declaration to partition the line into gate name, type
        std::string name, type;
        getline(ss, name, ','); // since the file is comma separated
        getline(ss, type, ',');
        for (int i = 0; i < type.length(); i++)
        {
            if (!isalpha(type[i])) // removes the spaces which might affect results
            {
                type.erase(i, 1);
            }
        }

        gate gate_i; // creates an instance of the gate to read the description and store it from the .cir file
        bool gate_found = false;
        for (int i = 0; i < gatesdict.size(); i++) // checks that the gate exists in the .lib gate dictionary
        {
            if (type == gatesdict[i].type) // compares by type for the existence of the gate
            {
                gate_i = gatesdict[i];
                gate_found = true;
                break; // breaks if the gate if found
            }
        }

        if (!gate_found) // message to indicate that the gate is not defined in the .lib
        {
            cout << "Gate type '" << type << "' not found in the library" << endl;
            continue;
        }

        string output;
        getline(ss, output, ','); // Reading the outputs
        gate_i.output = output;
        int n = type[type.size() - 1] - '0'; // Calculating the number of inputs for the gate
        // For example, if the gate is NAND2, then it has 2 inouts, etc...
        for (int i = 0; i < n; i++)
        {
            string name1; // gets the name of the gate
            getline(ss, name1, ',');
            gate_i.inputs.push_back(name1); // Reading the inputs
        }

        int max_input_delay = 0;                     // Compute total delay for the gate
        for (int i = 0; i < gate_i.inputs.size(); ++i) // looping over the gate inputs
       {
           string  input_name = gate_i.inputs[i];
           for (int j = 0; j < cir_gates.size(); ++j) // looping over each gates
             {
                  gate input_gate = cir_gates[j];
                  if (input_gate.output == input_name)
                    {
                      max_input_delay = max(max_input_delay, input_gate.delayofgate); // assigns the maximum delay for each of the inputs
                       // to be the delay of the intermediate values to be later added to the delay of the gate
                      break;
                   }
             }
        }


        gate_i.delayofgate += max_input_delay; // Assign total delay to the gate

        cir_gates.push_back(gate_i); // Store the gate with its computed delay
    }
    cir_file.close(); // Closing the .cir file
}
bool CircuitReader::getOutput(vector<bool> inputs, string gatename) //Function to get the output based on the gate name 
{
    int foundat = -1;
    for (int i = 0; i < gatesdict.size(); ++i)  // Loop to find the type of gate from the gatesdict (libfile gates)
    {
        if (gatesdict[i].type == gatename) 
        {
            foundat = i;
            break;
        }
    }
    if (foundat == -1) // Handling the case of not finding the gate in the libfile gates
    {
        cout << "Gate not found" << endl;
        return false;
    }
    else 
    {
        return gatesdict[foundat].outputfc(inputs); // return the value of the output depending on the gate type 
    }
}
void CircuitReader::accessStimFile(string pathname) // A function that reads StimFile and stores the information 
{                                                   // in the dataVector
    ifstream inFile;
    inFile.open(pathname); // Opening the stimfile based on its path or name
    if(!inFile.is_open()) // Handling the case of not finding the file
    cout << "File not found" <<endl;
    else{
    string line; 
    while (getline(inFile, line)) // A loop that iterates over the file and stores then line into 3 strings  
    {                             // via using stringstream. Then they are stored in the dataVector.    
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
    }
}
void CircuitReader::compute_circuit(int timestamp) // A function that computes the outputs of the gates in a certain timestamp
{
    map<string, bool> intermediate_values; // A map that stores intermediate values 

    for (int i = 0; i < cir_gates.size(); i++) // Loop over all the gates in the circuit to compute their outputs
    {
        bool output_value;
        gate& current_gate = cir_gates[i];

        vector<bool> gate_inputs; // A vector that stores the current inputs of the gate 

        for (int j = 0; j < current_gate.inputnums; j++) // Loop all over the inputs to process them
        {
            string input_name = current_gate.inputs[j];

            auto it = intermediate_values.find(input_name); // Check whether the input value is computed and stored  
            bool input_value;                               // in the intermediate_values vector
            if (it != intermediate_values.end())
            {
                input_value = it->second;
            }
            else
            {
                char input_variable = input_name[0]; 
                int input_index = input_variable - 'A'; // Calculating the index through the input name
                input_value = current_values[input_index];
            }

            gate_inputs.push_back(input_value); // Storing the value of the inputs in the gate_inputs vector
        }

        output_value = getOutput(gate_inputs, current_gate.type); // Computing the output of the gates based on the  
                                                                  // inputs and type of the gate  
        auto it = previous_values.find(current_gate.output);  
        if (it != previous_values.end() && it->second == output_value) // Check if there was a previous duplicate and   
        {                                                              // skip the current value if there was a duplicate
            continue;
        }
        // Recording the timestamp, output and the delay
        intermediateValues.push_back(IntermediateValue(timestamp + current_gate.delayofgate, current_gate.output, output_value));
        intermediate_values[current_gate.output] = output_value; 
        previous_values[current_gate.output] = output_value; // Storing the value in the previous_values vector
    }
}
void CircuitReader::SimulateProgram(string pathname) // A Function that prints the output in a .sim file
{
    string input("");
    for (int i = 0; i < inputs.size(); i++) // A loop that removes the spaces after the input names
    {
        input += inputs[i][0];
        intermediateValues.push_back(IntermediateValue(0, input, 0));
        input = "";
    }
    for (int i = 0; i < dataVector.size(); i++) // Looping over the dataVector to push all the input information in the intermediateValues vector
        intermediateValues.push_back(IntermediateValue(dataVector[i].timestamp, dataVector[i].variable, dataVector[i].value));
  

    vector<Data> outputs;
    for (int i = 0; i < inputs.size(); i++) // The inputs are considered at logic "0" in the beggining 
    {
        current_values.push_back(0);
    }
    compute_circuit(0); // The outputs of the gates are calculated in the beggining 

  
    for (int i = 0; i < dataVector.size(); i++) // The outputs are computed for all the timestamps
    {
        char c;
        c = dataVector[i].variable[0];
        current_values[int(c) - 65] = dataVector[i].value;
        compute_circuit(dataVector[i].timestamp);
 
    }
  
    std::sort(intermediateValues.begin(), intermediateValues.end(), compare_timestamp); // The vector is sorted based on the timestamps
    ofstream simfile(pathname);
    for (int i = 0; i < intermediateValues.size(); i++) // The output is printed in the .sim file
        simfile << intermediateValues[i].timestamp << "," << intermediateValues[i].variable << ","
        << (intermediateValues[i].value ? "1" : "0") << endl;
}

