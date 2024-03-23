#include <iostream>
#include "cir_reader.cpp"
#include "LogicGates.cpp"
int main()
{
    CircuitReader circuit;
    circuit.accessLibFile("DD1.lib");
    circuit.accessCirFile("circ2.cir");
    circuit.accessStimFile("circ_3Var.stim");
    circuit.SimulateProgram("simulation.sim");
    return 0;
}
