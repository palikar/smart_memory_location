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

    size_t temp_memory{0};
    size_t bulk_memory{0};

    void* temp{nullptr};
    void* bulk{nullptr};

    uint8_t* current_bulk_offset{nullptr};
    uint8_t* current_temp_offset{nullptr};
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
void deallocate_type(Allocator& allocator, TypedBlock<T> block)
{
    allocator.deallocate({(char*)block.memory, block.size});
}

template< typename Allocator, typename T>
void deallocate_type(Allocator* allocator, TypedBlock<T> block)
{
    allocator->deallocate({(char*)block.memory, block.size});
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
        g_Memory.temp_memory = temp_size;
        g_Memory.bulk_memory = bulk_size;

        const size_t total_size = g_Memory.temp_memory + g_Memory.bulk_memory;

        g_Memory.temp = mmap ( NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
        if (!g_Memory.temp) {
            assert(false);
        }
        g_Memory.bulk = (uint8_t*)g_Memory.temp + g_Memory.temp_memory;

        g_Memory.current_bulk_offset = (uint8_t*)g_Memory.bulk;
        g_Memory.current_temp_offset = (uint8_t*)g_Memory.temp;
        
        memset(g_Memory.temp, 0, total_size);

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
        munmap(g_Memory.temp, g_Memory.temp_memory + g_Memory.bulk_memory);
    }

    static char* push_size(size_t t_size)
    {
        auto res = (char*)g_Memory.current_bulk_offset;
        memset(res, 0, t_size);
        g_Memory.current_bulk_offset += t_size;
        return res;
    }

    template<typename T>
    static T* push_type()
    {
        auto res = (T*)g_Memory.current_bulk_offset;
        memset(res, 0, sizeof(T));
        g_Memory.current_bulk_offset += sizeof(T);
        return res;
    }

    template<typename T>
    static T* push_array(size_t t_count)
    {
        auto res = (T*)g_Memory.current_bulk_offset;
        memset(res, 0, sizeof(T)*t_count);
        g_Memory.current_bulk_offset += sizeof(T)*t_count;
        return res;
    }

    static char* push_size_temp(size_t t_size)
    {
        auto res = (char*)g_Memory.current_temp_offset;
        g_Memory.current_temp_offset += t_size;
        return res;
    }

    template<typename T>
    static T* push_type_temp()
    {
        auto res = (T*)g_Memory.current_temp_offset;
        memset(res, 0, sizeof(T));
        g_Memory.current_temp_offset += sizeof(T);
        return res;
    }

    template<typename T>
    static T* push_array_temp(size_t t_count)
    {
        auto res = (T*)g_Memory.current_temp_offset;
        memset(res, 0, sizeof(T)*t_count);
        g_Memory.current_temp_offset += sizeof(T)*t_count;
        return res;
    }

    static void reset_temp()
    {
        g_Memory.current_temp_offset = (uint8_t*)g_Memory.temp;
    }

};
