#include <iostream>

#include "memory.hpp"
#include "allocators.hpp"


void fun()
{
    TempAllocator tempAlloc;
    
    auto int_1 = allocate_type<int>(tempAlloc);
    *int_1 = 32;

    for (size_t i = 0; i < 30; ++i) {
        auto int_2 = allocate_type<int>(tempAlloc);
        *int_2 = 12;


    }
    
    tempAlloc.allocate(Megabytes(1));

    tempAlloc.destroy();
    Memory::reset_temp();
}

int main()
{
    MainAllocator globalAllocator;
    TempAllocator tempAllocator;
    Memory::init_memory(Megabytes(1), Megabytes(128));
    Memory::init_allocators(&globalAllocator, &tempAllocator);
    DEFER { Memory::destroy();};

    for (size_t i = 0; i < 30; ++i)
    {
        fun();
        
    }
    
    return 0;

}
