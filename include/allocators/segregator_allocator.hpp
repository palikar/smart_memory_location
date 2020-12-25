#pragma once

#include "memory.hpp"


template<size_t Threshold, typename First, typename Second >
struct Segregator
{

    First m_First;
    Second m_Second;
 
    MemoryBlock allocate(size_t t_Size)
    {
        if (t_Size <= Threshold)
        {
            return m_First.allocate(t_Size);
        }
        else
        {
            return m_Second.allocate(t_Size);
        }
    }

    void deallocate(MemoryBlock t_Block)
    {
        
        if (t_Block.size < Threshold)
        {
            return m_First.deallocate(t_Block);
        }
        else
        {
            return m_Second.deallocate(t_Block);
        }    
    }

    
    void owns(MemoryBlock t_Block)
    {
        
        if (t_Block.size < Threshold)
        {
            return m_First.owns(t_Block);
        }
        else
        {
            return m_Second.owns(t_Block);
        }    
    }

    void destroy()
    {
        
        m_First.destroy();
        m_Second.destroy();
    }


};
