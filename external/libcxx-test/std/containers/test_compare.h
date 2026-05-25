//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef TEST_COMPARE_H
#define TEST_COMPARE_H

template <class T>
struct test_equal_to
{
    int data_;
    explicit test_equal_to() : data_(0) {}
    explicit test_equal_to(int data) : data_(data) {}
    bool operator()(const T& a, const T& b) const
        { return a == b; }
    friend bool operator==(const test_equal_to& a, const test_equal_to& b)
        { return a.data_ == b.data_; }
};

template <class T>
struct test_less
{
    int data_;
    explicit test_less() : data_(0) {}
    explicit test_less(int data) : data_(data) {}
    bool operator()(const T& a, const T& b) const
        { return a < b; }
    friend bool operator==(const test_less& a, const test_less& b)
        { return a.data_ == b.data_; }
};

template <class T>
struct test_compare
{
    int data_;
    explicit test_compare() : data_(0) {}
    explicit test_compare(int data) : data_(data) {}
    template <class K1, class K2>
    bool operator()(const K1& a, const K2& b) const
        { return T()(a, b); }   
    friend bool operator==(const test_compare& a, const test_compare& b)
        { return a.data_ == b.data_; }
};

#endif // TEST_COMPARE_H
