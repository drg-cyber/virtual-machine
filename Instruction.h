/*
 * File:        Instruction.h
 * Written by:  NAME:AHMED MOHAMED DAFAALLA AHMED           ID:253UC255PR   (ShiftInstruction)
 *              NAME:MOHAMED ELMAKKI SHADAD MOHAMED MAHMUD  ID:253UC255FL   (ShiftInstruction)
 *              NAME:ALARRAJ MOHAMMED                       ID:253UC256CC   (MoveInstruction)
 *              NAME:TEH EN TONG                            ID:253UC245PN   (ArithmeticInstruction, IOInstruction, StackFlagInstruction)
 * Description: Declares the abstract Instruction base class and its five
 *              subclasses, each handling a different group of VM opcodes.
 */

#pragma once

#include <iostream>

#include "MyException.h"
#include "OpCode.h"
#include "CPU.h"

//=========================================================
// PARTS:-
//      Instruction base class
//      toBinary algorithem
//      toDecimal algorithem
//      rotateLeft algorithem
//      rotateRight algorithem
// DONE BY:-
//      NAME:AHMED MOHAMED DAFAALLA AHMED   ID:253UC255PR
//===========================================================

// abstract base class for all instructions, pure virtual execute() means
// this can't be instantiated on its own, only through the subclasses below
class Instruction
{
protected:
    OpCode opcode; // which operation this instruction represents

public:
    Instruction(OpCode op);
    virtual void execute(CPU &cpu) = 0; // every instruction has to define how it runs
    virtual ~Instruction(); // virtual so deleting through a base pointer works properly
};

// handles ROL, ROR, SHL, SHR
class ShiftInstruction : public Instruction
{
private:
    int destReg;
    int count; // how many bits to shift/rotate by

    // helpers for converting between decimal and bit array form, needed since
    // we do the shifting/rotating manually bit by bit
    void toBinary(int decimal, int bits[8]);
    signed char toDecimal(int bits[8]);

    void rotateLeft(int bits[8], int count);
    void rotateRight(int bits[8], int count);

//=========================================================
// PARTS:-
//      shiftLeft algorithem
//      shiftRight algorithem
//      execute method
// DONE BY:-
//      NAME:MOHAMED ELMAKKI SHADAD MOHAMED MAHMUD  ID:253UC255FL
//===========================================================

    void shiftLeft(int bits[8], int count);
    void shiftRight(int bits[8], int count);

public:
    ShiftInstruction(OpCode op, int destReg, int count);

    void execute(CPU &cpu) override;
};

//=========================================================
// PARTS:-
//      MoveInstruction class
// DONE BY:-
//      NAME:ALARRAJ MOHAMMED   ID:253UC256CC
//===========================================================

// handles MOV, LOAD, STORE — mode decides where the operand actually comes from
class MoveInstruction : public Instruction
{
private:
    int destReg;
    int operand;
    AddressMode mode;

public:
    MoveInstruction(OpCode op, int destReg, int operand, AddressMode mode);

    void execute(CPU &cpu) override;
};

//=========================================================
// PARTS:-
//      ArithmeticInstruction class
//      IOInstruction class
//      StackFlagInstruction class
// DONE BY:-
//      NAME:TEH EN TONG    ID:253UC245PN
//===========================================================

// handles ADD, SUB, MUL, DIV, INC, DEC
class ArithmeticInstruction : public Instruction
{
private:
    int destReg;
    int srcReg;
    bool isUnary; // true for INC/DEC since those only need one register
    bool isImmediateOperand;
public:
    // binary version, e.g. ADD destReg, srcReg
    ArithmeticInstruction(OpCode op, int destReg, int srcReg);

    // unary version, e.g. INC destReg
    ArithmeticInstruction(OpCode op, int destReg);

    ArithmeticInstruction(OpCode op, int destReg, int immediateValue, bool isImmediate);

    void execute(CPU &cpu) override;
};

// handles INPUT and DISPLAY
class IOInstruction : public Instruction
{
private:
    int reg;

public:
    IOInstruction(OpCode op, int reg);

    void execute(CPU &cpu) override;
};

// handles PUSH, POP, RESET — two constructors since PUSH/POP need a register
// but RESET works on a flag instead
class StackFlagInstruction : public Instruction
{
private:
    int reg;
    FlagType flag;

public:
    // for PUSH/POP
    StackFlagInstruction(OpCode op, int reg);

    // for RESET
    StackFlagInstruction(OpCode op, FlagType flag);

    void execute(CPU &cpu) override;
};