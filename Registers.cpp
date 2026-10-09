/*
 * File:        Registers.cpp
 * Written by:  NAME:ALARRAJ MOHAMMED   ID:253UC256CC
 * Description: Defines the base Registers class and the GeneralRegister
 *              subclass used to represent the CPU's general-purpose registers.
 */

#include "Registers.h"

// using member init list to set Reg to 0 right away, no garbage values at start
Registers::Registers() : Reg(0)
{
}

void Registers::setReg(signed char value)
{
    Reg = value;
}

signed char Registers::getReg() const
{
    return Reg;
}

// nothing extra to set up here, just calling the base constructor
GeneralRegister::GeneralRegister() : Registers()
{
}