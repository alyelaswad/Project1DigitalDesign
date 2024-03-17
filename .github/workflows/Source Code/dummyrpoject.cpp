#include <iostream>
#include "cir_reader.cpp"
#include "LogicGates.cpp"
int main()
{
    CircuitReader circuit;
    circuit.accessLibFile("DD1.lib");
    // circuit.readvectorgate();
    circuit.accessCirFile("circ5.cir");
    circuit.accessStimFile("circ_2Var.stim");

    return 0;
}