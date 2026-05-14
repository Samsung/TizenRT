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
#include <functional>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
struct is_transparent
{
private:
    struct two {char lx; char lxx;};
    template <class U> static two test(...);
    template <class U> static char test(typename U::is_transparent* = 0);
public:
    static const bool value = sizeof(test<T>(0)) == 1;
};


int tc_utilities_function_objects_logical_operations_transparent(void) {
    static_assert ( !is_transparent<std::logical_and<int>>::value, "" );
    static_assert ( !is_transparent<std::logical_and<std::string>>::value, "" );
    static_assert (  is_transparent<std::logical_and<void>>::value, "" );
    static_assert (  is_transparent<std::logical_and<>>::value, "" );

    static_assert ( !is_transparent<std::logical_or<int>>::value, "" );
    static_assert ( !is_transparent<std::logical_or<std::string>>::value, "" );
    static_assert (  is_transparent<std::logical_or<void>>::value, "" );
    static_assert (  is_transparent<std::logical_or<>>::value, "" );

    static_assert ( !is_transparent<std::logical_not<int>>::value, "" );
    static_assert ( !is_transparent<std::logical_not<std::string>>::value, "" );
    static_assert (  is_transparent<std::logical_not<void>>::value, "" );
    static_assert (  is_transparent<std::logical_not<>>::value, "" );

    return 0;
}
