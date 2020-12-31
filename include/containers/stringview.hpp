#pragma once

#include "memory.hpp"
#include "utils.hpp"
#include "SpookyV2.h"

#include "containers/string.hpp"

#include <cstring>

struct StringView
{

    const char* m_Beginning{nullptr};
    size_t m_Count{0};


    StringView()
    {
        m_Beginning = nullptr;
        m_Count = 0;
    }
    
    StringView(const char* t_Str)
    {
        m_Beginning = t_Str;
        m_Count = strlen(t_Str);
    }

    StringView(const char* t_Str, size_t t_Count)
    {
        m_Beginning = t_Str;
        m_Count = t_Count;
    }

    const char* data() const
    {
        return m_Beginning;
    }

    size_t count() const
    {
        return m_Count;
    }

    operator bool()
    {
        return m_Count != 0;
    }

};


StringView make_stringview(const char* t_Str)
{
    return StringView(t_Str);
}

StringView make_stringview(const String& t_Str)
{
    return StringView(t_Str.data(), t_Str.count());
}


template <>
struct hash<StringView> {
    static size_t hash_value(const StringView& obj) noexcept {
        return SpookyHash::Hash64(obj.data(), obj.count(), 0x435F4325F213AAB4);
    }
};

template <>
struct compare<StringView> {
    static int8_t cmp(const StringView& lhs, const StringView& rhs) noexcept {
        if(lhs.m_Count != rhs.m_Count || !lhs.m_Beginning || !rhs.m_Beginning)
        {
            return -1;
        }
        return strcmp(lhs.data(), rhs.data());
    }
};

template <>
struct invalid<StringView> {
    static int8_t is_invalid(const StringView& obj) noexcept {
        return std ::numeric_limits<size_t>::max() == obj.m_Count ? 1 : -1;
    }

    static StringView make_invalid() noexcept
    {
        StringView invalidString{nullptr};
        invalidString.m_Count = std ::numeric_limits<size_t>::max();
        return invalidString;
    }
};
