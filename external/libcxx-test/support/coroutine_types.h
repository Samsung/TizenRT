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

#ifndef SUPPORT_COROUTINE_TYPES_H
#define SUPPORT_COROUTINE_TYPES_H

#include <experimental/coroutine>

template <typename Ty> struct generator {
  struct promise_type {
    Ty current_value;
    std::experimental::suspend_always yield_value(Ty value) {
      this->current_value = value;
      return {};
    }
    std::experimental::suspend_always initial_suspend() { return {}; }
    std::experimental::suspend_always final_suspend() { return {}; }
    generator get_return_object() { return generator{this}; };
    void return_void() {}
    void unhandled_exception() {}
  };

  struct iterator {
    std::experimental::coroutine_handle<promise_type> _Coro;
    bool _Done;

    iterator(std::experimental::coroutine_handle<promise_type> Coro, bool Done)
        : _Coro(Coro), _Done(Done) {}

    iterator &operator++() {
      _Coro.resume();
      _Done = _Coro.done();
      return *this;
    }

    bool operator==(iterator const &_Right) const {
      return _Done == _Right._Done;
    }

    bool operator!=(iterator const &_Right) const { return !(*this == _Right); }

    Ty const &operator*() const { return _Coro.promise().current_value; }

    Ty const *operator->() const { return &(operator*()); }
  };

  iterator begin() {
    p.resume();
    return {p, p.done()};
  }

  iterator end() { return {p, true}; }

  generator(generator &&rhs) : p(rhs.p) { rhs.p = nullptr; }

  ~generator() {
    if (p)
      p.destroy();
  }

private:
  explicit generator(promise_type *p)
      : p(std::experimental::coroutine_handle<promise_type>::from_promise(*p)) {}

  std::experimental::coroutine_handle<promise_type> p;
};

#endif // SUPPORT_COROUTINE_TYPES_H
