# Arena Memory Allocator

## Overview
This project implements an arena memory allocator in C++. The arena allocator is a memory management technique that pre-allocates a large block of memory (the arena) and carves out smaller allocations from it. This approach is efficient for scenarios where there are numerous small allocations and deallocations.

## Functional Requirements
The arena memory allocator provides the following functionalities:
- **Memory Pool Creation**: Create a fixed-size memory pool for allocations.
- **Allocation**: Allocate memory chunks from the pool.
- **Deallocation**: Reset the memory pool for reuse.
- **Error Handling**: Manage out-of-memory conditions and ensure safe memory operations.

## Implementation Details

### 1. MemoryArena Class
The `MemoryArena` class encapsulates the functionalities of the arena memory allocator.

- **Attributes**:
  - `capacity`: The total size of the arena.
  - `used`: The amount of memory currently allocated.
  - `arena`: Pointer to the allocated memory block.
  - `lock`: Mutex for thread safety.

- **Constructor**:
  - Initializes the arena with the specified capacity and allocates memory using `malloc`.

- **Methods**:
  - `allocate(size_t reqSize)`: Allocates a requested size of memory from the arena. It returns a pointer to the allocated memory or `nullptr` if the allocation fails.
  - `clear()`: Resets the allocated memory, allowing it to be reused.
  - `release()`: Frees the allocated memory and destroys the mutex.

### 2. Usage Example
The main function demonstrates how to use the `MemoryArena` class:

```cpp
int main() {
    MemoryArena arena(1024); // Create an arena with 1024 bytes

    int* numbers = static_cast<int*>(arena.allocate(10 * sizeof(int))); // Allocate memory for 10 integers
    if (numbers) {
        for (int i = 0; i < 10; ++i) {
            numbers[i] = i; // Initialize allocated memory
            std::cout << numbers[i] << " "; // Print the numbers
        }
        std::cout << std::endl;
    }

    arena.clear(); // Reset the arena for future allocations
    arena.release(); // Free the allocated memory
    return 0;
}

