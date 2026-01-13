# Projeto Emulador Mini-RISC

Este projeto consiste em um emulador para a arquitetura **Mini-RISC** (baseada em RISC-V) e um conjunto de softwares de teste em Assembly.

## O que o projeto faz?

O sistema emula um computador completo com:
1.  **CPU Mini-RISC**: Executa instruções como `addi`, `beq`, `jal`, `sb`, `lbu`, etc. Note que os *opcodes* são customizados para esta arquitetura específica.
2.  **Memória RAM**: 32 MiB de memória para código e dados (iniciando em `0x80000000`).
3.  **Periférico de Saída (CharOut)**: Mapeado no endereço `0x10000000`. Permite escrever caracteres e números no terminal.

O fluxo de execução é:
1.  O emulador carrega um arquivo binário (`.bin`) para a memória.
2.  A CPU busca instruções da memória (Fetch), decodifica (Decode) e executa (Execute).
3.  O programa roda até encontrar a instrução `ebreak`.

## Estrutura de Arquivos

*   `emulator/`: Código fonte do emulador em C (`main.c`, `minirisc.c`, `platform.c`).
*   `embedded_software/`: Código Assembly de teste (`hello.s`) e Makefile para compilação cruzada.
*   `gcc_MINIRISC/`: Compilador RISC-V customizado (necessário para gerar os binários de teste).

## Como Compilar e Rodar

### Pré-requisitos
Certifique-se de estar na pasta raiz do projeto no terminal.

### 1. Compilar o Emulador
O emulador roda no seu próprio computador (host).

```bash
cd emulator
make
```

### 2. Compilar o Teste (Hello World)
O teste é compilado para a arquitetura Mini-RISC usando o compilador incluído.

```bash
cd ../embedded_software
make
```
Isso gerará o arquivo `build/esw.bin`.

### 3. Executar
Rode o emulador passando o binário gerado como argumento.

```bash
cd ../emulator
./build/emulator ../embedded_software/build/esw.bin
```

### Saída Esperada
```text
Loaded 83 bytes from ../embedded_software/build/esw.bin into memory.
Starting emulation...
Hello, World!
What's up?
EBREAK executed. Halting.
Emulation finished.
```

## Limpeza
Para limpar os arquivos gerados (builds):
```bash
cd emulator && make clean

---

# Mini-RISC Emulator Project

This project consists of an emulator for the **Mini-RISC** architecture (based on RISC-V) and a set of Assembly test software.

## What does the project do?

The system emulates a complete computer with:
1.  **Mini-RISC CPU**: Executes instructions such as `addi`, `beq`, `jal`, `sb`, `lbu`, etc. Note that the *opcodes* are customized for this specific architecture.
2.  **RAM Memory**: 32 MiB of memory for code and data (starting at `0x80000000`).
3.  **Output Peripheral (CharOut)**: Mapped at address `0x10000000`. Allows writing characters and numbers to the terminal.

The execution flow is:
1.  The emulator loads a binary file (`.bin`) into memory.
2.  The CPU fetches instructions from memory (Fetch), decodes (Decode), and executes (Execute).
3.  The program runs until it encounters the `ebreak` instruction.

## File Structure

*   `emulator/`: Emulator source code in C (`main.c`, `minirisc.c`, `platform.c`).
*   `embedded_software/`: Test Assembly code (`hello.s`) and Makefile for cross-compilation.
*   `gcc_MINIRISC/`: Custom RISC-V compiler (required to generate test binaries).

## How to Compile and Run

### Prerequisites
Make sure you are in the project root folder in the terminal.

### 1. Compile the Emulator
The emulator runs on your own computer (host).

```bash
cd emulator
make
```

### 2. Compile the Test (Hello World)
The test is compiled for the Mini-RISC architecture using the included compiler.

```bash
cd ../embedded_software
make
```
This will generate the `build/esw.bin` file.

### 3. Run
Run the emulator passing the generated binary as an argument.

```bash
cd ../emulator
./build/emulator ../embedded_software/build/esw.bin
```

### Expected Output
```text
Loaded 83 bytes from ../embedded_software/build/esw.bin into memory.
Starting emulation...
Hello, World!
What's up?
EBREAK executed. Halting.
Emulation finished.
```

## Cleaning
To clean generated files (builds):
```bash
cd emulator && make clean
cd ../embedded_software && make clean
```
