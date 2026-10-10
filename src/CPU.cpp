/*
 * File:        CPU.cpp
 * Written by:  NAME:AHMED MOHAMED DAFAALLA AHMED   ID:253UC255PR
 * Description: Central class that ties together the registers, flags,
 *              memory, and stack, and exposes a single interface for
 *              instructions to operate on.
 */

#include "CPU.h"

// Stack needs a size passed in since MyStack has no default constructor,
// that's why it's in the member init list
CPU::CPU()
    : Stack(8), PC(0), SI(0)
{
}

void CPU::setGeneralRegister(int index, signed char value)
{
    if (index >= 0 && index < 8)
        R[index].setReg(value);
    else
        throw MyException("NOT FOUND!");
}

signed char CPU::getGeneralRegister(int index) const
{
    if (index >= 0 && index < 8)
        return R[index].getReg();

    throw MyException("NOT FOUND!");
}

bool CPU::getFlag(FlagType F) const
{
    return flags.getFlags(F);
}

void CPU::setFlag(FlagType F, bool value)
{
    flags.setFlags(F, value);
}

void CPU::resetFlag(FlagType F)
{
    flags.resetFlags(F);
}

void CPU::checkFlags(int result)
{
    flags.checkFlags(result);
}

void CPU::storeMemory(int index, signed char value)
{
    memory.setMemory(index, value);
}

signed char CPU::loadMemory(int index) const
{
    return memory.getMemory(index);
}

// SI tracks how many items are on the stack, so it goes up on push
void CPU::pushStack(signed char value)
{
    Stack.push(value);
    SI++;
}

// and down on pop
signed char CPU::popStack()
{
    signed char val = Stack.pop();

    SI--;

    return val;
}

int CPU::getPC() const
{
    return PC;
}

void CPU::setPC(int value)
{
    PC = value;
}

void CPU::incPC()
{
    PC++;
}

int CPU::getSI() const
{
    return SI;
}

void CPU::setSI(int value)
{
    SI = value;
}