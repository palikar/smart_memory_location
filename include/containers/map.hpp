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
        memset(m_Data.memory, 0 , m_Data.size);
        m_Count = 0;
        m_Size = m_Data.size / sizeof(PairType);


    }

    void push_back(const Key& t_Key, const T& t_Element)
    {
        assert(m_Data.memory);
        assert(invalid<Key>::is_invalid(t_Key) < 0);

        if (m_Count == m_Size)
        {
            grow();
        }

        auto slot_index = hash<Key>::hash_value(t_Key) % m_Size;
        auto slot_memory = (PairType*)(m_Data.memory + slot_index*sizeof(PairType));

        while (slot_memory->key && compare<Key>::cmp(slot_memory->key, t_Key) != 0)
        {
            
            ++slot_memory;
        
            if(is_passed_the_end(slot_memory))
            {
                grow();
                push_back(t_Key, t_Element);
                return;
            }

        }

        ++m_Count;
        *slot_memory = PairType{t_Key, t_Element};
    }

    T& operator[](const Key& t_Key) const
    {
        assert(m_Data.memory);

        auto slot_index = hash<Key>::hash_value(t_Key) % m_Size;
        auto slot_memory = (PairType*)(m_Data.memory + slot_index*sizeof(PairType));


        std::cout << slot_memory->key.data() << "\n";
         
        while (compare<Key>::cmp(slot_memory->key, t_Key) != 0)
        {
            ++slot_memory;

            if (!slot_memory->key)
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

        auto slot_index = hash<Key>::hash_value(t_Key) % m_Size;
        auto slot_memory = (PairType*)(m_Data.memory + slot_index*sizeof(PairType));
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
        auto slot_index = hash<Key>::hash_value(t_Key) % m_Size;
        auto slot_memory = (PairType*)(m_Data.memory + slot_index*sizeof(PairType));
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
        auto firstPair = (PairType*)m_Data.memory;
        if(is_sentinel(firstPair) || is_empty(firstPair))
        {
            return next(firstPair);
        }
        return firstPair;

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
        return (char*)slot_memory >= (m_Data.memory + m_Data.size);
    }

    inline static bool is_sentinel(PairType* slot_memory)
    {
        return invalid<Key>::is_invalid(slot_memory->key) > 0;
    }

    inline static bool is_empty(PairType* slot_memory)
    {
        return !slot_memory->key;
    }

    
    void grow()
    {
        const auto newSize = (2*m_Size);

        auto newBlock = allocate_type<char>(g_Allocator, newSize*sizeof(PairType));
        m_Size = newBlock.size / sizeof(PairType);
        auto oldBlock = m_Data;
        m_Data = newBlock;

        auto pair = (PairType*)oldBlock.memory;
        do {
            while(is_sentinel(pair) || is_empty(pair))
            {
                ++pair;
            }

            push_back(pair->key, pair->value);
            ++pair;
            
        } while((char*)pair < oldBlock.memory + oldBlock.size);

        deallocate_type(*g_Allocator, oldBlock);
    }

    void shrink()
    {
        const auto newSize = (m_Size/2);
        auto newBlock = allocate_type<char>(g_Allocator, newSize*sizeof(PairType));
        m_Size = newBlock.size / sizeof(PairType);
        memcpy(newBlock.memory, m_Data.memory, m_Count*sizeof(PairType));
        m_Data = newBlock;
    }


};
