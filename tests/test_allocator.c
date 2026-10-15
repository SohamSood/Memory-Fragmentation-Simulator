#include "allocator.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    MemoryAllocator *allocator = allocator_create(256U, 32U);
    assert(allocator != NULL);

    int id1 = allocator_allocate(allocator, 40U);
    int id2 = allocator_allocate(allocator, 60U);
    int id3 = allocator_allocate(allocator, 70U);

    assert(id1 == 1);
    assert(id2 == 2);
    assert(id3 == 3);

    AllocationStats stats = {0};
    allocator_collect_stats(allocator, &stats);
    assert(stats.total_allocated_bytes > 0U);
    assert(stats.total_free_bytes > 0U);

    assert(allocator_deallocate(allocator, id2) == true);
    assert(allocator_deallocate(allocator, id3) == true);
    assert(allocator_deallocate(allocator, id1) == true);

    allocator_destroy(allocator);
    printf("All allocator tests passed.\n");
    return 0;
}
