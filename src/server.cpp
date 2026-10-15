#include <iostream>
#include "allocator.h"
#include "httplib.h"
#include <unordered_map>
int main() {
    httplib::Server server;
    MemoryAllocator* allocator = new MemoryAllocator(1000,100); //object of class in memoryallcator.h
    std::unordered_map<int, void* > allocations;

    server.Post("/initialize", [&](const httplib::Request& req,httplib::Response& res) {

        if (!req.has_param("heapSize") || !req.has_param("blockSize")) {
            res.status = 400;
            res.set_content(
                "Missing heapSize or blockSize",
                "text/plain"
            );
            return;
        }

        std::size_t heapSize = std::stoull(req.get_param_value("heapSize"));
        std::size_t blockSize = std::stoull(req.get_param_value("blockSize"));

        if (heapSize == 0 || blockSize == 0) {
            res.status = 400;
            res.set_content(
                "Invalid size",
                "text/plain"
            );
            return;
        }

        delete allocator;

        allocator = new MemoryAllocator(heapSize, blockSize);

        res.set_content(
            "Memory allocator initialized",
            "text/plain"
        );
    });

    server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content(
            "Memory Allocator Server is running!",
            "text/plain"
        );
    });

    server.Delete("/deallocate", [&](const httplib::Request& req, httplib::Response& res) {
        if(!req.has_param("id")) {
            res.status = 400;
            res.set_content("Missing id", "text/plain");
            return;
        }

        int id = std::stoi(req.get_param_value("id"));

        allocator->deallocate(id);
        res.set_content("Deallocated successfully", "text/plain");
    });

    server.Post("/allocate", [&](const httplib::Request& req, httplib::Response& res){
        if(!req.has_param("size")) {
            res.status = 400;
            res.set_content("Missing size", "text/plain");
            return;
        }
        std::size_t size = std::stoull(req.get_param_value("size"));
        if(size == 0) {
            res.status = 400;
            res.set_content("Enter Valid Size", "text/plain");
            return;
        }
        int id = allocator->allocate(size);
        if(id == -1) {
            res.status = 400;
            res.set_content("Allocation failed(Not enough space)", "text/plain");
            return;
        }
        res.set_content(
            "Allocation ID: "+std::to_string(id),
            "text/plain"
        );
    });

    server.Get("/memory",
    [&](const httplib::Request& req,
        httplib::Response& res) {

        std::string json = "[";

        const Block* blocks = allocator->getBlocks();
        std::size_t count = allocator->getBlockCount();

        for (std::size_t i = 0; i < count; i++) {

            if (i > 0) {
                json += ",";
            }

            json += "{";
            json += "\"block\":" + std::to_string(i) + ",";
            json += "\"start\":" + std::to_string(blocks[i].start) + ",";
            json += "\"size\":" + std::to_string(blocks[i].size) + ",";
            json += "\"used\":" + std::to_string(blocks[i].allocatedSize) + ",";
            json += "\"allocation_id\":" +
                    std::to_string(blocks[i].allocation_id) + ",";
            json += "\"free\":" +
                    std::string(blocks[i].free ? "true" : "false");
            json += "}";
        }

        json += "]";

        res.set_content(json, "application/json");
    });

    std::cout << "Server running at http://localhost:8080\n";

    server.listen("localhost", 8080);
    
    // int p1 = allocator->allocate(250);
    // allocator->printHeap();

    // int p2 = allocator->allocate(150);
    // allocator->printHeap();

    // allocator->deallocate(p1);
    // allocator->printHeap();

    // int p3 = allocator->allocate(300);
    // allocator->printHeap();

    // allocator->deallocate(p1); //free_ptr
    // allocator->printHeap();

    // allocator->deallocate(p2); //free_ptr
    // allocator->printHeap();

    // allocator->deallocate(p3); //free_ptr
    // allocator->printHeap();

    
    return 0;
}