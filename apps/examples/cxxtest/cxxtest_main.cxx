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



#include <media/MediaPlayer.hpp>

#include <tinyara/init.h>
#include <apps/platform/cxxinitialize.h>

//#include <media/MediaRecorder.hpp>

#include "jsdosa.h"

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

#undef CXXTEST_RTTI
#undef CXXTEST_ISTREAM
#undef CXXTEST_EXCEPTION

#ifdef CONFIG_UCLIBCXX_EXCEPTION
#define CXXTEST_EXCEPTION
#endif

void printString()
{
	std::cout << "printstring: " << std::endl;
}

/*
template<typename _Callable, typename... _Args>
void enqueue(_Callable&& __f, _Args&&... __args)
{
	//unique_lock<std::mutex> lock(*qMtx);
	//std::function<void()> func = std::bind(std::forward<_Callable>(__f), std::forward<_Args>(__args)...);	
	//cmdQueue.push(func);
	//cv.notify_one();
	std::cout << "18: " << std::endl;
}*/
//***************************************************************************
// Private Classes
//***************************************************************************

JSDOSA jsdosa;

//***************************************************************************
// Public Functions
//***************************************************************************
void test()
{
	jsdosa.set(3);
	printf("%d", jsdosa.get());
	MediaPlayer p;
	p.create();
	p.start();
	p.pause();
	p.resume();
	p.stop();
	p.destroy();
/*
	MediaRecorder r;
	r.create();
	r.start();
	r.pause();
	r.resume();
	r.stop();
	r.destroy();
	*/
}

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

		test();
		//enqueue(printString);

		return 0;
	}
}

