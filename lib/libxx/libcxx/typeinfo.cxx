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
//===------------------------- typeinfo.cpp -------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <stdlib.h>

#if defined(__APPLE__) || defined(LIBCXXRT) ||                                 \
    defined(LIBCXX_BUILDING_LIBCXXABI)
#include <cxxabi.h>
#endif

#include "typeinfo"

#if !defined(LIBCXXRT) && !defined(_LIBCPPABI_VERSION)

std::bad_cast::bad_cast() _NOEXCEPT
{
}

std::bad_typeid::bad_typeid() _NOEXCEPT
{
}

#ifndef __GLIBCXX__

std::bad_cast::~bad_cast() _NOEXCEPT
{
}

const char*
std::bad_cast::what() const _NOEXCEPT
{
  return "std::bad_cast";
}

std::bad_typeid::~bad_typeid() _NOEXCEPT
{
}

const char*
std::bad_typeid::what() const _NOEXCEPT
{
  return "std::bad_typeid";
}

#ifdef __APPLE__
  // On Darwin, the cxa_bad_* functions cannot be in the lower level library
  // because bad_cast and bad_typeid are defined in his higher level library
  void __cxxabiv1::__cxa_bad_typeid()
  {
#ifndef _LIBCPP_NO_EXCEPTIONS
     throw std::bad_typeid();
#else
     _VSTD::abort();
#endif
  }
  void __cxxabiv1::__cxa_bad_cast()
  {
#ifndef _LIBCPP_NO_EXCEPTIONS
      throw std::bad_cast();
#else
      _VSTD::abort();
#endif
  }
#endif

#endif  // !__GLIBCXX__
#endif  // !LIBCXXRT && !_LIBCPPABI_VERSION
