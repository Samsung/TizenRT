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
//***************************************************************************
// examples/cxxtest/cxxtest_main.cxx
//
//   Copyright (C) 2012, 2017 Gregory Nutt. All rights reserved.
//   Author: Qiang Yu, http://rgmp.sourceforge.net/wiki/index.php/Main_Page
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
// 3. Neither the name NuttX nor the names of its contributors may be
//    used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
// AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//***************************************************************************

//***************************************************************************
// Included Files
//***************************************************************************

#include <tinyara/config.h>

#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <stdexcept>
#include <cassert>
#include <functional>
#include <tuple>
#include <algorithm>
#include <list>

#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include <tinyara/init.h>
#include <apps/platform/cxxinitialize.h>

using namespace std;

//***************************************************************************
// Definitions
//***************************************************************************
// Configuration ************************************************************
// C++ initialization requires CXX initializer support

#if !defined(CONFIG_HAVE_CXX) || !defined(CONFIG_HAVE_CXXINITIALIZE)
#  warning Support for static initializers is NOT enabled
#  undef CONFIG_EXAMPLES_CXXTEST_CXXINITIALIZE
#endif

#define CXXTEST_RTTI
#undef CXXTEST_ISTREAM
#define CXXTEST_EXCEPTION
#define THREAD_TEST

//***************************************************************************
// Private Classes
//***************************************************************************

#ifdef CXXTEST_RTTI
class Base
{
public:
	virtual void printBase(void) {};
	virtual ~Base() {};
};

class Extend : public Base
{
public:
	void printExtend(void)
	{
		cout << "extend" << endl;
	}
};
#endif

//***************************************************************************
// Private Data
//***************************************************************************

//***************************************************************************
// Private Functions
//***************************************************************************
struct Foo
{
	Foo(int num) : num_(num) {}
	void print_add(int i) const
	{
		printf("%d\n", num_+i);
	}
	int num_;
};

static void print_num(int i)
{
	printf("%d\n", i);
}

struct PrintNum
{
	void operator()(int i) const
	{
		printf("%d\n", i);
	}
};

static void test_function(void)
{
	cout << endl << "============ Test STL(Function Templates) ===============" << endl;
	// store a free function
	function<void(int)> f_display = print_num;
	f_display(-9);

	// store a lambda
	function<void()> f_display_42 = []()
	{
		print_num(42);
	};
	f_display_42();

	// store the result of a call to bind
	function<void()> f_display_31337 = bind(print_num, 31337);
	f_display_31337();

	// store a call to a member function
	function<void(const Foo&, int)> f_add_display = &Foo::print_add;
	const Foo foo(314159);
	f_add_display(foo, 1);

	// store a call to a data member accessor
	function<int(Foo const&)> f_num = &Foo::num_;
	printf("num_: %d\n", f_num(foo));

	// store a call to a member function and object
	using placeholders::_1;
	function<void(int)> f_add_display2 = bind( &Foo::print_add, foo, _1 );
	f_add_display2(2);

	// store a call to a member function and object ptr
	function<void(int)> f_add_display3 = bind( &Foo::print_add, &foo, _1 );
	f_add_display3(3);

	// store a call to a function object
	function<void(int)> f_display_obj = PrintNum();
	f_display_obj(18);
}

static void test_tuple(void)
{
	cout << endl << "============ Test STL(Tuple) ============================" << endl;
	using wcs_t= tuple<float, float, float>;
	wcs_t g92_offset = wcs_t(1.0F, 2.1F, 3.2F);
	float x=1.234f, y=2.345f, z=3.456f;
	tie(x, y, z) = g92_offset;
	printf("Tuple tie X%8.4f Y%8.4f Z%8.4f\n", x, y, z);
	printf("Tuple X%8.4f Y%8.4f Z%8.4f\n", get<0>(g92_offset),  get<1>(g92_offset), get<2>(g92_offset) );
}

static void test_array(void)
{
	cout << endl << "============ Test STL(Array) ============================" << endl;
	array<int, 4> ta {{1,2,3,4}};
	for(const auto& s: ta) printf("%d ", s);
	printf("\n");
	int n= 10;
	generate(ta.begin(), ta.end(), [&n] { return n++; });
	for(const auto& s: ta) printf("%d ", s);
	printf("\n");
}

static void test_list(void)
{
	cout << endl << "============ Test STL(List) =============================" << endl;
	// constructors used in the same order as described above:
	list<int> first;                                // empty list of ints
	list<int> second (4,100);                       // four ints with value 100
	list<int> third (second.begin(),second.end());  // iterating through second
	list<int> fourth (third);                       // a copy of third

	// the iterator constructor can also be used to construct from arrays:
	int myints[] = {16,2,77,29};
	list<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

	cout << "The contents of fifth are: ";
	for (list<int>::iterator it = fifth.begin(); it != fifth.end(); it++)
		cout << *it << ' ';

	cout << endl;
}


//***************************************************************************
// Name: test_ostream
//***************************************************************************/

static void test_ofstream(void)
{
	cout << endl << "============ Test ofstream ==============================" << endl;
	ofstream ttyOut;
	printf("printf: Starting test_ostream\n");
	ttyOut.open ("/dev/console");
	if (!ttyOut.good())
	{
		printf("printf: Failed opening /dev/console\n");
		cout << "cout: Failed opening /dev/console" << endl;
		cout << " good()=" << ttyOut.good();
		cout << " eof()=" << ttyOut.eof();
		cout << " fail()=" << ttyOut.fail();
		cout << " bad()=" << ttyOut.bad() << endl;
	}
	else
	{
		printf("printf: Successfully opened /dev/console\n");
		cout << "cout: Successfully opened /dev/console" << endl;
		ttyOut << "Writing this to /dev/console\n";
		ttyOut.close();
	}
}

//***************************************************************************
// Name: test_iostream
//***************************************************************************/

static void test_iostream(void)
{
	cout << endl << "============ Test iostream ==============================" << endl;
	cout << "Hello, this is only a test" << endl;
	cout << "Print an int: "  <<  190  <<  endl;
	cout <<  "Print a char: "  <<  'd'  <<  endl;

#ifdef CXXTEST_ISTREAM
	int a;
	string s;

	cout << "Please type in an int:" << endl;
	cin >> a;
	cout << "You type in: " << a << endl;
	cout << "Please type in a string:" << endl;
	cin >> s;
	cout << "You type in: " << s << endl;
#endif
}

//***************************************************************************
// Name: test_stl
//***************************************************************************/

static void test_vectors(void)
{
	cout << endl << "============ Test STL(Vectors) ==========================" << endl;
	vector<int> v1;
	assert(v1.empty());

	v1.push_back(1);
	assert(!v1.empty());

	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	assert(v1.size() == 4);

	v1.pop_back();
	assert(v1.size() == 3);

	cout << "v1=" << v1[0] << ' ' << v1[1] << ' ' << v1[2] << endl;
	assert(v1[2] == 3);

	vector<int> v2 = v1;
	assert(v2 == v1);

	string words[4] = {"Hello", "World", "Good", "Luck"};
	vector<string> v3(words, words + 4);
	vector<string>::iterator it;
	for (it = v3.begin(); it != v3.end(); ++it)
	{
		cout << *it << ' ';
	}

	cout << endl;
	assert(v3[1] == "World");
}

static void test_map(void)
{
	cout << endl << "============ Test STL(Map) ==============================" << endl;

	map<int,string> m1;
	m1[12] = "Hello";
	m1[24] = "World";
	assert(m1.size() == 2);
	assert(m1[24] == "World");
}

static void test_stl(void)
{
	test_array();
	test_vectors();
	test_map();
	test_list();
	test_tuple();
	test_function();
}

#ifdef CXXTEST_RTTI
//***************************************************************************
// Name: test_rtti
//***************************************************************************/

static void test_rtti(void)
{
	cout << endl << "============ Test RTTI ==================================" << endl;

	Base *a = new Base();
	Base *b = new Extend();
	assert(a);
	assert(b);

	Extend *t = dynamic_cast<Extend *>(a);
	assert(t == NULL);

	t = dynamic_cast<Extend *>(b);
	assert(t);
	t->printExtend();

	delete a;
	delete b;
}
#endif

//***************************************************************************
// Name: test_exception
//***************************************************************************/

#ifdef CXXTEST_EXCEPTION
static void test_exception(void)
{
	cout << endl << "============ Test Exception =============================" << endl;
	try
	{
		throw runtime_error("runtime error");
	}

	catch (runtime_error &e)
	{
		cout << "Catch exception: " << e.what() << endl;
	}
}
#endif

#ifdef THREAD_TEST

static void pause_thread(int n)
{
	std::this_thread::sleep_for (std::chrono::seconds(n));
	std::cout << "pause of " << n << " seconds ended" << std::endl;
}

static void bar_thread()
{
	// do stuff...
}

//***************************************************************************
// Name: thread_operator_test (thread::operator=)
//
// Expected output:
// ----------------------------------------------------
// Spawning 5 threads...
// Done spawning threads. Now waiting for them to join:
// pause of 1 seconds ended
// pause of 2 seconds ended
// pause of 3 seconds ended
// pause of 4 seconds ended
// pause of 5 seconds ended
// All threads joined!
//***************************************************************************

static int thread_operator_test(void)
{
	std::thread threads[5];						// default-constructed threads

	std::cout << "Spawning 5 threads..." << std::endl;
	for (int i = 0; i < 5; ++i)
		threads[i] = std::thread(pause_thread, i + 1);   // move-assign threads

	std::cout << "Done spawning threads. Now waiting for them to join:" << std::endl;
	for (int i = 0; i < 5; ++i)
		threads[i].join();

	std::cout << "All threads joined!" << std::endl;

	return 0;
}

//***************************************************************************
// Name: thread_joinable_test (thread::joinable)
//
// Expected output:
// -----------------------------
// Joinable after construction:
// foo: false
// bar: true
// Joinable after joining:
// foo: false
// bar: false
//***************************************************************************

int thread_joinable_test(void)
{
	std::thread foo;
	std::thread bar(bar_thread);

	std::cout << "Joinable after construction:\n" << std::boolalpha;
	std::cout << "foo: " << foo.joinable() << std::endl;
	std::cout << "bar: " << bar.joinable() << std::endl;

	if (foo.joinable()) foo.join();
	if (bar.joinable()) bar.join();

	std::cout << "Joinable after joining:\n" << std::boolalpha;
	std::cout << "foo: " << foo.joinable() << std::endl;
	std::cout << "bar: " << bar.joinable() << std::endl;

	return 0;
}

//***************************************************************************
// Name: thread_join_test (thread::join)
//
// Expected output:
// -----------------------------
// Spawning 3 threads...
// Done spawning threads. Now waiting for them to join:
// pause of 1 seconds ended
// pause of 2 seconds ended
// pause of 3 seconds ended
// All threads joined!
//***************************************************************************

int thread_join_test(void)
{
	std::cout << "Spawning 3 threads..." << std::endl;

	std::thread t1 (pause_thread, 1);
	std::thread t2 (pause_thread, 2);
	std::thread t3 (pause_thread, 3);

	std::cout << "Done spawning threads. Now waiting for them to join:" << std::endl;
	t1.join();
	t2.join();
	t3.join();

	std::cout << "All threads joined!" << std::endl;

	return 0;
}
#endif

//***************************************************************************
// Public Functions
//***************************************************************************

//***************************************************************************
// Name: cxxtest_main
//***************************************************************************/

extern "C"
{
	int cxxtest_main(int argc, char *argv[])
	{
		// If C++ initialization for static constructors is supported, then do
		// that first

#ifdef CONFIG_EXAMPLES_CXXTEST_CXXINITIALIZE
		up_cxxinitialize();
#endif
		test_ofstream();
		test_iostream();
		test_stl();

#ifdef CXXTEST_RTTI
		test_rtti();
#endif

#ifdef CXXTEST_EXCEPTION
		test_exception();
#endif

#ifdef THREAD_TEST
		cout << endl << "============ thread::operator= test =====================" << endl;
		thread_operator_test();

		cout << endl << "============ thread::joinable test ======================" << endl;
		thread_joinable_test();

		cout << endl << "============ thread::join test ==========================" << endl;
		thread_join_test();
#endif
		return 0;
	}
}

