#include "allocator.h"

#include <stdio.h>
#include <time.h>

int main(void) {
    const size_t heap_size = 4096U;
    const size_t block_size = 64U;
    MemoryAllocator *allocator = allocator_create(heap_size, block_size);
    if (allocator == NULL) {
        printf("Allocator creation failed.\n");
        return 1;
    }

    clock_t start = clock();
    int ids[32];
    for (int i = 0; i < 32; ++i) {
        size_t request = 16U * ((size_t)(i % 3U) + 1U);
        ids[i] = allocator_allocate(allocator, request);
        if (ids[i] == INVALID_ALLOCATION_ID) {
            printf("Benchmark failed at allocation %d with size %zu\n", i, request);
            allocator_destroy(allocator);
            return 1;
        }
    }

    for (int i = 0; i < 16; ++i) {
        allocator_deallocate(allocator, ids[i]);
    }

    clock_t end = clock();
    double elapsed_ms = ((double)(end - start) / (double)CLOCKS_PER_SEC) * 1000.0;

    allocator_print_summary(allocator);
    printf("Benchmark time: %.2f ms\n", elapsed_ms);
    allocator_destroy(allocator);
    return 0;
}
