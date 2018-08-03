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

  Allocator allocator(5);
  std::cout << sizeof(Name) << "\n";
  auto p1 = allocator.allocate(sizeof(int));
  auto p2 = allocator.allocate(sizeof(int));
  auto p3 = allocator.allocate(sizeof(int));
  auto p4 = allocator.allocate(sizeof(int));
  auto p5 = allocator.allocate(sizeof(int));
  auto p6 = allocator.allocate(sizeof(int));
  auto p7 = allocator.allocate(sizeof(int));
  auto p8 = allocator.allocate(2*sizeof(Name));

  std::cout << p1.ptr << "\n";
  std::cout << p2.ptr << "\n";
  std::cout << p3.ptr << "\n";
  std::cout << p4.ptr << "\n";
  std::cout << p5.ptr << "\n";
  std::cout << p6.ptr << "\n";
  std::cout << p7.ptr << "\n";
  std::cout << p8.ptr << "\n";

  allocator.deallocate(p1);
  allocator.deallocate(p2);
  allocator.deallocate(p3);
  allocator.deallocate(p4);
  allocator.deallocate(p5);
  allocator.deallocate(p6);
  allocator.deallocate(p7);
  allocator.deallocate(p8);

  std::cout << "NEW--------------------New" << "\n";

   p1 = allocator.allocate(sizeof(double));
   p2 = allocator.allocate(sizeof(double));
   p3 = allocator.allocate(sizeof(double));
   p4 = allocator.allocate(sizeof(double));
   p5 = allocator.allocate(sizeof(double));
   p6 = allocator.allocate(sizeof(double));
   p7 = allocator.allocate(sizeof(double));
   p8 = allocator.allocate(sizeof(Name));

   std::cout << p1.ptr << "\n";
   std::cout << p2.ptr << "\n";
   std::cout << p3.ptr << "\n";
   std::cout << p4.ptr << "\n";
   std::cout << p5.ptr << "\n";
   std::cout << p6.ptr << "\n";
   std::cout << p7.ptr << "\n";
   std::cout << p8.ptr << "\n";

  

  

  
   return 0;

}
