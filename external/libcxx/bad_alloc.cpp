//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <new>
#include <__verbose_abort>

namespace std {

__attribute__((__noreturn__))
void __throw_bad_alloc()
{
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
    throw bad_alloc();
#else
    _LIBCPP_VERBOSE_ABORT("bad allocation");
#endif
}

} // namespace std