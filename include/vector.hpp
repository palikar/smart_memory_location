#pragma once

#include "memory.hpp"
#include "utils.hpp"


struct Vector
{
    using BlockType = TypedBlock<char>;

    BlockType m_Data;
    size_t m_Count;
    size_t m_Size;

    void init(size_t t_Size = 16)
    {
        m_Data = allocate_type<char>(g_Allocator, t_Size*sizeof(int));
        m_Count = 0;
        m_Size = m_Data.size / sizeof(int);
    }

    void push_back(const int& t_Element)
    {
        assert(m_Data.memory);
        if (m_Count == m_Size)
        {
            grow();
        }
        *(m_Data + m_Count*sizeof(int)) = t_Element;
        ++m_Count;
    }

    template<typename ... Args>
    void emplace_back(Args ... t_Args)
    {
        assert(m_Data.memory);
        if (m_Count == m_Size)
        {
            grow();
        }
        new((m_Data + m_Count*sizeof(int))) int(t_Args...);
        ++m_Count;
    }

    int& operator[](size_t t_Index) const
    {
        assert(m_Data.memory);
        return *((int*)(m_Data.memory + t_Index*sizeof(int)));
    }

    int& front() const
    {
        assert(m_Data.memory);
        return *((int*)(m_Data.memory));
    }
    
    int& back() const
    {
        assert(m_Data.memory);
        return *((int*)(m_Data.memory + m_Count*sizeof(int)));
    }

    void clear()
    {
        assert(m_Data.memory);
        m_Count = 0;
    }

    int* data() const
    {
        return (int*)m_Data.memory;
    }

    inline size_t count() const
    {
        return m_Count;
    }

  private:
  
    void grow()
    {
        const auto newSize = (2*m_Size);
        auto newBlock = allocate_type<char>(g_Allocator, newSize*sizeof(int));
        m_Size = newBlock.size / sizeof(int);
        memcpy(newBlock.memory, m_Data.memory, m_Data.size);
        m_Data = newBlock;        

    }

    void shrink()
    {

    }
    

};

