#pragma once
#include <limits>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <new>
#include <cassert>

// Note(Stan) : Clean up this
#include <iostream>

#include <type_traits>


namespace detail
{

template<typename Fun>
struct __scope_exit {
    Fun __fun;

    ~__scope_exit()
    {
        __fun();
    }

};

enum class __scope_guard{};

template<typename Fun>
__scope_exit<Fun>
operator+ (__scope_guard, Fun&& fun)
{
    return __scope_exit<Fun>{fun};
}


}

#define CONCAT_IMPL(s1, s2) s1##s2
#define CONCAT(s1, s2) CONCAT_IMPL(s1, s2)
#define ANONYMOUS_VARIABLE(str) CONCAT(str, __LINE__)
#define DEFER auto ANONYMOUS_VARIABLE(__guard) = detail::__scope_guard() + [&]()

#define Kilobytes(num) size_t(num*1024)
#define Megabytes(num) size_t(num*1024*1024)
#define Gigabytes(num) size_t(num*1024*1024*1024)


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

template<typename T>
struct compare
{
    static int8_t cmp(const T&, const T&) noexcept
    {
        return 0;
    }
};

template<typename T>
struct invalid
{
    static int8_t is_invalid(const T) noexcept
    {
        return 0;
    }

    static T make_invalid() noexcept
    {
        return {};
    }
};


#define ASL_HASH_INT(T)                                             \
    template <>                                                     \
    struct hash<T> {                                                \
        static size_t hash_value(T const& obj) noexcept {           \
            return detail::hash_int(static_cast<uint64_t>(obj));    \
        }                                                           \
    }


#define ASL_CMP_INT(T)                                                  \
    template <>                                                         \
    struct compare<T> {                                                 \
        static int8_t cmp(const T& lhs, const T& rhs) noexcept {        \
            return static_cast<uint64_t>(lhs) - static_cast<uint64_t>(rhs); \
        }                                                               \
    }



#define ASL_VALID_INT(T)                                            \
    template <>                                                     \
    struct invalid<T> {                                             \
        static int8_t is_invalid(const T& obj) noexcept {           \
            return std::numeric_limits<T>::max() == obj ? 1 : -1;   \
        }                                                           \
                                                                    \
        static T make_invalid() noexcept {                          \
            return std::numeric_limits<T>::max();                   \
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

ASL_CMP_INT(bool);
ASL_CMP_INT(char);
ASL_CMP_INT(signed char);
ASL_CMP_INT(unsigned char);
ASL_CMP_INT(char16_t);
ASL_CMP_INT(char32_t);
ASL_CMP_INT(short);
ASL_CMP_INT(unsigned short);
ASL_CMP_INT(int);
ASL_CMP_INT(unsigned int);
ASL_CMP_INT(long);
ASL_CMP_INT(long long);
ASL_CMP_INT(unsigned long);
ASL_CMP_INT(unsigned long long);


ASL_VALID_INT(bool);
ASL_VALID_INT(char);
ASL_VALID_INT(signed char);
ASL_VALID_INT(unsigned char);
ASL_VALID_INT(char16_t);
ASL_VALID_INT(char32_t);
ASL_VALID_INT(short);
ASL_VALID_INT(unsigned short);
ASL_VALID_INT(int);
ASL_VALID_INT(unsigned int);
ASL_VALID_INT(long);
ASL_VALID_INT(long long);
ASL_VALID_INT(unsigned long);
ASL_VALID_INT(unsigned long long);


struct BlobReader
{
    char* m_Buffer;

    template<typename T>
    T readType()
    {
        const auto value = *(T*)m_Buffer;
        m_Buffer += sizeof(T);
        return value;
    }

};


template<typename First, typename Second>
struct Pair
{
    First first{0};
    Second second{0};
};


template<typename T1, typename T2, typename T = std::common_type_t<T1, T2>>
T max(const T1& t_Element1, const T2& t_Element2)
{
    
    return compare<T>::cmp(t_Element1, t_Element2) > 0 ? t_Element1 : t_Element2;
}


template<typename T1, typename ... Ts>
auto max(const T1& t_Element1, const Ts& ... t_Elements)
{
    return max(t_Element1, max(t_Elements ...));
}

template<typename T1, typename T2, typename T = std::common_type_t<T1, T2>>
T min(const T1& t_Element1, const T2& t_Element2)
{
    
    return compare<T>::cmp(t_Element1, t_Element2) <= 0 ? t_Element1 : t_Element2;
}

template<typename T1, typename ... Ts>
auto min(const T1& t_Element1, const Ts& ... t_Elements)
{
    return min(t_Element1, min(t_Elements ...));
}

