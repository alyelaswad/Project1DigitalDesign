#include <iostream>
#include "cir_reader.cpp"
#include "LogicGates.cpp"
int main()
{
    CircuitReader circuit;
    circuit.accessLibFile("DD1.lib");
    vector<bool> fg={1,1,1};
    cout<<circuit.getOutput(fg,"OR");
    circuit.accessCirFile("circ2.cir");
    circuit.accessStimFile("circ_3Var.stim");
    circuit.SimulateProgram("simulation.sim");
    return 0;
}
