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

// template <class T, class D>
// struct hash<unique_ptr<T, D>>
// {
//     typedef unique_ptr<T, D> argument_type;
//     typedef size_t           result_type;
//     size_t operator()(const unique_ptr<T, D>& p) const;
// };

#include <memory>

#include <cassert>
#include <functional>

#include "test_macros.h"

#if TEST_STD_VER >= 11
#include "poisoned_hash_helper.h"
#include "deleter_types.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class ValueT, class Del>
void test_enabled_with_deleter() {
  using UPtr = std::unique_ptr<ValueT, Del>;
  using pointer = typename UPtr::pointer;
  using RawDel = typename std::decay<Del>::type;
  RawDel d(1);
  UPtr p(nullptr, std::forward<Del>(d));
  test_hash_enabled_for_type<UPtr>(p);
  test_hash_enabled_for_type<pointer>();
}

template <class ValueT, class Del>
void test_disabled_with_deleter() {
  using UPtr = std::unique_ptr<ValueT, Del>;
  using pointer = typename UPtr::pointer;
  test_hash_disabled_for_type<UPtr>();
  test_hash_disabled_for_type<pointer>();
}

template <class T>
struct std::hash<min_pointer<T, std::integral_constant<std::size_t, 1>>> {
  std::size_t operator()(min_pointer<T, std::integral_constant<size_t, 1>> p) const TEST_NOEXCEPT_FALSE {
    if (!p) return 0;
    return std::hash<T*>{}(std::addressof(*p));
  }
};

struct A {};

#endif // TEST_STD_VER >= 11

int tc_utilities_memory_util_smartptr_util_smartptr_hash_hash_unique_ptr(void) {
  {
    int* ptr = new int;
    std::unique_ptr<int> p(ptr);
    std::hash<std::unique_ptr<int> > f;
    std::size_t h = f(p);
    TC_ASSERT_EXPR(h == std::hash<int*>()(ptr));
  }
#if TEST_STD_VER >= 11
  {
    std::unique_ptr<int, PointerDeleter<int, 1>> pThrowingHash;
    std::hash<std::unique_ptr<int, PointerDeleter<int, 1>>> fThrowingHash;
    ASSERT_NOT_NOEXCEPT(fThrowingHash(pThrowingHash));
  }
  {
    test_enabled_with_deleter<int, Deleter<int>>();
    test_enabled_with_deleter<int[], Deleter<int[]>>();
    test_enabled_with_deleter<int, CopyDeleter<int>>();
    test_enabled_with_deleter<int, CopyDeleter<int[]>>();
    test_enabled_with_deleter<int, NCDeleter<int>&>();
    test_enabled_with_deleter<int[], NCDeleter<int[]>&>();
    test_enabled_with_deleter<int, NCConstDeleter<int> const&>();
    test_enabled_with_deleter<int[], NCConstDeleter<int[]> const&>();
  }
  {
    test_enabled_with_deleter<int, PointerDeleter<int, 1>>();
    test_enabled_with_deleter<int[], PointerDeleter<int[], 1>>();
    test_enabled_with_deleter<A, PointerDeleter<A, 1>>();
    test_enabled_with_deleter<A[], PointerDeleter<A[], 1>>();

#if TEST_STD_VER > 14
    test_disabled_with_deleter<int, PointerDeleter<int, 0>>();
    test_disabled_with_deleter<int[], PointerDeleter<int[], 0>>();
    test_disabled_with_deleter<A, PointerDeleter<A, 0>>();
    test_disabled_with_deleter<A[], PointerDeleter<A[], 0>>();
#endif
  }
#endif

  return 0;
}
