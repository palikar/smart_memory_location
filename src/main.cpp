#include <iostream>
#include <cstddef>
#include <assert.h>
#include <cstring>
#include <stdlib.h>
#include <string>

#include "fallback_allocator.hpp"
#include "mallocator.hpp"
#include "stack_allocator.hpp"
#include "utils.hpp"
#include "scope_guards.hpp"


class Name
{
private:
  int one_int;
  std::string name;
public:
  Name(std::string name):name(name)
    {
      std::cout << "New!" << "\n";
    };
  ~Name()
    {
      std::cout << "Bye!" << "\n";

    }
};




using Allocator = saloc::FallbackAllocator<
  saloc::StackAllocator<saloc::ONE_KB*10>,
  saloc::Mallocator<> >;





int main()
{
  Allocator aloc;
  std::cout << sizeof(Name) << "\n";
  saloc::Blk blk = aloc.allocate(sizeof(Name));
  {
    Name *p = new (blk.ptr) Name("sad");
    SCOPE_EXIT(p->~Name();aloc.deallocate(blk)); 
  }

  

  
  return 0;

}
