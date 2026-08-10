#ifndef BLOCK_H
#define BLOCK_H

#include <cstddef>

struct Block{
    std::size_t size;
    std::size_t allocatedSize;
    std::size_t start;
    std::size_t BlockIndex;
    int allocation_id;
    bool free;
};

#endif
