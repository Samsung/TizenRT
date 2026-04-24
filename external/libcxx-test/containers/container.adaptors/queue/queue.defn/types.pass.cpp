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

// template <class T, class Container = deque<T>>
// class queue
// {
// public:
//     typedef Container                                container_type;
//     typedef typename container_type::value_type      value_type;
//     typedef typename container_type::reference       reference;
//     typedef typename container_type::const_reference const_reference;
//     typedef typename container_type::size_type       size_type;
//
// protected:
//     container_type c;
// ...
// };

#include <queue>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct test
    : private std::queue<int>
{
    test()
    {
        c.push_back(1);
    }
};

struct C
{
    typedef int value_type;
    typedef int& reference;
    typedef const int& const_reference;
    typedef int size_type;
};

int tc_containers_container_adaptors_queue_queue_defn_types(void) {
    static_assert(( std::is_same<std::queue<int>::container_type, std::deque<int> >::value), "");
    static_assert(( std::is_same<std::queue<int, std::vector<int> >::container_type, std::vector<int> >::value), "");
    static_assert(( std::is_same<std::queue<int, std::vector<int> >::value_type, int>::value), "");
    static_assert(( std::is_same<std::queue<int>::reference, std::deque<int>::reference>::value), "");
    static_assert(( std::is_same<std::queue<int>::const_reference, std::deque<int>::const_reference>::value), "");
    static_assert(( std::is_same<std::queue<int>::size_type, std::deque<int>::size_type>::value), "");
    static_assert(( std::uses_allocator<std::queue<int>, std::allocator<int> >::value), "");
    static_assert((!std::uses_allocator<std::queue<int, C>, std::allocator<int> >::value), "");
    test t;

  return 0;
}
