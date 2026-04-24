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
// UNSUPPORTED: c++03

// <functional>

// template<CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);
// template<Returnable R, CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);

// https://llvm.org/PR22003

#include <functional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct DummyUnaryFunction
{
    template <typename S>
    int operator()(S const &) const { return 0; }
};

struct BadUnaryFunction
{
    template <typename S>
    constexpr int operator()(S const &) const
    {
        // Trigger a compile error if this function is instantiated.
        // The constexpr is needed so that it is instantiated while checking
        // __invoke_of<BadUnaryFunction &, ...>.
        static_assert(!std::is_same<S, S>::value, "Shit");
        return 0;
    }
};

int tc_utilities_function_objects_bind_func_bind_func_bind_bind_invoke_function_object(void) {
    // Check that BadUnaryFunction::operator()(S const &) is not
    // instantiated when checking if BadUnaryFunction is a nested bind
    // expression during b(0). See PR22003.
    auto b = std::bind(DummyUnaryFunction(), BadUnaryFunction());
    b(0);
    auto b2 = std::bind<long>(DummyUnaryFunction(), BadUnaryFunction());
    b2(0);

  return 0;
}
