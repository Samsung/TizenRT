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
// Make sure that std::allocate_shared works with an allocator type that is
// only explicitly convertible from another specialization of itself.

#include <cassert>
#include <cstddef>
#include <memory>
#include "libcxx_tc_common.h"

template <class T>
struct ExplicitAllocator {
    ExplicitAllocator() = default;
    template <class U>
    explicit ExplicitAllocator(ExplicitAllocator<U>) { }

    using value_type = T;
    T* allocate(std::size_t n) { return std::allocator<T>().allocate(n); }
    void deallocate(T* ptr, std::size_t n) { return std::allocator<T>().deallocate(ptr, n); }
};

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_create_allocate_shared_explicit_conversion(void) {
    std::shared_ptr<int> ptr = std::allocate_shared<int>(ExplicitAllocator<int>(), 0);
    (void)ptr;

    return 0;
}
