#include <iostream>
#include "cir_reader.cpp"
#include "LogicGates.cpp"
int main()
{
    CircuitReader circuit;
    circuit.accessLibFile("DD1.lib");
    // circuit.readvectorgate();
    circuit.accessCirFile("circ2.cir");

    cout << endl
         << circuit.getOutput(1, 1, "AND2");

    return 0;
}
