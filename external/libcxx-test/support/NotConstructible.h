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

#ifndef NOTCONSTRUCTIBLE_H
#define NOTCONSTRUCTIBLE_H

#include <functional>

class NotConstructible
{
    NotConstructible(const NotConstructible&);
    NotConstructible& operator=(const NotConstructible&);
public:
};

inline
bool
operator==(const NotConstructible&, const NotConstructible&)
{return true;}

namespace std
{

template <>
struct hash<NotConstructible>
{
    typedef NotConstructible argument_type;
    typedef std::size_t result_type;

    std::size_t operator()(const NotConstructible&) const {return 0;}
};

}

#endif  // NOTCONSTRUCTIBLE_H
