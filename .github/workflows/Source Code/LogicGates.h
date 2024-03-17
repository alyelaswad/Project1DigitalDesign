#ifndef LOGICGATES_H
#define LOGICGATES_H
class LogicGates
{
public:
    bool AND(bool, bool); // Computes the 2 input AND gate

    bool OR(bool, bool); // Computes the 2 input OR gate

    bool NOT(bool); // Computes the NOT gate

    bool NAND(bool, bool); // Computes the two input NAND gate

    bool NOR(bool, bool); // Computes the two input NOR gate
};

#endif

