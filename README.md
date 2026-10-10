# Virtual Machine

A custom 8-bit virtual machine implemented in C++ for a university project.

This project simulates a simple processor with general-purpose registers, memory, arithmetic operations, status flags, a stack, and a custom assembly language. It demonstrates fundamental concepts of CPU execution, instruction decoding, memory management, and processor state.

## Features

- Custom assembly instruction set
- Eight 8-bit general-purpose registers (`R0`–`R7`)
- 64 bytes of memory
- Eight-element stack
- Arithmetic and bitwise shift and rotation operations
- Processor status flags
- Console input and output instructions
- Sequential instruction execution
- Instruction-by-instruction execution tracing
- Processor state dumps to the console and a text file
- CMake-based build configuration

## Architecture

### Registers

The virtual machine provides eight general-purpose registers:

`R0`, `R1`, `R2`, `R3`, `R4`, `R5`, `R6`, and `R7`.

Each register stores a signed 8-bit value in the range `-128` to `127`.

Results are converted to the 8-bit representation when stored. On the intended two's-complement implementation, values wrap around when they exceed this range.

For example, the value `130` is represented as `-126` when converted to a signed 8-bit value.

### Memory

The virtual machine has 64 bytes of memory, addressed from `0` to `63`.

Memory can be accessed directly using a numeric address or indirectly through a register, depending on the instruction.

### Stack

The virtual machine has a stack with a capacity of eight values.

- `PUSH` adds a register value to the stack.
- `POP` removes the most recently pushed value and stores it in a register.

Attempting to push onto a full stack or pop from an empty stack raises an exception.

### Program Counter

The program counter (`PC`) tracks execution progress.

Instructions execute sequentially, and the program counter advances after each executed instruction.

The virtual machine does not implement jumps, conditional branches, or loops.

## Status Flags

The virtual machine maintains four status flags.

| Flag | Name | Description |
|---|---|---|
| `OF` | Overflow Flag | Set when the raw arithmetic result exceeds `127`. |
| `UF` | Underflow Flag | Set when the raw arithmetic result is below `-128`. |
| `CF` | Carry Flag | Set when the raw arithmetic result is outside the signed 8-bit range. |
| `ZF` | Zero Flag | Set when the raw result is exactly zero. |

Arithmetic flags are calculated from the result before it is converted to an 8-bit value.

For example, adding `100` and `50` produces a raw result of `150`. The stored result wraps to `-106`, while `OF` and `CF` are set.

The `ZF` flag is based on the raw result. Therefore, a raw result of `256` wraps to `0` when stored but does not set `ZF`.

The `RESET` instruction clears one specified flag.

Data movement, memory access, stack operations, and shift and rotation instructions do not update the arithmetic flags.

The `INPUT` instruction updates `OF`, `UF`, and `ZF`, but leaves `CF` unchanged.

## Assembly Language

Assembly programs contain one instruction per line.

- Instructions use uppercase mnemonics.
- Register names use the format `R0` through `R7`.
- Square brackets indicate memory addressing where required.
- Empty lines are ignored.
- Instructions execute in the order in which they appear.
- Inline comments are not supported.

### Instruction Set

In the tables below:

- `Rd` represents a destination register.
- `Rs` represents a source register.
- `n` represents a numeric operand or memory address, depending on the instruction.

### Data Movement

| Instruction | Description |
|---|---|
| `MOV Rd, n` | Store a numeric value in `Rd`. |
| `MOV Rd, Rs` | Copy the value of `Rs` into `Rd`. |
| `MOV Rd, [Rs]` | Load into `Rd` from the memory address stored in `Rs`. |
| `LOAD Rd, [n]` | Load into `Rd` from memory address `n`. |
| `LOAD Rd, [Rs]` | Load into `Rd` from the memory address stored in `Rs`. |
| `STORE Rd, n` | Store the value of `Rd` at memory address `n`. |
| `STORE n, Rs` | Store the value of `Rs` at memory address `n`. |
| `STORE [Rd], Rs` | Store the value of `Rs` at the memory address stored in `Rd`. |

Numeric operands are written without a `#` prefix in the syntax supported by the current parser.

### Arithmetic

| Instruction | Description |
|---|---|
| `ADD Rd, Rs` | Add the value of `Rs` to `Rd`. |
| `ADD Rd, n` | Add a numeric value to `Rd`. |
| `SUB Rd, Rs` | Subtract the value of `Rs` from `Rd`. |
| `SUB Rd, n` | Subtract a numeric value from `Rd`. |
| `MUL Rd, Rs` | Multiply `Rd` by the value of `Rs`. |
| `MUL Rd, n` | Multiply `Rd` by a numeric value. |
| `DIV Rd, Rs` | Divide `Rd` by the value of `Rs` using integer division. |
| `DIV Rd, n` | Divide `Rd` by a numeric value using integer division. |
| `INC Rd` | Increment `Rd` by one. |
| `DEC Rd` | Decrement `Rd` by one. |

Arithmetic operations update the status flags.

Integer division truncates toward zero. Division by zero raises an exception.

### Bitwise Shift and Rotation

| Instruction | Description |
|---|---|
| `ROL Rd, n` | Rotate the bits of `Rd` to the left. |
| `ROR Rd, n` | Rotate the bits of `Rd` to the right. |
| `SHL Rd, n` | Shift the bits of `Rd` left, filling empty positions with zeroes. |
| `SHR Rd, n` | Shift the bits of `Rd` right, filling empty positions with zeroes. |

These instructions operate on the register's 8-bit representation and do not update the status flags.

### Input and Output

| Instruction | Description |
|---|---|
| `INPUT Rd` | Read an integer from the console and store its converted 8-bit signed representation in `Rd`. |
| `DISPLAY Rs` | Print the signed value stored in `Rs`. |

The `INPUT` instruction sets `OF` when the input exceeds `127` and `UF` when it is below `-128`. It also updates `ZF` based on whether the original input is zero.

The input is converted and stored even when it falls outside the signed 8-bit range; the instruction does not reject values solely because they are out of range.

For example, an input of `130` is stored as `-126` in the intended two's-complement implementation.

### Stack

| Instruction | Description |
|---|---|
| `PUSH Rs` | Push the value of `Rs` onto the stack. |
| `POP Rd` | Pop a value from the stack into `Rd`. |

The stack can hold up to eight values.

### Flag Control

| Instruction | Description |
|---|---|
| `RESET OF` | Clear the overflow flag. |
| `RESET UF` | Clear the underflow flag. |
| `RESET CF` | Clear the carry flag. |
| `RESET ZF` | Clear the zero flag. |

## Example Program

An example assembly program is provided at:

`examples/program.asm`

The virtual machine can execute this example file directly. See the instructions below.

## Build Requirements

The project requires:

- A C++17-compatible compiler
- CMake 3.16 or newer
- MinGW-w64 on Windows, or another compatible C++ toolchain

## Building on Windows

The following commands assume that CMake and the MSYS2 MinGW-w64 compiler are installed at their standard locations used in this project.

Run the commands from the project root directory.

### 1. Configure the project

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe
```

### 2. Build the executable

```powershell
cmake --build build
```

After a successful build, the executable is generated at:

`build/vm.exe`

## Running the Virtual Machine

### Run the default example

From the project root, enter the build directory and run the executable:

```powershell
cd build
.\vm.exe
```

When no filename is supplied, the virtual machine loads `program.asm` from the current working directory.

The CMake configuration copies `examples/program.asm` into the build directory during configuration.

### Run a specific assembly file

To specify an assembly file explicitly, run:

```powershell
.\vm.exe ..\examples\program.asm
```

This command assumes the terminal is currently in the `build` directory.

## Output

During execution, the virtual machine prints a trace showing the processor state after each instruction.

After execution, the final processor state is displayed in the console and written to `output.txt` in the current working directory.

The state dump includes:

- General-purpose register values
- Status flags (`OF`, `UF`, `CF`, and `ZF`)
- Program counter
- All 64 memory locations

The dump is enclosed by `#Begin#` and `#End#` markers.

A simplified example of the output format is shown below:

```text
#Begin#
#Registers#0000#0011#0000#0044#0000#0000#0000#0000#
#Flags#0#0#0#0#
#PC#0005#
#Memory#
#0000#0000#0000#0000#0000#0000#0000#0000#
...
#End#
```

The values shown are illustrative; actual values depend on the program executed.

When the executable is run from the `build` directory, the output file is created at `build/output.txt`.

## Project Structure

```text
virtualMachine/
├── examples/
│   └── program.asm
├── include/
│   ├── CPU.h
│   ├── Flags.h
│   ├── Instruction.h
│   ├── Memory.h
│   ├── MyException.h
│   ├── MyQueue.h
│   ├── MyStack.h
│   ├── MyVector.h
│   ├── OpCode.h
│   ├── Registers.h
│   └── Runner.h
├── src/
│   ├── CPU.cpp
│   ├── Flags.cpp
│   ├── Instruction.cpp
│   ├── Main.cpp
│   ├── Memory.cpp
│   ├── MyException.cpp
│   ├── Registers.cpp
│   └── Runner.cpp
├── CMakeLists.txt
├── LICENSE
└── README.md
```

The `build/` directory is generated by CMake and does not need to be committed to version control.

## Limitations

This project is an educational virtual machine with a deliberately small instruction set.

- Instructions execute sequentially.
- Jump and branch instructions are not implemented.
- Loops are not supported.
- Inline assembly comments are not supported.
- Registers use a fixed 8-bit signed representation.
- Memory and stack sizes are fixed.
- The instruction set is custom and is not compatible with a real-world processor architecture.

## Contributors

This project was developed as a four-member university team project.

  -  Ahmed Dafaalla (drg-cyber) — Contributed approximately 55% of the project’s implementation, including all custom data structure classes (MyVector, MyStack, MyQueue), the CPU class, the Flags class, the Memory class, and the bitwise shift/rotation algorithms.

## License

See the [LICENSE](LICENSE) file for license information.