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

// template <class F> unspecified not_fn(F&& f);

#include <functional>
#include <type_traits>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "type_id.h"
#include "callable_types.h"
#include "libcxx_tc_common.h"

///////////////////////////////////////////////////////////////////////////////
//                        BOOL TEST TYPES
///////////////////////////////////////////////////////////////////////////////

struct EvilBool {
  static int bang_called;

  EvilBool(EvilBool const&) = default;
  EvilBool(EvilBool&&) = default;

  friend EvilBool operator!(EvilBool const& other) {
    ++bang_called;
    return EvilBool{!other.value};
  }

private:
  friend struct MoveOnlyCallable<EvilBool>;
  friend struct CopyCallable<EvilBool>;
  friend struct NoExceptCallable<EvilBool>;

  constexpr explicit EvilBool(bool x) : value(x) {}
  EvilBool& operator=(EvilBool const& other) = default;

public:
  bool value;
};

int EvilBool::bang_called = 0;

struct ExplicitBool {
  ExplicitBool(ExplicitBool const&) = default;
  ExplicitBool(ExplicitBool&&) = default;

  constexpr explicit operator bool() const { return value; }

private:
  friend struct MoveOnlyCallable<ExplicitBool>;
  friend struct CopyCallable<ExplicitBool>;

  constexpr explicit ExplicitBool(bool x) : value(x) {}
  constexpr ExplicitBool& operator=(bool x) {
      value = x;
      return *this;
  }

  bool value;
};


struct NoExceptEvilBool {
  NoExceptEvilBool(NoExceptEvilBool const&) = default;
  NoExceptEvilBool(NoExceptEvilBool&&) = default;
  NoExceptEvilBool& operator=(NoExceptEvilBool const& other) = default;

  constexpr explicit NoExceptEvilBool(bool x) : value(x) {}

  friend NoExceptEvilBool operator!(NoExceptEvilBool const& other) noexcept {
    return NoExceptEvilBool{!other.value};
  }

  bool value;
};


TEST_CONSTEXPR_CXX20
bool constructor_tests()
{
    {
        using T = MoveOnlyCallable<bool>;
        T value(true);
        using RetT = decltype(std::not_fn(std::move(value)));
        static_assert(std::is_move_constructible<RetT>::value, "");
        static_assert(!std::is_copy_constructible<RetT>::value, "");
        static_assert(!std::is_move_assignable<RetT>::value, "");
        static_assert(!std::is_copy_assignable<RetT>::value, "");
        auto ret = std::not_fn(std::move(value));
        // test it was moved from
        TC_ASSERT_EXPR(value.value == false);
        // test that ret() negates the original value 'true'
        TC_ASSERT_EXPR(ret() == false);
        TC_ASSERT_EXPR(ret(0, 0.0, "blah") == false);
        // Move ret and test that it was moved from and that ret2 got the
        // original value.
        auto ret2 = std::move(ret);
        TC_ASSERT_EXPR(ret() == true);
        TC_ASSERT_EXPR(ret2() == false);
        TC_ASSERT_EXPR(ret2(42) == false);
    }
    {
        using T = CopyCallable<bool>;
        T value(false);
        using RetT = decltype(std::not_fn(value));
        static_assert(std::is_move_constructible<RetT>::value, "");
        static_assert(std::is_copy_constructible<RetT>::value, "");
        static_assert(!std::is_move_assignable<RetT>::value, "");
        static_assert(!std::is_copy_assignable<RetT>::value, "");
        auto ret = std::not_fn(value);
        // test that value is unchanged (copied not moved)
        TC_ASSERT_EXPR(value.value == false);
        // test 'ret' has the original value
        TC_ASSERT_EXPR(ret() == true);
        TC_ASSERT_EXPR(ret(42, 100) == true);
        // move from 'ret' and check that 'ret2' has the original value.
        auto ret2 = std::move(ret);
        TC_ASSERT_EXPR(ret() == false);
        TC_ASSERT_EXPR(ret2() == true);
        TC_ASSERT_EXPR(ret2("abc") == true);
        // initialize not_fn with rvalue
        auto ret3 = std::not_fn(std::move(value));
        TC_ASSERT_EXPR(ret(0) == false);
        TC_ASSERT_EXPR(ret3(0) == true);
    }
    {
        using T = CopyAssignableWrapper;
        T value(true);
        T value2(false);
        using RetT = decltype(std::not_fn(value));
        static_assert(std::is_move_constructible<RetT>::value, "");
        static_assert(std::is_copy_constructible<RetT>::value, "");
        auto ret = std::not_fn(value);
        TC_ASSERT_EXPR(ret() == false);
        auto ret2 = std::not_fn(value2);
        TC_ASSERT_EXPR(ret2() == true);
    }
    {
        using T = MoveAssignableWrapper;
        T value(true);
        T value2(false);
        using RetT = decltype(std::not_fn(std::move(value)));
        static_assert(std::is_move_constructible<RetT>::value, "");
        static_assert(!std::is_copy_constructible<RetT>::value, "");
        static_assert(!std::is_copy_assignable<RetT>::value, "");
        auto ret = std::not_fn(std::move(value));
        TC_ASSERT_EXPR(ret() == false);
        auto ret2 = std::not_fn(std::move(value2));
        TC_ASSERT_EXPR(ret2() == true);
    }
    return true;
}

void return_type_tests()
{
    using std::is_same;
    {
        using T = CopyCallable<bool>;
        auto ret = std::not_fn(T{false});
        static_assert(is_same<decltype(ret()), bool>::value, "");
        static_assert(is_same<decltype(ret("abc")), bool>::value, "");
        TC_ASSERT_EXPR(ret() == true);
    }
    {
        using T = CopyCallable<ExplicitBool>;
        auto ret = std::not_fn(T{true});
        static_assert(is_same<decltype(ret()), bool>::value, "");
        static_assert(is_same<decltype(ret(std::string("abc"))), bool>::value, "");
        TC_ASSERT_EXPR(ret() == false);
    }
    {
        using T = CopyCallable<EvilBool>;
        auto ret = std::not_fn(T{false});
        static_assert(is_same<decltype(ret()), EvilBool>::value, "");
        EvilBool::bang_called = 0;
        auto value_ret = ret();
        TC_ASSERT_EXPR(EvilBool::bang_called == 1);
        TC_ASSERT_EXPR(value_ret.value == true);
        ret();
        TC_ASSERT_EXPR(EvilBool::bang_called == 2);
    }
}

// Other tests only test using objects with call operators. Test various
// other callable types here.
TEST_CONSTEXPR_CXX20
bool other_callable_types_test()
{
    { // test with function pointer
        auto ret = std::not_fn(returns_true);
        TC_ASSERT_EXPR(ret() == false);
    }
    { // test with lambda
        auto returns_value = [](bool value) { return value; };
        auto ret = std::not_fn(returns_value);
        TC_ASSERT_EXPR(ret(true) == false);
        TC_ASSERT_EXPR(ret(false) == true);
    }
    { // test with pointer to member function
        MemFunCallable mt(true);
        const MemFunCallable mf(false);
        auto ret = std::not_fn(&MemFunCallable::return_value);
        TC_ASSERT_EXPR(ret(mt) == false);
        TC_ASSERT_EXPR(ret(mf) == true);
        TC_ASSERT_EXPR(ret(&mt) == false);
        TC_ASSERT_EXPR(ret(&mf) == true);
    }
    { // test with pointer to member function
        MemFunCallable mt(true);
        MemFunCallable mf(false);
        auto ret = std::not_fn(&MemFunCallable::return_value_nc);
        TC_ASSERT_EXPR(ret(mt) == false);
        TC_ASSERT_EXPR(ret(mf) == true);
        TC_ASSERT_EXPR(ret(&mt) == false);
        TC_ASSERT_EXPR(ret(&mf) == true);
    }
    { // test with pointer to member data
        MemFunCallable mt(true);
        const MemFunCallable mf(false);
        auto ret = std::not_fn(&MemFunCallable::value);
        TC_ASSERT_EXPR(ret(mt) == false);
        TC_ASSERT_EXPR(ret(mf) == true);
        TC_ASSERT_EXPR(ret(&mt) == false);
        TC_ASSERT_EXPR(ret(&mf) == true);
    }
    return true;
}

void throws_in_constructor_test()
{
#ifndef TEST_HAS_NO_EXCEPTIONS
    struct ThrowsOnCopy {
      ThrowsOnCopy(ThrowsOnCopy const&) {
#ifndef _LIBCPP_NO_EXCEPTIONS
        throw 42;
      }
#endif // _LIBCPP_NO_EXCEPTIONS
      ThrowsOnCopy() = default;
      bool operator()() const {
        TC_ASSERT_EXPR(false);
#ifdef TEST_COMPILER_MSVC
        __assume(0);
#else
        __builtin_unreachable();
#endif
      }
    };
    {
        ThrowsOnCopy cp;
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            (void)std::not_fn(cp);
            TC_ASSERT_EXPR(false);
        } catch (int const& value) {
            TC_ASSERT_EXPR(value == 42);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }
#endif
}

TEST_CONSTEXPR_CXX20
bool call_operator_sfinae_test() {
    { // wrong number of arguments
        using T = decltype(std::not_fn(returns_true));
        static_assert(std::is_invocable<T>::value, ""); // callable only with no args
        static_assert(!std::is_invocable<T, bool>::value, "");
    }
    { // violates const correctness (member function pointer)
        using T = decltype(std::not_fn(&MemFunCallable::return_value_nc));
        static_assert(std::is_invocable<T, MemFunCallable&>::value, "");
        static_assert(!std::is_invocable<T, const MemFunCallable&>::value, "");
    }
    { // violates const correctness (call object)
        using Obj = CopyCallable<bool>;
        using NCT = decltype(std::not_fn(Obj{true}));
        using CT = const NCT;
        static_assert(std::is_invocable<NCT>::value, "");
        static_assert(!std::is_invocable<CT>::value, "");
    }
    { // returns bad type with no operator!
        auto fn = [](auto x) { return x; };
        using T = decltype(std::not_fn(fn));
        static_assert(std::is_invocable<T, bool>::value, "");
        static_assert(!std::is_invocable<T, std::string>::value, "");
    }
    return true;
}

TEST_CONSTEXPR_CXX20
bool call_operator_forwarding_test()
{
    using Fn = ForwardingCallObject;
    Fn::State st;
    auto obj = std::not_fn(Fn{st});
    const auto& c_obj = obj;
    { // test zero args
        obj();
        TC_ASSERT_EXPR(st.check_call<>(CT_NonConst | CT_LValue));
        std::move(obj)();
        TC_ASSERT_EXPR(st.check_call<>(CT_NonConst | CT_RValue));
        c_obj();
        TC_ASSERT_EXPR(st.check_call<>(CT_Const | CT_LValue));
        std::move(c_obj)();
        TC_ASSERT_EXPR(st.check_call<>(CT_Const | CT_RValue));
    }
    { // test value categories
        int x = 42;
        const int cx = 42;
        obj(x);
        TC_ASSERT_EXPR(st.check_call<int&>(CT_NonConst | CT_LValue));
        obj(cx);
        TC_ASSERT_EXPR(st.check_call<const int&>(CT_NonConst | CT_LValue));
        obj(std::move(x));
        TC_ASSERT_EXPR(st.check_call<int&&>(CT_NonConst | CT_LValue));
        obj(std::move(cx));
        TC_ASSERT_EXPR(st.check_call<const int&&>(CT_NonConst | CT_LValue));
        obj(42);
        TC_ASSERT_EXPR(st.check_call<int&&>(CT_NonConst | CT_LValue));
    }
    { // test value categories - rvalue
        int x = 42;
        const int cx = 42;
        std::move(obj)(x);
        TC_ASSERT_EXPR(st.check_call<int&>(CT_NonConst | CT_RValue));
        std::move(obj)(cx);
        TC_ASSERT_EXPR(st.check_call<const int&>(CT_NonConst | CT_RValue));
        std::move(obj)(std::move(x));
        TC_ASSERT_EXPR(st.check_call<int&&>(CT_NonConst | CT_RValue));
        std::move(obj)(std::move(cx));
        TC_ASSERT_EXPR(st.check_call<const int&&>(CT_NonConst | CT_RValue));
        std::move(obj)(42);
        TC_ASSERT_EXPR(st.check_call<int&&>(CT_NonConst | CT_RValue));
    }
    { // test value categories - const call
        int x = 42;
        const int cx = 42;
        c_obj(x);
        TC_ASSERT_EXPR(st.check_call<int&>(CT_Const | CT_LValue));
        c_obj(cx);
        TC_ASSERT_EXPR(st.check_call<const int&>(CT_Const | CT_LValue));
        c_obj(std::move(x));
        TC_ASSERT_EXPR(st.check_call<int&&>(CT_Const | CT_LValue));
        c_obj(std::move(cx));
        TC_ASSERT_EXPR(st.check_call<const int&&>(CT_Const | CT_LValue));
        c_obj(42);
        TC_ASSERT_EXPR(st.check_call<int&&>(CT_Const | CT_LValue));
    }
    { // test value categories - const call rvalue
        int x = 42;
        const int cx = 42;
        std::move(c_obj)(x);
        TC_ASSERT_EXPR(st.check_call<int&>(CT_Const | CT_RValue));
        std::move(c_obj)(cx);
        TC_ASSERT_EXPR(st.check_call<const int&>(CT_Const | CT_RValue));
        std::move(c_obj)(std::move(x));
        TC_ASSERT_EXPR(st.check_call<int&&>(CT_Const | CT_RValue));
        std::move(c_obj)(std::move(cx));
        TC_ASSERT_EXPR(st.check_call<const int&&>(CT_Const | CT_RValue));
        std::move(c_obj)(42);
        TC_ASSERT_EXPR(st.check_call<int&&>(CT_Const | CT_RValue));
    }
    { // test multi arg
        using String = const char *;
        const double y = 3.14;
        String s = "abc";
        obj(42, std::move(y), s, String{"foo"});
        TC_ASSERT_EXPR((st.check_call<int&&, const double&&, String&, String&&>(CT_NonConst | CT_LValue)));
        std::move(obj)(42, std::move(y), s, String{"foo"});
        TC_ASSERT_EXPR((st.check_call<int&&, const double&&, String&, String&&>(CT_NonConst | CT_RValue)));
        c_obj(42, std::move(y), s, String{"foo"});
        TC_ASSERT_EXPR((st.check_call<int&&, const double&&, String&, String&&>(CT_Const  | CT_LValue)));
        std::move(c_obj)(42, std::move(y), s, String{"foo"});
        TC_ASSERT_EXPR((st.check_call<int&&, const double&&, String&, String&&>(CT_Const  | CT_RValue)));
    }
    return true;
}

TEST_CONSTEXPR_CXX20
bool call_operator_noexcept_test()
{
    {
        using T = ConstCallable<bool>;
        T value(true);
        auto ret = std::not_fn(value);
        static_assert(!noexcept(ret()), "call should not be noexcept");
        auto const& cret = ret;
        static_assert(!noexcept(cret()), "call should not be noexcept");
    }
    {
        using T = NoExceptCallable<bool>;
        T value(true);
        auto ret = std::not_fn(value);
        LIBCPP_STATIC_ASSERT(noexcept(!std::__invoke(value)), "");
#if TEST_STD_VER > 14
        static_assert(noexcept(!std::invoke(value)), "");
#endif
        static_assert(noexcept(ret()), "call should be noexcept");
        auto const& cret = ret;
        static_assert(noexcept(cret()), "call should be noexcept");
    }
    {
        using T = NoExceptCallable<NoExceptEvilBool>;
        T value(true);
        auto ret = std::not_fn(value);
        static_assert(noexcept(ret()), "call should not be noexcept");
        auto const& cret = ret;
        static_assert(noexcept(cret()), "call should not be noexcept");
    }
    {
        using T = NoExceptCallable<EvilBool>;
        T value(true);
        auto ret = std::not_fn(value);
        static_assert(!noexcept(ret()), "call should not be noexcept");
        auto const& cret = ret;
        static_assert(!noexcept(cret()), "call should not be noexcept");
    }
    return true;
}

TEST_CONSTEXPR_CXX20
bool test_lwg2767() {
    // See https://cplusplus.github.io/LWG/lwg-defects.html#2767
    struct Abstract { virtual void f() const = 0; };
    struct Derived : public Abstract { void f() const {} };
    struct F { constexpr bool operator()(Abstract&&) { return false; } };
    {
        Derived d;
        Abstract &a = d;
        bool b = std::not_fn(F{})(std::move(a));
        TC_ASSERT_EXPR(b);
    }
    return true;
}

int tc_utilities_function_objects_func_not_fn_not_fn(void) {
    constructor_tests();
    return_type_tests();
    other_callable_types_test();
    throws_in_constructor_test();
    call_operator_sfinae_test(); // somewhat of an extension
    call_operator_forwarding_test();
    call_operator_noexcept_test();
    test_lwg2767();

#if TEST_STD_VER >= 20
    static_assert(constructor_tests());
    static_assert(other_callable_types_test());
    static_assert(call_operator_sfinae_test()); // somewhat of an extension
    static_assert(call_operator_forwarding_test());
    static_assert(call_operator_noexcept_test());
    static_assert(test_lwg2767());
#endif

    return 0;
}
