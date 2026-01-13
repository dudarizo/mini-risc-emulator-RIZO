# Mini-RISC Emulator Project

⚠️ **Important notice**  
The Mini-RISC toolchain (`gcc_MINIRISC`) is **not included** in this repository due to GitHub file upload limits.  
It must be installed separately or provided by the course.

## Project Overview

This project consists of a software emulator for the **Mini-RISC** architecture (inspired by RISC-V) and a set of Assembly test programs.  
The emulator models a simplified computer system including a CPU, memory, and memory-mapped input/output.

## What does the project do?

The system emulates:
1. **Mini-RISC CPU**  
   Executes instructions such as `addi`, `beq`, `jal`, `sb`, `lbu`, etc.  
   Instruction opcodes are customized for this specific Mini-RISC architecture.
2. **RAM Memory**  
   32 MiB of memory for code and data, starting at address `0x80000000`.
3. **Output Peripheral (CharOut)**  
   Memory-mapped at address `0x10000000`, allowing characters and numbers to be printed to the terminal.

## Execution Flow

1. The emulator loads a binary file (`.bin`) into memory.
2. The CPU performs the Fetch / Decode / Execute cycle.
3. Execution stops when the `ebreak` instruction is encountered.

## File Structure

- `emulator/` — Emulator source code written in C (`main.c`, `minirisc.c`, `platform.c`, headers and Makefile).
- `embedded_software/` — Assembly test program (`hello.s`) and Makefile for cross-compilation.
- `gcc_MINIRISC/` — Custom Mini-RISC toolchain (**not included** in this repository).

## How to Compile and Run

Make sure you are in the project root directory.

### Compile the Emulator

```bash
cd emulator
make
```

### Compile the Test Program (Hello World)

```bash
cd ../embedded_software
make
```

This generates the file build/esw.bin.

### Run the Emulator

```bash
cd ../emulator
./build/emulator ../embedded_software/build/esw.bin
```

### Expected Output

```bash
Loaded 83 bytes from ../embedded_software/build/esw.bin into memory.
Starting emulation...
Hello, World!
What's up?
EBREAK executed. Halting.
Emulation finished.
```

### Cleaning

cd emulator && make clean
cd ../embedded_software && make clean
