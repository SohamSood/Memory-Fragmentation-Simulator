# Memory Fragmentation Simulator

A complete C-based project that simulates a fixed-size heap allocator and visualizes how memory becomes fragmented over time.

## Overview

This project models a simplified memory allocator that divides a heap into fixed-size blocks. The simulator demonstrates:

- dynamic allocation and deallocation
- first-fit allocation behavior
- internal fragmentation
- external fragmentation
- heap-state visualization
- allocation statistics

The implementation is written in C and designed for learning memory management concepts in a simple, interactive way.

## Features

- Fixed-size heap simulation
- Fixed-size memory blocks
- Allocation by request size
- Deallocation by allocation ID
- Formation of free gaps between allocated regions
- Stats for fragmentation and free space
- Command-line simulator interface
- Built and tested with GCC

## How the Simulator Works

The heap is divided into blocks of a defined size. When a program requests memory, the allocator searches for a contiguous run of free blocks large enough to satisfy the request.

Example:

```text
Heap size: 256 bytes
Block size: 32 bytes
Total blocks: 8

[ALLOC][ALLOC][FREE][FREE][ALLOC][FREE][FREE][FREE]
```

If the request is larger than the remaining free space in one block, it spans multiple blocks.

## Internal vs External Fragmentation

### Internal Fragmentation

This happens when a block is larger than the data actually needed. The remainder of the block is wasted.

Example:

```text
Request: 40 bytes
Block size: 32 bytes
Required blocks: 2
Wasted space in the last block: 32 - 8 = 24 bytes
```

### External Fragmentation

This happens when free memory is separated into many small gaps. Even though enough total memory is free, a large contiguous region may not be available.

## Project Structure

```text
Memory-Fragmentation-Simulator/
├── README.md
├── Makefile
├── .gitignore
├── src/
│   ├── allocator.h
│   ├── allocator.c
│   ├── block.h
│   └── main.c
├── tests/
│   └── test_allocator.c
└── dashboard/
    ├── index.html
    ├── style.css
    └── script.js
```

## Build and Run

Using GCC directly:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -Isrc src/main.c src/allocator.c -o memory_simulator
./memory_simulator
```

On Windows PowerShell:

```powershell
./build.ps1
./memory_simulator.exe
```

Run the test suite:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -Isrc tests/test_allocator.c src/allocator.c -o tests/test_allocator
./tests/test_allocator
```

Or with PowerShell:

```powershell
./test.ps1
```

If GNU Make is available, the project also includes a standard Makefile:

```bash
make
make test
make clean
```

## Example Commands

Once the simulator starts, use commands such as:

```text
memory-sim> allocate 40
memory-sim> allocate 60
memory-sim> show
memory-sim> stats
memory-sim> free 1
memory-sim> quit
```

## Command Reference

- allocate <size> : allocate a block of memory of the given size
- free <id> : release memory by allocation ID
- show : print the heap layout
- stats : show total used/free bytes and fragmentation percentages
- reset : recreate the allocator
- help : display commands
- quit : exit the simulator

## Concepts Demonstrated

- heap memory model
- fixed-size partitions
- allocation algorithms
- fragmentation analysis
- memory reuse and deallocation
- C memory management practice

## Requirements

- GCC or Clang
- GNU Make
- Linux/macOS/Windows with a C compiler

## Notes

This project focuses on understanding the behavior of fixed-size allocators rather than implementing a full OS-style allocator. It is intentionally simple and educational, making it a good starting point for systems programming and memory management studies.

