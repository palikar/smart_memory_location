#pragma once
#include "utils.hpp"
#include "memory.hpp"



template<class Primary, class Fallback>
struct FallbackAllocator
{
    Primary m_Primary;
    Fallback m_Fallback;

    MemoryBlock allocate(std::size_t size)
    {
        MemoryBlock ptr = m_Primary.allocate(size);
        
        if (!ptr.memory)
        {
            ptr = m_Fallback.allocate(size);
        }
        return ptr;
    }
    
    void deallocate(MemoryBlock ptr)
    {
        if (m_Primary.owns(ptr))
            m_Primary.deallocate(ptr);
        else
            m_Fallback.deallocate(ptr);
    }

    bool owns(MemoryBlock ptr)
    {
        return m_Primary.owns(ptr) || m_Fallback.owns(ptr);
    }

    
    void destroy()
    {
        m_Primary.destroy();
        m_Fallback.destroy();
    }    


    
};

  

