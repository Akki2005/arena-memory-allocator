#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <cassert>
#include <mutex>

using namespace std;

class MemoryArena {
public:
    MemoryArena(size_t cap);
    ~MemoryArena();

    void* allocate(size_t reqSize);
    void clear();
    void release();

private:
    size_t totalCapacity;
    size_t usedCapacity;
    void* buffer;
    mutex lock;
};

MemoryArena::MemoryArena(size_t cap) : totalCapacity(cap), usedCapacity(0) {
    buffer = malloc(cap);
    assert(buffer != nullptr);
}

MemoryArena::~MemoryArena() {
    free(buffer);
}

void* MemoryArena::allocate(size_t reqSize) {
    lock_guard<mutex> guard(lock);
    reqSize = (reqSize + 7) & ~static_cast<size_t>(7);
    if (usedCapacity + reqSize > totalCapacity) {
        return nullptr;
    }
    void* allocatedMemory = static_cast<char*>(buffer) + usedCapacity;
    usedCapacity += reqSize;
    return allocatedMemory;
}

void MemoryArena::clear() {
    lock_guard<mutex> guard(lock);
    usedCapacity = 0;
}

void MemoryArena::release() {
    lock_guard<mutex> guard(lock);
    free(buffer);
    buffer = nullptr;
    totalCapacity = 0;
    usedCapacity = 0;
}

int main() {
    const size_t memorySize = 1024;
    MemoryArena arena(memorySize);

    void* chunk1 = arena.allocate(100);
    cout << "Allocated 100 bytes at: " << chunk1 << endl;

    void* chunk2 = arena.allocate(200);
    cout << "Allocated 200 bytes at: " << chunk2 << endl;

    arena.clear();
    cout << "Memory arena cleared." << endl;

    void* chunk3 = arena.allocate(300);
    cout << "Allocated 300 bytes at: " << chunk3 << endl;

    arena.release();
    cout << "Memory arena released." << endl;

    return 0;
}
