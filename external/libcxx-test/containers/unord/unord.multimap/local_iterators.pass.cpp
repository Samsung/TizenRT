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
// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multimap

// local_iterator       begin (size_type n);
// local_iterator       end   (size_type n);
// const_local_iterator begin (size_type n) const;
// const_local_iterator end   (size_type n) const;
// const_local_iterator cbegin(size_type n) const;
// const_local_iterator cend  (size_type n) const;

#include <unordered_map>
#include <string>
#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multimap_local_iterators(void) {
    {
        typedef std::unordered_multimap<int, std::string> C;
        typedef std::pair<int, std::string> P;
        typedef C::local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        C::size_type b = c.bucket(0);
        I i = c.begin(b);
        I j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("one");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 1);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("two");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 2);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");

        b = c.bucket(5);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(6);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
    }
    {
        typedef std::unordered_multimap<int, std::string> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        C::size_type b = c.bucket(0);
        I i = c.begin(b);
        I j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("one");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 1);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("two");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 2);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");

        b = c.bucket(5);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(6);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
    }
    {
        typedef std::unordered_multimap<int, std::string> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        C::size_type b = c.bucket(0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("one");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 1);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("two");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 2);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");

        b = c.bucket(5);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(6);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
    }
    {
        typedef std::unordered_multimap<int, std::string> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        C::size_type b = c.bucket(0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("one");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 1);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("two");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 2);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");

        b = c.bucket(5);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(6);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_multimap<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        typedef C::local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        C::size_type b = c.bucket(0);
        I i = c.begin(b);
        I j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("one");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 1);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("two");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 2);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");

        b = c.bucket(5);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(6);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
    }
    {
        typedef std::unordered_multimap<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        C::size_type b = c.bucket(0);
        I i = c.begin(b);
        I j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("one");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 1);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("two");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 2);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");

        b = c.bucket(5);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(6);
        i = c.begin(b);
        j = c.end(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
    }
    {
        typedef std::unordered_multimap<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        C::size_type b = c.bucket(0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("one");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 1);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("two");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 2);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");

        b = c.bucket(5);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(6);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
    }
    {
        typedef std::unordered_multimap<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        C::size_type b = c.bucket(0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("one");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 1);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 2);
        {
            std::set<std::string> s;
            s.insert("two");
            s.insert("four");
            for ( int n = 0; n < 2; ++n )
            {
                TC_ASSERT_EXPR(i->first == 2);
                TC_ASSERT_EXPR(s.find(i->second) != s.end());
                s.erase(s.find(i->second));
                ++i;
            }
        }

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 1);
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");

        b = c.bucket(5);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);

        b = c.bucket(6);
        i = c.cbegin(b);
        j = c.cend(b);
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
    }
#endif

  return 0;
}
