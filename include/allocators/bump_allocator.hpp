#pragma once
#include "utils.hpp"
#include "memory.hpp"



struct BumpAllocator
{

    MemoryBlock allocate(std::size_t t_Size)
    {
        auto size = roundToAlign(t_Size);
        auto memory = g_Memory.push_size(size);
        return {memory, size};

    }
    
    void deallocate(MemoryBlock)
    {}

    bool owns(MemoryBlock)
    {
        return true;
    }
    
    void destroy()
    {}
    
};
