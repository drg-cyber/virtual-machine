/*
 * File:        Registers.h
 * Written by:  NAME:ALARRAJ MOHAMMED   ID:253UC256CC
 * Description: Defines the base Registers class and the GeneralRegister
 *              subclass used to represent the CPU's general-purpose registers.
 */

#pragma once

class Registers
{
protected:
    signed char Reg; // holds the value stored in this register, signed char since registers are 8-bit

public:
    Registers(); // default constructor, sets Reg to some starting value

    void setReg(signed char value); // mutator, updates the register's value

    signed char getReg() const; // accessor, just returns what's in Reg
};

// GeneralRegister inherits from Registers since it IS a register, just used for general purpose data
class GeneralRegister : public Registers
{
public:
    GeneralRegister();
};