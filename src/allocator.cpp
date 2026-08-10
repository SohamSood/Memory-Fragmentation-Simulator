#include "allocator.h" //use memoryallocator class defined in allocator.h
#include <iostream> //for printing the heap
MemoryAllocator::MemoryAllocator(std::size_t sizeOfHeap,std::size_t sizeOfBlock) {
    HeapSize = sizeOfHeap;
    BlockSize = sizeOfBlock;
    heap = new char[HeapSize];

    blockCount = HeapSize / BlockSize;
    blocks = new Block[blockCount];
    for(std::size_t i = 0;i<blockCount; i++) {
        blocks[i].size = BlockSize;
        blocks[i].allocatedSize = 0;
        blocks[i].BlockIndex = i;
        blocks[i].start = i * BlockSize;
        blocks[i].allocation_id = -1;
        blocks[i].free = true;
    }
}

void* MemoryAllocator::allocate(std::size_t size) { //actual implementation would be here
    if(size == 0) {
        return nullptr;
    }
    std::size_t requiredBlocks = (size + BlockSize - 1) / BlockSize;

    std::size_t consecutiveFreeBlocks = 0;
    std::size_t startBlockIndex = 0;
    bool found = false;
    for(std::size_t i = 0;i<blockCount;i++) {
        if(blocks[i].free) {
            if(consecutiveFreeBlocks == 0) {
                startBlockIndex = i;
            }
            consecutiveFreeBlocks++;
            if(consecutiveFreeBlocks >= requiredBlocks) {
                found = true;
                break;
            }
        } else {
            consecutiveFreeBlocks = 0;
        }
    }
    if(found) {
        for(std::size_t i = startBlockIndex; i < startBlockIndex + requiredBlocks; i++) {
            blocks[i].free = false;
            blocks[i].allocation_id = startBlockIndex; //assigning the allocation id to the starting block index
            if(i == startBlockIndex + requiredBlocks -1) {
                if(size%BlockSize != 0) {
                    blocks[i].allocatedSize = size%BlockSize;
                } else {
                    blocks[i].allocatedSize = BlockSize;
                }
            } else {
                blocks[i].allocatedSize = BlockSize;
            }
        }
         return static_cast<void*>(
            heap + blocks[startBlockIndex].start
        );
    }

    return nullptr;
}
MemoryAllocator::~MemoryAllocator() {
    delete[] heap;
    delete[] blocks;
}
void MemoryAllocator::deallocate(void* ptr) {
    // Placeholder implementation
    if(ptr == nullptr) return;

    //find offset from begining of heap
    std::size_t offset = static_cast<char*>(ptr) - heap;

    //find the first blockk 
    std::size_t blockIndex = offset / BlockSize;

    //allocationid
    int id = blocks[blockIndex].allocation_id;

    for(std::size_t i = 0; i < blockCount; i++) {
        if(blocks[i].allocation_id == id) {
            blocks[i].free = true;
            blocks[i].allocatedSize = 0;
            blocks[i].allocation_id = -1;
        }
    }
}
void MemoryAllocator::printHeap() {
    for (std::size_t i = 0; i < blockCount; i++) {
        std::cout << "Block " << i
                  << " | Start: "<< blocks[i].start
                  << " | Size: " << blocks[i].size
                  << " | Used: " << blocks[i].allocatedSize
                  << " | Free: " << blocks[i].free
                  << '\n';
    }
}