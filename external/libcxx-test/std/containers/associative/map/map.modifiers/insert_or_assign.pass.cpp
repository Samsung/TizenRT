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
//
// UNSUPPORTED: c++03, c++11, c++14

// <map>

// class map

// template <class M>
//  pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);            // C++17
// template <class M>
//  pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);                 // C++17
// template <class M>
//  iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);   // C++17
// template <class M>
//  iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);        // C++17

#include <map>
#include <cassert>
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

    int get() const {return int_;}
    bool moved() const {return int_ == -1;}
};


int tc_containers_associative_map_map_modifiers_insert_or_assign(void) {
    { // pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
        typedef std::map<int, Moveable> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r;
        for ( int i = 0; i < 20; i += 2 )
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
        typedef std::map<Moveable, Moveable> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r;
        for ( int i = 0; i < 20; i += 2 )
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
        typedef std::map<int, Moveable> M;
        M m;
        M::iterator r;
        for ( int i = 0; i < 20; i += 2 )
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

        // wrong hint: begin()
        Moveable mv3(7, 7.0);
        r = m.insert_or_assign(m.begin(), 4, std::move(mv3));
        TC_ASSERT_EXPR(m.size() == 11);
        TC_ASSERT_EXPR(mv3.moved());           // was moved from
        TC_ASSERT_EXPR(r->first        == 4);  // key
        TC_ASSERT_EXPR(r->second.get() == 7);  // value

        Moveable mv4(9, 9.0);
        r = m.insert_or_assign(m.begin(), 5, std::move(mv4));
        TC_ASSERT_EXPR(m.size() == 12);
        TC_ASSERT_EXPR(mv4.moved());           // was moved from
        TC_ASSERT_EXPR(r->first        == 5);  // key
        TC_ASSERT_EXPR(r->second.get() == 9);  // value

        // wrong hint: end()
        Moveable mv5(11, 11.0);
        r = m.insert_or_assign(m.end(), 6, std::move(mv5));
        TC_ASSERT_EXPR(m.size() == 12);
        TC_ASSERT_EXPR(mv5.moved());           // was moved from
        TC_ASSERT_EXPR(r->first        == 6);  // key
        TC_ASSERT_EXPR(r->second.get() == 11); // value

        Moveable mv6(13, 13.0);
        r = m.insert_or_assign(m.end(), 7, std::move(mv6));
        TC_ASSERT_EXPR(m.size() == 13);
        TC_ASSERT_EXPR(mv6.moved());           // was moved from
        TC_ASSERT_EXPR(r->first        == 7);  // key
        TC_ASSERT_EXPR(r->second.get() == 13); // value

        // wrong hint: third element
        Moveable mv7(15, 15.0);
        r = m.insert_or_assign(std::next(m.begin(), 2), 8, std::move(mv7));
        TC_ASSERT_EXPR(m.size() == 13);
        TC_ASSERT_EXPR(mv7.moved());           // was moved from
        TC_ASSERT_EXPR(r->first        == 8);  // key
        TC_ASSERT_EXPR(r->second.get() == 15); // value

        Moveable mv8(17, 17.0);
        r = m.insert_or_assign(std::next(m.begin(), 2), 9, std::move(mv8));
        TC_ASSERT_EXPR(m.size() == 14);
        TC_ASSERT_EXPR(mv8.moved());           // was moved from
        TC_ASSERT_EXPR(r->first        == 9);  // key
        TC_ASSERT_EXPR(r->second.get() == 17); // value
    }
    { // iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);
        typedef std::map<Moveable, Moveable> M;
        M m;
        M::iterator r;
        for ( int i = 0; i < 20; i += 2 )
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

        // wrong hint: begin()
        Moveable mvkey3(6, 6.0);
        Moveable mv3(8, 8.0);
        r = m.insert_or_assign(m.begin(), std::move(mvkey3), std::move(mv3));
        TC_ASSERT_EXPR(m.size() == 11);
        TC_ASSERT_EXPR(mv3.moved());           // was moved from
        TC_ASSERT_EXPR(!mvkey3.moved());       // was not moved from
        TC_ASSERT_EXPR(r->first == mvkey3);    // key
        TC_ASSERT_EXPR(r->second.get() == 8);  // value

        Moveable mvkey4(7, 7.0);
        Moveable mv4(9, 9.0);
        r = m.insert_or_assign(m.begin(), std::move(mvkey4), std::move(mv4));
        TC_ASSERT_EXPR(m.size() == 12);
        TC_ASSERT_EXPR(mv4.moved());           // was moved from
        TC_ASSERT_EXPR(mvkey4.moved());        // was moved from
        TC_ASSERT_EXPR(r->first.get()  == 7);  // key
        TC_ASSERT_EXPR(r->second.get() == 9);  // value

        // wrong hint: end()
        Moveable mvkey5(8, 8.0);
        Moveable mv5(10, 10.0);
        r = m.insert_or_assign(m.end(), std::move(mvkey5), std::move(mv5));
        TC_ASSERT_EXPR(m.size() == 12);
        TC_ASSERT_EXPR(mv5.moved());           // was moved from
        TC_ASSERT_EXPR(!mvkey5.moved());       // was not moved from
        TC_ASSERT_EXPR(r->first == mvkey5);    // key
        TC_ASSERT_EXPR(r->second.get() == 10); // value

        Moveable mvkey6(9, 9.0);
        Moveable mv6(11, 11.0);
        r = m.insert_or_assign(m.end(), std::move(mvkey6), std::move(mv6));
        TC_ASSERT_EXPR(m.size() == 13);
        TC_ASSERT_EXPR(mv6.moved());           // was moved from
        TC_ASSERT_EXPR(mvkey6.moved());        // was moved from
        TC_ASSERT_EXPR(r->first.get()  == 9);  // key
        TC_ASSERT_EXPR(r->second.get() == 11); // value

        // wrong hint: third element
        Moveable mvkey7(10, 10.0);
        Moveable mv7(12, 12.0);
        r = m.insert_or_assign(std::next(m.begin(), 2), std::move(mvkey7), std::move(mv7));
        TC_ASSERT_EXPR(m.size() == 13);
        TC_ASSERT_EXPR(mv7.moved());           // was moved from
        TC_ASSERT_EXPR(!mvkey7.moved());       // was not moved from
        TC_ASSERT_EXPR(r->first == mvkey7);    // key
        TC_ASSERT_EXPR(r->second.get() == 12); // value

        Moveable mvkey8(11, 11.0);
        Moveable mv8(13, 13.0);
        r = m.insert_or_assign(std::next(m.begin(), 2), std::move(mvkey8), std::move(mv8));
        TC_ASSERT_EXPR(m.size() == 14);
        TC_ASSERT_EXPR(mv8.moved());           // was moved from
        TC_ASSERT_EXPR(mvkey8.moved());        // was moved from
        TC_ASSERT_EXPR(r->first.get()  == 11); // key
        TC_ASSERT_EXPR(r->second.get() == 13); // value
    }

  return 0;
}
