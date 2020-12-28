#pragma once
#include "utils.hpp"
#include "memory.hpp"

template <size_t N>
struct StackAllocator
{
    char m_Stack[N];
    char* m_Pos;
    
    StackAllocator() : m_Pos(m_Stack)
    {
        memset(m_Stack, 0, N);        
    }

    MemoryBlock allocate(size_t t_Size)
    {
        size_t  n1 = roundToAlign(t_Size);
        if ((size_t)((m_Stack + N) - m_Pos) <  n1)
        {
            return {nullptr, 0};
        }
        MemoryBlock t_Blockk = {m_Pos, n1};
        m_Pos += n1;
        return t_Blockk;
    }

    void deallocate(const MemoryBlock t_Blockk)
    {
        auto ptr = static_cast<char*>(t_Blockk.memory);
        if(ptr + t_Blockk.size == m_Pos)
        {
            m_Pos = ptr;
        }    
    }

    bool owns(MemoryBlock t_Blockk)
    {
        return t_Blockk.memory >= m_Stack && t_Blockk.memory < m_Stack + N;
    }

    void destroy()
    {}    

    
};

