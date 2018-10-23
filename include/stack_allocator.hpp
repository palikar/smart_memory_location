#pragma once
#include "utils.hpp"

namespace saloc
{  
  template <std::size_t N>
  class StackAllocator : public saloc::Allocator
  {
  private:
    char stack[N]; // the buffer for all the pointers
    char* pos; // the current position in buffer
  public:
    StackAllocator() : pos(stack)
      {
        std::memset(stack, 0, N);        
      }

    Blk allocate(const std::size_t n)
      {
        std::size_t  n1 = saloc::roundToAllign(n);
        if ((stack + N) - pos <  n1)
        {
          return {nullptr, 0};
        }
        Blk blk = {pos, n1};
        pos += n1;
        return blk;
      }

    void deallocateAll()
      {
        std::memset(stack, 0, (pos - stack));
        pos = stack;
      }

    void deallocate(const Blk blk)
      {
        auto ptr = static_cast<char*>(blk.ptr);
        if(ptr + blk.size == pos)
        {
          pos = ptr;
        }    
      }

    bool owns(Blk blk)
      {
        return blk.ptr >= stack && blk.ptr < stack + N;
      }
  };
}
