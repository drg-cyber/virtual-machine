/*
 * File:        Memory.cpp
 * Written by:  NAME:ALARRAJ MOHAMMED   ID:253UC256CC
 * Description: Represents the VM's 64-byte main memory, with bounds-checked
 *              read/write access.
 */

#include "Memory.h"

// constructor just zeroes out memory right away so we don't start with garbage
Memory::Memory()
{
    initMem();
}

void Memory::initMem()
{
    for (int i = 0; i < m_size; i++)
        M[i] = 0;
}

// bounds check before returning, throws if index is out of range
signed char Memory::getMemory(int index) const
{
    if (index >= 0 && index < m_size)
        return M[index];

    throw MyException("MEMORY INDEX NOT FOUND!");
}

// same bounds check as getMemory, just for writing instead
void Memory::setMemory(int index, signed char value)
{
    if (index >= 0 && index < m_size)
        M[index] = value;
    else
        throw MyException("MEMORY INDEX NOT FOUND!");
}