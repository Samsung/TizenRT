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
// template<class T>
// class enable_shared_from_this
// {
// protected:
//     enable_shared_from_this();
//     enable_shared_from_this(enable_shared_from_this const&);
//     enable_shared_from_this& operator=(enable_shared_from_this const&);
//     ~enable_shared_from_this();
// public:
//     shared_ptr<T> shared_from_this();
//     shared_ptr<T const> shared_from_this() const;
//     weak_ptr<T> weak_from_this() noexcept;                         // C++17
//     weak_ptr<T const> weak_from_this() const noexecpt;             // C++17
// };

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"
#include "libcxx_tc_common.h"

struct T
    : public std::enable_shared_from_this<T>
{
};

struct Y : T {};

struct Z : Y {};

void nullDeleter(void*) {}

struct Foo : virtual public std::enable_shared_from_this<Foo>
{
    virtual ~Foo() {}
};

struct Bar : public Foo {
    Bar(int) {}
};


struct PrivateBase : private std::enable_shared_from_this<PrivateBase> {
};


int tc_utilities_memory_util_smartptr_util_smartptr_enab_enable_shared_from_this(void) {
    globalMemCounter.reset();
    {  // https://llvm.org/PR18843
    std::shared_ptr<T const> t1(new T);
    std::shared_ptr<T const> t2(std::make_shared<T>());
    }
    { // https://llvm.org/PR27115
    int x = 42;
    std::shared_ptr<Bar> t1(new Bar(42));
    TC_ASSERT_EXPR(t1->shared_from_this() == t1);
    std::shared_ptr<Bar> t2(std::make_shared<Bar>(x));
    TC_ASSERT_EXPR(t2->shared_from_this() == t2);
    }
    {
    std::shared_ptr<Y> p(new Z);
    std::shared_ptr<T> q = p->shared_from_this();
    TC_ASSERT_EXPR(p == q);
    TC_ASSERT_EXPR(!p.owner_before(q) && !q.owner_before(p)); // p and q share ownership
    }
    {
    std::shared_ptr<Y> p = std::make_shared<Z>();
    std::shared_ptr<T> q = p->shared_from_this();
    TC_ASSERT_EXPR(p == q);
    TC_ASSERT_EXPR(!p.owner_before(q) && !q.owner_before(p)); // p and q share ownership
    }
    {
      typedef std::shared_ptr<PrivateBase> APtr;
      APtr a1 = std::make_shared<PrivateBase>();
      TC_ASSERT_EXPR(a1.use_count() == 1);
    }
    // Test LWG issue 2529. Only reset '__weak_ptr_' when it's already expired.
    // https://cplusplus.github.io/LWG/lwg-defects.html#2529
    // Test two different ways:
    // * Using 'weak_from_this().expired()' in C++17.
    // * Using 'shared_from_this()' in all dialects.
    {
        TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
        T* ptr = new T;
        std::shared_ptr<T> s(ptr);
        {
            // Don't re-initialize the "enable_shared_from_this" base
            // because it already references a non-expired shared_ptr.
            std::shared_ptr<T> s2(ptr, &nullDeleter);
        }
#if TEST_STD_VER > 14
        // The enable_shared_from_this base should still be referencing
        // the original shared_ptr.
        TC_ASSERT_EXPR(!ptr->weak_from_this().expired());
#endif
#ifndef TEST_HAS_NO_EXCEPTIONS
        {
#ifndef _LIBCPP_NO_EXCEPTIONS
            try {
                std::shared_ptr<T> new_s = ptr->shared_from_this();
                TC_ASSERT_EXPR(new_s == s);
            } catch (std::bad_weak_ptr const&) {
                TC_ASSERT_EXPR(false);
            } catch (...) {
                TC_ASSERT_EXPR(false);
            }
#endif // _LIBCPP_NO_EXCEPTIONS
        }
#endif
        s.reset();
        TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    }
    // Test LWG issue 2529 again. This time check that an expired pointer
    // is replaced.
    {
        TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
        T* ptr = new T;
        std::weak_ptr<T> weak;
        {
            std::shared_ptr<T> s(ptr, &nullDeleter);
            TC_ASSERT_EXPR(ptr->shared_from_this() == s);
            weak = s;
            TC_ASSERT_EXPR(!weak.expired());
        }
        TC_ASSERT_EXPR(weak.expired());
        weak.reset();

#ifndef TEST_HAS_NO_EXCEPTIONS
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            TEST_IGNORE_NODISCARD ptr->shared_from_this();
            TC_ASSERT_EXPR(false);
        } catch (std::bad_weak_ptr const&) {
        } catch (...) { TC_ASSERT_EXPR(false); }
#endif
        {
            std::shared_ptr<T> s2(ptr, &nullDeleter);
            TC_ASSERT_EXPR(ptr->shared_from_this() == s2);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        delete ptr;
        TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    }
    // Test weak_from_this_methods
#if TEST_STD_VER > 14
    {
        T* ptr = new T;
        const T* cptr = ptr;

        static_assert(noexcept(ptr->weak_from_this()), "Operation must be noexcept");
        static_assert(noexcept(cptr->weak_from_this()), "Operation must be noexcept");

        std::weak_ptr<T> my_weak = ptr->weak_from_this();
        TC_ASSERT_EXPR(my_weak.expired());

        std::weak_ptr<T const> my_const_weak = cptr->weak_from_this();
        TC_ASSERT_EXPR(my_const_weak.expired());

        // Enable shared_from_this with ptr.
        std::shared_ptr<T> sptr(ptr);
        my_weak = ptr->weak_from_this();
        TC_ASSERT_EXPR(!my_weak.expired());
        TC_ASSERT_EXPR(my_weak.lock().get() == ptr);
    }
#endif

  return 0;
}
