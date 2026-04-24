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
// UNSUPPORTED: c++03, c++11

// <algorithm>

// template<class Iter>
//   void sort_heap(Iter first, Iter last);

#include <algorithm>
#include <cassert>
#include <random>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Stats {
  int compared = 0;
  int copied   = 0;
  int moved    = 0;
} stats;

struct MyInt {
  int value;
  explicit MyInt(int xval) : value(xval) {}
  MyInt(const MyInt& other) : value(other.value) { ++stats.copied; }
  MyInt(MyInt&& other) : value(other.value) { ++stats.moved; }
  MyInt& operator=(const MyInt& other) {
    value = other.value;
    ++stats.copied;
    return *this;
  }
  MyInt& operator=(MyInt&& other) {
    value = other.value;
    ++stats.moved;
    return *this;
  }
  friend bool operator<(const MyInt& a, const MyInt& b) {
    ++stats.compared;
    return a.value < b.value;
  }
};

int tc_algorithms_alg_sorting_alg_heap_operations_sort_heap_complexity(void) {
  constexpr int N = (1 << 20);
  std::vector<MyInt> v;
  v.reserve(N);
  std::mt19937 g;
  for (int i = 0; i < N; ++i) {
    v.emplace_back(i);
  }
  for (int logn = 10; logn <= 20; ++logn) {
    const int n = (1 << logn);
    auto first  = v.begin();
    auto last   = v.begin() + n;
    const int debug_elements = std::min(100, n);
    // Multiplier 2 because of comp(a,b) comp(b, a) checks.
    const int debug_comparisons = 2 * (debug_elements + 1) * debug_elements;
    std::shuffle(first, last, g);
    std::make_heap(first, last);
    // The exact stats of our current implementation are recorded here.
    stats = {};
    std::sort_heap(first, last);
    LIBCPP_ASSERT(stats.copied == 0);
    LIBCPP_ASSERT(stats.moved <= 2 * n + n * logn);
#if !_LIBCPP_ENABLE_DEBUG_MODE
    LIBCPP_ASSERT(stats.compared <= n * logn);
    (void)debug_comparisons;
#else
    LIBCPP_ASSERT(stats.compared <= 2 * n * logn + debug_comparisons);
#endif
    LIBCPP_ASSERT(std::is_sorted(first, last));
  }
  return 0;
}
