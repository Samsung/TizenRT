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
// <functional>

#include <functional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Incomplete;
template<class T> struct Holder { T t; };
typedef Holder<Incomplete> *Ptr;

struct A {
    Ptr no_args() const { return nullptr; }
    Ptr one_arg(Ptr p) const { return p; }
    void one_arg_void(Ptr) const { }
};

int tc_utilities_function_objects_func_memfn_robust_against_adl(void) {
    A a;
    A *pa = &a;
    const A *cpa = &a;
    Ptr x = nullptr;
    const Ptr cx = nullptr;
    std::mem_fn(&A::no_args)(a);
    std::mem_fn(&A::no_args)(pa);
    std::mem_fn(&A::no_args)(*cpa);
    std::mem_fn(&A::no_args)(cpa);
    std::mem_fn(&A::one_arg)(a, x);
    std::mem_fn(&A::one_arg)(pa, x);
    std::mem_fn(&A::one_arg)(a, cx);
    std::mem_fn(&A::one_arg)(pa, cx);
    std::mem_fn(&A::one_arg)(*cpa, x);
    std::mem_fn(&A::one_arg)(cpa, x);
    std::mem_fn(&A::one_arg)(*cpa, cx);
    std::mem_fn(&A::one_arg)(cpa, cx);
    std::mem_fn(&A::one_arg_void)(a, x);
    std::mem_fn(&A::one_arg_void)(pa, x);
    std::mem_fn(&A::one_arg_void)(a, cx);
    std::mem_fn(&A::one_arg_void)(pa, cx);
    std::mem_fn(&A::one_arg_void)(*cpa, x);
    std::mem_fn(&A::one_arg_void)(cpa, x);
    std::mem_fn(&A::one_arg_void)(*cpa, cx);
    std::mem_fn(&A::one_arg_void)(cpa, cx);
    return 0;
}
