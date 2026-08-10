#include <iostream>
#include "allocator.h"

int main() {

    MemoryAllocator* allocator = new MemoryAllocator(1000,100); //object of class in memoryallcator.h

    
    void* p1 = allocator->allocate(250);
    allocator->printHeap();

    void* p2 = allocator->allocate(150);
    allocator->printHeap();

    allocator->deallocate(p1);
    allocator->printHeap();

    void* p3 = allocator->allocate(300);
    allocator->printHeap();

    allocator->deallocate(p1); //free_ptr
    allocator->printHeap();

    allocator->deallocate(p2); //free_ptr
    allocator->printHeap();

    allocator->deallocate(p3); //free_ptr
    allocator->printHeap();

    
    return 0;
}