#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "MyVector.h"
#include "Instruction.h"
#include "CPU.h"
#include "OpCode.h"
#include "Flags.h"
#include "MyException.h"

class Runner
{
private:
    
    struct Token
    {
        std::string text;
        bool wasBracketed;
    };

    
    MyVector<std::string> rawLines;
    MyVector<Instruction*> program;
    CPU cpu;

    
    MyVector<Token> tokenize(const std::string &line);
    OpCode stringToOpCode(const std::string &mnemonic);

    bool isRegisterToken(const Token &tok);
    int  registerIndexFromToken(const Token &tok);
    bool isFlagToken(const Token &tok);
    FlagType flagFromToken(const Token &tok);

    Instruction* decodeLine(const std::string &line, int lineNumber);

    
    std::string formatField(int value) const;
    void writeDump(std::ostream &out) const;

public:
    Runner();
    ~Runner();

    void loadProgram(const std::string &filename);
    void decodeAll();
    void runAll();
    void dump(const std::string &outputFilename) const;
    void runAllTraced(std::ostream &out = std::cout);
};