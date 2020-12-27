#pragma once

#include "memory.hpp"
#include "utils.hpp"



template<typename Key, typename Value>
struct KeyValue
{
    Key key{0};
    Value value{0};

};

template<typename Key, typename T>
struct Map
{
    using PairType = KeyValue<Key, T>;
    using BlockType = TypedBlock<char>;

    BlockType m_Data;
    size_t m_Count;
    size_t m_Size;
    

    void init(size_t t_Size = 16)
    {
        m_Data = allocate_type<char>(g_Allocator, (t_Size + 8)*sizeof(PairType));
        m_Count = 0;
        m_Size = m_Data.size / sizeof(T);
    }

    void push_back(const Key& t_Key, const T& t_Element)
    {
        assert(m_Data.memory);
        assert(!invalid<Key>::is_invalid(t_Key));

        if (m_Count == m_Size)
        {
            grow();
        }

        auto slot_index = hash<Key>::hash_value(t_Key)& m_Size;
        auto slot_memory = (PairType*)(m_Data + slot_index*sizeof(T));
        size_t cnt{0};

        while (!slot_memory->key && compare<Key>::cmp(slot_memory->key, t_Key) == 0)
        {
            ++slot_memory;
            ++cnt;
            if(is_passed_the_end(slot_memory))
            {
                slot_memory = (PairType*)(m_Data + (cnt + slot_index)*sizeof(T));
            }

        }

        ++m_Count;
        *slot_memory = PairType{t_Key, t_Element};
    }

    T& operator[](const Key& t_Key) const
    {
        assert(m_Data.memory);

        auto slot_index = hash<Key>::hash_value(t_Key)& m_Size;
        auto slot_memory = (PairType*)(m_Data.memory + slot_index*sizeof(T));
        while (compare<Key>::cmp(slot_memory->key,t_Key) != 0)
        {
            ++slot_memory;

            if (slot_memory->key == 0)
            {
                assert(false);
            }

            if (is_passed_the_end(slot_memory))
            {
                assert(false);
            }
        }

        return slot_memory->value;;
    }

    T* at(const Key& t_Key) const
    {

        assert(m_Data.memory);

        auto slot_index = hash<Key>::hash_value(t_Key)& m_Size;
        auto slot_memory = (PairType*)(m_Data.memory + slot_index*sizeof(T));
        while (compare<Key>::cmp(slot_memory->key,t_Key) != 0)
        {
            ++slot_memory;

            if (slot_memory->key == 0)
            {
                return nullptr;
            }

            if (is_passed_the_end(slot_memory))
            {
                return nullptr;
            }


        }

        return &slot_memory->value;;

    }

    void clear()
    {
        m_Count = 0;
        memset(m_Data.memory, 0, m_Data.size);
    }

    size_t count() const
    {
        return m_Count;
    }

    bool erase(const Key& t_Key)
    {
        auto slot_index = hash<Key>::hash_value(t_Key)& m_Size;
        auto slot_memory = (PairType*)(m_Data.memory + slot_index*sizeof(T));
        while (slot_memory->key != t_Key)
        {
            ++slot_memory;

            if (slot_memory->key == 0)
            {
                return false;
            }

            if (is_passed_the_end(slot_memory))
            {
                return true;
            }
        }

        slot_memory->key = invalid<Key>::make_invalid();
        return true;
    }

    PairType* first()
    {
        return (PairType*)m_Data.memory;
    }

    PairType* next(PairType* t_Pair)
    {
        ++t_Pair;
        while(is_sentinel(t_Pair) || is_empty(t_Pair))
        {
            if(is_passed_the_end(t_Pair))
            {
                return nullptr;
            }

            ++t_Pair;
        }

        return t_Pair;
    }

  private:

    inline  bool is_passed_the_end(PairType* slot_memory) const
    {
        return (char*)slot_memory >= m_Data.memory + m_Size*sizeof(PairType);
    }

    inline static bool is_sentinel(PairType* slot_memory)
    {
        return invalid<Key>::is_invalid(slot_memory->key);
    }

    inline static bool is_empty(PairType* slot_memory)
    {
        return slot_memory->key == 0;
    }

    
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
