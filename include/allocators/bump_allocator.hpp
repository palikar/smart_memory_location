#pragma once

#include "utils.hpp"
#include "memory.hpp"

struct BumpAllocator
{

    MemoryBlock allocate(size_t t_Size)
    {
        auto size = roundToAlign(t_Size);
        auto memory = Memory::push_size(size);
        if (memory >= (char*)g_Memory.bulk + g_Memory.bulk_memory)
        {
            return {};
        }

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

struct TempBumpAllocator
{

    MemoryBlock allocate(size_t t_Size)
    {
        auto size = roundToAlign(t_Size);
        auto memory = Memory::push_size_temp(size);
        
        
        if ((char*)g_Memory.temp + g_Memory.temp_memory <= memory)
        {
            return {};
        }
        
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
