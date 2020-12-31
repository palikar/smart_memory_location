#pragma once

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

using SmallAllocator = FallbackAllocator<GeneralAllocator<BlockAllocator, Mallocator, Megabytes(4)>, Mallocator>;

using GeneralType = Segregator<256, SmallAllocator, LargeAllocator>;    

using MainAllocator = WrappedAllocator<GeneralType>;


// Large branch of the temp allocator
using StepAllocator = CascadeAllocator<LinearAllocator<Mallocator, Kilobytes(8)>, Mallocator>;
using SimpleTempAllocator = FallbackAllocator<TempBumpAllocator, StepAllocator>;


// Small branch of the temp allocator
using SimpleStackAllocator = StackAllocator<Kilobytes(16)>;
using SmallStepAllocator = CascadeAllocator<LinearAllocator<Mallocator, Kilobytes(1)>, Mallocator>;

using StackTempAllocator = FallbackAllocator<SimpleStackAllocator, SmallStepAllocator>;

using TempType = Segregator<256, StackTempAllocator, SimpleTempAllocator>;

using TempAllocator = WrappedAllocator<TempType>;
