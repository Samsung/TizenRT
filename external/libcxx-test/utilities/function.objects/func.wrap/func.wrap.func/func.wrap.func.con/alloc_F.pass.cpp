/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
// <functional>
// REQUIRES: c++11 || c++14

// class function<R(ArgTypes...)>

// template<class F, class A> function(allocator_arg_t, const A&, F);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"
#include "count_new.h"
#include "../function_types.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER >= 11
struct RValueCallable {
    template <class ...Args>
    void operator()(Args&&...) && {}
};
struct LValueCallable {
    template <class ...Args>
    void operator()(Args&&...) & {}
};
#endif

template <class T>
struct non_default_test_allocator : test_allocator<T> {
  non_default_test_allocator() = delete;
  using test_allocator<T>::test_allocator;
};

test_allocator_statistics alloc_stats;

class DummyClass {};

template <class FuncType, class AllocType>
void test_FunctionObject(AllocType& alloc)
{
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    FunctionObject target;
    TC_ASSERT_EXPR(FunctionObject::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    std::function<FuncType> f2(std::allocator_arg, alloc, target);
    TC_ASSERT_EXPR(FunctionObject::count == 2);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    TC_ASSERT_EXPR(f2.template target<FunctionObject>());
    TC_ASSERT_EXPR(f2.template target<FuncType>() == 0);
    TC_ASSERT_EXPR(f2.template target<FuncType*>() == 0);
    }
    TC_ASSERT_EXPR(FunctionObject::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
}


template <class FuncType, class AllocType>
void test_FreeFunction(AllocType& alloc)
{
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    FuncType* target = &FreeFunction;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    std::function<FuncType> f2(std::allocator_arg, alloc, target);
    // The allocator may not fit in the small object buffer, if we allocated
    // check it was done via the allocator.
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(alloc_stats.alloc_count));
    TC_ASSERT_EXPR(f2.template target<FuncType*>());
    TC_ASSERT_EXPR(*f2.template target<FuncType*>() == target);
    TC_ASSERT_EXPR(f2.template target<FuncType>() == 0);
    TC_ASSERT_EXPR(f2.template target<DummyClass>() == 0);
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
}

template <class TargetType, class FuncType, class AllocType>
void test_MemFunClass(AllocType& alloc)
{
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    TargetType target = &MemFunClass::foo;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    std::function<FuncType> f2(std::allocator_arg, alloc, target);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(alloc_stats.alloc_count));
    TC_ASSERT_EXPR(f2.template target<TargetType>());
    TC_ASSERT_EXPR(*f2.template target<TargetType>() == target);
    TC_ASSERT_EXPR(f2.template target<FuncType*>() == 0);
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
}

template <class Alloc>
void test_for_alloc(Alloc& alloc) {
    test_FunctionObject<int()>(alloc);
    test_FunctionObject<int(int)>(alloc);
    test_FunctionObject<int(int, int)>(alloc);
    test_FunctionObject<int(int, int, int)>(alloc);

    test_FreeFunction<int()>(alloc);
    test_FreeFunction<int(int)>(alloc);
    test_FreeFunction<int(int, int)>(alloc);
    test_FreeFunction<int(int, int, int)>(alloc);

    test_MemFunClass<int(MemFunClass::*)() const, int(MemFunClass&)>(alloc);
    test_MemFunClass<int(MemFunClass::*)(int) const, int(MemFunClass&, int)>(alloc);
    test_MemFunClass<int(MemFunClass::*)(int, int) const, int(MemFunClass&, int, int)>(alloc);
}

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_alloc_F(void) {
  globalMemCounter.reset();
  {
    bare_allocator<DummyClass> bare_alloc;
    test_for_alloc(bare_alloc);
  }
    {
        non_default_test_allocator<DummyClass> non_default_alloc(42, &alloc_stats);
        test_for_alloc(non_default_alloc);
    }
#if TEST_STD_VER >= 11
    {
        using Fn = std::function<void(int, int, int)>;
        static_assert(std::is_constructible<Fn, std::allocator_arg_t, std::allocator<int>, LValueCallable&>::value, "");
        static_assert(std::is_constructible<Fn, std::allocator_arg_t, std::allocator<int>, LValueCallable>::value, "");
        static_assert(!std::is_constructible<Fn, std::allocator_arg_t, std::allocator<int>, RValueCallable&>::value, "");
        static_assert(!std::is_constructible<Fn, std::allocator_arg_t, std::allocator<int>, RValueCallable>::value, "");
    }
#endif


  return 0;
}
