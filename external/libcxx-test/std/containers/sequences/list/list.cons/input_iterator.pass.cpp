//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <list>

// template <class InputIterator>
//   list(InputIterator first, InputIterator last, const Allocator& = Allocator());

#include <list>
#include <cassert>
#include "test_macros.h"
#include "test_iterators.h"
#include "test_allocator.h"
#include "min_allocator.h"
#if TEST_STD_VER >= 11
#include "emplace_constructible.h"
#include "container_test_types.h"
#include "libcxx_tc_common.h"
#endif

void basic_test()
{
    {
        int a[] = {0, 1, 2, 3};
        std::list<int> l(cpp17_input_iterator<const int*>(a),
                         cpp17_input_iterator<const int*>(a + sizeof(a)/sizeof(a[0])));
        TC_ASSERT_EXPR(l.size() == sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == sizeof(a)/sizeof(a[0]));
        int j = 0;
        for (std::list<int>::const_iterator i = l.begin(), e = l.end(); i != e; ++i, ++j)
            TC_ASSERT_EXPR(*i == j);
    }
    {
        int a[] = {0, 1, 2, 3};
        std::list<int> l(cpp17_input_iterator<const int*>(a),
                         cpp17_input_iterator<const int*>(a + sizeof(a)/sizeof(a[0])),
                         std::allocator<int>());
        TC_ASSERT_EXPR(l.size() == sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == sizeof(a)/sizeof(a[0]));
        int j = 0;
        for (std::list<int>::const_iterator i = l.begin(), e = l.end(); i != e; ++i, ++j)
            TC_ASSERT_EXPR(*i == j);
    }
    {
        int a[] = {0, 1, 2, 3};
        // Add 2 for implementations that dynamically allocate a sentinel node and container proxy.
        std::list<int, limited_allocator<int, sizeof(a)/sizeof(a[0]) + 2> > l(cpp17_input_iterator<const int*>(a),
                         cpp17_input_iterator<const int*>(a + sizeof(a)/sizeof(a[0])));
        TC_ASSERT_EXPR(l.size() == sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == sizeof(a)/sizeof(a[0]));
        int j = 0;
        for (std::list<int>::const_iterator i = l.begin(), e = l.end(); i != e; ++i, ++j)
            TC_ASSERT_EXPR(*i == j);
    }
#if TEST_STD_VER >= 11
    {
        int a[] = {0, 1, 2, 3};
        std::list<int, min_allocator<int>> l(cpp17_input_iterator<const int*>(a),
                         cpp17_input_iterator<const int*>(a + sizeof(a)/sizeof(a[0])));
        TC_ASSERT_EXPR(l.size() == sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == sizeof(a)/sizeof(a[0]));
        int j = 0;
        for (std::list<int, min_allocator<int>>::const_iterator i = l.begin(), e = l.end(); i != e; ++i, ++j)
            TC_ASSERT_EXPR(*i == j);
    }
    {
        int a[] = {0, 1, 2, 3};
        std::list<int, min_allocator<int>> l(cpp17_input_iterator<const int*>(a),
                         cpp17_input_iterator<const int*>(a + sizeof(a)/sizeof(a[0])),
                         min_allocator<int>());
        TC_ASSERT_EXPR(l.size() == sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == sizeof(a)/sizeof(a[0]));
        int j = 0;
        for (std::list<int, min_allocator<int>>::const_iterator i = l.begin(), e = l.end(); i != e; ++i, ++j)
            TC_ASSERT_EXPR(*i == j);
    }
#endif
}


void test_emplacable_concept() {
#if TEST_STD_VER >= 11
  int arr1[] = {42};
  int arr2[] = {1, 101, 42};
  {
    using T = EmplaceConstructible<int>;
    using It = random_access_iterator<int*>;
    {
      std::list<T> v(It(arr1), It(std::end(arr1)));
      auto I = v.begin();
      TC_ASSERT_EXPR(I->value == 42);
    }
    {
      std::list<T> v(It(arr2), It(std::end(arr2)));
      auto I = v.begin();
      TC_ASSERT_EXPR(I->value == 1);
      ++I;
      TC_ASSERT_EXPR(I->value == 101);
      ++I;
      TC_ASSERT_EXPR(I->value == 42);
    }
  }
  {
    using T = EmplaceConstructible<int>;
    using It = cpp17_input_iterator<int*>;
    {
      std::list<T> v(It(arr1), It(std::end(arr1)));
      auto I = v.begin();
      TC_ASSERT_EXPR(I->value == 42);
    }
    {
      std::list<T> v(It(arr2), It(std::end(arr2)));
      auto I = v.begin();
      //TC_ASSERT_EXPR(v[0].copied == 0);
      TC_ASSERT_EXPR(I->value == 1);
      //TC_ASSERT_EXPR(v[1].copied == 0);
      ++I;
      TC_ASSERT_EXPR(I->value == 101);
      ++I;
      TC_ASSERT_EXPR(I->value == 42);
    }
  }
#endif
}


void test_emplacable_concept_with_alloc() {
#if TEST_STD_VER >= 11
  int arr1[] = {42};
  int arr2[] = {1, 101, 42};
  {
    using T = EmplaceConstructible<int>;
    using It = random_access_iterator<int*>;
    std::allocator<T> a;
    {
      std::list<T> v(It(arr1), It(std::end(arr1)), a);
      auto I = v.begin();
      TC_ASSERT_EXPR(I->value == 42);
    }
    {
      std::list<T> v(It(arr2), It(std::end(arr2)), a);
      auto I = v.begin();
      TC_ASSERT_EXPR(I->value == 1);
      ++I;
      TC_ASSERT_EXPR(I->value == 101);
      ++I;
      TC_ASSERT_EXPR(I->value == 42);
    }
  }
  {
    using T = EmplaceConstructible<int>;
    using It = cpp17_input_iterator<int*>;
    std::allocator<T> a;
    {
      std::list<T> v(It(arr1), It(std::end(arr1)), a);
      auto I = v.begin();
      TC_ASSERT_EXPR(I->value == 42);
    }
    {
      std::list<T> v(It(arr2), It(std::end(arr2)), a);
      auto I = v.begin();
      //TC_ASSERT_EXPR(v[0].copied == 0);
      TC_ASSERT_EXPR(I->value == 1);
      //TC_ASSERT_EXPR(v[1].copied == 0);
      ++I;
      TC_ASSERT_EXPR(I->value == 101);
      ++I;
      TC_ASSERT_EXPR(I->value == 42);
    }
  }
#endif
}

void test_ctor_under_alloc() {
#if TEST_STD_VER >= 11
  int arr1[] = {42};
  int arr2[] = {1, 101, 42};
  {
    using C = TCT::list<>;
    using It = forward_iterator<int*>;
    {
      ExpectConstructGuard<int&> G(1);
      C v(It(arr1), It(std::end(arr1)));
    }
    {
      ExpectConstructGuard<int&> G(3);
      C v(It(arr2), It(std::end(arr2)));
    }
  }
  {
    using C = TCT::list<>;
    using It = cpp17_input_iterator<int*>;
    {
      ExpectConstructGuard<int&> G(1);
      C v(It(arr1), It(std::end(arr1)));
    }
    {
      ExpectConstructGuard<int&> G(3);
      C v(It(arr2), It(std::end(arr2)));
    }
  }
#endif
}

void test_ctor_under_alloc_with_alloc() {
#if TEST_STD_VER >= 11
  int arr1[] = {42};
  int arr2[] = {1, 101, 42};
  {
    using C = TCT::list<>;
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
    using C = TCT::list<>;
    using It = cpp17_input_iterator<int*>;
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
#endif
}


int tc_containers_sequences_list_list_cons_input_iterator(void) {
  basic_test();
  test_emplacable_concept();
  test_emplacable_concept_with_alloc();
  test_ctor_under_alloc();
  test_ctor_under_alloc_with_alloc();

  return 0;
}
