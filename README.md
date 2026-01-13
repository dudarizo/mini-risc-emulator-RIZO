# Mini-RISC Emulator Project

⚠️ **Important note**

The Mini-RISC toolchain (`gcc_MINIRISC`) is **not included** in this repository due to GitHub file size limits.  
It must be installed separately. 

---

## Project Overview

This project consists of a **software emulator** for a simplified RISC-V–like architecture called **Mini-RISC**, developed as part of a Computer Architecture course.

The goal is to understand the internal functioning of a processor by implementing a complete software emulator, covering:
- instruction execution,
- memory management,
- and memory-mapped I/O.

---

## What Does the Project Do?

The system emulates a complete computer composed of:

1. **Mini-RISC CPU**
   - Implements a Fetch / Decode / Execute cycle.
   - Supports a minimal subset of RISC-V–like instructions:
     `LUI`, `AUIPC`, `JAL`, `JALR`, `BEQ`, `ADDI`, `LBU`, `SB`, and `EBREAK`.
   - Instruction opcodes are **customized** for this Mini-RISC architecture.

2. **RAM Memory**
   - 32 MiB of RAM.
   - Base address: `0x80000000`.

3. **Output Peripheral (CharOut)**
   - Memory-mapped at address `0x10000000`.
   - Writing bytes or words to this address prints output to the terminal.

---

## Execution Flow

1. The emulator loads a raw binary file (`.bin`) into RAM.
2. The CPU repeatedly performs:
   - Fetch instruction from memory,
   - Decode instruction fields,
   - Execute the instruction.
3. Execution stops when the `EBREAK` instruction is encountered.

---

## Repository Structure

```text
.
├── README.md
├── emulator/
│   ├── main.c          # Program entry point
│   ├── minirisc.c      # CPU implementation
│   ├── minirisc.h
│   ├── platform.c      # Memory and I/O platform
│   ├── platform.h
│   └── Makefile        # Builds the emulator
│
└── embedded_software/
    ├── hello.s         # Assembly test program
    └── Makefile        # Cross-compilation Makefile
```

## How to Compile and Run

### Prerequisites
- GCC installed on the host machine.
- Mini-RISC toolchain (`gcc_MINIRISC`) installed locally or provided by the course.

---

### 1. Compile the Emulator (Host)

```bash
cd emulator
make
```

This generates the executable:

```bash
build/emulator
```

### 2. Compile the Test Program (Hello World)
```bash
cd ../embedded_software
make
```

This produces:

```bash
build/esw.bin
```

### 3. Run the Emulator

```bash
cd ../emulator
./build/emulator ../embedded_software/build/esw.bin
```

Expected Output

```bash
Loaded 83 bytes from ../embedded_software/build/esw.bin into memory.
Starting emulation...
Hello, World!
What's up?
EBREAK executed. Halting.
Emulation finished.
```

### Cleaning the Project

```bash
cd emulator && make clean
cd ../embedded_software && make clean
``` 
