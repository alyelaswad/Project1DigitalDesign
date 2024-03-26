#include <iostream>
#include "cir_reader.cpp"
#include "LogicGates.cpp"
int main()
{
    CircuitReader circuit;
    circuit.accessLibFile("DD1.lib");
    circuit.accessCirFile("circ5.cir");
    circuit.accessStimFile("circ_2Var.stim");
    circuit.SimulateProgram("simulation.sim");
    return 0;
}
