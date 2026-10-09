
//=================== Work Contribution ========================
//NAME:AHMED MOHAMED DAFAALLA AHMED	ID:253UC255PR
//work: MyVector class\file
//      MyStack class\file
//	    MyQueue class\file
//	    MyException class\file
//	    CPU class\file
//      Instruction base class
//      rotateLeft algorithm
//      rotateRight algorithm
//	    OpCode file
//
//--------------------------------------------------------------
//NAME:MOHAMED ELMAKKI SHADAD MOHAMED MAHMUD	ID:253UC255FL
//work: ShiftInstruction class
//	    Runner class\file
//      toBinary algorithm
//      toDecimal algorithm
//      shiftLeft algorithm
//      shiftRight algorithm
//      execute method
//
//-------------------------------------------------------------- 
//NAME:ALARRAJ MOHAMMED    ID:253UC256CC
//work: Memory class\file
//	    Flags class\file
//	    MoveInstruction class
//	    Registers file
//
//--------------------------------------------------------------   
//NAME:TEH EN TONG	ID:253UC245PN
//      ArithmeticInstruction class
//      IOInstruction class
//      StackFlagInstruction class
//
//==============================================================

#include <iostream>
#include "Runner.h"
#include "MyException.h"

int main()
{
    Runner runner;

    try
    {
        runner.loadProgram("program.asm");
        runner.decodeAll();
        runner.runAllTraced();
        runner.dump("output.txt");
    }
    catch (MyException &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}