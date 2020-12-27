#pragma once

#include "memory.hpp"
#include "utils.hpp"


namespace detail
{

template <typename T>
T rotr(T x, unsigned k) {
    return (x >> k) | (x << (8U * sizeof(T) - k));
}


inline size_t hash_int(uint64_t x) {

    auto h1 = x * UINT64_C(0xA24BAED4963EE407);
    auto h2 = rotr(x, 32U) * UINT64_C(0x9FB21C651E98DF25);
    auto h = rotr(h1 + h2, 32U);
    return static_cast<size_t>(h);
}


}



template<typename T>
struct hash
{
    static size_t hash_value(const T&) noexcept
    {
        return 0;
    }
};


template<typename Value>
struct KeyValue
{
    int key{0};
    Value value{0};

};

#define ASL_HASH_INT(T)                                             \
    template <>                                                     \
    struct hash<T> {                                                \
        static size_t hash_value(T const& obj) noexcept {           \
            return detail::hash_int(static_cast<uint64_t>(obj));    \
        }                                                           \
    }

ASL_HASH_INT(bool);
ASL_HASH_INT(char);
ASL_HASH_INT(signed char);
ASL_HASH_INT(unsigned char);
ASL_HASH_INT(char16_t);
ASL_HASH_INT(char32_t);
ASL_HASH_INT(short);
ASL_HASH_INT(unsigned short);
ASL_HASH_INT(int);
ASL_HASH_INT(unsigned int);
ASL_HASH_INT(long);
ASL_HASH_INT(long long);
ASL_HASH_INT(unsigned long);
ASL_HASH_INT(unsigned long long);



template<typename T>
struct Map
{
    using PairType = KeyValue<T>;
    using BlockType = TypedBlock<char>;
    const static size_t SENTINEL_VALUE = 0xFFFFFFFFFFFFFFFF;

    BlockType m_Data;
    size_t m_Count;
    size_t m_Size;

    void init(size_t t_Size = 16)
    {
        m_Data = allocate_type<char>(g_Allocator, (t_Size + 8)*sizeof(PairType));
        m_Count = 0;
        m_Size = m_Data.size / sizeof(T);
    }

    void push_back(int t_Key, const T& t_Element)
    {
        assert(m_Data.memory);
        assert(t_Key != 0);

        if (m_Count == m_Size)
        {
            grow();
        }

        auto slot_index = hash<int>::hash_value(t_Key)& m_Size;
        auto slot_memory = (PairType*)(m_Data + slot_index*sizeof(T));
        size_t cnt{0};

        while (slot_memory->key != 0 && slot_memory->key != t_Key)
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

    T& operator[](int t_Key) const
    {
        assert(m_Data.memory);

        auto slot_index = hash<int>::hash_value(t_Key)& m_Size;
        auto slot_memory = (PairType*)(m_Data.memory + slot_index*sizeof(T));
        while (slot_memory->key != t_Key)
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

    T* at(int t_Key) const
    {

        assert(m_Data.memory);

        auto slot_index = hash<int>::hash_value(t_Key)& m_Size;
        auto slot_memory = (PairType*)(m_Data.memory + slot_index*sizeof(T));
        while (slot_memory->key != t_Key)
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

    bool erase(int t_Key)
    {
        auto slot_index = hash<int>::hash_value(t_Key)& m_Size;
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

        slot_memory->key = SENTINEL_VALUE;
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
        return (size_t)slot_memory->key == SENTINEL_VALUE;
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
