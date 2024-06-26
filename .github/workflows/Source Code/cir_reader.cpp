#include "cir_reader.h" // including the header file
#include "LogicGates.h" // this header file has the logic gates functionalities
#include <algorithm>    //
#include <vector>       // the main data structure used was vectors, this allowed us to use its predefined functions such as size, find, sort, etc..
#include <map>
#include <unordered_map>
#include <functional>
#include <stack> // this allowed us to map each variable to its boolean value
#include <queue>
#include <set>
#include <unordered_set>
#include <fstream>
bool isNotSpace(char ch)
{  //checks if the character is not a space and returns a bool value
    return (ch != ' ');
}
bool isOutputOfPreviousGate(string output, vector<gate> &previousGates)
{ //checks if the output is found in fhe pervious gates and returna a bool value
    for (const auto &gate : previousGates)
    { //loops throught the vector of pervious gates and checks if its output is the same output given as a paramter
        if (gate.output == output)
        { // if yes it returns true
            return true;
        }
    }
    return false; // Output is not found in previous gates
}
bool compare_timestamp(IntermediateValue &a, IntermediateValue &b) // compares the values based on their timestamp
{ //compare two timestamps and return a bool value
    return a.timestamp < b.timestamp;
}
int precedence(char op)
{ // function to make sure precedence rules are applied
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

    typedef bool (LogicGates::*BinaryOperation)(bool, bool); // this will make a member of functions found in class LogicGates
    unordered_map<char, BinaryOperation> binaryOperations = {
        {'&', &LogicGates::AND},
        {'|', &LogicGates::OR}}; //mapping each gate to its symbol

    for (char ch : express)
    { //checks what the first letter in expression starts with
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
        { //checks if the character is an operator 
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch))
            { //checks precedence and if operators not equal empty
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
    return 0;
}

void CircuitReader::accessCirFile(std::string pathname)
{
    ifstream cir_file(pathname); // Open the .cir file
    if (!cir_file.is_open())
    {
        cout << "Error: Unable to open file" << endl;
        return;
    }

    string line, convert("");
    while (getline(cir_file, line) && line != "COMPONENTS:") // until all the inputs are read
    {
        if (line == "INPUTS:") // we ignore the lines were INPUTS is written
                               // as it does not add any value to the data
            continue;          // goes to the next iteration, ignoring the loop body
        else
        {
            for (int i = 0; i < line.length(); i++)
            {
                if (!isNotSpace(line[i])) // removes the spaces which might affect results
                {
                    line.erase(i, 1);
                }
            }
            inputs.push_back(line); // Storing the inputs
        }
    }

    while (getline(cir_file, line)) // reading the gate description
    {
        if (line == "COMPONENTS:") // we ignore the lines where COMPONENTS is written
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
            terminate();
        }

        string output;
        getline(ss, output, ','); // Reading the outputs
        for (int i = 0; i < output.length(); i++)
        {
            if (!isNotSpace(output[i])) // removes the spaces which might affect results
            {
                output.erase(i, 1);
            }
        }
        gate_i.output = output;
        // For example, if the gate is NAND2, then it has 2 inouts, etc...
        string name1;
        while (getline(ss, name1, ','))
        {
            for (int i = 0; i < name1.length(); i++)
            {
                if (!isNotSpace(name1[i])) // removes the spaces which might affect results
                {
                    name1.erase(i, 1);
                }
            }
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
    bool inputFound = false;
    unordered_set<string> inputSet(inputs.begin(), inputs.end()); // The inputs in the .cir file are stored in a set

    for (const auto &gate : cir_gates)
    {
        for (string input : gate.inputs) 
        {
            if (inputSet.find(input) == inputSet.end() && !isOutputOfPreviousGate(input, cir_gates)) // We check each output to see if it is 
            {                                                                                        // in the inputs of the .cir file or if it is an output of a previous gate
                cout << "One of the inputs of the gates is invalid" << endl;
                terminate();
            }
        }
    }
}
bool CircuitReader::getOutput(std::vector<bool> inputs, std::string gatename) // A function that takes the vector of inputs and type of gate and returns the output
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

            IntermediateValue temp;

            getline(ss, parcer, ',');
            temp.timestamp = stoi(parcer);

            getline(ss, parcer, ',');
            for (int i = 0; i < parcer.length(); i++)
            {
                if (!isNotSpace(parcer[i])) // removes the spaces which might affect results
                {
                    parcer.erase(i, 1);
                }
            }
            temp.variable = parcer;

            getline(ss, parcer, ',');
            temp.value = stoi(parcer);

            dataVector.push_back(temp);
        }
    }
}
void CircuitReader::compute_circuit(int timestamp)
{
    unordered_map<string, bool> currentStates; // Maps variable names to their current values
    std::sort(dataVector.begin(), dataVector.end(), [](const IntermediateValue &a, const IntermediateValue &b)
              { return a.timestamp < b.timestamp; });

    // Update current states based on stimuli data at the given timestamp
    for (const auto &data : dataVector)
    {
        if (data.timestamp > timestamp)
        {
            break; // Exit the loop once we pass the target timestamp
        }
        currentStates[data.variable] = data.value;
    }

    // Compute outputs of gates
    for (const auto &gatee : cir_gates)
    {
        if (gatee.output.empty())
        {
            continue; // Skip gates without output
        }

        vector<bool> inputss;
        for (const auto &inputName : gatee.inputs)
        {
            auto it = currentStates.find(inputName);
            if (it != currentStates.end())
            {
                inputss.push_back(it->second);
            }
            else
            {
                cerr << "Error: Input variable " << inputName << " not found." << endl;
                inputss.push_back(false); // Default value
            }
        }

        if (timestamp != 0)
        {
            // Evaluate the gate output using getOutput function
            bool gateOutput = getOutput(inputss, gatee.type);

            // Update the current state with the gate's output
            currentStates[gatee.output] = gateOutput;

            // Check if the timestamp is greater than 0 and update the previous value

            bool previousValue = getPreviousValue(gatee.output);
            if (gateOutput != previousValue)
            {
                // Output this value only if it's different from the previous value
                intermediateValues.push_back(IntermediateValue(timestamp + gatee.delayofgate, gatee.output, gateOutput));
                // Update the previous value to the current value
                updatePreviousValue(gatee.output, gateOutput);
            }
        }
        else
        {
            // Evaluate the gate output using getOutput function
            bool gateOutput = getOutput(inputss, gatee.type);
            // Update the current state with the gate's output
            currentStates[gatee.output] = gateOutput;
            intermediateValues.push_back(IntermediateValue(timestamp + gatee.delayofgate, gatee.output, gateOutput));
            updatePreviousValue(gatee.output, gateOutput);
        }
        int h;
        int timestampfloop = timestamp;
        // Check if the output matches any input
        for (int i = 0; i < inputs.size(); ++i)
        {
            const auto &input = inputs[i];
            vector<bool> temp; // temp to be able to make sure that the vector that the function recieve is focussed on the last output
            if (gatee.output == input)
            {
                for (int j = 0; j < gatee.inputs.size(); j++)
                {
                    temp.push_back(inputss[j]);// this pushes the first bool inputs in the temp vector
                    h = j;
                }
                
                int z = 0;
                while (z < 100)
                { // Limit the loop to 50 iterations
                    z++;

                    // Recompute the gate output
                    bool gateOutput = getOutput(temp, gatee.type);
                    // Update the current state with the updated output
                    currentStates[gatee.output] = gateOutput;
                    // Replace the first occurrence of gate output in inputs with its value
                    temp[h] = gateOutput;// last output is insterted into temp vector so it can be computed after
                    intermediateValues.push_back(IntermediateValue(timestampfloop + gatee.delayofgate, gatee.output, gateOutput));
                    timestampfloop += gatee.delayofgate; // Update timestamp
                }
            }
        }
        bool gateOutput = getOutput(inputss, gatee.type);
        currentStates[gatee.output] = gateOutput;
        intermediateValues.push_back(IntermediateValue(timestampfloop + gatee.delayofgate, gatee.output, gateOutput));
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
    for (int i = 0; i < inputs.size(); i++)
    {
        if (dataVector[i].timestamp != 0)
            dataVector.insert(dataVector.begin() + i, IntermediateValue(0, inputs[i], 0));
    }
    for (int i = 0; i < inputs.size(); i++)
    {
        if (dataVector[i].timestamp == 0 && dataVector[i].value == 0 && dataVector[i].variable != inputs[i])
            dataVector.insert(dataVector.begin() + i, IntermediateValue(0, inputs[i], 0));
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

    std::set<IntermediateValue> uniqueIntermediateValues;
    for (int i = 0; i < dataVector.size(); i++)
    {

        uniqueIntermediateValues.insert(dataVector[i]);
    }

    // Insert intermediate values into the set to remove duplicates
    for (int i = 0; i < intermediateValues.size(); i++)
    {
        uniqueIntermediateValues.insert(intermediateValues[i]);
    }
    ofstream outfile(pathname);
    // Output intermediate values to file
    for (const auto &value : uniqueIntermediateValues)
    {
        outfile << value.timestamp << "," << value.variable << "," << value.value << endl;
    }
}
