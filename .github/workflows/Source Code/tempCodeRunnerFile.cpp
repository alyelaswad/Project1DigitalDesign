#include <iostream>
#include "cir_reader.cpp"
#include "LogicGates.cpp"
int main()
{
    CircuitReader circuit;
    circuit.accessLibFile("DD1.lib");
    circuit.readvectorgate();
    vector<bool> fg={1,1,1};
    cout<<circuit.getOutput(fg,"OR");
    circuit.accessCirFile("circ5.cir");
    circuit.accessStimFile("circ_2Var.stim");
    circuit.SimulateProgram("simulation.sim");
    return 0;
}
