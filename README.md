# Memory Fragmentation Simulator

A C++ implementation of a fixed-size heap memory allocator that demonstrates **dynamic memory allocation, deallocation, internal fragmentation, and external fragmentation**.

## Overview

This project simulates how memory can be managed using a custom memory allocator instead of relying directly on the standard `malloc()`/`free()` functions.

The allocator creates a fixed-size heap divided into fixed-size blocks. Blocks can then be allocated and released during program execution.

## Features

* Fixed-size simulated heap
* Fixed-size memory blocks
* Custom memory allocation
* Memory deallocation
* Heap state visualization
* Internal fragmentation demonstration
* External fragmentation demonstration
* Written in C++

## How It Works

The simulated heap is divided into multiple blocks:

```text
Heap
+------+------+------+------+------+------+
|  0   |  1   |  2   |  3   |  4   |  5   |
+------+------+------+------+------+------+
```

When memory is requested, the allocator searches for available blocks and allocates them.

For example:

```text
Request: 2 blocks

+------+------+------+------+------+------+
| USED | USED | FREE | FREE | FREE | FREE |
+------+------+------+------+------+------+
```

When allocated memory is released, the corresponding blocks become available again.

## Fragmentation

### Internal Fragmentation

Internal fragmentation occurs when allocated memory contains unused space because allocation happens in fixed-size blocks.

For example, if a block has a size of `64 bytes` but only `40 bytes` are required:

```text
+--------------------------------+
|     40 bytes used | 24 unused |
+--------------------------------+
```

The unused `24 bytes` represent internal fragmentation.

### External Fragmentation

External fragmentation occurs when enough free memory exists overall, but it is divided into separate non-contiguous regions.

```text
+------+------+------+------+------+------+
| USED | FREE | USED | FREE | USED | FREE |
+------+------+------+------+------+------+
```

Although free blocks are available, a larger contiguous allocation may not be possible.

## Project Structure

```text
Memory-Fragmentation-Simulator/
│
├── src/
│   ├── allocator.h
│   ├── allocator.cpp
│   └── main.cpp
│
├── .gitignore
└── README.md
```

## Compilation

Using `g++`:

```bash
g++ -Wall -Wextra -g3 src/main.cpp src/allocator.cpp -o main
```

Run:

```bash
./main
```

On Windows:

```powershell
.\main.exe
```

## Technologies

* C++
* Object-Oriented Programming
* Dynamic Memory Allocation
* Memory Management
* Data Structures
* Systems Programming

## Concepts Demonstrated

* Heap memory
* Memory allocation
* Memory deallocation
* Fixed-size memory blocks
* Internal fragmentation
* External fragmentation
* Pointers
* Object-oriented programming
* Low-level memory management

## Future Improvements

* First-fit allocation
* Best-fit allocation
* Worst-fit allocation
* Variable-size blocks
* Memory compaction
* Allocation statistics
* Fragmentation percentage calculation
* Interactive command-line interface
