#pragma once
#include "utils.hpp"

namespace saloc
{

  template<class Primary, class Fallback>
  class FallbackAllocator : private Primary, private Fallback 
  {
  public:    
    Blk allocate(std::size_t size)
      {
        Blk ptr = Primary::allocate(size);
        if (!ptr.ptr)
          ptr = Fallback::allocate(size);
        return ptr;
      }
    
    void deallocate(Blk ptr)
      {
        if (Primary::owns(ptr))
          Primary::deallocate(ptr);
        else
          Fallback::deallocate(ptr);
      }

    bool owns(Blk ptr)
      {
        return Primary::owns(ptr) || Fallback::owns(ptr);
      }
    
  };

  

}
