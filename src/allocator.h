#ifndef ALLOCATOR_H //if not defined already
#define ALLOCATOR_H //then we define allocator h header file
#include "block.h"

#include <cstddef> //includes size_t
class MemoryAllocator {
private: 
    char* heap;
    std::size_t HeapSize;
    std::size_t BlockSize;
    
    Block* blocks;
    std::size_t blockCount;
public:
    MemoryAllocator(std::size_t size, std::size_t blockSize);
    ~MemoryAllocator();
    void* allocate(std::size_t size); //declare a function
    void deallocate(void* ptr); //again declare a function
    void printHeap(); //function to print the heap
    const Block* getBlocks() const;
    std::size_t getBlockCount() const;
};
#endif // ALLOCATR_H // free(ptr)