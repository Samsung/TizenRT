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
// <queue>

// template <class T, class Container = vector<T>,
//           class Compare = less<typename Container::value_type>>
// class priority_queue
// {
// public:
//     typedef Container                                container_type;
//     typedef Compare                                  value_compare; // LWG#2684
//     typedef typename container_type::value_type      value_type;
//     typedef typename container_type::reference       reference;
//     typedef typename container_type::const_reference const_reference;
//     typedef typename container_type::size_type       size_type;
//
// protected:
//     container_type c;
//     Compare comp;

#include <queue>
#include <cassert>
#include <deque>
#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct test
    : private std::priority_queue<int>
{
    test()
    {
        c.push_back(1);
        TC_ASSERT_EXPR(comp(1, 2));
    }
};

struct C
{
    typedef int value_type;
    typedef int& reference;
    typedef const int& const_reference;
    typedef int size_type;
};

int tc_containers_container_adaptors_priority_queue_types(void) {
    static_assert(( std::is_same<std::priority_queue<int>::container_type, std::vector<int> >::value), "");
    static_assert(( std::is_same<std::priority_queue<int, std::deque<int> >::container_type, std::deque<int> >::value), "");
    static_assert(( std::is_same<std::priority_queue<int, std::deque<int> >::value_type, int>::value), "");
    static_assert(( std::is_same<std::priority_queue<int>::reference, std::vector<int>::reference>::value), "");
    static_assert(( std::is_same<std::priority_queue<int>::const_reference, std::vector<int>::const_reference>::value), "");
    static_assert(( std::is_same<std::priority_queue<int>::size_type, std::vector<int>::size_type>::value), "");
    static_assert(( std::is_same<std::priority_queue<int>::value_compare, std::less<int> >::value), "");
    static_assert(( std::is_same<std::priority_queue<int, std::deque<int> >::value_compare, std::less<int> >::value), "");
    static_assert(( std::is_same<std::priority_queue<int, std::deque<int>, std::greater<int> >::value_compare, std::greater<int> >::value), "");
    static_assert(( std::uses_allocator<std::priority_queue<int>, std::allocator<int> >::value), "");
    static_assert((!std::uses_allocator<std::priority_queue<int, C>, std::allocator<int> >::value), "");
    test t;

  return 0;
}
