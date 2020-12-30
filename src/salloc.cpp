#include <iostream>

#include "memory.hpp"
#include "allocators.hpp"

struct Name
{
    int a;
    float b;
    unsigned c;

};

int main()
{
    MainAllocator globalAllocator;
    TempAllocator tempAllocator;
    Memory::init_memory(Megabytes(1), Megabytes(128));
    Memory::init_allocators(&globalAllocator, &tempAllocator);
    DEFER { Memory::destroy();};
    

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
