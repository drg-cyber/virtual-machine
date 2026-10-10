/*
 * File:        Memory.h
 * Written by:  NAME:ALARRAJ MOHAMMED   ID:253UC256CC
 * Description: Represents the VM's 64-byte main memory, with bounds-checked
 *              read/write access.
 */

#pragma once

#include "MyException.h"

// represents the VM's memory, flat 1D array under the hood
// even though the spec shows it as 2D for display purposes
class Memory
{
private:
    static const int m_size = 64; // fixed size memory, 64 cells
    signed char M[m_size];         // the actual memory array

public:
    Memory();

    void initMem(); // clears/resets all memory cells

    signed char getMemory(int index) const; // read a value at a given index

    void setMemory(int index, signed char value); // write a value at a given index
};