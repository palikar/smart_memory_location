#pragma once
#include "utils.hpp"

namespace saloc
{

  template <bool Cleaning = false>
  class Mallocator
  {
  public:    
    Blk allocate(std::size_t n)
      {
        
        return {malloc(n), n};
      }

    void deallocate(Blk ptr)
      {
        if (Cleaning)
        {
          std::memset(ptr.ptr,0, ptr.size);
        }
        free(ptr.ptr);
      }
  };
}
