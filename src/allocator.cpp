#include "allocator.h" //use memoryallocator class defined in allocator.h
#include <iostream> //for printing the heap
static int next_allocation_id = 1;
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

int MemoryAllocator::allocate(std::size_t size) { //actual implementation would be here
    if(size == 0) {
        return -1;
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
            blocks[i].allocation_id = next_allocation_id; //assigning the allocation id to the starting block index
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
        next_allocation_id++;
        return next_allocation_id - 1;
    }
    return -1;
}
MemoryAllocator::~MemoryAllocator() {
    delete[] heap;
    delete[] blocks;
}
void MemoryAllocator::deallocate(int allocator_id) {
    // Placeholder implementation
    bool removed = false;
    for(std::size_t i = 0; i < blockCount; i++) {
        if(blocks[i].allocation_id == allocator_id) {
            blocks[i].free = true;
            blocks[i].allocatedSize = 0;
            blocks[i].allocation_id = -1;
            removed = true;
        }
    }
}
void MemoryAllocator::printHeap() {
    for (std::size_t i = 0; i < blockCount; i++) {
        std::cout << "Block " << i
                  << " | Start: "<< blocks[i].start
                  << " | Size: " << blocks[i].size
                  << " | Used: " << blocks[i].allocatedSize
                  << " | Allocation ID: " << blocks[i].allocation_id
                  << " | Free: " << blocks[i].free
                  << '\n';
    }
}

const Block* MemoryAllocator::getBlocks() const {
    return blocks;
}

std::size_t MemoryAllocator::getBlockCount() const {
    return blockCount;
}