#pragma once

#include "memory.hpp"
#include "utils.hpp"


template<typename T>
struct List
{
    struct Block
    {
        T m_Element;
        TypedBlock<Block> m_Next;
    };
    
    using BlockType = TypedBlock<Block>;

    BlockType m_Data;
    size_t m_Count;
    
    void init()
    {
        m_Count = 0;
    }

    void push_back(const T& t_Element)
    {
        if(m_Count == 0)
        {
            m_Data = galloc<Block>();
            m_Data->m_Element = t_Element;
            m_Data->m_Next = nullptr;
            ++m_Count;
            return;
        }
        assert(m_Data.memory);

        auto current = m_Data;
        while(current->m_Next) { current = current->m_Next;}
        current->m_Next = galloc<Block>();
        current->m_Next->m_Element = t_Element;
        current->m_Next->m_Next = nullptr;
        ++m_Count;
        
    }

    template<typename ... Args>
    void emplace_back(Args ... t_Args)
    {
        assert(m_Data.memory);
        
        if(m_Count == 0)
        {
            m_Data = galloc<Block>();
            new(&m_Data->m_Element) T(t_Args ...);
            m_Data->m_Next = nullptr;
            ++m_Count;
            return;
        }

        auto current = m_Data;
        while(current->m_Next) { current = current->m_Next;}
        current->m_Next = galloc<Block>();
        new(&current->m_Next->m_Element) T(t_Args ...);
        current->m_Next->m_Next = nullptr;
        ++m_Count;

    }

    inline T& operator[](size_t t_Index) const
    {
        assert(m_Data.memory);
        assert(t_Index < m_Count);

        auto current = m_Data;
        size_t cnt = 0;
        while(cnt < t_Index)
        {
            current = current->m_Next;
            ++cnt;
        }
        
        return current->m_Element;
    }

    inline T& front() const
    {
        return this->operator[](0);
    }

    inline T& back() const
    {
        return this->operator[](m_Count - 1);
    }

    void clear()
    {
        assert(m_Data.memory);
        
        destroy();
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
        
        auto current = m_Data;
        size_t cnt = 0;
        while(cnt < m_Count - 1)
        {
            auto next = current->m_Next;
            gdealloc(current);
            current = next;
            ++cnt;
        }
        m_Count = 0;        
    }

    void erase(size_t t_Index)
    {
        
        auto current = m_Data;
        size_t cnt = 0;
        while(cnt < t_Index - 1 )
        {
            current = current->m_Next;
            ++cnt;
        }
        auto nextNext = current->m_Next->m_Next;
        gdealloc(current->m_Next);
        current->m_Next = nextNext;
        --m_Count;
        
    
    }

    void destroy()
    {
        auto current = m_Data;
        size_t cnt = 0;
        while(cnt < m_Count - 1)
        {
            auto next = current->m_Next;
            gdealloc(current);
            current = next;
            ++cnt;
        }
        m_Count = 0;    
    }    


};

