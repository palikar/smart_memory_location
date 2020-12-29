#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>


#include "platform.hpp"
#include "memory.hpp"

using ArenaAllocator = FixedArenaAllocator<Mallocator, sizeof(Block), 58>;
using BlockAllocator = CascadeAllocator<ArenaAllocator, FallbackAllocator<LinearAllocator<Mallocator, 512*2>, Mallocator>>;
using LargeAllocator = GeneralAllocator<BlockAllocator, BumpAllocator, Megabytes(64)>;
using SmallAllocator = FallbackAllocator<GeneralAllocator<BlockAllocator, Mallocator, Megabytes(4)>, Mallocator>;
using GeneralType = Segregator<256, SmallAllocator, LargeAllocator>;    
using MainAllocator = WrappedAllocator<GeneralType>;    

struct SomeWeird
{
    int a;
    float b;
    float c;
    double d;
    unsigned e;
};

int main()
{

    g_Memory.init_memory(Megabytes(1), Megabytes(128));
    MainAllocator allocator;
    g_Allocator = &allocator;
    
    Platform::init();
    
    DEFER { Platform::destory(); allocator.destroy(); g_Memory.destroy();};

    // auto file = Platform::openFile("file_3.txt", FileMode::Write);
    // Platform::writeToFile<int>(file, 65);
    // Platform::writeToFile<int>(file, 67);
    // Platform::writeToFile<int>(file, 68);
    // Platform::writeToFile<float>(file, 43.34);
    // Platform::writeToFile<SomeWeird>(file, {43,43.2, 43.1, 43.3, 23});

    // auto file = Platform::openFile("file_3.txt", FileMode::Read);
    // auto contents = Platform::readWholeFileBinary(file);

    // BlobReader reader{contents};
    
    // std::cout << reader.readType<int>() << "\n";
    // std::cout << reader.readType<int>() << "\n";
    // std::cout << reader.readType<int>() << "\n";
    // std::cout << reader.readType<float>() << "\n";
    
    // auto var = reader.readType<SomeWeird>();
    // std::cout << var.a << "\n";
    // std::cout << var.b << "\n";
    // std::cout << var.c << "\n";
    // std::cout << var.d << "\n";
    // std::cout << var.e << "\n";

    // std::cout << *(int*)(contents.memory + sizeof(int)*0) << "\n";
    // std::cout << *(int*)(contents.memory + sizeof(int)*1) << "\n";
    // std::cout << *(int*)(contents.memory + sizeof(int)*2) << "\n";
    // std::cout << *(float*)(contents.memory + sizeof(int)*3) << "\n";
    

    // auto contents = Platform::readWholeFileBinary(file);
    // for (size_t i = 0; i < contents.size; ++i)
    // {
    //     printf("0x%x", (*(contents.memory+i)) & 0xff);
    // }

    
    // Platform::closeFile(file);
    
    // Platform::print("Hello world!\n");
    // Platform::print(contents);




}
