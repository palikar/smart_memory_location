#pragma once

#include "utils.hpp"
#include "memory.hpp"

struct BumpAllocator
{

    MemoryBlock allocate(size_t t_Size)
    {
        auto size = roundToAlign(t_Size);
        auto memory = Memory::push_size(size);

        if (memory >= (char*)g_Memory.m_BulkMemory + g_Memory.m_BulkMemorySize)
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
        
        
        if ((char*)g_Memory.m_TempMemory + g_Memory.m_TempMemorySize <= memory)
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
