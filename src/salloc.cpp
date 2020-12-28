#include <iostream>

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

struct Name
{
    int a;
    float b;
    unsigned c;

};

int main()
{
    g_Memory.init_memory(Megabytes(1), Megabytes(128));
    
    using ArenaAllocator = FixedArenaAllocator<Mallocator, sizeof(Block), 58>;
    using BlockAllocator = CascadeAllocator<ArenaAllocator, FallbackAllocator<LinearAllocator<Mallocator, 512*2>, Mallocator>>;

    using LargeAllocator = GeneralAllocator<BlockAllocator, BumpAllocator, Megabytes(64)>;
    using SmallAllocator = FallbackAllocator<LinearAllocator<Mallocator, 512*2>, Mallocator>;
    using General = Segregator<256, SmallAllocator, LargeAllocator>;    
    
    General allocator;
    

    // DEFER {
    //     allocator.destroy();
    //     g_Memory.destroy();
    // };

    // for (size_t i = 0; i < 10; ++i) {
    //     [[maybe_unused]]auto blk = allocate_type<Name>(allocator, 40);
    //     // deallocate_type(allocator, blk);
    // }
    
    // auto blk1 = allocate_type<int>(allocator);
    // auto blk2 = allocate_type<float>(allocator);
    // auto blk3 = allocate_type<Name>(allocator);
    // *blk1 = 42;
    // std::cout << *blk1 << "\n";

    // blk3->a = 43;
    // blk3->b = 43.32;
    // std::cout << blk3->a << "\n";
    // std::cout << blk3->b << "\n";


    // deallocate_type(allocator, blk2);
    // deallocate_type(allocator, blk3);
    // deallocate_type(allocator, blk1);


    
    // blk1 = allocate_type<int>(allocator);
    // blk2 = allocate_type<float>(allocator);
    // blk3 = allocate_type<Name>(allocator);

    // *blk1 = 44;
    // std::cout << *blk1 << "\n";

    // blk3->a = 44;
    // blk3->b = 44.32;
    // std::cout << blk3->a << "\n";
    // std::cout << blk3->b << "\n";


    // deallocate_type(allocator, blk2);
    // deallocate_type(allocator, blk3);
    // deallocate_type(allocator, blk1);


    

    return 0;

}
