#include "Runner.h"

// ====================== Constructor / Destructor ======================

Runner::Runner()
{
}

Runner::~Runner()
{

    // Release all dynamically allocated Instruction objects
    // stored inside the program container to prevent memory leaks.
    program.deleteAll();
}

// ====================== File Loading ======================

void Runner::loadProgram(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
        throw MyException("Could not open file: " + filename);  //Throw an exception when the file doesn't open

    std::string line;

    while (std::getline(file, line))
    {
        if (!line.empty())
            rawLines.push_back(line);     // Store each non-empty assembly instruction line
    }

    file.close();      
}

// ====================== Tokenizing ======================

MyVector<Runner::Token> Runner::tokenize(const std::string &line)
{
    MyVector<Token> tokens;        
    size_t pos = 0;

    while (pos < line.size())
    {
        while (pos < line.size() && (line[pos] == ' ' || line[pos] == ','))   // Skip delimiters between operands
            pos++;

        if (pos >= line.size())
            break;

        bool bracketed = false;     // Detect memory addressing syntax

        if (line[pos] == '[')
        {
            bracketed = true;      // Skip the opening '[' and remember that
            pos++;
        }

        size_t start = pos;
        size_t end = line.find_first_of(" ,]", pos);      // Find the end of the current token

        if (end == std::string::npos)
            end = line.size();

        std::string text = line.substr(start, end - start);
        pos = end;

        if (pos < line.size() && line[pos] == ']')
            pos++;

        tokens.push_back(Token{ text, bracketed });      // Save both the operand text and whether it originally appeared inside brackets
    }

    return tokens;
}

// ====================== Mnemonic -> OpCode ======================

OpCode Runner::stringToOpCode(const std::string &mnemonic)
{
    if (mnemonic == "MOV")
        return OpCode::MOV;
    else if (mnemonic == "LOAD")
        return OpCode::LOAD;
    else if (mnemonic == "STORE")
        return OpCode::STORE;
    else if (mnemonic == "ADD")
        return OpCode::ADD;
    else if (mnemonic == "SUB")
        return OpCode::SUB;
    else if (mnemonic == "MUL")         //--------------------------------------------------------------
        return OpCode::MUL;
    else if (mnemonic == "DIV")
        return OpCode::DIV;
    else if (mnemonic == "INC")
        return OpCode::INC;
    else if (mnemonic == "DEC")         //This section checks the instruction and return it to OpCode
        return OpCode::DEC;
    else if (mnemonic == "ROL")
        return OpCode::ROL;
    else if (mnemonic == "ROR")
        return OpCode::ROR;
    else if (mnemonic == "SHL")         //---------------------------------------------------------------
        return OpCode::SHL;
    else if (mnemonic == "SHR")
        return OpCode::SHR;
    else if (mnemonic == "INPUT")
        return OpCode::INPUT;
    else if (mnemonic == "DISPLAY")
        return OpCode::DISPLAY;
    else if (mnemonic == "PUSH")
        return OpCode::PUSH;
    else if (mnemonic == "POP")
        return OpCode::POP;
    else if (mnemonic == "RESET")
        return OpCode::RESET;
    else
        throw MyException("Invalid instruction: " + mnemonic);
}

// ====================== Operand Classification Helpers ======================

bool Runner::isRegisterToken(const Token &tok)
{
    return !tok.text.empty() && tok.text[0] == 'R';     // Register operands always begin with 'R'
}

int Runner::registerIndexFromToken(const Token &tok)
{
    if (!isRegisterToken(tok))
        throw MyException("Expected a register token: " + tok.text);    

    return std::stoi(tok.text.substr(1));       // Convert register text into its numerical index
}

bool Runner::isFlagToken(const Token &tok)
{
    return tok.text == "CF" || tok.text == "OF"         
        || tok.text == "UF" || tok.text == "ZF";     // Check whether the token represents one of the four valid CPU flags
}

FlagType Runner::flagFromToken(const Token &tok)
{
    if (tok.text == "CF") return FlagType::CF;
    else if (tok.text == "OF") return FlagType::OF;       
    else if (tok.text == "UF") return FlagType::UF;
    else if (tok.text == "ZF") return FlagType::ZF;
    else throw MyException("Invalid flag: " + tok.text);
}

// ====================== Line Decoding ======================


// Break the assembly statement into individual tokens.
Instruction* Runner::decodeLine(const std::string &line, int lineNumber)
{
    MyVector<Token> tokens = tokenize(line);

    if (tokens.size() == 0)
        throw MyException("Empty instruction on line " + std::to_string(lineNumber));

        // First token is always the mnemonic.
        OpCode op = stringToOpCode(tokens[0].text);     // Convert the text instruction into an OpCode enum.
   
        /* MOV, LOAD and STORE support multiple addressing modes:
        Immediate, Register, Direct memory, Register-indirect memory */ 
    if (op == OpCode::MOV || op == OpCode::LOAD || op == OpCode::STORE)
    {
        if (tokens.size() != 3)
            throw MyException("Wrong operand count on line " + std::to_string(lineNumber));

         /* t1 and t2 represent the two operands.
         Their meaning depends on the instruction
         and the addressing mode being used.   */ 
        Token &t1 = tokens[1];
        Token &t2 = tokens[2];

        if (op == OpCode::STORE)
        {
            /* Store the value in R1 into the memory
             address contained inside R2 */
            if (t1.wasBracketed)
            {
                int destReg = registerIndexFromToken(t2);
                int operand = registerIndexFromToken(t1);
                return new MoveInstruction(op, destReg, operand, AddressMode::REGISTER_INDIRECT);
            }

            // Store the value in R1 directly into memory location 43  
            if (isRegisterToken(t1) && !isRegisterToken(t2))
            {
                int destReg = registerIndexFromToken(t1);
                int operand = std::stoi(t2.text);
                return new MoveInstruction(op, destReg, operand, AddressMode::DIRECT);
            }

            // STORE 20, R3  ->  value = R3 (t2), address = 20 (t1)   
            if (!isRegisterToken(t1) && isRegisterToken(t2))
            {
                int destReg = registerIndexFromToken(t2);
                int operand = std::stoi(t1.text);
                return new MoveInstruction(op, destReg, operand, AddressMode::DIRECT);
            }

            throw MyException("Invalid STORE operands on line " + std::to_string(lineNumber));
        }

        // For MOV and LOAD instructions, the first operand is always the destination register
        int destReg = registerIndexFromToken(t1);
        
        // Brackets indicate memory access
        if (t2.wasBracketed)
        {
            // [R2] means register-indirect addressing, The memory address is stored inside R2
            if (isRegisterToken(t2))
                return new MoveInstruction(op, destReg, registerIndexFromToken(t2), AddressMode::REGISTER_INDIRECT);
            // [20] means direct memory addressing
            else
                return new MoveInstruction(op, destReg, std::stoi(t2.text), AddressMode::DIRECT);
        }
        else
        {
            if (isRegisterToken(t2))
                return new MoveInstruction(op, destReg, registerIndexFromToken(t2), AddressMode::REGISTER);
            else
                return new MoveInstruction(op, destReg, std::stoi(t2.text), AddressMode::IMMEDIATE);
        }
    }

    // Arithmetic instructions support: Register operands & Immediate operands
    if (op == OpCode::ADD || op == OpCode::SUB ||
        op == OpCode::MUL || op == OpCode::DIV)
    {
        if (tokens.size() != 3)
            throw MyException("Wrong operand count on line " + std::to_string(lineNumber));

        int destReg = registerIndexFromToken(tokens[1]);

        // Determine whether the second operand is a register or an immediate value
        if (isRegisterToken(tokens[2]))
        {
            int srcReg = registerIndexFromToken(tokens[2]);
            return new ArithmeticInstruction(op, destReg, srcReg);
        }
        else
        {
            int immediateValue = std::stoi(tokens[2].text);
            return new ArithmeticInstruction(op, destReg, immediateValue, true);
        }
    }

    // INC and DEC operate on only one register
    if (op == OpCode::INC || op == OpCode::DEC)
    {
        if (tokens.size() != 2)
            throw MyException("Wrong operand count on line " + std::to_string(lineNumber));

        int destReg = registerIndexFromToken(tokens[1]);
        return new ArithmeticInstruction(op, destReg);
    }

    // Rotation and shift instructions require: Destination register & Shift/rotation count
    if (op == OpCode::ROL || op == OpCode::ROR ||
        op == OpCode::SHL || op == OpCode::SHR)
    {
        if (tokens.size() != 3)
            throw MyException("Wrong operand count on line " + std::to_string(lineNumber));

        int destReg = registerIndexFromToken(tokens[1]);
        int count   = std::stoi(tokens[2].text);
        return new ShiftInstruction(op, destReg, count);
    }
    
    // Input and output instructions operate on a single register operand
    if (op == OpCode::INPUT || op == OpCode::DISPLAY)
    {
        if (tokens.size() != 2)
            throw MyException("Wrong operand count on line " + std::to_string(lineNumber));

        int reg = registerIndexFromToken(tokens[1]);
        return new IOInstruction(op, reg);
    }

    /* Stack instructions operate on one register
    PUSH: Save register value onto the stack
    POP: Retrieve the top stack value into a register */

    if (op == OpCode::PUSH || op == OpCode::POP)
    {
        if (tokens.size() != 2)
            throw MyException("Wrong operand count on line " + std::to_string(lineNumber));

        int reg = registerIndexFromToken(tokens[1]);
        return new StackFlagInstruction(op, reg);
    }

    // RESET clears one of the CPU flags: CF,ZF,UF,OF
    if (op == OpCode::RESET)
    {
        if (tokens.size() != 2)
            throw MyException("Wrong operand count on line " + std::to_string(lineNumber));

        FlagType flag = flagFromToken(tokens[1]);
        return new StackFlagInstruction(op, flag);
    }

    /* Execution should never reach this point.
       Every supported instruction should have
       been handled in one of the previous blocks */
    throw MyException("decodeLine: unhandled opcode on line " + std::to_string(lineNumber));
}

// ====================== Decode-All / Run-All ======================

/* Decode every raw assembly instruction into a concrete
   Instruction object and store it inside the program container */
void Runner::decodeAll()
{
    // Process each assembly line one at a time
    for (int i = 0; i < rawLines.size(); i++)
    {
        // Convert the assembly text into its corresponding Instruction-derived object
        Instruction *instr = decodeLine(rawLines[i], i + 1);
       
        // Store the decoded instruction for execution later.
        program.push_back(instr);
    }
}


/* Execute every decoded instruction sequentially

    The Runner behaves like an interpreter:
    1. Fetch instruction from the program container
    2. Execute the instruction
    3. Increment the Program Counter (PC)
    4. Repeat until all instructions have executed */
void Runner::runAll()
{
    // Execute instructions in the same order 
    for (int i = 0; i < program.size(); i++)
    {
        /* Polymorphic call.
           The correct execute() function is chosen
           according to the actual instruction type */
        program[i]->execute(cpu);

        // Move the Program Counter to the next instruction.
        cpu.incPC();
    }
}

// ====================== Output Formatting ======================

// Convert integers into the fixed-width format required
std::string Runner::formatField(int value) const
{
    // Determine whether the value is negative so that the '-' sign can be restored later
    bool negative = value < 0;
    int absValue = negative ? -value : value;
    std::string digits = std::to_string(absValue);

    // Pad the number with leading zeros until the required field width is reached (Positive: 4 digits, Negative: 3 digits + '-')
    while (digits.length() < (negative ? 3 : 4))
        digits = "0" + digits;

    return (negative ? "-" : "") + digits;
}

void Runner::writeDump(std::ostream &out) const
{
    out << "#Begin#" << std::endl;

    out << "#Registers#";
    for (int i = 0; i < 8; i++)
        out << formatField(cpu.getGeneralRegister(i)) << "#";
    out << std::endl;

    out << "#Flags#"
        << cpu.getFlag(FlagType::OF) << "#"
        << cpu.getFlag(FlagType::UF) << "#"
        << cpu.getFlag(FlagType::CF) << "#"
        << cpu.getFlag(FlagType::ZF) << "#"
        << std::endl;

    out << "#PC#" << formatField(cpu.getPC()) << "#" << std::endl;

    out << "#Memory#" << std::endl;
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            int address = row * 8 + col;
            out << "#" << formatField(cpu.loadMemory(address));
        }
        out << "#" << std::endl;
    }

    out << "#End#" << std::endl;
}

void Runner::dump(const std::string &outputFilename) const
{
    // Display the final machine state on the console
    writeDump(std::cout);

    std::ofstream outFile(outputFilename);
    if (!outFile.is_open())
        throw MyException("Could not open output file: " + outputFilename);

    // Write the same machine state to the output file
    writeDump(outFile);
    outFile.close();
}

void Runner::runAllTraced(std::ostream &out)
{
    for (int i = 0; i < program.size(); i++)
    {
        program[i]->execute(cpu);
        cpu.incPC();

        out << "---- After instruction " << (i + 1) << ": "
            << " ----" << std::endl;
        writeDump(out);
    }
}