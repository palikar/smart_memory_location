#pragma once

#include <cctype>
#include <unistd.h>
#include <sys/mman.h>
#include "utils.hpp"



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

    uint8_t* current_offset{nullptr};

    void init_memory(size_t temp_size, size_t bulk_size)
    {
        // Todo(Stan): Add error checking here!
        temp_memory = temp_size;
        bulk_memory = bulk_size;

        const size_t total_size = temp_memory + bulk_memory;

        temp = mmap ( NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
        if (!temp) {
            std::cout << "There is no memory!" << "\n";
            return;

        }
        bulk = (uint8_t*)temp + temp_memory;

        current_offset = (uint8_t*)bulk;

        memset(temp,0, total_size);

    }

    void destroy()
    {
        munmap(temp, temp_memory + bulk_memory);
    }

    char* push_size(size_t t_size)
    {
        auto res = (char*)current_offset;
        memset(res, 0, t_size);
        current_offset += t_size;
        return res;
    }

    template<typename T>
    T* push_type()
    {
        auto res = (T*)current_offset;
        memset(res, 0, sizeof(T));
        current_offset += sizeof(T);
        return res;
    }

    template<typename T>
    T* push_array(size_t t_count)
    {
        auto res = (T*)current_offset;
        memset(res, 0, sizeof(T)*t_count);
        current_offset += sizeof(T)*t_count;
        return res;
    }

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

inline MemoryState g_Memory;
inline BaseAllocator* g_Allocator;


