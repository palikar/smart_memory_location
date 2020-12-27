#include "memory.hpp"
#include "vector.hpp"
#include "map.hpp"
#include "string.hpp"

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

using MainAllocator = WrappedAllocator<GeneralType>;


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

    // String name{"Stanislav Arnaudov"};
    // name.set("Stanislav ArnaudovStanislav Arnaudov");
    
    

    Map<String, float> map;
    map.init();
 
    map.push_back("String", 42);

    std::cout << map["String"] << "\n";

    // for (size_t i = 0; i < 30; ++i)
    // {
    //     map.push_back(1 + i*32, 30.43 / (i + 1));
    // }

    // map.erase(1);
    
    // auto it = map.first();
    // while (it) {
    //     auto& [key, value] = *it;
    //     std::cout << key << " : " << value << "\n";
    //     it = map.next(it);
    // };

// Vector<int> intVec;
// intVec.init();
        
// for (size_t i = 0; i < 34; ++i)
// {
//     intVec.push_back(i);
// }

// intVec.erase(2);
// intVec.erase(intVec.data() + 2);

// for (size_t i = 0; i < intVec.count(); ++i)
// {
//     std::cout << intVec[i] << "\n";
// }
    

    



    
    return 0;
}
