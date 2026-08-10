#include <iostream>
#include "allocator.h"
#include "httplib.h"

int main() {
    httplib::Server server;
    MemoryAllocator* allocator = new MemoryAllocator(1000,100); //object of class in memoryallcator.h

    server.Get("/", [](const httplib::Request& req,
                       httplib::Response& res) {

        res.set_content(
            "Memory Allocator Server is running!",
            "text/plain"
        );
    });

    std::cout << "Server running at http://localhost:8080\n";

    server.listen("localhost", 8080);
    
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