#pragma once

#include "memory.hpp"

#include <algorithm>
#include <iostream>

// @Note(Stan): we can optionally add tags here
template<typename Primary>
struct TrackingAllocator : public Primary
{

    size_t m_Allocations{0};
    size_t m_Deallocations{0};
    size_t m_UsedMemory{0};
    size_t m_PeakMemory{0};
    
    
    MemoryBlock allocate(std::size_t t_Size)
    {

        auto res = Primary::allocate(t_Size);
        ++m_Allocations;
        m_UsedMemory += res.size;
        m_PeakMemory = std::max(m_PeakMemory, m_UsedMemory);
        return res;

    }
    
    void deallocate(MemoryBlock t_Block)
    {
        --m_Deallocations;
        m_UsedMemory += t_Block.size;
        Primary::deallocate(t_Block);
    }

    void owns(MemoryBlock t_Block)
    {
        Primary::owns(t_Block);
    }

    void destroy()
    {
        Primary::destroy();
        std::cout << "Allocations: " << m_Allocations << "\n";
        std::cout << "Deallocations: " << m_Deallocations << "\n";
        std::cout << "PeakMemory: " << m_PeakMemory << "\n";

    }
    
};

