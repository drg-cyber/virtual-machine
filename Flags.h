/*
 * File:        Flags.h
 * Written by:  NAME:ALARRAJ MOHAMMED   ID:253UC256CC
 * Description: Represents the CPU's OF/UF/CF/ZF flag bits and the logic for
 *              updating them after arithmetic operations.
 */

#pragma once
#include <iostream>

// order here matches the flag order used in the CPU display (Overflow, Underflow, Carry, Zero)
enum class FlagType
{
    OF,
    UF,
    CF,
    ZF
};

class Flags
{
private:
    bool O, U, C, Z; // one bool per flag, keeping short names since they map 1-1 with FlagType

public:
    Flags();

    void initFlags(); // sets all flags back to false

    void resetFlags(FlagType flag); // clears just one specific flag

    bool getFlags(FlagType flag) const; // returns the current state of a given flag

    void setFlags(FlagType flag, bool F); // manually sets a flag to true/false

    void checkFlags(int result); // works out OF/UF/ZF/CF all at once based on an arithmetic result
};