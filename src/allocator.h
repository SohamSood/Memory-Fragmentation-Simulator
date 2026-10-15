#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdbool.h>
#include <stddef.h>

#define INVALID_ALLOCATION_ID (-1)

typedef struct {
    size_t size;
    size_t allocated_size;
    size_t start;
    size_t block_index;
    int allocation_id;
    bool free;
} MemoryBlock;

typedef struct {
    size_t heap_size;
    size_t block_size;
    size_t block_count;
    MemoryBlock *blocks;
    int next_allocation_id;
} MemoryAllocator;

typedef struct {
    size_t total_allocated_bytes;
    size_t total_free_bytes;
    size_t largest_free_region_bytes;
    size_t internal_fragmentation_bytes;
    double external_fragmentation_percent;
    size_t used_blocks;
    size_t free_blocks;
} AllocationStats;

MemoryAllocator *allocator_create(size_t heap_size, size_t block_size);
void allocator_destroy(MemoryAllocator *allocator);

int allocator_allocate(MemoryAllocator *allocator, size_t request_size);
bool allocator_deallocate(MemoryAllocator *allocator, int allocation_id);

void allocator_print_heap(const MemoryAllocator *allocator);
void allocator_print_summary(const MemoryAllocator *allocator);
void allocator_collect_stats(const MemoryAllocator *allocator, AllocationStats *stats);
const MemoryBlock *allocator_get_blocks(const MemoryAllocator *allocator);
size_t allocator_get_block_count(const MemoryAllocator *allocator);

#endif
