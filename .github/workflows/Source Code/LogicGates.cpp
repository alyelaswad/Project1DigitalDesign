#include "LogicGates.h"
bool LogicGates::AND(bool A, bool B)
{
    return A && B;
}
bool LogicGates::OR(bool A, bool B)
{
    return A || B;
}
bool LogicGates::NOT(bool A)
{
    return !A;
}
bool LogicGates::NAND(bool A, bool B)
{
    return NOT(AND(A, B));
}
bool LogicGates::NOR(bool A, bool B)
{
    return NOT(OR(A, B));
}