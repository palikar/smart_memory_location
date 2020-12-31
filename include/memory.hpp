#pragma once

#include "utils.hpp"
#include <sys/mman.h>


struct MemoryBlock
{
    char* memory{nullptr};
    size_t size{0};
};

inline MemoryBlock g_NullBlock{0};

struct BaseAllocator
{

    virtual MemoryBlock allocate(size_t t_size) = 0;
    virtual void deallocate(MemoryBlock t_block) = 0;
    virtual MemoryBlock allocate_aligned(size_t t_size, size_t t_alignment) = 0;
    virtual void dallocate_aligned(MemoryBlock t_block) = 0;
    virtual void destroy() = 0;


    virtual ~BaseAllocator(){}
};

struct MemoryState
{

    size_t m_TempMemorySize{0};
    size_t m_BulkMemorySize{0};

    uint8_t* m_TempMemory{nullptr};
    uint8_t* m_BulkMemory{nullptr};

    uint8_t* m_BulkMemoryOffset{nullptr};
    uint8_t* m_TempMemoryOffset{nullptr};
};

template<typename T>
struct TypedBlock
{
    T* memory{nullptr};
    size_t size{0};

    T& operator*()
    {
        return *memory;
    }

    T* operator->()
    {
        return memory;
    }

    T* operator+(size_t offset)
    {
        return memory + offset;
    }

    void operator=(T* value)
    {
        memory = value;
    }

    void operator[](size_t index)
    {
        return memory[index];
    }

    operator bool()
    {
        return !!memory;
    }

    operator T*()
    {
        return memory;
    }


};

using ByteBlock = TypedBlock<char>;


inline size_t roundToAlign(size_t t_size)
{
    int a;
    a = t_size-1;
    a = a >> 4;
    a = a + 1;
    return a << 4;
}

template<typename Allocator>
struct WrappedAllocator : BaseAllocator
{
    Allocator m_Allocator;

    MemoryBlock allocate(size_t t_Size) override
    {
        return m_Allocator.allocate(t_Size);
    }

    void deallocate(MemoryBlock t_Block) override
    {
        m_Allocator.deallocate(t_Block);
    }

    MemoryBlock allocate_aligned(size_t, size_t) override
    {
        return {};
    }

    void dallocate_aligned(MemoryBlock) override
    {
    }

    void destroy() override
    {
        m_Allocator.destroy();
    }

};

inline MemoryState g_Memory;
inline BaseAllocator* g_Allocator;
inline BaseAllocator* g_TempAllocator;

struct Memory
{

    static void init_memory(size_t temp_size, size_t bulk_size)
    {
        // Todo(Stan): Add error checking here!
        g_Memory.m_TempMemorySize = temp_size;
        g_Memory.m_BulkMemorySize = bulk_size;

        const size_t total_size = g_Memory.m_TempMemorySize + g_Memory.m_BulkMemorySize;

        g_Memory.m_TempMemory = (uint8_t*)mmap ( NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
        if (!g_Memory.m_TempMemory) {
            assert(false);
        }
        g_Memory.m_BulkMemory = (uint8_t*)g_Memory.m_TempMemory + g_Memory.m_TempMemorySize;

        g_Memory.m_BulkMemoryOffset = (uint8_t*)g_Memory.m_BulkMemory;
        g_Memory.m_TempMemoryOffset = (uint8_t*)g_Memory.m_TempMemory;
        
        memset(g_Memory.m_TempMemory, 0, total_size);

    }

    static void init_allocators(BaseAllocator* t_Global, BaseAllocator* t_Temporary)
    {
        g_Allocator = t_Global;
        g_TempAllocator = t_Temporary;
    }

    static void destroy()
    {
        reset_temp();
        g_Allocator->destroy();
        g_TempAllocator->destroy();
        munmap(g_Memory.m_TempMemory, g_Memory.m_TempMemorySize + g_Memory.m_BulkMemorySize);
    }

    static char* push_size(size_t t_size)
    {
        auto res = (char*)g_Memory.m_BulkMemoryOffset;
        memset(res, 0, t_size);
        g_Memory.m_BulkMemoryOffset += t_size;
        return res;
    }

    template<typename T>
    static T* push_type()
    {
        auto res = (T*)g_Memory.m_BulkMemoryOffset;
        memset(res, 0, sizeof(T));
        g_Memory.m_BulkMemoryOffset += sizeof(T);
        return res;
    }

    template<typename T>
    static T* push_array(size_t t_count)
    {
        auto res = (T*)g_Memory.m_BulkMemoryOffset;
        memset(res, 0, sizeof(T)*t_count);
        g_Memory.m_BulkMemoryOffset += sizeof(T)*t_count;
        return res;
    }

    static char* push_size_temp(size_t t_size)
    {
        auto res = (char*)g_Memory.m_TempMemoryOffset;
        g_Memory.m_TempMemoryOffset += t_size;
        return res;
    }

    template<typename T>
    static T* push_type_temp()
    {
        auto res = (T*)g_Memory.m_TempMemoryOffset;
        memset(res, 0, sizeof(T));
        g_Memory.m_TempMemoryOffset += sizeof(T);
        return res;
    }

    template<typename T>
    static T* push_array_temp(size_t t_count)
    {
        auto res = (T*)g_Memory.m_TempMemoryOffset;
        memset(res, 0, sizeof(T)*t_count);
        g_Memory.m_TempMemoryOffset += sizeof(T)*t_count;
        return res;
    }

    static void reset_temp()
    {
        g_Memory.m_TempMemoryOffset = (uint8_t*)g_Memory.m_TempMemory;
    }

};


template<typename T, typename Allocator>
TypedBlock<T> allocate_type(Allocator& allocator, size_t count = 1)
{
    auto res = allocator.allocate(sizeof(T)*count);
    return {(T*)res.memory, res.size};

}

template<typename T, typename Allocator>
TypedBlock<T> allocate_type(Allocator* allocator, size_t count = 1)
{
    auto res = allocator->allocate(sizeof(T)*count);
    return {(T*)res.memory, res.size};

}

template< typename Allocator, typename T>
void deallocate_type(Allocator& allocator, const TypedBlock<T>& block)
{
    allocator.deallocate({(char*)block.memory, block.size});
}

template< typename Allocator, typename T>
void deallocate_type(Allocator* allocator, const TypedBlock<T>& block)
{
    allocator->deallocate({(char*)block.memory, block.size});
}

template< typename T>
void galloc(size_t count = 1)
{
    allocate_type<T>(g_Allocator, count);
}

template< typename T>
void gdealloc(const TypedBlock<T>& block)
{
    deallocate_type(g_Allocator, block);
}

template< typename T>
void talloc(size_t count = 1)
{
    allocate_type<T>(g_Allocator, count);
}
