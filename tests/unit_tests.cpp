#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include "utils.hpp"
#include "fallback_allocator.hpp"
#include "freelist_allocator.hpp"
#include "mallocator.hpp"
#include "stack_allocator.hpp"
#include "scope_guards.hpp"



TEST(Allocators, Mallocator)
{
  double res = 0.5;

  ASSERT_NEAR(res, 0.5, 1.0e-11);
}



TEST(Allocators, FallbackAllocator)
{
  double res = 0.5;

  ASSERT_NEAR(res, 0.5, 1.0e-11);
}
