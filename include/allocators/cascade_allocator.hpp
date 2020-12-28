#pragma once


#include "memory.hpp"


template<typename Allocator>
struct AllocatorBlock
{
    Allocator m_Allocator;
    TypedBlock<AllocatorBlock<Allocator>> next;
};

template<typename Allocator, typename Primary>
struct CascadeAllocator : private Primary
{
    using BlockType = AllocatorBlock<Allocator>;

    TypedBlock<AllocatorBlock<Allocator>> m_Allocators;


    MemoryBlock allocate(size_t t_Size)
    {

        if(!m_Allocators)
        {
            m_Allocators = allocate_type<BlockType, Primary>(*static_cast<Primary*>(this));
            new(m_Allocators.memory) BlockType();

        }

        BlockType* current = m_Allocators;
        while (current)
        {

            auto res = current->m_Allocator.allocate(t_Size);
            if(res.memory)
            {
                return res;
            }

            if(!current->next)
            {
                break;
            }
            
            current = current->next;
        }

        current->next = allocate_type<AllocatorBlock<Allocator>, Primary>(*static_cast<Primary*>(this));

        new(current->next.memory) BlockType();
        auto res = current->next->m_Allocator.allocate(t_Size);
        return res;

    }

    void deallocate(MemoryBlock t_Block)
    {

        BlockType* current = m_Allocators;
        while (current)
        {
            if(current->m_Allocator.owns(t_Block))
            {
                current->m_Allocator.deallocate(t_Block);
                return;
            }
            current = current->next.memory;
        }
        assert(false);

    }

    bool owns(MemoryBlock t_Block)
    {
        
        BlockType* current = m_Allocators;
        while (current)
        {
            if(current->m_Allocator.owns(t_Block))
            {
                return true;
                
            }
        }
        return false;

    }

    void destroy()
    {

        auto current = m_Allocators;
        while (current)
        {
            current->m_Allocator.destroy();
            auto next = current->next;
            deallocate_type(*static_cast<Primary*>(this), current);
            current = next;
        }
        
        Primary::destroy();

    }

    
};
