#include "memory.hpp"

#include "allocators/general_allocator.hpp"
#include "allocators/arena_allocator.hpp"
#include "allocators/fallback_allocator.hpp"
#include "allocators/malloc_allocator.hpp"
#include "allocators/segregator_allocator.hpp"
#include "allocators/stack_allocator.hpp"
#include "allocators/cascade_allocator.hpp"
#include "allocators/linear_allocator.hpp"
#include "allocators/tracking_allocator.hpp"
#include "allocators/bump_allocator.hpp"

using ArenaAllocator = FixedArenaAllocator<Mallocator, sizeof(Block), 58>;
using BlockAllocator = CascadeAllocator<ArenaAllocator, FallbackAllocator<LinearAllocator<Mallocator, 512*2>, Mallocator>>;
using LargeAllocator = GeneralAllocator<BlockAllocator, BumpAllocator, Megabytes(64)>;
using SmallAllocator = FallbackAllocator<LinearAllocator<Mallocator, 512*2>, Mallocator>;
using GeneralType = Segregator<256, SmallAllocator, LargeAllocator>;    

template<typename Allocator>
struct TypedAllocator : BaseAllocator
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

using MainAllocator = TypedAllocator<GeneralType>;


struct Name
{
    int a;
    float b;
    unsigned c;

};


int main()
{
    g_Memory.init_memory(Megabytes(1), Megabytes(128));
    MainAllocator allocator;
    DEFER { allocator.destroy(); g_Memory.destroy(); };
    g_Allocator = &allocator;

    Vector intVec;
    intVec.init();

        
    std::cout << "count: " << intVec.count() << "\n";
    std::cout << "size: " << intVec.m_Size << "\n";
    
    for (size_t i = 0; i < intVec.count(); ++i)
    {
        std::cout << intVec[i] << "\n";
    }
    



    
    return 0;
}
