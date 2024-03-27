#include <iostream>
#include "cir_reader.cpp"
#include "LogicGates.cpp"
int main()
{
    CircuitReader circuit;
    string pathname;
    ifstream File;
    // cout << "Enter Pathname of the .lib file:\n";
    // cin >> pathname;
    // File.open(pathname);
    // if (!File.is_open())
    // {
    //     cout << "Could not read library file, please recheck the path.\n";
    //     return 1;
    // }
    circuit.accessLibFile("cells.lib");
    // File.close();

    // cout << "Enter Pathname of the .cir file:\n";
    // cin >> pathname;
    // File.open(pathname);
    // if (!File.is_open())
    // {
    //     cout << "Could not read circuit file, please recheck the path.\n";
    //     return 1;
    // }
    circuit.accessCirFile("circ2.cir");
    // File.close();

    // cout << "Enter Pathname of the .stim file:\n";
    // cin >> pathname;
    // File.open(pathname);
    // if (!File.is_open())
    // {
    //     cout << "Could not read stimuli file, please recheck the path.\n";
    //     return 1;
    // }
    circuit.accessStimFile("circ_3Var.stim");
    // File.close();

    circuit.SimulateProgram("simulation.sim");
    return 0;
}
// Infinite circuits
// 0 initlization
// repeating non-changed values