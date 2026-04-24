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

// <memory>

// shared_ptr

// template<class T, class A, class... Args>
// shared_ptr<T> allocate_shared(const A& a, Args&&... args);

// This test checks that allocator_traits::construct is used in allocate_shared
// as requested in C++20 (via P0674R1).

#include "test_macros.h"

#include <cassert>
#include <cstdint>
#include <memory>
#include <new>
#include <utility>
#include "libcxx_tc_common.h"

static bool construct_called = false;
static bool destroy_called = false;
static unsigned allocator_id = 0;

template <class T>
struct MyAllocator {
public:
  typedef T value_type;
  typedef T* pointer;

  unsigned id = 0;

  MyAllocator() = default;
  MyAllocator(int i) : id(i) {}

  template <class U>
  MyAllocator(MyAllocator<U> const& other) : id(other.id){};

  pointer allocate(std::ptrdiff_t n) {
    return pointer(static_cast<T*>(::operator new(n * sizeof(T))));
  }

  void deallocate(pointer p, std::ptrdiff_t) { return ::operator delete(p); }

  template <typename ...Args>
  void construct(T* p, Args&& ...args) {
    construct_called = true;
    destroy_called = false;
    allocator_id = id;
    ::new (p) T(std::forward<Args>(args)...);
  }

  void destroy(T* p) {
    construct_called = false;
    destroy_called = true;
    allocator_id = id;
    p->~T();
  }
};

struct Private;

class Factory {
public:
  static std::shared_ptr<Private> allocate();
};

template <class T>
struct FactoryAllocator;

struct Private {
  int id;

private:
  friend FactoryAllocator<Private>;
  Private(int i) : id(i) {}
  ~Private() {}
};

template <class T>
struct FactoryAllocator : std::allocator<T> {
  FactoryAllocator() = default;

  template <class T1>
  FactoryAllocator(FactoryAllocator<T1>) {}

  template <class T1>
  struct rebind {
    typedef FactoryAllocator<T1> other;
  };

  void construct(void* p, int id) { ::new (p) Private(id); }
  void destroy(Private* p) { p->~Private(); }
};

std::shared_ptr<Private> Factory::allocate() {
  FactoryAllocator<Private> factory_alloc;
  return std::allocate_shared<Private>(factory_alloc, 42);
}

struct mchar {
  char c;
};

struct Foo {
  int val;

  Foo(int v) : val(v) {}

  Foo(Foo a, Foo b) : val(a.val + b.val) {}
};

struct Bar {
  std::max_align_t y;
};

void test_aligned(void* p, std::size_t align) {
  TC_ASSERT_EXPR(reinterpret_cast<std::uintptr_t>(p) % align == 0);
}

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_create_allocate_shared_construct(void) {
  {
    std::shared_ptr<int> p = std::allocate_shared<int>(MyAllocator<int>());
    TC_ASSERT_EXPR(construct_called);
  }
  TC_ASSERT_EXPR(destroy_called);
  {
    std::shared_ptr<Foo> p =
        std::allocate_shared<Foo>(MyAllocator<Foo>(), Foo(42), Foo(100));
    TC_ASSERT_EXPR(construct_called);
    TC_ASSERT_EXPR(p->val == 142);
  }
  TC_ASSERT_EXPR(destroy_called);
  { // Make sure allocator is copied.
    std::shared_ptr<int> p = std::allocate_shared<int>(MyAllocator<int>(3));
    TC_ASSERT_EXPR(allocator_id == 3);

    allocator_id = 0;
  }
  TC_ASSERT_EXPR(allocator_id == 3);

  {
    std::shared_ptr<int> p = std::allocate_shared<int>(MyAllocator<int>(), 42);
    TC_ASSERT_EXPR(construct_called);
    TC_ASSERT_EXPR(*p == 42);
  }
  TC_ASSERT_EXPR(destroy_called);

  { // Make sure allocator is properly re-bound.
    std::shared_ptr<int> p =
        std::allocate_shared<int>(MyAllocator<mchar>(), 42);
    TC_ASSERT_EXPR(construct_called);
    TC_ASSERT_EXPR(*p == 42);
  }
  TC_ASSERT_EXPR(destroy_called);

  {
    // Make sure that we call the correct allocator::construct. Private has a private constructor
    // so the construct method must be called on its friend Factory's allocator
    // (Factory::Allocator).
    std::shared_ptr<Private> p = Factory().allocate();
    TC_ASSERT_EXPR(p->id == 42);
  }

  {
    std::shared_ptr<Bar> p;
    test_aligned(p.get(), alignof(Bar));
  }

  return 0;
}
