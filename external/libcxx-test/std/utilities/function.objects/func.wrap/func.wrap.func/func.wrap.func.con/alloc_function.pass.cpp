//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <functional>
// REQUIRES: c++11 || c++14

// class function<R(ArgTypes...)>

// template<class A> function(allocator_arg_t, const A&, const function&);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"
#include "count_new.h"
#include "../function_types.h"
#include "libcxx_tc_common.h"

template <class T>
struct non_default_test_allocator : test_allocator<T> {
  non_default_test_allocator() = delete;
  using test_allocator<T>::test_allocator;
};

class DummyClass {};

template <class FuncType, class AllocType>
void test_FunctionObject(AllocType& alloc)
{
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    // Construct function from FunctionObject.
    std::function<FuncType> f = FunctionObject();
    TC_ASSERT_EXPR(FunctionObject::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(f.template target<FunctionObject>());
    RTTI_ASSERT(f.template target<FuncType>() == 0);
    RTTI_ASSERT(f.template target<FuncType*>() == 0);
    // Copy function with allocator
    std::function<FuncType> f2(std::allocator_arg, alloc, f);
    TC_ASSERT_EXPR(FunctionObject::count == 2);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(2));
    RTTI_ASSERT(f2.template target<FunctionObject>());
    RTTI_ASSERT(f2.template target<FuncType>() == 0);
    RTTI_ASSERT(f2.template target<FuncType*>() == 0);
    }
    TC_ASSERT_EXPR(FunctionObject::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
}

template <class FuncType, class AllocType>
void test_FreeFunction(AllocType& alloc)
{
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    // Construct function from function pointer.
    FuncType* target = &FreeFunction;
    std::function<FuncType> f = target;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.template target<FuncType*>());
    RTTI_ASSERT(*f.template target<FuncType*>() == target);
    RTTI_ASSERT(f.template target<FuncType>() == 0);
    // Copy function with allocator
    std::function<FuncType> f2(std::allocator_arg, alloc, f);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f2.template target<FuncType*>());
    RTTI_ASSERT(*f2.template target<FuncType*>() == target);
    RTTI_ASSERT(f2.template target<FuncType>() == 0);
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
}

template <class TargetType, class FuncType, class AllocType>
void test_MemFunClass(AllocType& alloc)
{
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    // Construct function from function pointer.
    TargetType target = &MemFunClass::foo;
    std::function<FuncType> f = target;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.template target<TargetType>());
    RTTI_ASSERT(*f.template target<TargetType>() == target);
    RTTI_ASSERT(f.template target<FuncType*>() == 0);
    // Copy function with allocator
    std::function<FuncType> f2(std::allocator_arg, alloc, f);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f2.template target<TargetType>());
    RTTI_ASSERT(*f2.template target<TargetType>() == target);
    RTTI_ASSERT(f2.template target<FuncType*>() == 0);
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
}

template <class Alloc>
void test_for_alloc(Alloc& alloc)
{
    // Large FunctionObject -- Allocation should occur
    test_FunctionObject<int()>(alloc);
    test_FunctionObject<int(int)>(alloc);
    test_FunctionObject<int(int, int)>(alloc);
    test_FunctionObject<int(int, int, int)>(alloc);
    // Free function -- No allocation should occur
    test_FreeFunction<int()>(alloc);
    test_FreeFunction<int(int)>(alloc);
    test_FreeFunction<int(int, int)>(alloc);
    test_FreeFunction<int(int, int, int)>(alloc);
    // Member function -- No allocation should occur.
    test_MemFunClass<int(MemFunClass::*)() const, int(MemFunClass&)>(alloc);
    test_MemFunClass<int(MemFunClass::*)(int) const, int(MemFunClass&, int)>(alloc);
    test_MemFunClass<int(MemFunClass::*)(int, int) const, int(MemFunClass&, int, int)>(alloc);
}

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_alloc_function(void) {
  globalMemCounter.reset();
  {
    bare_allocator<DummyClass> alloc;
    test_for_alloc(alloc);
  }
  {
    non_default_test_allocator<DummyClass> alloc(42);
    test_for_alloc(alloc);
  }

  return 0;
}
