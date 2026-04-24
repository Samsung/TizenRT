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

// XFAIL: availability-bad_any_cast-missing && !no-exceptions

// <any>

// template <class ValueType>
// ValueType const any_cast(any const&);
//
// template <class ValueType>
// ValueType any_cast(any &);
//
// template <class ValueType>
// ValueType any_cast(any &&);

#include <any>
#include <type_traits>
#include <cassert>

#include "any_helpers.h"
#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test that the operators are NOT marked noexcept.
void test_cast_is_not_noexcept() {
    std::any a;
    static_assert(!noexcept(std::any_cast<int>(static_cast<std::any&>(a))), "");
    static_assert(!noexcept(std::any_cast<int>(static_cast<std::any const&>(a))), "");
    static_assert(!noexcept(std::any_cast<int>(static_cast<std::any &&>(a))), "");
}

// Test that the return type of any_cast is correct.
void test_cast_return_type() {
    std::any a;
    static_assert(std::is_same<decltype(std::any_cast<int>(a)), int>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int const>(a)), int>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int&>(a)), int&>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int const&>(a)), int const&>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int&&>(a)), int&&>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int const&&>(a)), int const&&>::value, "");

    static_assert(std::is_same<decltype(std::any_cast<int>(std::move(a))), int>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int const>(std::move(a))), int>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int&>(std::move(a))), int&>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int const&>(std::move(a))), int const&>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int&&>(std::move(a))), int&&>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int const&&>(std::move(a))), int const&&>::value, "");

    const std::any& ca = a;
    static_assert(std::is_same<decltype(std::any_cast<int>(ca)), int>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int const>(ca)), int>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int const&>(ca)), int const&>::value, "");
    static_assert(std::is_same<decltype(std::any_cast<int const&&>(ca)), int const&&>::value, "");
}

template <class Type, class ConstT = Type>
void checkThrows(std::any& a)
{
#if !defined(TEST_HAS_NO_EXCEPTIONS)
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        TEST_IGNORE_NODISCARD std::any_cast<Type>(a);
        TC_ASSERT_EXPR(false);
    } catch (const std::bad_any_cast&) {
        // do nothing
    } catch (...) {
        TC_ASSERT_EXPR(false);
    }
#endif // _LIBCPP_NO_EXCEPTIONS

#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        TEST_IGNORE_NODISCARD std::any_cast<ConstT>(static_cast<const std::any&>(a));
        TC_ASSERT_EXPR(false);
    } catch (const std::bad_any_cast&) {
        // do nothing
    } catch (...) {
        TC_ASSERT_EXPR(false);
    }
#endif // _LIBCPP_NO_EXCEPTIONS

#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        using RefType = typename std::conditional<
            std::is_lvalue_reference<Type>::value,
            typename std::remove_reference<Type>::type&&,
            Type
        >::type;
        TEST_IGNORE_NODISCARD std::any_cast<RefType>(static_cast<std::any&&>(a));
        TC_ASSERT_EXPR(false);
    } catch (const std::bad_any_cast&) {
            // do nothing
    } catch (...) {
        TC_ASSERT_EXPR(false);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
#else
    (TEST_IGNORE_NODISCARD a);
#endif
}

void test_cast_empty() {
    // None of these operations should allocate.
    DisableAllocationGuard g; (TEST_IGNORE_NODISCARD g);
    std::any a;
    checkThrows<int>(a);
}

template <class Type>
void test_cast_to_reference() {
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a = Type(42);
        const std::any& ca = a;
        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 1);

        // Try a cast to a bad type.
        // NOTE: Type cannot be an int.
        checkThrows<int>(a);
        checkThrows<int&, int const&>(a);
        checkThrows<Type*, Type const*>(a);
        checkThrows<Type const*>(a);

        // Check getting a type by reference from a non-const lvalue any.
        {
            Type& v = std::any_cast<Type&>(a);
            TC_ASSERT_EXPR(v.value == 42);

            Type const &cv = std::any_cast<Type const&>(a);
            TC_ASSERT_EXPR(&cv == &v);
        }
        // Check getting a type by reference from a const lvalue any.
        {
            Type const& v = std::any_cast<Type const&>(ca);
            TC_ASSERT_EXPR(v.value == 42);

            Type const &cv = std::any_cast<Type const&>(ca);
            TC_ASSERT_EXPR(&cv == &v);
        }
        // Check getting a type by reference from a const rvalue any.
        {
            Type const& v = std::any_cast<Type const&>(std::move(ca));
            TC_ASSERT_EXPR(v.value == 42);

            Type const &cv = std::any_cast<Type const&>(std::move(ca));
            TC_ASSERT_EXPR(&cv == &v);
        }
        // Check getting a type by reference from a const rvalue any.
        {
            Type&& v = std::any_cast<Type&&>(std::move(a));
            TC_ASSERT_EXPR(v.value == 42);
            TC_ASSERT_EXPR(std::any_cast<Type&>(a).value == 42);

            Type&& cv = std::any_cast<Type&&>(std::move(a));
            TC_ASSERT_EXPR(&cv == &v);
            TC_ASSERT_EXPR(std::any_cast<Type&>(a).value == 42);
        }
        // Check getting a type by reference from a const rvalue any.
        {
            Type const&& v = std::any_cast<Type const&&>(std::move(a));
            TC_ASSERT_EXPR(v.value == 42);
            TC_ASSERT_EXPR(std::any_cast<Type&>(a).value == 42);

            Type const&& cv = std::any_cast<Type const&&>(std::move(a));
            TC_ASSERT_EXPR(&cv == &v);
            TC_ASSERT_EXPR(std::any_cast<Type&>(a).value == 42);
        }
        // Check that the original object hasn't been changed.
        assertContains<Type>(a, 42);

        // Check that no objects have been created/copied/moved.
        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 1);
    }
    TC_ASSERT_EXPR(Type::count == 0);
}

template <class Type>
void test_cast_to_value() {
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a = Type(42);
        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 1);

        // Try a cast to a bad type.
        // NOTE: Type cannot be an int.
        checkThrows<int>(a);
        checkThrows<int&, int const&>(a);
        checkThrows<Type*, Type const*>(a);
        checkThrows<Type const*>(a);

        Type::reset(); // NOTE: reset does not modify Type::count
        // Check getting Type by value from a non-const lvalue any.
        // This should cause the non-const copy constructor to be called.
        {
            Type t = std::any_cast<Type>(a);

            TC_ASSERT_EXPR(Type::count == 2);
            TC_ASSERT_EXPR(Type::copied == 1);
            TC_ASSERT_EXPR(Type::const_copied == 0);
            TC_ASSERT_EXPR(Type::non_const_copied == 1);
            TC_ASSERT_EXPR(Type::moved == 0);
            TC_ASSERT_EXPR(t.value == 42);
        }
        TC_ASSERT_EXPR(Type::count == 1);
        Type::reset();
        // Check getting const Type by value from a non-const lvalue any.
        // This should cause the const copy constructor to be called.
        {
            Type t = std::any_cast<Type const>(a);

            TC_ASSERT_EXPR(Type::count == 2);
            TC_ASSERT_EXPR(Type::copied == 1);
            TC_ASSERT_EXPR(Type::const_copied == 0);
            TC_ASSERT_EXPR(Type::non_const_copied == 1);
            TC_ASSERT_EXPR(Type::moved == 0);
            TC_ASSERT_EXPR(t.value == 42);
        }
        TC_ASSERT_EXPR(Type::count == 1);
        Type::reset();
        // Check getting Type by value from a non-const lvalue any.
        // This should cause the const copy constructor to be called.
        {
            Type t = std::any_cast<Type>(static_cast<const std::any&>(a));

            TC_ASSERT_EXPR(Type::count == 2);
            TC_ASSERT_EXPR(Type::copied == 1);
            TC_ASSERT_EXPR(Type::const_copied == 1);
            TC_ASSERT_EXPR(Type::non_const_copied == 0);
            TC_ASSERT_EXPR(Type::moved == 0);
            TC_ASSERT_EXPR(t.value == 42);
        }
        TC_ASSERT_EXPR(Type::count == 1);
        Type::reset();
        // Check getting Type by value from a non-const rvalue any.
        // This should cause the non-const copy constructor to be called.
        {
            Type t = std::any_cast<Type>(static_cast<std::any&&>(a));

            TC_ASSERT_EXPR(Type::count == 2);
            TC_ASSERT_EXPR(Type::moved == 1);
            TC_ASSERT_EXPR(Type::copied == 0);
            TC_ASSERT_EXPR(Type::const_copied == 0);
            TC_ASSERT_EXPR(Type::non_const_copied == 0);
            TC_ASSERT_EXPR(t.value == 42);
            TC_ASSERT_EXPR(std::any_cast<Type&>(a).value == 0);
            std::any_cast<Type&>(a).value = 42; // reset the value
        }
        TC_ASSERT_EXPR(Type::count == 1);
        Type::reset();
        // Check getting const Type by value from a non-const rvalue any.
        // This should cause the const copy constructor to be called.
        {
            Type t = std::any_cast<Type const>(static_cast<std::any&&>(a));

            TC_ASSERT_EXPR(Type::count == 2);
            TC_ASSERT_EXPR(Type::copied == 0);
            TC_ASSERT_EXPR(Type::const_copied == 0);
            TC_ASSERT_EXPR(Type::non_const_copied == 0);
            TC_ASSERT_EXPR(Type::moved == 1);
            TC_ASSERT_EXPR(t.value == 42);
            TC_ASSERT_EXPR(std::any_cast<Type&>(a).value == 0);
            std::any_cast<Type&>(a).value = 42; // reset the value
        }
        TC_ASSERT_EXPR(Type::count == 1);
        Type::reset();
        // Check getting Type by value from a const rvalue any.
        // This should cause the const copy constructor to be called.
        {
            Type t = std::any_cast<Type>(static_cast<const std::any&&>(a));

            TC_ASSERT_EXPR(Type::count == 2);
            TC_ASSERT_EXPR(Type::copied == 1);
            TC_ASSERT_EXPR(Type::const_copied == 1);
            TC_ASSERT_EXPR(Type::non_const_copied == 0);
            TC_ASSERT_EXPR(Type::moved == 0);
            TC_ASSERT_EXPR(t.value == 42);
            TC_ASSERT_EXPR(std::any_cast<Type&>(a).value == 42);
        }
        // Ensure we still only have 1 Type object alive.
        TC_ASSERT_EXPR(Type::count == 1);

        // Check that the original object hasn't been changed.
        assertContains<Type>(a, 42);
    }
    TC_ASSERT_EXPR(Type::count == 0);
}

int tc_utilities_any_any_nonmembers_any_cast_any_cast_reference(void) {
    test_cast_is_not_noexcept();
    test_cast_return_type();
    test_cast_empty();
    test_cast_to_reference<small>();
    test_cast_to_reference<large>();
    test_cast_to_value<small>();
    test_cast_to_value<large>();

  return 0;
}
