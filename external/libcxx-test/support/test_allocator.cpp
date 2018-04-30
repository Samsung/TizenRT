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

#include "test_allocator.h"
#include <private_constructor.hpp>
#include "DefaultOnly.h"
#include "Counter.h"

int test_alloc_base::count = 0;
int test_alloc_base::time_to_throw = 0;
int test_alloc_base::alloc_count = 0;
int test_alloc_base::throw_after = INT_MAX;


bool operator < ( const PrivateConstructor &lhs, const PrivateConstructor &rhs ) { return lhs.get() < rhs.get(); }

bool operator < ( const PrivateConstructor &lhs, int rhs ) { return lhs.get() < rhs; }
bool operator < ( int lhs, const PrivateConstructor &rhs ) { return lhs < rhs.get(); }

std::ostream & operator << ( std::ostream &os, const PrivateConstructor &foo ) { return os << foo.get (); }

int DefaultOnly::count = 0;
int Counter_base::gConstructed = 0;
