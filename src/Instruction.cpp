/*
 * File:        Instruction.cpp
 * Written by:  NAME:AHMED MOHAMED DAFAALLA AHMED           ID:253UC255PR   (ShiftInstruction)
 *              NAME:MOHAMED ELMAKKI SHADAD MOHAMED MAHMUD  ID:253UC255FL   (ShiftInstruction)
 *              NAME:ALARRAJ MOHAMMED                       ID:253UC256CC   (MoveInstruction)
 *              NAME:TEH EN TONG                            ID:253UC245PN   (ArithmeticInstruction, IOInstruction, StackFlagInstruction)
 * Description: Implements the abstract Instruction base class and its five
 *              subclasses, each handling a different group of VM opcodes.
 */

//=========================================================
// PARTS:-
//      Instruction base class
//      rotateLeft algorithem
//      rotateRight algorithem
// DONE BY:-
//      NAME:AHMED MOHAMED DAFAALLA AHMED   ID:253UC255PR
//===========================================================
#include "Instruction.h"

Instruction::Instruction(OpCode op)
    : opcode(op)
{
}

Instruction::~Instruction()
{
}

// ====================== ShiftInstruction ======================

ShiftInstruction::ShiftInstruction(OpCode op, int destReg, int count)
    : Instruction(op), destReg(destReg), count(count)
{
}

// converts a signed decimal number into an 8 bit array, bits[0] is MSB
// adding 256 for negatives so we get the correct two's complement bits
void ShiftInstruction::toBinary(int decimal, int bits[8])
{
    if (decimal < 0)
        decimal += 256;

    for (int i = 7; i >= 0; i--)
    {
        bits[i] = decimal % 2;
        decimal /= 2;
    }
}

// does the opposite of toBinary, turns the bit array back into a signed char
// subtracting 256 if it's over 127 so it wraps back into signed range
signed char ShiftInstruction::toDecimal(int bits[8])
{
    int decimal = 0;

    for (int i = 0; i < 8; i++)
        decimal = decimal * 2 + bits[i];

    if (decimal > 127)
        decimal -= 256;

    return (signed char)decimal;
}

// rotates the bits left by count, bits that fall off the front wrap to the back
void ShiftInstruction::rotateLeft(int bits[8], int count)
{
    count %= 8; // handles if count is >= 8
                // e.g, if cout is 9 it becomes 1

    for (int j = 0; j < count; j++)
    {
        int firstBit = bits[0];

        for (int i = 0; i < 7; i++)
            bits[i] = bits[i + 1];

        bits[7] = firstBit;
    }
}

// same idea as rotateLeft but the other direction
void ShiftInstruction::rotateRight(int bits[8], int count)
{
    count %= 8;

    for (int j = 0; j < count; j++)
    {
        int lastBit = bits[7];

        for (int i = 7; i > 0; i--)
            bits[i] = bits[i - 1];

        bits[0] = lastBit;
    }
}

//=========================================================
// PARTS:-
//      toBinary algorithem
//      toDecimal algorithem
//      shiftLeft algorithem
//      shiftRight algorithem
//      execute method
// DONE BY:-
//      NAME:MOHAMED ELMAKKI SHADAD MOHAMED MAHMUD  ID:253UC255FL
//===========================================================

// shift left, bits pushed out the front are gone for good and zeros fill in from the back
void ShiftInstruction::shiftLeft(int bits[8], int count)
{
    int original[8];

    for (int i = 0; i < 8; i++)
        original[i] = bits[i];

    for (int i = 0; i < 8; i++)
        bits[i] = (i + count <= 7) ? original[i + count] : 0;
}

// shift right, same as above but mirrored
void ShiftInstruction::shiftRight(int bits[8], int count)
{
    int original[8];

    for (int i = 0; i < 8; i++)
        original[i] = bits[i];

    for (int i = 0; i < 8; i++)
        bits[i] = (i - count >= 0) ? original[i - count] : 0;
}

// grabs the register value, converts to bits, does whichever operation matches
// the opcode, then converts back and writes it into the register
void ShiftInstruction::execute(CPU &cpu)
{
    signed char current = cpu.getGeneralRegister(destReg);

    int bits[8];

    toBinary(current, bits);

    switch (opcode)
    {
    case OpCode::ROL:
        rotateLeft(bits, count);
        break;

    case OpCode::ROR:
        rotateRight(bits, count);
        break;

    case OpCode::SHL:
        shiftLeft(bits, count);
        break;

    case OpCode::SHR:
        shiftRight(bits, count);
        break;
    }

    cpu.setGeneralRegister(destReg, toDecimal(bits));
}

//=========================================================
// PARTS:-
//      MoveInstruction class
// DONE BY:-
//      NAME:ALARRAJ MOHAMMED   ID:253UC256CC
//===========================================================

// ====================== MoveInstruction ======================

MoveInstruction::MoveInstruction(OpCode op,
                                 int destReg,
                                 int operand,
                                 AddressMode mode)
    : Instruction(op),
      destReg(destReg),
      operand(operand),
      mode(mode)
{
}

// handles MOV, LOAD, and STORE, behavior changes depending on the address mode
void MoveInstruction::execute(CPU &cpu)
{
    switch (opcode)
    {
    case OpCode::MOV:

        // MOV can take a straight value, a register's value, or a value from
        // the address a register points to
        if (mode == AddressMode::IMMEDIATE)
        {
            int value = operand;
            cpu.setGeneralRegister(destReg, (signed char)value);
        }
        else if (mode == AddressMode::REGISTER)
        {
            int value = cpu.getGeneralRegister(operand);
            cpu.setGeneralRegister(destReg, (signed char)value);
        }
        else if (mode == AddressMode::REGISTER_INDIRECT)
        {
            int targetAddress = cpu.getGeneralRegister(operand);
            int value = cpu.loadMemory(targetAddress);
            cpu.setGeneralRegister(destReg, (signed char)value);
        }

        break;

    case OpCode::LOAD:

        // LOAD only makes sense from memory, either a direct address or
        // through a register holding the address
        if (mode == AddressMode::DIRECT)
        {
            int value = cpu.loadMemory(operand);
            cpu.setGeneralRegister(destReg, (signed char)value);
        }
        else if (mode == AddressMode::REGISTER_INDIRECT)
        {
            int targetAddress = cpu.getGeneralRegister(operand);
            int value = cpu.loadMemory(targetAddress);
            cpu.setGeneralRegister(destReg, (signed char)value);
        }

        break;

    case OpCode::STORE:

        // STORE is the reverse of LOAD, writes destReg's value into memory
        if (mode == AddressMode::DIRECT)
        {
            cpu.storeMemory(operand,
                            cpu.getGeneralRegister(destReg));
        }
        else if (mode == AddressMode::REGISTER_INDIRECT)
        {
            int targetAddress = cpu.getGeneralRegister(operand);

            cpu.storeMemory(targetAddress,
                            cpu.getGeneralRegister(destReg));
        }

        break;

    default:
        // shouldn't ever hit this unless something's really wrong upstream
        throw MyException(
            "MoveInstruction received an unrecognised opcode");
    }
}

//=========================================================
// PARTS:-
//      ArithmeticInstruction class
//      IOInstruction class
//      StackFlagInstruction class
// DONE BY:-
//      NAME:TEH EN TONG    ID:253UC245PN
//===========================================================

// ====================== ArithmeticInstruction ======================

// binary version, used for ADD/SUB/MUL/DIV where we need two registers
ArithmeticInstruction::ArithmeticInstruction(OpCode op,
                                             int destReg,
                                             int srcReg)
    : Instruction(op),
      destReg(destReg),
      srcReg(srcReg),
      isUnary(false),
      isImmediateOperand(false)
{
}

// unary version, used for INC/DEC, srcReg set to -1 since it's unused
ArithmeticInstruction::ArithmeticInstruction(OpCode op,
                                             int destReg)
    : Instruction(op),
      destReg(destReg),
      srcReg(-1),
      isUnary(true),
      isImmediateOperand(false)
{
}

ArithmeticInstruction::ArithmeticInstruction(OpCode op, int destReg, int immediateValue, bool isImmediate)
    : Instruction(op), destReg(destReg), srcReg(immediateValue),
      isUnary(false), isImmediateOperand(isImmediate)
{
}

void ArithmeticInstruction::execute(CPU &cpu)
{
    // sanity check, makes sure whoever built this instruction used the right
    // constructor for the opcode (e.g. didn't call the binary one with INC)
    bool opcodeIsUnary =
        (opcode == OpCode::INC || opcode == OpCode::DEC);

    if (opcodeIsUnary != isUnary)
    {
        throw MyException(
            "ArithmeticInstruction constructed with mismatched opcode/arity");
    }

    int destVal = cpu.getGeneralRegister(destReg);
    int result = 0;
    int srcVal = isImmediateOperand ? srcReg : (isUnary ? 0 : cpu.getGeneralRegister(srcReg));

    switch (opcode)
    {
    case OpCode::ADD:
        result = destVal + srcVal;
        break;

    case OpCode::SUB:
        result = destVal - srcVal;
        break;

    case OpCode::MUL:
        result = destVal * srcVal;
        break;

    case OpCode::DIV:
    {
        if (srcVal == 0)
            throw MyException("Division by zero");

        result = destVal / srcVal;
        break;
    }

    case OpCode::INC:
        result = destVal + 1;
        break;

    case OpCode::DEC:
        result = destVal - 1;
        break;

    default:
        throw MyException(
            "ArithmeticInstruction received an unrecognised opcode");
    }

    // update flags based on the result before truncating it back to a signed char
    cpu.checkFlags(result);
    cpu.setGeneralRegister(destReg, (signed char)result);
}

// ====================== IOInstruction ======================

IOInstruction::IOInstruction(OpCode op, int reg)
    : Instruction(op),
      reg(reg)
{
}

void IOInstruction::execute(CPU &cpu)
{
    switch (opcode)
    {
    case OpCode::INPUT:
    {
        std::cout << "?";

        int value;
        std::cin >> value;

        // checking overflow/underflow/zero on the raw input before it gets
        // truncated into a signed char
        cpu.setFlag(FlagType::OF, value > 127);
        cpu.setFlag(FlagType::UF, value < -128);
        cpu.setFlag(FlagType::ZF, value == 0);

        cpu.setGeneralRegister(reg, (signed char)value);
        break;
    }

    case OpCode::DISPLAY:
        std::cout << (int)cpu.getGeneralRegister(reg)
                  << std::endl;
        break;

    default:
        throw MyException(
            "IOInstruction received an unrecognised opcode");
    }
}

// ====================== StackFlagInstruction ======================

// used for PUSH/POP, flag defaults to ZF but it's not actually used in this case
StackFlagInstruction::StackFlagInstruction(OpCode op,
                                           int reg)
    : Instruction(op),
      reg(reg),
      flag(FlagType::ZF)
{
}

// used for RESET, reg set to -1 since we're working with a flag instead
StackFlagInstruction::StackFlagInstruction(OpCode op,
                                           FlagType flag)
    : Instruction(op),
      reg(-1),
      flag(flag)
{
}

void StackFlagInstruction::execute(CPU &cpu)
{
    switch (opcode)
    {
    case OpCode::PUSH:
        cpu.pushStack(cpu.getGeneralRegister(reg));
        break;

    case OpCode::POP:
        cpu.setGeneralRegister(reg,
                               cpu.popStack());
        break;

    case OpCode::RESET:
        cpu.resetFlag(flag);
        break;

    default:
        throw MyException(
            "StackFlagInstruction received an unrecognised opcode");
    }
}