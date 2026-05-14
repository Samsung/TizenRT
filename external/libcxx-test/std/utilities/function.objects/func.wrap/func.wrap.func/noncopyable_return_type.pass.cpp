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
// UNSUPPORTED: c++03, c++11, c++14

// FIXME: Building this in MSVC mode fails when instantiating two cases of
// std::function that only differ in constness of the return type, with this
// error:
// include/c++/v1/__functional/function.h:254:31: error: definition with same mangled name '??0?$__base@$$A6AXXZ@__function@__1@std@@QEAA@XZ' as another definition
//     _LIBCPP_INLINE_VISIBILITY __base() {}
// include/c++/v1/__functional/function.h:254:31: note: previous definition is here
// XFAIL: msvc

// <functional>

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Prevent warning on the `const NonCopyable()` function type.
TEST_CLANG_DIAGNOSTIC_IGNORED("-Wignored-qualifiers")
TEST_GCC_DIAGNOSTIC_IGNORED("-Wignored-qualifiers")

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(NonCopyable&&) = delete;
    friend bool operator==(NonCopyable, NonCopyable) { return true; }
};

struct LargeLambda {
    int a[100];
    NonCopyable operator()() const { return NonCopyable(); }
    NonCopyable operator()(int) const { return NonCopyable(); }
    NonCopyable f() const { return NonCopyable(); }
};

void test()
{
    std::function<NonCopyable()> f1a = []() { return NonCopyable(); };
    std::function<NonCopyable()> f2a = +[]() { return NonCopyable(); };
    std::function<NonCopyable()> f3a = LargeLambda();
    std::function<NonCopyable()> f4a = std::ref(f1a);
    std::function<NonCopyable(int)> f1b = [](int) { return NonCopyable(); };
    std::function<NonCopyable(int)> f2b = +[](int) { return NonCopyable(); };
    std::function<NonCopyable(int)> f3b = LargeLambda();
    std::function<NonCopyable(int)> f4b = std::ref(f1b);

    TC_ASSERT_EXPR(f1a() == f2a());
    TC_ASSERT_EXPR(f3a() == f4a());
    TC_ASSERT_EXPR(f1b(1) == f2b(1));
    TC_ASSERT_EXPR(f3b(1) == f4b(1));
}

void const_test()
{
    std::function<const NonCopyable()> f1a = []() { return NonCopyable(); };
    std::function<const NonCopyable()> f2a = +[]() { return NonCopyable(); };
    std::function<const NonCopyable()> f3a = LargeLambda();
    std::function<const NonCopyable()> f4a = std::ref(f1a);
    std::function<const NonCopyable(int)> f1b = [](int) { return NonCopyable(); };
    std::function<const NonCopyable(int)> f2b = +[](int) { return NonCopyable(); };
    std::function<const NonCopyable(int)> f3b = LargeLambda();
    std::function<const NonCopyable(int)> f4b = std::ref(f1b);

    TC_ASSERT_EXPR(f1a() == f2a());
    TC_ASSERT_EXPR(f3a() == f4a());
    TC_ASSERT_EXPR(f1b(1) == f2b(1));
    TC_ASSERT_EXPR(f3b(1) == f4b(1));
}

void void_test()
{
    std::function<void()> f1a = []() { return NonCopyable(); };
    std::function<void()> f2a = +[]() { return NonCopyable(); };
    std::function<void()> f3a = LargeLambda();
    std::function<void()> f4a = std::ref(f1a);
    std::function<void(int)> f1b = [](int) { return NonCopyable(); };
    std::function<void(int)> f2b = +[](int) { return NonCopyable(); };
    std::function<void(int)> f3b = LargeLambda();
    std::function<void(int)> f4b = std::ref(f1b);
}

void const_void_test()
{
    std::function<const void()> f1a = []() { return NonCopyable(); };
    std::function<const void()> f2a = +[]() { return NonCopyable(); };
    std::function<const void()> f3a = LargeLambda();
    std::function<const void()> f4a = std::ref(f1a);
    std::function<const void(int)> f1b = [](int) { return NonCopyable(); };
    std::function<const void(int)> f2b = +[](int) { return NonCopyable(); };
    std::function<const void(int)> f3b = LargeLambda();
    std::function<const void(int)> f4b = std::ref(f1b);
}

void member_pointer_test()
{
    std::function<NonCopyable(LargeLambda*)> f1a = &LargeLambda::f;
    std::function<NonCopyable(LargeLambda&)> f2a = &LargeLambda::f;
    LargeLambda ll;
    TC_ASSERT_EXPR(f1a(&ll) == f2a(ll));

    static_assert(std::is_convertible_v<NonCopyable (LargeLambda::*)(), std::function<NonCopyable(LargeLambda*)>>);
    static_assert(std::is_convertible_v<NonCopyable (LargeLambda::*)(), std::function<NonCopyable(LargeLambda&)>>);
    static_assert(std::is_convertible_v<NonCopyable (LargeLambda::*)() const, std::function<NonCopyable(LargeLambda*)>>);
    static_assert(std::is_convertible_v<NonCopyable (LargeLambda::*)() const, std::function<NonCopyable(LargeLambda&)>>);
    static_assert(std::is_convertible_v<NonCopyable (LargeLambda::*)() const, std::function<NonCopyable(const LargeLambda*)>>);
    static_assert(std::is_convertible_v<NonCopyable (LargeLambda::*)() const, std::function<NonCopyable(const LargeLambda&)>>);

    // Verify we have SFINAE against invoking a pointer-to-data-member in a way that would have to copy the NonCopyable.
    static_assert(!std::is_convertible_v<NonCopyable LargeLambda::*, std::function<NonCopyable(LargeLambda*)>>);
    static_assert(!std::is_convertible_v<NonCopyable LargeLambda::*, std::function<NonCopyable(LargeLambda&)>>);
    static_assert(!std::is_convertible_v<NonCopyable LargeLambda::*, std::function<NonCopyable&(const LargeLambda&)>>);
    static_assert(std::is_convertible_v<NonCopyable LargeLambda::*, std::function<NonCopyable&(LargeLambda*)>>);
    static_assert(std::is_convertible_v<NonCopyable LargeLambda::*, std::function<NonCopyable&(LargeLambda&)>>);
    static_assert(std::is_convertible_v<NonCopyable LargeLambda::*, std::function<const NonCopyable&(const LargeLambda&)>>);
}

void ctad_test()
{
    std::function f1a = []() { return NonCopyable(); };
    std::function f2a = +[]() { return NonCopyable(); };
    std::function f1b = [](int) { return NonCopyable(); };
    std::function f2b = +[](int) { return NonCopyable(); };
    static_assert(std::is_same_v<decltype(f1a), std::function<NonCopyable()>>);
    static_assert(std::is_same_v<decltype(f2a), std::function<NonCopyable()>>);
    static_assert(std::is_same_v<decltype(f1b), std::function<NonCopyable(int)>>);
    static_assert(std::is_same_v<decltype(f2b), std::function<NonCopyable(int)>>);
}

int tc_utilities_function_objects_func_wrap_func_wrap_func_noncopyable_return_type(void) {
    test();
    const_test();
    void_test();
    const_void_test();
    member_pointer_test();
    ctad_test();
    return 0;
}
