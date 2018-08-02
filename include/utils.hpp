#pragma once
#include <iostream>
#include <cstddef>
#include <assert.h>
#include <cstring>
#include <stdlib.h>

namespace saloc
{
  constexpr std::size_t ONE_KB = 1024;
  constexpr std::size_t WORD_SIZE = sizeof(size_t);
  
  std::size_t roundToAllign(std::size_t n, std::size_t alignment = saloc::WORD_SIZE)
  {
    return ((n  / alignment) + 1) * alignment;
  }  
  struct Blk
  {
    void* ptr;
    std::size_t size;
  };

  class Allocator
  {
    virtual Blk allocate(std::size_t n) = 0;
    virtual void deallocate(std::size_t n) = 0;
  };
  
}
