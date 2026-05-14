//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: no-exceptions

// (bug report: https://llvm.org/PR58392)
// Check that vector constructors don't leak memory when an operation inside the constructor throws an exception

#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>

#include "count_new.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

template <class T>
struct Allocator {
  using value_type      = T;
  using is_always_equal = std::false_type;

  template <class U>
  Allocator(const Allocator<U>&) {}

  Allocator(bool should_throw = true) {
    if (should_throw)
#ifndef _LIBCPP_NO_EXCEPTIONS
      throw 0;
  }
#endif // _LIBCPP_NO_EXCEPTIONS

  T* allocate(std::size_t n) { return std::allocator<T>().allocate(n); }
  void deallocate(T* ptr, std::size_t n) { std::allocator<T>().deallocate(ptr, n); }

  template <class U>
  friend bool operator==(const Allocator&, const Allocator<U>&) { return true; }
};

struct ThrowingT {
  int* throw_after_n_ = nullptr;
  ThrowingT() { throw 0; }

  ThrowingT(int& throw_after_n) : throw_after_n_(&throw_after_n) {
    if (throw_after_n == 0)
#ifndef _LIBCPP_NO_EXCEPTIONS
      throw 0;
    --throw_after_n;
  }
#endif // _LIBCPP_NO_EXCEPTIONS

  ThrowingT(const ThrowingT&) {
    if (throw_after_n_ == nullptr || *throw_after_n_ == 0)
#ifndef _LIBCPP_NO_EXCEPTIONS
      throw 1;
    --*throw_after_n_;
  }
#endif // _LIBCPP_NO_EXCEPTIONS

  ThrowingT& operator=(const ThrowingT&) {
    if (throw_after_n_ == nullptr || *throw_after_n_ == 0)
#ifndef _LIBCPP_NO_EXCEPTIONS
      throw 1;
    --*throw_after_n_;
    return *this;
  }
#endif // _LIBCPP_NO_EXCEPTIONS
};

template <class IterCat>
struct Iterator {
  using iterator_category = IterCat;
  using difference_type   = std::ptrdiff_t;
  using value_type        = int;
  using reference         = int&;
  using pointer           = int*;

  int i_;
  Iterator(int i = 0) : i_(i) {}
  int& operator*() {
    if (i_ == 1)
#ifndef _LIBCPP_NO_EXCEPTIONS
      throw 1;
    return i_;
  }
#endif // _LIBCPP_NO_EXCEPTIONS

  friend bool operator==(const Iterator& lhs, const Iterator& rhs) { return lhs.i_ == rhs.i_; }

  friend bool operator!=(const Iterator& lhs, const Iterator& rhs) { return lhs.i_ != rhs.i_; }

  Iterator& operator++() {
    ++i_;
    return *this;
  }

  Iterator operator++(int) {
    auto tmp = *this;
    ++i_;
    return tmp;
  }
};

void check_new_delete_called() {
  TC_ASSERT_EXPR(globalMemCounter.new_called == globalMemCounter.delete_called);
  TC_ASSERT_EXPR(globalMemCounter.new_array_called == globalMemCounter.delete_array_called);
  TC_ASSERT_EXPR(globalMemCounter.aligned_new_called == globalMemCounter.aligned_delete_called);
  TC_ASSERT_EXPR(globalMemCounter.aligned_new_array_called == globalMemCounter.aligned_delete_array_called);
}

int tc_containers_sequences_vector_vector_cons_exceptions(void) {
  using AllocVec = std::vector<int, Allocator<int> >;
#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // vector()
    AllocVec vec;
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(size_type) from type
    std::vector<ThrowingT> get_alloc(1);
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#if TEST_STD_VER >= 14
#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(size_type, value_type) from type
    int throw_after = 1;
    ThrowingT v(throw_after);
    std::vector<ThrowingT> get_alloc(1, v);
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(size_type, const allocator_type&) from allocator
    Allocator<int> alloc(false);
    AllocVec get_alloc(0, alloc);
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(size_type, const allocator_type&) from the type
    std::vector<ThrowingT> vec(1, std::allocator<ThrowingT>());
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();
#endif  // TEST_STD_VER >= 14

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(InputIterator, InputIterator) from input iterator
    std::vector<int> vec((Iterator<std::input_iterator_tag>()), Iterator<std::input_iterator_tag>(2));
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(InputIterator, InputIterator) from forward iterator
    std::vector<int> vec((Iterator<std::forward_iterator_tag>()), Iterator<std::forward_iterator_tag>(2));
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(InputIterator, InputIterator) from allocator
    int a[] = {1, 2};
    AllocVec vec(cpp17_input_iterator<int*>(a), cpp17_input_iterator<int*>(a + 2));
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(InputIterator, InputIterator, const allocator_type&) from input iterator
    std::allocator<int> alloc;
    std::vector<int> vec(Iterator<std::input_iterator_tag>(), Iterator<std::input_iterator_tag>(2), alloc);
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(InputIterator, InputIterator, const allocator_type&) from forward iterator
    std::allocator<int> alloc;
    std::vector<int> vec(Iterator<std::forward_iterator_tag>(), Iterator<std::forward_iterator_tag>(2), alloc);
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(InputIterator, InputIterator, const allocator_type&) from allocator
    int a[] = {1, 2};
    Allocator<int> alloc(false);
    AllocVec vec(cpp17_input_iterator<int*>(a), cpp17_input_iterator<int*>(a + 2), alloc);
  } catch (int) {
    // FIXME: never called.
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(InputIterator, InputIterator, const allocator_type&) from allocator
    int a[] = {1, 2};
    Allocator<int> alloc(false);
    AllocVec vec(forward_iterator<int*>(a), forward_iterator<int*>(a + 2), alloc);
  } catch (int) {
    // FIXME: never called.
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(const vector&) from type
    std::vector<ThrowingT> vec;
    int throw_after = 0;
    vec.emplace_back(throw_after);
    auto vec2 = vec;
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(const vector&, const allocator_type&) from type
    std::vector<ThrowingT> vec;
    int throw_after = 1;
    vec.emplace_back(throw_after);
    std::vector<ThrowingT> vec2(vec, std::allocator<int>());
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(vector&&, const allocator_type&) from type
    std::vector<ThrowingT, Allocator<ThrowingT> > vec(Allocator<ThrowingT>(false));
    int throw_after = 1;
    vec.emplace_back(throw_after);
    std::vector<ThrowingT, Allocator<ThrowingT> > vec2(std::move(vec), Allocator<ThrowingT>(false));
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#if TEST_STD_VER >= 11
#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(initializer_list<value_type>) from type
    int throw_after = 1;
    std::vector<ThrowingT> vec({ThrowingT(throw_after)});
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();

#ifndef _LIBCPP_NO_EXCEPTIONS
  try { // Throw in vector(initializer_list<value_type>, const allocator_type&) constructor from type
    int throw_after = 1;
    std::vector<ThrowingT> vec({ThrowingT(throw_after)}, std::allocator<ThrowingT>());
  } catch (int) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  check_new_delete_called();
#endif // TEST_STD_VER >= 11

  return 0;
}
