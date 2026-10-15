#ifndef BLOCK_H
#define BLOCK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    size_t size;
    size_t allocated_size;
    size_t start;
    size_t block_index;
    int allocation_id;
    bool free;
} Block;

#endif
