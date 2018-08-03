#pragma once
#include "utils.hpp"
#include <utility>

namespace saloc
{

  template<class A, std::size_t min_size, std::size_t max_size>
  class FreelistAllocator
  {

  private:
    A parent;
    struct Node {Node* next;};
    Node* root;
    std::size_t batch_size;
  public:

    FreelistAllocator(std::size_t batch_size):
      root(nullptr),batch_size(std::move(batch_size))
      {}

    
    Blk allocate(std::size_t size)
      {
        size = size < sizeof(void*) ? sizeof(void*) : size;
        if (min_size <= size && size < max_size && this->root != nullptr)
        {
          Blk blk = {this->root, size};
          this->root = (Node*)(this->root->next);
          return blk;
        }
        Blk blk = parent.allocate(size * this->batch_size);
        char* start = (char *)(blk.ptr);
        for (std::size_t i = 1; i < this->batch_size - 1; ++i)
        {
          Node* node = (Node*)(start + size*i);
          node->next = (Node*)(start + size*i + size);
        }
        this->root = (Node*)(start + size);        
        return {start, size};

      }
    
    void deallocate(Blk blk)
      {
        if (!(min_size <= blk.size && blk.size < max_size)) parent.deallocate(blk);
        else
        {
          auto p = static_cast<Node*>(blk.ptr);
          p->next = this->root;
          this->root = p;
        }

      }

    bool owns(Blk blk)
      {
        return (min_size <= blk.size && blk.size < max_size) || parent.owns(blk);
      }

    
  };

  

}
