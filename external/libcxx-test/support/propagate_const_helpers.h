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
 // A lightweight class, with pointer-like methods, that contains an int
struct X
{
    int i_;

    constexpr const int &operator*() const { return i_; }
    constexpr int &operator*() { return i_; }
    constexpr const int *get() const { return &i_; }
    constexpr int *get() { return &i_; }
    constexpr const int *operator->() const { return &i_; }
    constexpr int *operator->() { return &i_; }

    constexpr X(int i) : i_(i) {}
};

struct XWithImplicitIntStarConversion
{
    int i_;

    constexpr const int &operator*() const { return i_; }
    constexpr int &operator*() { return i_; }
    constexpr const int *get() const { return &i_; }
    constexpr int *get() { return &i_; }
    constexpr const int *operator->() const { return &i_; }
    constexpr int *operator->() { return &i_; }
    constexpr operator int* () { return &i_; }

    constexpr XWithImplicitIntStarConversion(int i) : i_(i) {}
};

struct XWithImplicitConstIntStarConversion
{
    int i_;

    constexpr const int &operator*() const { return i_; }
    constexpr int &operator*() { return i_; }
    constexpr const int *get() const { return &i_; }
    constexpr int *get() { return &i_; }
    constexpr const int *operator->() const { return &i_; }
    constexpr int *operator->() { return &i_; }
    constexpr operator const int* () const { return &i_; }

    constexpr XWithImplicitConstIntStarConversion(int i) : i_(i) {}
};

struct ExplicitX
{
    int i_;

    constexpr const int &operator*() const { return i_; }
    constexpr int &operator*() { return i_; }
    constexpr const int *get() const { return &i_; }
    constexpr int *get() { return &i_; }
    constexpr const int *operator->() const { return &i_; }
    constexpr int *operator->() { return &i_; }

    constexpr explicit ExplicitX(int i) : i_(i) {}
};

struct MoveConstructibleFromX
{
    int i_;

    constexpr const int &operator*() const { return i_; }
    constexpr int &operator*() { return i_; }
    constexpr const int *get() const { return &i_; }
    constexpr int *get() { return &i_; }
    constexpr const int *operator->() const { return &i_; }
    constexpr int *operator->() { return &i_; }

    constexpr MoveConstructibleFromX(int i) : i_(i) {}
    constexpr MoveConstructibleFromX(X&& x) : i_(x.i_) {}
};

struct ExplicitMoveConstructibleFromX
{
    int i_;

    constexpr const int &operator*() const { return i_; }
    constexpr int &operator*() { return i_; }
    constexpr const int *get() const { return &i_; }
    constexpr int *get() { return &i_; }
    constexpr const int *operator->() const { return &i_; }
    constexpr int *operator->() { return &i_; }

    constexpr ExplicitMoveConstructibleFromX(int i) : i_(i) {}
    constexpr explicit ExplicitMoveConstructibleFromX(X&& x) : i_(x.i_) {}
};

struct CopyConstructibleFromX
{
    int i_;

    constexpr const int &operator*() const { return i_; }
    constexpr int &operator*() { return i_; }
    constexpr const int *get() const { return &i_; }
    constexpr int *get() { return &i_; }
    constexpr const int *operator->() const { return &i_; }
    constexpr int *operator->() { return &i_; }

    constexpr CopyConstructibleFromX(int i) : i_(i) {}
    constexpr CopyConstructibleFromX(const X& x) : i_(x.i_) {}
};

struct ExplicitCopyConstructibleFromX
{
    int i_;

    constexpr const int &operator*() const { return i_; }
    constexpr int &operator*() { return i_; }
    constexpr const int *get() const { return &i_; }
    constexpr int *get() { return &i_; }
    constexpr const int *operator->() const { return &i_; }
    constexpr int *operator->() { return &i_; }

    constexpr ExplicitCopyConstructibleFromX(int i) : i_(i) {}
    constexpr explicit ExplicitCopyConstructibleFromX(const X& x) : i_(x.i_) {}
};

