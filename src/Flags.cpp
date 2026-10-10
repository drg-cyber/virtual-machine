/*
 * File:        Flags.cpp
 * Written by:  NAME:ALARRAJ MOHAMMED   ID:253UC256CC
 * Description: Represents the CPU's OF/UF/CF/ZF flag bits and the logic for
 *              updating them after arithmetic operations.
 */

#include "Flags.h"
#include <iostream>

// starts with all flags cleared
Flags::Flags()
{
    initFlags();
}

void Flags::initFlags() { O = U = C = Z = false; }

void Flags::resetFlags(FlagType flag)
{
    switch (flag)
    {
    case FlagType::OF:
        O = false;
        break;
    case FlagType::UF:
        U = false;
        break;
    case FlagType::CF:
        C = false;
        break;
    case FlagType::ZF:
        Z = false;
        break;
    }
}

bool Flags::getFlags(FlagType flag) const
{
    switch (flag)
    {
    case FlagType::OF:
        return O;

    case FlagType::UF:
        return U;

    case FlagType::CF:
        return C;

    case FlagType::ZF:
        return Z;
    }
    return false; // shouldn't ever hit this, just here so the compiler doesn't complain
}

void Flags::setFlags(FlagType flag, bool F)
{
    switch (flag)
    {
    case FlagType::OF:
        this->O = F;
        break;

    case FlagType::UF:
        this->U = F;
        break;

    case FlagType::CF:
        this->C = F;
        break;

    case FlagType::ZF:
        this->Z = F;
        break;
    }
}

// one function that updates all 4 flags at once based on the result of an arithmetic op
// carry flag is basically true whenever overflow or underflow happens
void Flags::checkFlags(int result)
{
    setFlags(FlagType::OF, result > 127);
    setFlags(FlagType::UF, result < -128);
    setFlags(FlagType::CF, result > 127 || result < -128);
    setFlags(FlagType::ZF, result == 0);
}