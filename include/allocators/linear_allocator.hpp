#pragma once

#include "memory.hpp"


template<typename Primary, size_t Size>
struct LinearAllocator : private Primary
{
    MemoryBlock m_Arena{0};
    char* m_Offest{nullptr};

    MemoryBlock allocate(std::size_t t_Size)
    {
        if (!m_Arena.memory)
        {
            m_Arena = Primary::allocate(Size);
            m_Offest = m_Arena.memory;
        }
        
        if (m_Offest >= m_Arena.memory + Size)
        {
            return {};
        }

        auto size = roundToAlign(t_Size);

        if (m_Offest + size >= m_Arena.memory + Size)
        {
            return {};
        }
        
        MemoryBlock  blk{m_Offest, size};
        m_Offest += size;
        return blk;

    }
    
    void deallocate(MemoryBlock)
    {
    }

    bool owns(MemoryBlock t_Block)
    {
        return t_Block.memory >= m_Arena.memory
            && t_Block.memory < m_Arena.memory + Size;
    }

    
    void destroy()
    {
        if (m_Arena.memory)
        {

            Primary::deallocate(m_Arena);
        }

    }
    
};

