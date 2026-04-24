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
// <vector>

// template <class InputIter> vector(InputIter first, InputIter last,
//                                   const allocator_type& a);

#include <vector>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_iterators.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"
#if TEST_STD_VER >= 11
#include "emplace_constructible.h"
#include "container_test_types.h"
#include "libcxx_tc_common.h"
#endif

template <class C, class Iterator, class A>
TEST_CONSTEXPR_CXX20 void test(Iterator first, Iterator last, const A& a) {
  C c(first, last, a);
  LIBCPP_ASSERT(c.__invariants());
  TC_ASSERT_EXPR(c.size() == static_cast<std::size_t>(std::distance(first, last)));
  LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
  for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e;
       ++i, ++first)
    TC_ASSERT_EXPR(*i == *first);
}

#if TEST_STD_VER >= 11

template <class T>
struct implicit_conv_allocator : min_allocator<T> {
  TEST_CONSTEXPR implicit_conv_allocator(void*) {}
  TEST_CONSTEXPR implicit_conv_allocator(const implicit_conv_allocator&) = default;

  template <class U>
  TEST_CONSTEXPR implicit_conv_allocator(implicit_conv_allocator<U>) {}
};

#endif

TEST_CONSTEXPR_CXX20 void basic_tests() {
  {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
    int* an = a + sizeof(a) / sizeof(a[0]);
    std::allocator<int> alloc;
    test<std::vector<int> >(cpp17_input_iterator<const int*>(a),
                            cpp17_input_iterator<const int*>(an), alloc);
    test<std::vector<int> >(forward_iterator<const int*>(a),
                            forward_iterator<const int*>(an), alloc);
    test<std::vector<int> >(bidirectional_iterator<const int*>(a),
                            bidirectional_iterator<const int*>(an), alloc);
    test<std::vector<int> >(random_access_iterator<const int*>(a),
                            random_access_iterator<const int*>(an), alloc);
    test<std::vector<int> >(a, an, alloc);
  }
#if TEST_STD_VER >= 11
  {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
    int* an = a + sizeof(a) / sizeof(a[0]);
    min_allocator<int> alloc;
    test<std::vector<int, min_allocator<int> > >(
        cpp17_input_iterator<const int*>(a), cpp17_input_iterator<const int*>(an), alloc);
    test<std::vector<int, min_allocator<int> > >(
        forward_iterator<const int*>(a), forward_iterator<const int*>(an),
        alloc);
    test<std::vector<int, min_allocator<int> > >(
        bidirectional_iterator<const int*>(a),
        bidirectional_iterator<const int*>(an), alloc);
    test<std::vector<int, min_allocator<int> > >(
        random_access_iterator<const int*>(a),
        random_access_iterator<const int*>(an), alloc);
    test<std::vector<int, min_allocator<int> > >(a, an, alloc);
    test<std::vector<int, implicit_conv_allocator<int> > >(a, an, nullptr);
  }
  {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
    int* an = a + sizeof(a) / sizeof(a[0]);
    safe_allocator<int> alloc;
    test<std::vector<int, safe_allocator<int> > >(
        cpp17_input_iterator<const int*>(a), cpp17_input_iterator<const int*>(an), alloc);
    test<std::vector<int, safe_allocator<int> > >(
        forward_iterator<const int*>(a), forward_iterator<const int*>(an), alloc);
    test<std::vector<int, safe_allocator<int> > >(
        bidirectional_iterator<const int*>(a), bidirectional_iterator<const int*>(an), alloc);
    test<std::vector<int, safe_allocator<int> > >(
        random_access_iterator<const int*>(a), random_access_iterator<const int*>(an), alloc);
    test<std::vector<int, safe_allocator<int> > >(a, an, alloc);
  }
#endif
}

TEST_CONSTEXPR_CXX20 void emplaceable_concept_tests() {
#if TEST_STD_VER >= 11
  int arr1[] = {42};
  int arr2[] = {1, 101, 42};
  {
    using T = EmplaceConstructible<int>;
    using It = forward_iterator<int*>;
    using Alloc = std::allocator<T>;
    Alloc a;
    {
      std::vector<T> v(It(arr1), It(std::end(arr1)), a);
      TC_ASSERT_EXPR(v[0].value == 42);
    }
    {
      std::vector<T> v(It(arr2), It(std::end(arr2)), a);
      TC_ASSERT_EXPR(v[0].value == 1);
      TC_ASSERT_EXPR(v[1].value == 101);
      TC_ASSERT_EXPR(v[2].value == 42);
    }
  }
  {
    using T = EmplaceConstructibleAndMoveInsertable<int>;
    using It = cpp17_input_iterator<int*>;
    using Alloc = std::allocator<T>;
    Alloc a;
    {
      std::vector<T> v(It(arr1), It(std::end(arr1)), a);
      TC_ASSERT_EXPR(v[0].copied == 0);
      TC_ASSERT_EXPR(v[0].value == 42);
    }
    {
      std::vector<T> v(It(arr2), It(std::end(arr2)), a);
      TC_ASSERT_EXPR(v[0].value == 1);
      TC_ASSERT_EXPR(v[1].value == 101);
      TC_ASSERT_EXPR(v[2].copied == 0);
      TC_ASSERT_EXPR(v[2].value == 42);
    }
  }
#endif
}

void test_ctor_under_alloc() {
#if TEST_STD_VER >= 11
  int arr1[] = {42};
  int arr2[] = {1, 101, 42};
  {
    using C = TCT::vector<>;
    using It = forward_iterator<int*>;
    using Alloc = typename C::allocator_type;
    Alloc a;
    {
      ExpectConstructGuard<int&> G(1);
      C v(It(arr1), It(std::end(arr1)), a);
    }
    {
      ExpectConstructGuard<int&> G(3);
      C v(It(arr2), It(std::end(arr2)), a);
    }
  }
  {
    using C = TCT::vector<>;
    using It = cpp17_input_iterator<int*>;
    using Alloc = typename C::allocator_type;
    Alloc a;
    {
      ExpectConstructGuard<int&> G(1);
      C v(It(arr1), It(std::end(arr1)), a);
    }
    {
      //ExpectConstructGuard<int&> G(3);
      //C v(It(arr2), It(std::end(arr2)), a);
    }
  }
#endif
}

TEST_CONSTEXPR_CXX20 bool test() {
  basic_tests();
  emplaceable_concept_tests(); // See PR34898

  return true;
}

int tc_containers_sequences_vector_vector_cons_construct_iter_iter_alloc(void) {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif
  test_ctor_under_alloc();

  return 0;
}
