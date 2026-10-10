/*
 * File:        CPU.h
 * Written by:  NAME:AHMED MOHAMED DAFAALLA AHMED   ID:253UC255PR
 * Description: Central class that ties together the registers, flags,
 *              memory, and stack, and exposes a single interface for
 *              instructions to operate on.
 */

#pragma once

#include "MyException.h"
#include "Registers.h"
#include "Flags.h"
#include "Memory.h"
#include "MyStack.h"

// this ties together all the hardware pieces (registers, flags, memory, stack)
// and gives the instructions a single interface to work with instead of touching
// each piece directly
class CPU
{
private:
    GeneralRegister R[8]; // the 8 general purpose registers, R0 to R7
    Flags flags;
    Memory memory;
    MyStack<signed char> Stack; // fixed size 8 like the spec says

    int PC; // program counter, points to the current instruction
    int SI; // stack index, tracks how many items are on the stack

public:
    CPU();

    void setGeneralRegister(int index, signed char value);

    signed char getGeneralRegister(int index) const;

    bool getFlag(FlagType F) const;

    void setFlag(FlagType F, bool value);

    void resetFlag(FlagType F);

    void checkFlags(int result); // updates all flags at once, passed straight through to Flags

    void storeMemory(int index, signed char value);

    signed char loadMemory(int index) const;

    void pushStack(signed char value);

    signed char popStack();

    int getPC() const;

    void setPC(int value);

    void incPC(); // moves PC forward by one after an instruction runs

    int getSI() const;

    void setSI(int value);
};