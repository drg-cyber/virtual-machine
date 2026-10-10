/*
 * File:        OpCode.h
 * Written by:  NAME:AHMED MOHAMED DAFAALLA AHMED   ID:253UC255PR
 * Description: Defines the OpCode and AddressMode enums used across all
 *              instruction classes.
 */

// grouped by which instruction subclass handles them, matches how we split up Instruction.h

#pragma once

enum class OpCode {
    // MoveInstruction
    MOV, LOAD, STORE,
    // ArithmeticInstruction
    ADD, SUB, MUL, DIV, INC, DEC,
    // ShiftInstruction
    ROL, ROR, SHL, SHR,
    // IOInstruction
    INPUT, DISPLAY,
    // StackFlagInstruction
    PUSH, POP, RESET
};

// the different ways an operand can be given in an instruction, from the spec
enum class AddressMode { 
    IMMEDIATE,          // value is given directly
    REGISTER,           // value is in a register
    REGISTER_INDIRECT,  // register holds an address, value is at that address
    DIRECT              // address is given directly
};