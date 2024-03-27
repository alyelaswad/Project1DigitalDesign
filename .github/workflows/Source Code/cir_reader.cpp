#include "cir_reader.h" // including the header file
#include "LogicGates.h" // this header file has the logic gates functionalities
#include <algorithm>    //
#include <vector>       // the main data structure used was vectors, this allowed us to use its predefined functions such as size, find, sort, etc..
#include <map>
#include <unordered_map>
#include <functional>
#include <stack>                                                   // this allowed us to map each variable to its boolean value
bool compare_timestamp(IntermediateValue &a, IntermediateValue &b) // compares the values based on their timestamp
{
    return a.timestamp < b.timestamp;
}
int precedence(char op)
{
    if (op == '~')
        return 3; // Highest precedence for NOT
    else if (op == '&')
        return 2;
    else if (op == '|')
        return 1;
    else
        return 0; // Lowest precedence for parentheses
}

bool evaluateexpression(string express)
{
    stack<char> operators;
    stack<bool> operands;
    LogicGates G;

    typedef bool (LogicGates::*BinaryOperation)(bool, bool);
    unordered_map<char, BinaryOperation> binaryOperations = {
        {'&', &LogicGates::AND},
        {'|', &LogicGates::OR}};

    for (char ch : express)
    {
        if (ch == '(')
        {
            operators.push(ch);
        }
        else if (ch == ')')
        {
            while (!operators.empty() && operators.top() != '(')
            {
                char op = operators.top();
                operators.pop();
                if (op == '~')
                {
                    bool operand = operands.top();
                    operands.pop();
                    operands.push(G.NOT(operand)); // Perform NOT operation
                }
                else
                {
                    bool operand2 = operands.top();
                    operands.pop();
                    bool operand1 = operands.top();
                    operands.pop();
                    operands.push((G.*(binaryOperations[op]))(operand1, operand2));
                }
            }
            operators.pop(); // Pop '('
        }
        else if (ch == '&' || ch == '|' || ch == '~')
        {
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch))
            {
                char op = operators.top();
                operators.pop();
                if (op == '~')
                {
                    bool operand = operands.top();
                    operands.pop();
                    operands.push(G.NOT(operand)); // Perform NOT operation
                }
                else
                {
                    bool operand2 = operands.top();
                    operands.pop();
                    bool operand1 = operands.top();
                    operands.pop();
                    operands.push((G.*(binaryOperations[op]))(operand1, operand2));
                }
            }
            operators.push(ch);
        }
        else if (ch == '0' || ch == '1')
        {
            operands.push(ch == '0' ? false : true); // Push boolean values instead of characters
        }
    }

    // After processing the entire expression, we might have remaining operators to evaluate
    while (!operators.empty())
    {
        char op = operators.top();
        operators.pop();
        if (op == '~')
        {
            bool operand = operands.top();
            operands.pop();
            operands.push(G.NOT(operand)); // Perform NOT operation
        }
        else
        {
            bool operand2 = operands.top();
            operands.pop();
            bool operand1 = operands.top();
            operands.pop();
            operands.push((G.*(binaryOperations[op]))(operand1, operand2));
        }
    }

    // At the end, operands stack should contain the final result
    return operands.top();
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
bool gate::outputfc(vector<bool> inputs, string type, vector<gate> gatesdict)
{
    for (int i = 0; i < gatesdict.size(); i++)
    {
        if (gatesdict[i].type == type)
        {                                                // finds where the type in gatesdict
            string expression = gatesdict[i].expression; // assigns expression at index i to expression
            for (int j = 0; j < inputs.size(); j++)
            {
                string placeholder = "i" + to_string(j + 1);
                size_t pos = expression.find(placeholder);
                while (pos != string::npos)
                { // replaces i1 and i2 with boolean values from inputs
                    expression.replace(pos, placeholder.length(), inputs[j] ? "1" : "0");
                    pos = expression.find(placeholder, pos + 1);
                }
            }
            bool result = evaluateexpression(expression); // evaluate using the string computed in the while function
            return result;
        }
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
        // For example, if the gate is NAND2, then it has 2 inouts, etc...
        string name1;
        while (getline(ss, name1, ','))
        {
            gate_i.inputs.push_back(name1); // Reading the inputs
        }

        int max_input_delay = 0;                       // Compute total delay for the gate
        for (int i = 0; i < gate_i.inputs.size(); ++i) // looping over the gate inputs
        {
            string input_name = gate_i.inputs[i];
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
bool CircuitReader::getOutput(std::vector<bool> inputs, std::string gatename)
{
    int foundat = -1;
    for (int i = 0; i < gatesdict.size(); i++)
    { // loops through gates dict
        if (gatesdict[i].type == gatename)
        {                // checks if the gate name is the same as the one found in Gates dict
            foundat = i; // gets its index to be passed in return
            break;
        }
    }
    if (foundat == -1)
    {
        std::cout << "Gate not found" << std::endl; // exists if gate not found;
        return false;
    }
    else
    {
        return gatesdict[foundat].outputfc(inputs, gatename, gatesdict); // return inputs and gatename and gatesdict
    }
}
void CircuitReader::accessStimFile(string pathname) // A function that reads StimFile and stores the information
{                                                   // in the dataVector
    ifstream inFile;
    inFile.open(pathname); // Opening the stimfile based on its path or name
    if (!inFile.is_open()) // Handling the case of not finding the file
        cout << "File not found" << endl;
    else
    {
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
    map<string, bool> intermediate_values;     // A map that stores intermediate values
    for (int i = 0; i < cir_gates.size(); i++) // Loop over all the gates in the circuit to compute their outputs
    {
        bool output_value;
        gate &current_gate = cir_gates[i]; // Use reference instead of copy

        vector<bool> gate_inputs; // A vector that stores the current inputs of the gate
        // cout << current_gate.inputnums << " ";
        for (int j = 0; j < current_gate.inputnums; j++) // Loop all over the inputs to process them
        {
            string input_name = current_gate.inputs[j];
            // cout << "Input: " << input_name << "  ";
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
                // cout << " Value: " << input_value << endl;
            }
            // cout << input_value << endl;

            gate_inputs.push_back(input_value); // Storing the value of the inputs in the gate_inputs vector
        }
        output_value = getOutput(gate_inputs, current_gate.type); // Computing the output of the gates based on the
        auto it = previous_values.find(current_gate.output);
        // if (it != previous_values.end() && it->second == output_value) // Check if there was a previous duplicate and
        // {                                                              // skip the current value if there was a duplicate
        //     continue;
        // }
        // inputs and type of the gate
        // Recording the timestamp, output and the delay
        intermediateValues.push_back(IntermediateValue(timestamp + current_gate.delayofgate, current_gate.output, output_value));
        intermediate_values[current_gate.output] = output_value;
        // previous_values[current_gate.output] = output_value; // Storing the value in the previous_values vector
    }
}
void CircuitReader::SimulateProgram(string pathname)
{
    // Clear any previous data
    intermediateValues.clear();
    current_values.clear();

    // Initialize inputs
    for (int i = 0; i < inputs.size(); i++)
    {
        current_values.push_back(0);
    }

    // Process stimuli read by StimfileReader
    for (int i = 0; i < dataVector.size(); i++)
    {
        char c = dataVector[i].variable[0];
        current_values[int(c) - 65] = dataVector[i].value;
    }

    // Compute initial circuit state
    compute_circuit(0);

    // Process stimuli for each timestamp
    for (int i = 0; i < dataVector.size(); i++)
    {
        compute_circuit(dataVector[i].timestamp);
    }

    // Sort intermediate values based on timestamps
    std::sort(intermediateValues.begin(), intermediateValues.end(), compare_timestamp);

    // Write intermediate values to simulation file
    ofstream simfile(pathname);
    for (int i = 0; i < intermediateValues.size(); i++)
    {
        simfile << intermediateValues[i].timestamp << "," << intermediateValues[i].variable << ","
                << (intermediateValues[i].value ? "1" : "0") << endl;
    }

    // Output intermediate values to console for debugging
    for (int i = 0; i < intermediateValues.size(); i++)
    {
        cout << intermediateValues[i].timestamp << " " << intermediateValues[i].variable
             << " " << intermediateValues[i].value << endl;
    }
}