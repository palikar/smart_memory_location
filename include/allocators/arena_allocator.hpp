#pragma once

#include "memory.hpp"


template<typename Primary, size_t BlockSize, size_t BlockCount>
struct FixedArenaAllocator : public Primary 
{
    MemoryBlock m_Arena{0};
    char* m_Offest{nullptr};
    char* m_FreeList[BlockCount]{0};
    int m_ListEntry{0};
 
    MemoryBlock allocate(size_t)
    {
        if (!m_Arena.memory)
        {
            m_Arena = Primary::allocate(BlockSize*BlockCount);
            m_Offest = m_Arena.memory;
            memset(m_FreeList, 0, BlockCount*sizeof(char*));
            m_ListEntry = 0;
        }

        if (m_ListEntry == 0 && m_Offest >= m_Arena.memory + BlockCount*BlockSize)
        {
            return {};
        }
        if (m_Offest + BlockSize > m_Arena.memory + BlockCount*BlockSize)
        {
            return {};
        }

        assert(m_ListEntry > 0 ||  m_Offest < m_Arena.memory + BlockCount*BlockSize);

        if(m_ListEntry > 0)
        {
            return {m_FreeList[--m_ListEntry], BlockSize };
        }
        
        assert(m_Offest <= m_Arena.memory + BlockCount*BlockSize);

        MemoryBlock  blk{m_Offest, BlockSize};
        m_Offest += BlockSize;
        return blk;

        
        
    }

    void deallocate(MemoryBlock t_Block)
    {
        m_FreeList[m_ListEntry++] = t_Block.memory;
    }

    bool owns(MemoryBlock t_Block)
    {
        return t_Block.size == BlockSize
            && t_Block.memory >= m_Arena.memory
            && t_Block.memory <= m_Arena.memory + BlockCount*BlockSize;
    }

    void destroy()
    {
        Primary::deallocate(m_Arena);
    }


};
