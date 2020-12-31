#pragma once

#include "memory.hpp"
#include "utils.hpp"
#include "SpookyV2.h"

#include <cstring>

struct TempString
{
    using BlockType = TypedBlock<char>;

    BlockType m_Data{0};
    size_t m_Count{0};
    size_t m_Size{0};

    TempString()
    {
     
    }

    void init(size_t t_Size) 
    {
        m_Data = talloc<char>(t_Size*sizeof(char));
        m_Size = m_Data.size;
        m_Count = 0;
    }

    TempString(const char* buf)
    {
        auto size = std::strlen(buf)+1;
        m_Data = talloc<char>(size);
        m_Count = size;
        strcpy(m_Data.memory, buf);
        m_Size = m_Data.size;
    }

    operator bool()
    {
        return m_Size != 0;
    }

    void set_invalid()
    {
        m_Count = 0xFFFFFFFFFFFFFFFF;
    }

    void set(const char* buf)
    {
        auto size = std::strlen(buf);
        if (size < m_Size)
        {
            grow(size);
        }
        m_Count = size;
        strcpy(m_Data.memory, buf);
    }

    void append(char t_Element)
    {
        assert(m_Data.memory);
        if (m_Count == m_Size)
        {
            grow((size_t)m_Size*1.5);
        }
        *(m_Data + m_Count) = t_Element;
        ++m_Count;
    }

    void append(char* t_Contents, size_t t_Size)
    {
        assert(m_Data.memory);
        if (m_Count == m_Size)
        {
            grow((size_t)m_Size*1.5);
        }
        memcpy(m_Data + m_Count, t_Contents, t_Size);
        m_Count += t_Size;
    }

    inline char operator[](size_t t_Index) const
    {
        assert(m_Data.memory);
        assert(t_Index < m_Size);
        return *((char*)(m_Data.memory + t_Index));
    }

    void clear()
    {
        assert(m_Data.memory);
        m_Count = 0;
    }

    inline char* data() const
    {
        return m_Data.memory;
    }

    inline size_t count() const
    {
        return m_Count;
    }

    inline bool empty() const
    {
        return m_Count == 0;
    }

    void erase(size_t t_Index)
    {
        memcpy(m_Data.memory + t_Index, m_Data.memory + (t_Index + 1) , (m_Count - t_Index - 1));
        --m_Count;
        if (m_Count < m_Size/2)
        {
            shrink();
        }
    }

    void erase(size_t t_Start, size_t t_End)
    {
        memcpy(m_Data.memory + t_Start, m_Data.memory + (t_End + 1) , (m_Count - t_End - 1));
        --m_Count;
        if (m_Count < m_Size/2)
        {
            shrink();
        }
    }

    const char* data()
    {
        return m_Data.memory;
    }

    bool empty()
    {
        return m_Count == 0;
    }

    void destory()
    {
    }
    

  private:

    void grow(size_t t_Size)
    {
        const auto newSize = t_Size;
        auto newBlock = talloc<char>(newSize);
        m_Size = newBlock.size;
        memcpy(newBlock.memory, m_Data.memory, m_Data.size);
        m_Data = newBlock;
    }

    void shrink()
    {
        const auto newSize = (m_Size/2);
        auto newBlock = talloc<char>(newSize);
        m_Size = newBlock.size;
        memcpy(newBlock.memory, m_Data.memory, m_Count);
        m_Data = newBlock;
    }


};

template <>
struct hash<TempString> {
    static size_t hash_value(const TempString& obj) noexcept {
        return SpookyHash::Hash64(obj.data(), obj.count(), 0x435F4325F213AAB4);
    }
};


template <>
struct compare<TempString> {
    static int8_t cmp(const TempString& lhs, const TempString& rhs) noexcept {
        if(lhs.m_Count != rhs.m_Count || !lhs.m_Data.memory || !rhs.m_Data.memory)
        {
            return -1;
        }
        return strcmp(lhs.data(), rhs.data());
    }
};


template <>
struct invalid<TempString> {
    static int8_t is_invalid(const TempString& obj) noexcept {
        return std::numeric_limits<size_t>::max() == obj.m_Count ? 1 : -1;
    }

    static TempString make_invalid() noexcept
    {
        TempString invalidString;
        invalidString.set_invalid();
        return invalidString;
    }
};

