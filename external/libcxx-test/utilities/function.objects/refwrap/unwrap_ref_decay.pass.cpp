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
//
// template <class T>
// struct unwrap_ref_decay;
//
// template <class T>
// using unwrap_ref_decay_t = typename unwrap_ref_decay<T>::type;

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <functional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"


template <typename T, typename Result>
void check() {
  static_assert(std::is_same_v<typename std::unwrap_ref_decay<T>::type, Result>);
  static_assert(std::is_same_v<typename std::unwrap_ref_decay<T>::type, std::unwrap_ref_decay_t<T>>);
}

struct T { };

int tc_utilities_function_objects_refwrap_unwrap_ref_decay(void) {
  check<T,             T>();
  check<T&,            T>();
  check<T const,       T>();
  check<T const&,      T>();
  check<T*,            T*>();
  check<T const*,      T const*>();
  check<T[3],          T*>();
  check<T const [3],   T const*>();
  check<T (),          T (*)()>();
  check<T (int) const, T (int) const>();
  check<T (int) &,     T (int) &>();
  check<T (int) &&,    T (int) &&>();

  check<std::reference_wrapper<T>,         T&>();
  check<std::reference_wrapper<T>&,        T&>();
  check<std::reference_wrapper<T const>,   T const&>();
  check<std::reference_wrapper<T const>&,  T const&>();
  check<std::reference_wrapper<T*>,        T*&>();
  check<std::reference_wrapper<T*>&,       T*&>();
  check<std::reference_wrapper<T const*>,  T const*&>();
  check<std::reference_wrapper<T const*>&, T const*&>();
  check<std::reference_wrapper<T[3]>,      T (&)[3]>();
  check<std::reference_wrapper<T[3]>&,     T (&)[3]>();
  check<std::reference_wrapper<T ()>,      T (&)()>();
  check<std::reference_wrapper<T ()>&,     T (&)()>();

  return 0;
}
