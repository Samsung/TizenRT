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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// This test requires support for aligned allocation to test overaligned types.
// XFAIL: availability-aligned_allocation-missing

// https://reviews.llvm.org/D129198 is not in AppleClang 14
// XFAIL: stdlib=apple-libc++ && target={{.+}}-apple-macosx10.13{{(.0)?}} && apple-clang-14

// <memory>

// shared_ptr

// template<class T> shared_ptr<T> make_shared(size_t N); // T is U[]
//
// template<class T>
// shared_ptr<T> make_shared(size_t N, const remove_extent_t<T>& u); // T is U[]

// Ignore error about requesting a large alignment not being ABI compatible with older AIX systems.
#ifdef _AIX
# pragma clang diagnostic ignored "-Waix-compat"
#endif

#include <cassert>
#include <concepts>
#include <cstdint> // std::uintptr_t
#include <memory>
#include <utility>

#include "operator_hijacker.h"
#include "test_macros.h"
#include "types.h"
#include "libcxx_tc_common.h"

template <class T, class ...Args>
concept CanMakeShared = requires(Args&& ...args) {
  { std::make_shared<T>(std::forward<Args>(args)...) } -> std::same_as<std::shared_ptr<T>>;
};

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_create_make_shared_array_unbounded(void) {
  // Check behavior for a zero-sized array
  {
    // Without passing an initial value
    {
      using Array = int[];
      std::shared_ptr<Array> ptr = std::make_shared<Array>(0);
      TC_ASSERT_EXPR(ptr != nullptr);
    }

    // Passing an initial value
    {
      using Array = int[];
      std::shared_ptr<Array> ptr = std::make_shared<Array>(0, 42);
      TC_ASSERT_EXPR(ptr != nullptr);
    }
  }

  // Check behavior for a 1-sized array
  {
    // Without passing an initial value
    {
      using Array = int[];
      std::shared_ptr<Array> ptr = std::make_shared<Array>(1);
      TC_ASSERT_EXPR(ptr != nullptr);
      TC_ASSERT_EXPR(ptr[0] == 0);
    }

    // Passing an initial value
    {
      using Array = int[];
      std::shared_ptr<Array> ptr = std::make_shared<Array>(1, 42);
      TC_ASSERT_EXPR(ptr != nullptr);
      TC_ASSERT_EXPR(ptr[0] == 42);
    }
  }

  // Make sure we initialize elements correctly
  {
    // Without passing an initial value
    {
      using Array = int[];
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
      for (unsigned i = 0; i < 8; ++i) {
        TC_ASSERT_EXPR(ptr[i] == 0);
      }
    }
    {
      using Array = int[][3];
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
      for (unsigned i = 0; i < 8; ++i) {
        TC_ASSERT_EXPR(ptr[i][0] == 0);
        TC_ASSERT_EXPR(ptr[i][1] == 0);
        TC_ASSERT_EXPR(ptr[i][2] == 0);
      }
    }
    {
      using Array = int[][3][2];
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
      for (unsigned i = 0; i < 8; ++i) {
        TC_ASSERT_EXPR(ptr[i][0][0] == 0);
        TC_ASSERT_EXPR(ptr[i][0][1] == 0);
        TC_ASSERT_EXPR(ptr[i][1][0] == 0);
        TC_ASSERT_EXPR(ptr[i][1][1] == 0);
        TC_ASSERT_EXPR(ptr[i][2][0] == 0);
        TC_ASSERT_EXPR(ptr[i][2][1] == 0);
      }
    }

    // Passing an initial value
    {
      using Array = int[];
      int init = 42;
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
      for (unsigned i = 0; i < 8; ++i) {
        TC_ASSERT_EXPR(ptr[i] == init);
      }
    }
    {
      using Array = int[][3];
      int init[3] = {42, 43, 44};
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
      for (unsigned i = 0; i < 8; ++i) {
        TC_ASSERT_EXPR(ptr[i][0] == 42);
        TC_ASSERT_EXPR(ptr[i][1] == 43);
        TC_ASSERT_EXPR(ptr[i][2] == 44);
      }
    }
    {
      using Array = int[][3][2];
      int init[3][2] = {{31, 32}, {41, 42}, {51, 52}};
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
      for (unsigned i = 0; i < 8; ++i) {
        TC_ASSERT_EXPR(ptr[i][0][0] == 31);
        TC_ASSERT_EXPR(ptr[i][0][1] == 32);
        TC_ASSERT_EXPR(ptr[i][1][0] == 41);
        TC_ASSERT_EXPR(ptr[i][1][1] == 42);
        TC_ASSERT_EXPR(ptr[i][2][0] == 51);
        TC_ASSERT_EXPR(ptr[i][2][1] == 52);
      }
    }
  }

  // Make sure array elements are destroyed in reverse order
  {
    // Without passing an initial value
    {
      using Array = DestroyInReverseOrder[];
      DestroyInReverseOrder::reset();
      {
        std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
        TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 8);
      }
      TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 0);
    }
    {
      using Array = DestroyInReverseOrder[][3];
      DestroyInReverseOrder::reset();
      {
        std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
        TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 8 * 3);
      }
      TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 0);
    }
    {
      using Array = DestroyInReverseOrder[][3][2];
      DestroyInReverseOrder::reset();
      {
        std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
        TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 8 * 3 * 2);
      }
      TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 0);
    }

    // Passing an initial value
    {
      using Array = DestroyInReverseOrder[];
      int count = 0;
      DestroyInReverseOrder init(&count);
      int init_count = 1;
      {
        std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
        TC_ASSERT_EXPR(count == 8 + init_count);
      }
      TC_ASSERT_EXPR(count == init_count);
    }
    {
      using Array = DestroyInReverseOrder[][3];
      int count = 0;
      DestroyInReverseOrder init[3] = {&count, &count, &count};
      int init_count = 3;
      {
        std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
        TC_ASSERT_EXPR(count == 8 * 3 + init_count);
      }
      TC_ASSERT_EXPR(count == init_count);
    }
    {
      using Array = DestroyInReverseOrder[][3][2];
      int count = 0;
      DestroyInReverseOrder init[3][2] = {{&count, &count}, {&count, &count}, {&count, &count}};
      int init_count = 3 * 2;
      {
        std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
        TC_ASSERT_EXPR(count == 8 * 3 * 2 + init_count);
      }
      TC_ASSERT_EXPR(count == init_count);
    }
  }

  // Count the number of copies being made
  {
    // Without passing an initial value
    {
      using Array = CountCopies[];
      CountCopies::reset();
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
      TC_ASSERT_EXPR(CountCopies::copies() == 0);
    }
    {
      using Array = CountCopies[][3];
      CountCopies::reset();
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
      TC_ASSERT_EXPR(CountCopies::copies() == 0);
    }
    {
      using Array = CountCopies[][3][2];
      CountCopies::reset();
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
      TC_ASSERT_EXPR(CountCopies::copies() == 0);
    }

    // Passing an initial value
    {
      using Array = CountCopies[];
      int copies = 0;
      CountCopies init(&copies);
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
      TC_ASSERT_EXPR(copies == 8);
    }
    {
      using Array = CountCopies[][3];
      int copies = 0;
      CountCopies init[3] = {&copies, &copies, &copies};
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
      TC_ASSERT_EXPR(copies == 8 * 3);
    }
    {
      using Array = CountCopies[][3][2];
      int copies = 0;
      CountCopies init[3][2] = {{&copies, &copies}, {&copies, &copies}, {&copies, &copies}};
      std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
      TC_ASSERT_EXPR(copies == 8 * 3 * 2);
    }
  }

  // Make sure array elements are aligned properly when the array contains an overaligned type.
  //
  // Here, we don't need to test both the with-initial-value and without-initial-value code paths,
  // since we're just checking the alignment and both are going to use the same code path unless
  // the implementation is completely crazy.
  {
    auto check_alignment = []<class T> {
      {
        using Array = T[];
        std::shared_ptr ptr = std::make_shared<Array>(8);
        for (int i = 0; i < 8; ++i) {
          T* p = std::addressof(ptr[i]);
          TC_ASSERT_EXPR(reinterpret_cast<std::uintptr_t>(p) % alignof(T) == 0);
        }
      }
      {
        using Array = T[][3];
        std::shared_ptr ptr = std::make_shared<Array>(8);
        for (int i = 0; i < 8; ++i) {
          for (int j = 0; j < 3; ++j) {
            T* p = std::addressof(ptr[i][j]);
            TC_ASSERT_EXPR(reinterpret_cast<std::uintptr_t>(p) % alignof(T) == 0);
          }
        }
      }
      {
        using Array = T[][3][2];
        std::shared_ptr ptr = std::make_shared<Array>(8);
        for (int i = 0; i < 8; ++i) {
          for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 2; ++k) {
              T* p = std::addressof(ptr[i][j][k]);
              TC_ASSERT_EXPR(reinterpret_cast<std::uintptr_t>(p) % alignof(T) == 0);
            }
          }
        }
      }
    };

    struct Empty { };
    check_alignment.operator()<Empty>();
    check_alignment.operator()<OverAligned>();
    check_alignment.operator()<MaxAligned>();

    // test non corner cases as well while we're at it
    struct Foo { int i; char c; };
    check_alignment.operator()<int>();
    check_alignment.operator()<Foo>();
  }

  // Make sure that we destroy all the elements constructed so far when an exception
  // is thrown. Also make sure that we do it in reverse order of construction.
#ifndef TEST_HAS_NO_EXCEPTIONS
  {
    struct Sentinel : ThrowOnConstruction, DestroyInReverseOrder { };

    // Without passing an initial value
    {
      using Array = Sentinel[];
      for (int i = 0; i < 8; ++i) {
        ThrowOnConstruction::throw_after(i);
        DestroyInReverseOrder::reset();
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
          std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
          TC_ASSERT_EXPR(false);
        } catch (ThrowOnConstruction::exception const&) {
          TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 0);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
      }
    }
    {
      using Array = Sentinel[][3];
      for (int i = 0; i < 8 * 3; ++i) {
        ThrowOnConstruction::throw_after(i);
        DestroyInReverseOrder::reset();
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
          std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
          TC_ASSERT_EXPR(false);
        } catch (ThrowOnConstruction::exception const&) {
          TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 0);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
      }
    }
    {
      using Array = Sentinel[][3][2];
      for (int i = 0; i < 8 * 3 * 2; ++i) {
        ThrowOnConstruction::throw_after(i);
        DestroyInReverseOrder::reset();
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
          std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
          TC_ASSERT_EXPR(false);
        } catch (ThrowOnConstruction::exception const&) {
          TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 0);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
      }
    }

    // Passing an initial value
    {
      using Array = Sentinel[];
      for (int i = 0; i < 8; ++i) {
        DestroyInReverseOrder::reset();
        ThrowOnConstruction::reset();
        Sentinel init;
        ThrowOnConstruction::throw_after(i);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
          std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
          TC_ASSERT_EXPR(false);
        } catch (ThrowOnConstruction::exception const&) {
          TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 1);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
      }
    }
    {
      using Array = Sentinel[][3];
      for (int i = 0; i < 8 * 3; ++i) {
        DestroyInReverseOrder::reset();
        ThrowOnConstruction::reset();
        Sentinel init[3] = {};
        ThrowOnConstruction::throw_after(i);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
          std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
          TC_ASSERT_EXPR(false);
        } catch (ThrowOnConstruction::exception const&) {
          TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 3);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
      }
    }
    {
      using Array = Sentinel[][3][2];
      for (int i = 0; i < 8 * 3 * 2; ++i) {
        DestroyInReverseOrder::reset();
        ThrowOnConstruction::reset();
        Sentinel init[3][2] = {};
        ThrowOnConstruction::throw_after(i);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
          std::shared_ptr<Array> ptr = std::make_shared<Array>(8, init);
          TC_ASSERT_EXPR(false);
        } catch (ThrowOnConstruction::exception const&) {
          TC_ASSERT_EXPR(DestroyInReverseOrder::alive() == 3 * 2);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
      }
    }
  }
#endif // TEST_HAS_NO_EXCEPTIONS

  // Make sure the version without an initialization argument works even for non-movable types
  {
    using Array = NonMovable[][3];
    std::shared_ptr<Array> ptr = std::make_shared<Array>(8);
    (void)ptr;
  }

    // Make sure std::make_shared handles badly-behaved types properly
    {
      using Array = operator_hijacker[];
      std::shared_ptr<Array> p1 = std::make_shared<Array>(3);
      std::shared_ptr<Array> p2 = std::make_shared<Array>(3, operator_hijacker());
      TC_ASSERT_EXPR(p1 != nullptr);
      TC_ASSERT_EXPR(p2 != nullptr);
    }

  // Check that we SFINAE-away for invalid arguments
  {
    struct T { };
    static_assert( CanMakeShared<T[], std::size_t>);
    static_assert( CanMakeShared<T[], std::size_t, T>);
    static_assert(!CanMakeShared<T[], std::size_t, T, int>); // too many arguments
  }

  return 0;
}
