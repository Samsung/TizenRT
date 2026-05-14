//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++03, c++11, c++14

// <unordered_map>

// class unordered_map

// template <class M>
//  pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);            // C++17
// template <class M>
//  pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);                 // C++17
// template <class M>
//  iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);   // C++17
// template <class M>
//  iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);        // C++17

#include <unordered_map>
#include <cassert>
#include <iterator>
#include <tuple>

#include "test_macros.h"
#include "libcxx_tc_common.h"


class Moveable
{
    Moveable(const Moveable&);
    Moveable& operator=(const Moveable&);

    int int_;
    double double_;
public:
    Moveable() : int_(0), double_(0) {}
    Moveable(int i, double d) : int_(i), double_(d) {}
    Moveable(Moveable&& x)
        : int_(x.int_), double_(x.double_)
            {x.int_ = -1; x.double_ = -1;}
    Moveable& operator=(Moveable&& x)
        {int_ = x.int_; x.int_ = -1;
         double_ = x.double_; x.double_ = -1;
         return *this;
        }

    bool operator==(const Moveable& x) const
        {return int_ == x.int_ && double_ == x.double_;}
    bool operator<(const Moveable& x) const
        {return int_ < x.int_ || (int_ == x.int_ && double_ < x.double_);}
    std::size_t hash () const { return std::hash<int>()(int_) + std::hash<double>()(double_); }

    int get() const {return int_;}
    bool moved() const {return int_ == -1;}
};

namespace std {
    template <> struct hash<Moveable> {
        std::size_t operator () (const Moveable &m) const { return m.hash(); }
    };
}

int tc_containers_unord_unord_map_unord_map_modifiers_insert_or_assign(void) {

    { // pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
        typedef std::unordered_map<int, Moveable> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r;
        for (int i = 0; i < 20; i += 2)
            m.emplace ( i, Moveable(i, (double) i));
        TC_ASSERT_EXPR(m.size() == 10);

        for (int i=0; i < 20; i += 2)
        {
            Moveable mv(i+1, i+1);
            r = m.insert_or_assign(i, std::move(mv));
            TC_ASSERT_EXPR(m.size() == 10);
            TC_ASSERT_EXPR(!r.second);                    // was not inserted
            TC_ASSERT_EXPR(mv.moved());                   // was moved from
            TC_ASSERT_EXPR(r.first->first == i);          // key
            TC_ASSERT_EXPR(r.first->second.get() == i+1); // value
        }

        Moveable mv1(5, 5.0);
        r = m.insert_or_assign(-1, std::move(mv1));
        TC_ASSERT_EXPR(m.size() == 11);
        TC_ASSERT_EXPR(r.second);                    // was inserted
        TC_ASSERT_EXPR(mv1.moved());                 // was moved from
        TC_ASSERT_EXPR(r.first->first        == -1); // key
        TC_ASSERT_EXPR(r.first->second.get() == 5);  // value

        Moveable mv2(9, 9.0);
        r = m.insert_or_assign(3, std::move(mv2));
        TC_ASSERT_EXPR(m.size() == 12);
        TC_ASSERT_EXPR(r.second);                   // was inserted
        TC_ASSERT_EXPR(mv2.moved());                // was moved from
        TC_ASSERT_EXPR(r.first->first        == 3); // key
        TC_ASSERT_EXPR(r.first->second.get() == 9); // value

        Moveable mv3(-1, 5.0);
        r = m.insert_or_assign(117, std::move(mv3));
        TC_ASSERT_EXPR(m.size() == 13);
        TC_ASSERT_EXPR(r.second);                     // was inserted
        TC_ASSERT_EXPR(mv3.moved());                  // was moved from
        TC_ASSERT_EXPR(r.first->first        == 117); // key
        TC_ASSERT_EXPR(r.first->second.get() == -1);  // value
    }
    { // pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);
        typedef std::unordered_map<Moveable, Moveable> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r;
        for (int i = 0; i < 20; i += 2)
            m.emplace ( Moveable(i, (double) i), Moveable(i+1, (double) i+1));
        TC_ASSERT_EXPR(m.size() == 10);

        Moveable mvkey1(2, 2.0);
        Moveable mv1(4, 4.0);
        r = m.insert_or_assign(std::move(mvkey1), std::move(mv1));
        TC_ASSERT_EXPR(m.size() == 10);
        TC_ASSERT_EXPR(!r.second);                  // was not inserted
        TC_ASSERT_EXPR(!mvkey1.moved());            // was not moved from
        TC_ASSERT_EXPR(mv1.moved());                // was moved from
        TC_ASSERT_EXPR(r.first->first == mvkey1);   // key
        TC_ASSERT_EXPR(r.first->second.get() == 4); // value

        Moveable mvkey2(3, 3.0);
        Moveable mv2(5, 5.0);
        r = m.try_emplace(std::move(mvkey2), std::move(mv2));
        TC_ASSERT_EXPR(m.size() == 11);
        TC_ASSERT_EXPR(r.second);                   // was inserted
        TC_ASSERT_EXPR(mv2.moved());                // was moved from
        TC_ASSERT_EXPR(mvkey2.moved());             // was moved from
        TC_ASSERT_EXPR(r.first->first.get()  == 3); // key
        TC_ASSERT_EXPR(r.first->second.get() == 5); // value
    }
    { // iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
        typedef std::unordered_map<int, Moveable> M;
        M m;
        M::iterator r;
        for (int i = 0; i < 20; i += 2)
            m.emplace ( i, Moveable(i, (double) i));
        TC_ASSERT_EXPR(m.size() == 10);
        M::const_iterator it = m.find(2);

        Moveable mv1(3, 3.0);
        r = m.insert_or_assign(it, 2, std::move(mv1));
        TC_ASSERT_EXPR(m.size() == 10);
        TC_ASSERT_EXPR(mv1.moved());           // was moved from
        TC_ASSERT_EXPR(r->first        == 2);  // key
        TC_ASSERT_EXPR(r->second.get() == 3);  // value

        Moveable mv2(5, 5.0);
        r = m.insert_or_assign(it, 3, std::move(mv2));
        TC_ASSERT_EXPR(m.size() == 11);
        TC_ASSERT_EXPR(mv2.moved());           // was moved from
        TC_ASSERT_EXPR(r->first        == 3);  // key
        TC_ASSERT_EXPR(r->second.get() == 5);  // value
    }
    { // iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);
        typedef std::unordered_map<Moveable, Moveable> M;
        M m;
        M::iterator r;
        for (int i = 0; i < 20; i += 2)
            m.emplace ( Moveable(i, (double) i), Moveable(i+1, (double) i+1));
        TC_ASSERT_EXPR(m.size() == 10);
        M::const_iterator it = std::next(m.cbegin());

        Moveable mvkey1(2, 2.0);
        Moveable mv1(4, 4.0);
        r = m.insert_or_assign(it, std::move(mvkey1), std::move(mv1));
        TC_ASSERT_EXPR(m.size() == 10);
        TC_ASSERT_EXPR(mv1.moved());          // was moved from
        TC_ASSERT_EXPR(!mvkey1.moved());      // was not moved from
        TC_ASSERT_EXPR(r->first == mvkey1);   // key
        TC_ASSERT_EXPR(r->second.get() == 4); // value

        Moveable mvkey2(3, 3.0);
        Moveable mv2(5, 5.0);
        r = m.insert_or_assign(it, std::move(mvkey2), std::move(mv2));
        TC_ASSERT_EXPR(m.size() == 11);
        TC_ASSERT_EXPR(mv2.moved());           // was moved from
        TC_ASSERT_EXPR(mvkey2.moved());        // was moved from
        TC_ASSERT_EXPR(r->first.get()  == 3);  // key
        TC_ASSERT_EXPR(r->second.get() == 5);  // value
    }


  return 0;
}
