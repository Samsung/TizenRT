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
// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef SUPPORT_DEMANGLE_H
#define SUPPORT_DEMANGLE_H

#include "test_macros.h"
#include <string>
#include <cstdlib>

#if !defined(TEST_HAS_NO_DEMANGLE)
# if defined(__GNUC__) || defined(__clang__)
#   if __has_include("cxxabi.h") && !defined(_LIBCPP_ABI_MICROSOFT)
#     include "cxxabi.h"
#   else
#     define TEST_HAS_NO_DEMANGLE
#   endif
# else
#   define TEST_HAS_NO_DEMANGLE
# endif
#endif

#if defined(TEST_HAS_NO_DEMANGLE)
inline std::string demangle(const char* mangled_name) {
  return mangled_name;
}
#else
template <size_t N> struct Printer;
inline std::string demangle(const char* mangled_name) {
  int status = 0;
  char* out = __cxxabiv1::__cxa_demangle(mangled_name, nullptr, nullptr, &status);
  if (out != nullptr) {
    std::string res(out);
    std::free(out);
    return res;
  }
  return mangled_name;
}
#endif

#endif // SUPPORT_DEMANGLE_H
