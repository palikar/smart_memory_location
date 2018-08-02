#pragma once
#include "utils.hpp"

namespace saloc
{

  template<std::size_t min_size, std::size_t max_size, std::size_t max_elem=100>
    class FreelistAllocator
  {
  public:    
    Blk allocate(std::size_t size)
      {}
    
    void deallocate(Blk ptr)
      {}

    
  };

  

}
