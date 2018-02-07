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
//===------------------------ stdexcept.cpp -------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "stdexcept"
#include "new"
#include "string"
#include "system_error"
#include "__refstring"

/* For _LIBCPPABI_VERSION */
#if defined(LIBCXX_BUILDING_LIBCXXABI) || defined(__APPLE__) || defined(LIBCXXRT)
#include <cxxabi.h>
#endif

static_assert(sizeof(std::__libcpp_refstring) == sizeof(const char *), "");


namespace std  // purposefully not using versioning namespace
{

logic_error::logic_error(const string& msg) : __imp_(msg.c_str())
{
}

logic_error::logic_error(const char* msg) : __imp_(msg)
{
}

logic_error::logic_error(const logic_error& le) _NOEXCEPT : __imp_(le.__imp_)
{
}

logic_error&
logic_error::operator=(const logic_error& le) _NOEXCEPT
{
    __imp_ = le.__imp_;
    return *this;
}

#if !defined(_LIBCPPABI_VERSION) && !defined(LIBSTDCXX)

logic_error::~logic_error() _NOEXCEPT
{
}

const char*
logic_error::what() const _NOEXCEPT
{
    return __imp_.c_str();
}

#endif

runtime_error::runtime_error(const string& msg) : __imp_(msg.c_str())
{
}

runtime_error::runtime_error(const char* msg) : __imp_(msg)
{
}

runtime_error::runtime_error(const runtime_error& le) _NOEXCEPT
  : __imp_(le.__imp_)
{
}

runtime_error&
runtime_error::operator=(const runtime_error& le) _NOEXCEPT
{
    __imp_ = le.__imp_;
    return *this;
}

#if !defined(_LIBCPPABI_VERSION) && !defined(LIBSTDCXX)

runtime_error::~runtime_error() _NOEXCEPT
{
}

const char*
runtime_error::what() const _NOEXCEPT
{
    return __imp_.c_str();
}

domain_error::~domain_error() _NOEXCEPT {}
invalid_argument::~invalid_argument() _NOEXCEPT {}
length_error::~length_error() _NOEXCEPT {}
out_of_range::~out_of_range() _NOEXCEPT {}

range_error::~range_error() _NOEXCEPT {}
overflow_error::~overflow_error() _NOEXCEPT {}
underflow_error::~underflow_error() _NOEXCEPT {}

#endif

}  // std
