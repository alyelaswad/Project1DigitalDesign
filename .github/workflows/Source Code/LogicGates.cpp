#include "LogicGates.h"
bool LogicGates::AND(bool A, bool B)
{
    return A && B; // Using the AND method embedded in the c++ language
}
bool LogicGates::OR(bool A, bool B)
{
    return A || B; // Using the OR method embedded in the c++ language
}
bool LogicGates::NOT(bool A)
{
    return !A; // Using the NOT method embedded in the c++ language
}
bool LogicGates::NAND(bool A, bool B)
{
    return NOT(AND(A, B)); // Calls NOT after AND gates to achieve NAND
}
bool LogicGates::NOR(bool A, bool B)
{
    return NOT(OR(A, B));  // Calls NOT after OR gates to achieve NOR
}
