#include <iostream>

#include "memory.hpp"

#include "containers/vector.hpp"
#include "containers/map.hpp"
#include "containers/string.hpp"

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


    Map<String, float> map;
    map.init();
 
    map.push_back("Stanislav", 42);
    map.push_back("Marina", 44);
    map.push_back("Marina_1", 44);
    map.push_back("Marina_2", 445);
    map.push_back("Marina_3", 446);
    map.push_back("Marina_4", 447);
    map.push_back("Marina_5", 448);
    map.push_back("Marina_6", 449);
    map.push_back("Marina_7", 4410);
    map.push_back("Marina_8", 4411);


    std::cout << map["Stanislav"] << '\n';
    std::cout << map["Marina"] << '\n';
    std::cout << map["Marina_1"] << '\n';
    std::cout << map["Marina_2"] << '\n';
    std::cout << map["Marina_3"] << '\n';
    std::cout << map["Marina_4"] << '\n';
    std::cout << map["Marina_5"] << '\n';
    std::cout << map["Marina_6"] << '\n';
    std::cout << map["Marina_7"] << '\n';
    std::cout << map["Marina_8"] << '\n';

    

    auto it = map.first();
    size_t count{1};
    while (it) {
        auto& [key, value] = *it;
        std::cout << count++ << " --> " << key.data() << " : " << value << "\n";
        it = map.next(it);
    };

    
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
