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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef SUPPORT_USER_DEFINED_INTEGRAL_HPP
#define SUPPORT_USER_DEFINED_INTEGRAL_HPP

template <class T>
struct UserDefinedIntegral
{
    UserDefinedIntegral() : value(0) {}
    UserDefinedIntegral(T v) : value(v) {}
    operator T() const { return value; }
    T value;
};

// Poison the arithmetic and comparison operations
template <class T, class U>
void operator+(UserDefinedIntegral<T>, UserDefinedIntegral<U>);

template <class T, class U>
void operator-(UserDefinedIntegral<T>, UserDefinedIntegral<U>);

template <class T, class U>
void operator*(UserDefinedIntegral<T>, UserDefinedIntegral<U>);

template <class T, class U>
void operator/(UserDefinedIntegral<T>, UserDefinedIntegral<U>);

template <class T, class U>
void operator==(UserDefinedIntegral<T>, UserDefinedIntegral<U>);

template <class T, class U>
void operator!=(UserDefinedIntegral<T>, UserDefinedIntegral<U>);

template <class T, class U>
void operator<(UserDefinedIntegral<T>, UserDefinedIntegral<U>);

template <class T, class U>
void operator>(UserDefinedIntegral<T>, UserDefinedIntegral<U>);

template <class T, class U>
void operator<=(UserDefinedIntegral<T>, UserDefinedIntegral<U>);

template <class T, class U>
void operator>=(UserDefinedIntegral<T>, UserDefinedIntegral<U>);

#endif // SUPPORT_USER_DEFINED_INTEGRAL_HPP
