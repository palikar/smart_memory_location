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


int main()
{

    g_Memory.init_memory(Megabytes(1), Megabytes(128));
    MainAllocator allocator;
    g_Allocator = &allocator;
    
    Platform::init();

    DEFER { Platform::destory(); allocator.destroy(); g_Memory.destroy();};

    auto file = Platform::openFile("file.txt");
    auto contents = Platform::readWholeFile(file);
    Platform::closeFile(file);
    
    Platform::print("Hello world!\n");
    Platform::print(contents);




}
