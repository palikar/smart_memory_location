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


using StepAllocator = CascadeAllocator<LinearAllocator<Mallocator, Kilobytes(8)>, Mallocator>;
using SimpleTempAllocator = FallbackAllocator<TempBumpAllocator, StepAllocator>;

using SimpleStackAllocator = StackAllocator<Kilobytes(16)>;
using SmallStepAllocator = CascadeAllocator<LinearAllocator<Mallocator, Kilobytes(1)>, Mallocator>;
using StackTempAllocator = FallbackAllocator<SimpleStackAllocator, SmallStepAllocator>;

using TempAllocator = Segregator<256, StackTempAllocator, SimpleTempAllocator>;


void fun()
{
    TempAllocator tempAlloc;
    
    auto int_1 = allocate_type<int>(tempAlloc);
    *int_1 = 32;
// std::cout << *int_1 << "\n";
    for (size_t i = 0; i < 30; ++i) {
        auto int_2 = allocate_type<int>(tempAlloc);
        *int_2 = 12;
        // std::cout << *int_2 << "\n";

    }
    
    tempAlloc.allocate(Megabytes(1));

    tempAlloc.destroy();
    g_Memory.reset_temp();
}

int main()
{
    g_Memory.init_memory(Megabytes(16), Megabytes(32));

    for (size_t i = 0; i < 30; ++i)
    {
        fun();
        
    }
    
    return 0;

}
