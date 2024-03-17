#include <iostream>
#include "cir_reader.h"

int main()
{
    CircuitReader  circuit;
    circuit.accessLibFile("text1.txt");
    circuit.accessCirFile("text2.txt");
 

    return 0;
}
