#pragma once
#include <functional>
#include "utils.hpp"


namespace scope_guarding
{
  template<typename Fun>
  struct ScopeGuard
  {
    Fun fun;
    ScopeGuard(Fun fun):fun(fun){};
    ~ScopeGuard(){fun();};
  };
  template<typename F>
  ScopeGuard<F> make_scope_guard(F f)
  {
    return ScopeGuard<F>(f);
  }
}
#define CONCATENATE_STR_IMPL(s1,s2) s1##s2
#define CONCATENATE_STR(s1, s2) CONCATENATE_STR_IMPL(s1, s2)
#define SCOPE_EXIT(code) auto CONCATENATE_STR(scope_exit_, __LINE__) = scope_guarding::make_scope_guard([&](){code;})
