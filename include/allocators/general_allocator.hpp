#pragma once

#include "memory.hpp"

inline size_t roundGood(size_t t_size)
{
    int v = t_size - 1;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}


struct Block
{
    char* memory;
    size_t size;
    size_t used;

    TypedBlock<Block> next;

};


template<typename BlockAllocator, typename Allocator, size_t Size>
struct GeneralAllocator : public BlockAllocator, public Allocator
{

    TypedBlock<Block> m_Blocks;
    MemoryBlock m_BaseMemory;

    void init()
    {

        m_BaseMemory = Allocator::allocate(Size);

        m_Blocks = allocate_type<Block>(*static_cast<BlockAllocator*>(this));
        m_Blocks->memory = m_BaseMemory.memory;
        m_Blocks->size = Size;
        m_Blocks->used = 0;
        m_Blocks->next = nullptr;
    }

    MemoryBlock allocate(size_t t_Size)
    {
        if(!m_Blocks)
        {
            init();
        }
        
        MemoryBlock memoryBlock{0};

        const auto size = roundGood(t_Size);
        auto current = m_Blocks;

        do{

            if((current->size - current->used) >= size)
            {
                memoryBlock.memory = current->memory;
                memoryBlock.size = size;

                current->used = size;
                split(current);

                return memoryBlock;
            }

            current = current->next;
        } while(current);

        assert(memoryBlock.size != 0);

        return memoryBlock;
    }

    void deallocate(MemoryBlock t_Block)
    {

        auto current = m_Blocks;

        do{
            if(t_Block.memory == current->memory)
            {
                current->used -= current->size;
                maybeMerge(current);
                break;
            }

            current = current->next;
        } while(current);

    }

    bool owns(MemoryBlock t_Block)
    {
        auto current = m_Blocks;

        do{
            if (t_Block.memory == current->memory)
            {
                return true;
            }
            current = current->next;
        } while(current);

        return false;
    }

    void destroy()
    {
        Allocator::deallocate(m_BaseMemory);
        Allocator::destroy();
        BlockAllocator::destroy();
    }

  private:
    
    void split(TypedBlock<Block> t_Block)
    {
        if (t_Block->used == t_Block->size)
        {
            return;
        }

        auto newBlock = allocate_type<Block>(*static_cast<BlockAllocator*>(this));
        newBlock->memory = t_Block->memory + t_Block->used;
        newBlock->size = t_Block->size - t_Block->used;
        newBlock->used = 0;

        if(!t_Block->next)
        {
            t_Block->next = newBlock;
            newBlock->next = nullptr;
        }
        else
        {
            newBlock = t_Block->next;
            t_Block->next = newBlock;

        }

        t_Block->size = t_Block->used;
    }

    void maybeMerge(TypedBlock<Block> t_Block)
    {
        if(!t_Block->next)
        {
            return;
        }

        if (t_Block->next->used == 0 && t_Block->used == 0)
        {
            t_Block->size += t_Block->next->size;
            const auto nextNext = t_Block->next->next;
            deallocate_type(*static_cast<BlockAllocator*>(this),t_Block->next);
            t_Block->next = nextNext;

        }

    }




};
