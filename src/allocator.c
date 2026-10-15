#include "allocator.h"

#include <stdio.h>
#include <stdlib.h>

static size_t calculate_required_blocks(size_t request_size, size_t block_size) {
    if (request_size == 0) {
        return 0;
    }

    return (request_size + block_size - 1U) / block_size;
}

static size_t min_size(size_t a, size_t b) {
    return (a < b) ? a : b;
}

MemoryAllocator *allocator_create(size_t heap_size, size_t block_size) {
    if (heap_size == 0 || block_size == 0) {
        return NULL;
    }

    MemoryAllocator *allocator = (MemoryAllocator *)calloc(1, sizeof(*allocator));
    if (allocator == NULL) {
        return NULL;
    }

    allocator->heap_size = heap_size;
    allocator->block_size = block_size;
    allocator->block_count = (heap_size + block_size - 1U) / block_size;
    allocator->next_allocation_id = 1;
    allocator->blocks = (MemoryBlock *)calloc(allocator->block_count, sizeof(*allocator->blocks));
    if (allocator->blocks == NULL) {
        free(allocator);
        return NULL;
    }

    for (size_t i = 0; i < allocator->block_count; ++i) {
        allocator->blocks[i].size = block_size;
        allocator->blocks[i].allocated_size = 0U;
        allocator->blocks[i].start = i * block_size;
        allocator->blocks[i].block_index = i;
        allocator->blocks[i].allocation_id = INVALID_ALLOCATION_ID;
        allocator->blocks[i].free = true;
    }

    return allocator;
}

void allocator_destroy(MemoryAllocator *allocator) {
    if (allocator == NULL) {
        return;
    }

    free(allocator->blocks);
    free(allocator);
}

int allocator_allocate(MemoryAllocator *allocator, size_t request_size) {
    if (allocator == NULL || request_size == 0U) {
        return INVALID_ALLOCATION_ID;
    }

    size_t required_blocks = calculate_required_blocks(request_size, allocator->block_size);
    if (required_blocks == 0U) {
        return INVALID_ALLOCATION_ID;
    }

    size_t start_block = 0U;
    size_t run_length = 0U;
    bool found = false;

    for (size_t i = 0; i < allocator->block_count; ++i) {
        if (allocator->blocks[i].free) {
            if (run_length == 0U) {
                start_block = i;
            }
            ++run_length;
            if (run_length >= required_blocks) {
                found = true;
                break;
            }
        } else {
            run_length = 0U;
        }
    }

    if (!found) {
        return INVALID_ALLOCATION_ID;
    }

    int allocation_id = allocator->next_allocation_id;
    size_t remaining = request_size;

    for (size_t i = 0; i < required_blocks; ++i) {
        MemoryBlock *block = &allocator->blocks[start_block + i];
        size_t used = min_size(remaining, allocator->block_size);

        block->free = false;
        block->allocation_id = allocation_id;
        block->allocated_size = used;
        block->size = allocator->block_size;
        remaining -= used;
    }

    allocator->next_allocation_id += 1;
    return allocation_id;
}

bool allocator_deallocate(MemoryAllocator *allocator, int allocation_id) {
    if (allocator == NULL || allocation_id == INVALID_ALLOCATION_ID) {
        return false;
    }

    bool found = false;
    for (size_t i = 0; i < allocator->block_count; ++i) {
        MemoryBlock *block = &allocator->blocks[i];
        if (block->allocation_id == allocation_id) {
            block->free = true;
            block->allocation_id = INVALID_ALLOCATION_ID;
            block->allocated_size = 0U;
            found = true;
        }
    }

    return found;
}

static size_t calculate_largest_free_region(const MemoryAllocator *allocator) {
    size_t largest = 0U;
    size_t current = 0U;

    for (size_t i = 0; i < allocator->block_count; ++i) {
        if (allocator->blocks[i].free) {
            ++current;
            if (current > largest) {
                largest = current;
            }
        } else {
            current = 0U;
        }
    }

    return largest * allocator->block_size;
}

void allocator_collect_stats(const MemoryAllocator *allocator, AllocationStats *stats) {
    size_t total_free_bytes = 0U;
    size_t total_allocated_bytes = 0U;
    size_t internal_fragmentation = 0U;
    size_t used_blocks = 0U;
    size_t free_blocks = 0U;

    if (allocator == NULL || stats == NULL) {
        return;
    }

    for (size_t i = 0; i < allocator->block_count; ++i) {
        const MemoryBlock *block = &allocator->blocks[i];
        if (block->free) {
            total_free_bytes += allocator->block_size;
            free_blocks += 1U;
        } else {
            total_allocated_bytes += block->allocated_size;
            used_blocks += 1U;
            if (block->allocated_size < allocator->block_size && block->allocated_size > 0U) {
                internal_fragmentation += (allocator->block_size - block->allocated_size);
            }
        }
    }

    stats->total_allocated_bytes = total_allocated_bytes;
    stats->total_free_bytes = total_free_bytes;
    stats->largest_free_region_bytes = calculate_largest_free_region(allocator);
    stats->internal_fragmentation_bytes = internal_fragmentation;
    stats->used_blocks = used_blocks;
    stats->free_blocks = free_blocks;

    if (total_free_bytes == 0U) {
        stats->external_fragmentation_percent = 0.0;
    } else {
        double largest_free = (double)stats->largest_free_region_bytes;
        double free_space = (double)total_free_bytes;
        stats->external_fragmentation_percent = (1.0 - (largest_free / free_space)) * 100.0;
    }
}

void allocator_print_heap(const MemoryAllocator *allocator) {
    if (allocator == NULL) {
        return;
    }

    printf("Heap (%zu bytes, block size %zu bytes, %zu blocks)\n",
           allocator->heap_size,
           allocator->block_size,
           allocator->block_count);

    printf("+-----+----------------+----------------+--------+---------------+\n");
    printf("| idx | start          | size           | used   | status        |\n");
    printf("+-----+----------------+----------------+--------+---------------+\n");

    for (size_t i = 0; i < allocator->block_count; ++i) {
        const MemoryBlock *block = &allocator->blocks[i];
        printf("| %3zu | %14zu | %14zu | %6zu | %-13s |\n",
               i,
               block->start,
               block->size,
               block->allocated_size,
               block->free ? "FREE" : "ALLOC");
    }

    printf("+-----+----------------+----------------+--------+---------------+\n");
}

void allocator_print_summary(const MemoryAllocator *allocator) {
    AllocationStats stats;
    allocator_collect_stats(allocator, &stats);

    printf("Total allocated bytes: %zu\n", stats.total_allocated_bytes);
    printf("Total free bytes: %zu\n", stats.total_free_bytes);
    printf("Largest free region: %zu bytes\n", stats.largest_free_region_bytes);
    printf("Internal fragmentation: %zu bytes\n", stats.internal_fragmentation_bytes);
    printf("External fragmentation: %.2f%%\n", stats.external_fragmentation_percent);
    printf("Used blocks: %zu | Free blocks: %zu\n", stats.used_blocks, stats.free_blocks);
}

const MemoryBlock *allocator_get_blocks(const MemoryAllocator *allocator) {
    if (allocator == NULL) {
        return NULL;
    }

    return allocator->blocks;
}

size_t allocator_get_block_count(const MemoryAllocator *allocator) {
    if (allocator == NULL) {
        return 0U;
    }

    return allocator->block_count;
}
