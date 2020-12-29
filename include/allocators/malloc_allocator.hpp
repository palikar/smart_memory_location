#pragma once
#include "utils.hpp"
#include "memory.hpp"
#include <cstdlib>


class Mallocator
{
  public:    
    MemoryBlock allocate(std::size_t n)
    {

        return {(char*)malloc(n), n};
    }

    void deallocate(MemoryBlock t_block)
    {
        free(t_block.memory);
    }


    bool owns(MemoryBlock)
    {
        return false;
    }
    
    void destroy()
    {
        
    }

};
