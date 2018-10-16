#include <iostream>
#include <cstddef>
#include <assert.h>
#include <cstring>
#include <stdlib.h>
#include <string>

#include "fallback_allocator.hpp"
#include "mallocator.hpp"
#include "stack_allocator.hpp"
#include "freelist_allocator.hpp"
#include "utils.hpp"
#include "scope_guards.hpp"



#include <easylogging++.h>
#include <easylogging++.cc>
INITIALIZE_EASYLOGGINGPP


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

using Allocator = saloc::FreelistAllocator<saloc::Mallocator<>, 8, 10>;
int main()
{
//   Allocator aloc;
//   std::cout << sizeof(Name) << "\n";
//   saloc::Blk blk = aloc.allocate(sizeof(Name));
//   {
//     Name *p = new (blk.ptr) Name("sad");
//     SCOPE_EXIT(p->~Name();aloc.deallocate(blk)); 
//   }

  LOG(INFO) << "Where you from, my nigga?";
  


  

  
  return 0;

}
