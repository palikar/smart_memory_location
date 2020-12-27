#pragma once

#include "memory.hpp"
#include "utils.hpp"


template<typename T>
struct Vector
{
    using BlockType = TypedBlock<char>;

    BlockType m_Data;
    size_t m_Count;
    size_t m_Size;

    void init(size_t t_Size = 16)
    {
        m_Data = allocate_type<char>(g_Allocator, t_Size*sizeof(T));
        m_Count = 0;
        m_Size = m_Data.size / sizeof(T);
    }

    void push_back(const T& t_Element)
    {
        assert(m_Data.memory);
        if (m_Count == m_Size)
        {
            grow();
        }
        *(m_Data + m_Count*sizeof(T)) = t_Element;
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
        new((m_Data + m_Count*sizeof(T))) T(t_Args...);
        ++m_Count;
    }

    inline T& operator[](size_t t_Index) const
    {
        assert(m_Data.memory);
        assert(t_Index < m_Size);
        return *((T*)(m_Data.memory + t_Index*sizeof(T)));
    }

    inline T& front() const
    {
        assert(m_Data.memory);
        return *((T*)(m_Data.memory));
    }

    inline T& back() const
    {
        assert(m_Data.memory);
        return *((T*)(m_Data.memory + m_Count*sizeof(T)));
    }

    void clear()
    {
        assert(m_Data.memory);
        m_Count = 0;
    }

    inline T* data() const
    {
        return (T*)m_Data.memory;
    }

    inline size_t count() const
    {
        return m_Count;
    }

    inline bool empty() const
    {
        return m_Count == 0;
    }

    void pop_back()
    {
        --m_Count;
        if (m_Count < m_Size/2)
        {
            shrink();
        }
    }

    void erase(size_t t_Index)
    {
        memcpy(m_Data.memory + sizeof(T)*t_Index, m_Data.memory + sizeof(T)*(t_Index + 1) , sizeof(T)*(m_Count - t_Index - 1));
        --m_Count;
        if (m_Count < m_Size/2)
        {
            shrink();
        }
    }

    void erase(size_t t_Start, size_t t_End)
    {
        memcpy(m_Data.memory + sizeof(T)*t_Start, m_Data.memory + sizeof(T)*(t_End + 1) , sizeof(T)*(m_Count - t_End - 1));
        --m_Count;
        if (m_Count < m_Size/2)
        {
            shrink();
        }
    }

    void erase(T* element)
    {
        
        memcpy(element, element + 1, (T*)(m_Data.memory + m_Count*sizeof(T)) - (element+1));
        --m_Count;
        if (m_Count < m_Size/2)
        {
            shrink();
        }
    }
    

  private:

    void grow()
    {
        const auto newSize = (2*m_Size);
        auto newBlock = allocate_type<char>(g_Allocator, newSize*sizeof(T));
        m_Size = newBlock.size / sizeof(T);
        memcpy(newBlock.memory, m_Data.memory, m_Data.size);
        m_Data = newBlock;
    }

    void shrink()
    {
        const auto newSize = (m_Size/2);
        auto newBlock = allocate_type<char>(g_Allocator, newSize*sizeof(T));
        m_Size = newBlock.size / sizeof(T);
        memcpy(newBlock.memory, m_Data.memory, m_Count*sizeof(T));
        m_Data = newBlock;
    }


};
