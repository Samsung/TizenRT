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

#ifndef __PRIVATE_CONSTRUCTOR__H
#define __PRIVATE_CONSTRUCTOR__H

#include <iostream>

struct PrivateConstructor {

    PrivateConstructor static make ( int v ) { return PrivateConstructor(v); }
    int get () const { return val; }
private:
    PrivateConstructor ( int v ) : val(v) {}
    int val;
    };


//declare as inline function to prevent linking error (multiple definition error)
inline bool operator < ( const PrivateConstructor &lhs, const PrivateConstructor &rhs ) { return lhs.get() < rhs.get(); }

inline bool operator < ( const PrivateConstructor &lhs, int rhs ) { return lhs.get() < rhs; }
inline bool operator < ( int lhs, const PrivateConstructor &rhs ) { return lhs < rhs.get(); }

inline std::ostream & operator << ( std::ostream &os, const PrivateConstructor &foo ) { return os << foo.get (); }

#endif
