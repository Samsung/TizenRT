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
// <memory>

// shared_ptr

// template<class T, class U>
//     shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<U>& r) noexcept;
// template<class T, class U>
//     shared_ptr<T> reinterpret_pointer_cast(shared_ptr<U>&& r) noexcept;

#include "test_macros.h"

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

struct A {
  int x;
};

struct Base { };
struct Derived : public Base { };

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_cast_reinterpret_pointer_cast(void) {
  {
    const std::shared_ptr<A> pA(new A);
    ASSERT_NOEXCEPT(std::reinterpret_pointer_cast<int>(pA));
    std::shared_ptr<int> pi = std::reinterpret_pointer_cast<int>(pA);
    std::shared_ptr<A> pA2 = std::reinterpret_pointer_cast<A>(pi);
    TC_ASSERT_EXPR(pA2.get() == pA.get());
    TC_ASSERT_EXPR(!pi.owner_before(pA) && !pA.owner_before(pi));
  }
  {
    const std::shared_ptr<A> pA;
    std::shared_ptr<int> pi = std::reinterpret_pointer_cast<int>(pA);
    std::shared_ptr<A> pA2 = std::reinterpret_pointer_cast<A>(pi);
    TC_ASSERT_EXPR(pA2.get() == pA.get());
    TC_ASSERT_EXPR(!pi.owner_before(pA) && !pA.owner_before(pi));
  }
  {
    const std::shared_ptr<A> pA(new A);
    std::shared_ptr<int> pi = std::reinterpret_pointer_cast<int>(pA);
    pA->x = 42;
    TC_ASSERT_EXPR(*pi == 42);
  }
  {
    const std::shared_ptr<Derived> pDerived(new Derived);
    std::shared_ptr<Base> pBase = std::reinterpret_pointer_cast<Base>(pDerived);
    std::shared_ptr<Derived> pDerived2 = std::reinterpret_pointer_cast<Derived>(pBase);
    TC_ASSERT_EXPR(pDerived2.get() == pDerived2.get());
    TC_ASSERT_EXPR(!pBase.owner_before(pDerived) && !pDerived.owner_before(pBase));
  }
  {
    const std::shared_ptr<Base> pBase(new Base);
    std::shared_ptr<Derived> pDerived = std::reinterpret_pointer_cast<Derived>(pBase);
    std::shared_ptr<Base> pBase2 = std::reinterpret_pointer_cast<Base>(pDerived);
    TC_ASSERT_EXPR(pBase2.get() == pBase.get());
    TC_ASSERT_EXPR(!pDerived.owner_before(pBase) && !pBase.owner_before(pDerived));
  }
#if TEST_STD_VER > 14
  {
    const std::shared_ptr<A[8]> pA;
    std::shared_ptr<int[8]> pi = std::reinterpret_pointer_cast<int[8]>(pA);
    std::shared_ptr<A[8]> pA2 = std::reinterpret_pointer_cast<A[8]>(pi);
    TC_ASSERT_EXPR(pA2.get() == pA.get());
    TC_ASSERT_EXPR(!pi.owner_before(pA) && !pA.owner_before(pi));
  }
#endif // TEST_STD_VER > 14
#if TEST_STD_VER > 20
  {
    A* pA_raw = new A;
    std::shared_ptr<A> pA(pA_raw);
    ASSERT_NOEXCEPT(std::reinterpret_pointer_cast<int>(std::move(pA)));
    std::shared_ptr<int> pi = std::reinterpret_pointer_cast<int>(std::move(pA));
    TC_ASSERT_EXPR(pA.get() == nullptr);
    TC_ASSERT_EXPR(pi.use_count() == 1);
    std::shared_ptr<A> pA2 = std::reinterpret_pointer_cast<A>(std::move(pi));
    TC_ASSERT_EXPR(pi.get() == nullptr);
    TC_ASSERT_EXPR(pA2.get() == pA_raw);
    TC_ASSERT_EXPR(pA2.use_count() == 1);
  }
#endif // TEST_STD_VER > 20

  return 0;
}
