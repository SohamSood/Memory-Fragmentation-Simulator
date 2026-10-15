#include "allocator.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void trim_newline(char *text) {
    size_t length = strlen(text);
    while (length > 0U && (text[length - 1U] == '\n' || text[length - 1U] == '\r')) {
        text[--length] = '\0';
    }
}

static void print_help(void) {
    printf("Commands:\n");
    printf("  allocate <size>   Allocate <size> bytes from the heap\n");
    printf("  free <id>         Deallocate block with the given allocation id\n");
    printf("  show              Display the heap map\n");
    printf("  stats             Display allocation statistics\n");
    printf("  reset             Rebuild the allocator\n");
    printf("  help              Show this help menu\n");
    printf("  quit              Exit the simulator\n");
}

static MemoryAllocator *rebuild_allocator(size_t heap_size, size_t block_size) {
    MemoryAllocator *allocator = allocator_create(heap_size, block_size);
    if (allocator == NULL) {
        printf("Failed to initialize allocator.\n");
        return NULL;
    }

    printf("Allocator initialized: heap=%zu bytes, block=%zu bytes\n", heap_size, block_size);
    return allocator;
}

static int parse_size_value(const char *text, size_t *value) {
    char *end = NULL;
    long parsed = strtol(text, &end, 10);
    if (text == end || *text == '\0' || parsed <= 0 || parsed > (long)SIZE_MAX) {
        return -1;
    }

    *value = (size_t)parsed;
    return 0;
}

static int parse_id_value(const char *text, int *value) {
    char *end = NULL;
    long parsed = strtol(text, &end, 10);
    if (text == end || *text == '\0' || parsed < 0) {
        return -1;
    }

    *value = (int)parsed;
    return 0;
}

int main(void) {
    const size_t initial_heap_size = 256U;
    const size_t initial_block_size = 32U;
    MemoryAllocator *allocator = rebuild_allocator(initial_heap_size, initial_block_size);
    char input[128];

    if (allocator == NULL) {
        return 1;
    }

    print_help();

    while (1) {
        printf("\nmemory-sim> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        trim_newline(input);

        if (input[0] == '\0') {
            continue;
        }

        if (strcmp(input, "help") == 0) {
            print_help();
            continue;
        }

        if (strcmp(input, "show") == 0) {
            allocator_print_heap(allocator);
            continue;
        }

        if (strcmp(input, "stats") == 0) {
            allocator_print_summary(allocator);
            continue;
        }

        if (strcmp(input, "reset") == 0) {
            allocator_destroy(allocator);
            allocator = rebuild_allocator(initial_heap_size, initial_block_size);
            if (allocator == NULL) {
                return 1;
            }
            continue;
        }

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            break;
        }

        if (strncmp(input, "allocate ", 9U) == 0) {
            size_t size = 0U;
            char *argument = input + 9;
            if (parse_size_value(argument, &size) != 0) {
                printf("Invalid size. Example: allocate 80\n");
                continue;
            }

            int id = allocator_allocate(allocator, size);
            if (id == INVALID_ALLOCATION_ID) {
                printf("Allocation failed: not enough contiguous space available.\n");
            } else {
                printf("Allocated %zu bytes. Allocation ID: %d\n", size, id);
            }
            continue;
        }

        if (strncmp(input, "free ", 5U) == 0) {
            int allocation_id = INVALID_ALLOCATION_ID;
            char *argument = input + 5;
            if (parse_id_value(argument, &allocation_id) != 0) {
                printf("Invalid allocation ID. Example: free 2\n");
                continue;
            }

            if (allocator_deallocate(allocator, allocation_id)) {
                printf("Allocation ID %d was freed.\n", allocation_id);
            } else {
                printf("Allocation ID %d was not found.\n", allocation_id);
            }
            continue;
        }

        printf("Unknown command: '%s'\n", input);
        print_help();
    }

    allocator_destroy(allocator);
    printf("Goodbye.\n");
    return 0;
}
