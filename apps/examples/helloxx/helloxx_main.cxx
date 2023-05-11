/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
// examples/helloxx/helloxx_main.cxx
//
//   Copyright (C) 2009, 2011-2013 Gregory Nutt. All rights reserved.
//   Author: Gregory Nutt <gnutt@nuttx.org>
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
#include <debug.h>

#include <tinyara/init.h>

//***************************************************************************
// Definitions
//***************************************************************************
// Debug ********************************************************************
// Non-standard debug that may be enabled just for testing the constructors

#ifndef CONFIG_DEBUG
#  undef CONFIG_DEBUG_CXX
#endif

#ifdef CONFIG_DEBUG_CXX
#  define cxxdbg              dbg
#  define cxxlldbg            lldbg
#  ifdef CONFIG_DEBUG_VERBOSE
#    define cxxvdbg           vdbg
#    define cxxllvdbg         llvdbg
#  else
#    define cxxvdbg(...)
#    define cxxllvdbg(...)
#  endif
#else
#  define cxxdbg(...)
#  define cxxlldbg(...)
#  define cxxvdbg(...)
#  define cxxllvdbg(...)
#endif

//***************************************************************************
// Private Classes
//***************************************************************************

class CHelloWorld
{
public:
	CHelloWorld(void) : mSecret(42)
	{
		cxxdbg("Constructor: mSecret=%d\n", mSecret);
	}

	~CHelloWorld(void)
	{
		cxxdbg("Destructor\n");
	}

	bool HelloWorld(void)
	{
		cxxdbg("HelloWorld: mSecret=%d\n", mSecret);

		if (mSecret != 42)
		{
			printf("CHelloWorld::HelloWorld: CONSTRUCTION FAILED!\n");
			return false;
		}
		else
		{
			printf("CHelloWorld::HelloWorld: Hello, World!!\n");
			return true;
		}
	}

private:
	int mSecret;
};

//***************************************************************************
// Private Data
//***************************************************************************

// Define a statically constructed CHellowWorld instance if C++ static
// initializers are supported by the platform

#if defined(CONFIG_HAVE_CXXINITIALIZE) || defined(CONFIG_BINFMT_CONSTRUCTORS)
static CHelloWorld g_HelloWorld;
#endif

//***************************************************************************
// Public Functions
//***************************************************************************

/****************************************************************************
 * Name: helloxx_main
 ****************************************************************************/

extern "C"
{
	int helloxx_main(int argc, char *argv[])
	{
		// Print the cpp version used
		printf("c++ version used : %d\n", __cplusplus);

		// Exercise an explictly instantiated C++ object

		CHelloWorld *pHelloWorld = new CHelloWorld;
		printf("helloxx_main: Saying hello from the dynamically constructed instance\n");
		pHelloWorld->HelloWorld();

		// Exercise an C++ object instantiated on the stack

#ifndef CONFIG_EXAMPLES_HELLOXX_NOSTACKCONST
		CHelloWorld HelloWorld;

		printf("helloxx_main: Saying hello from the instance constructed on the stack\n");
		HelloWorld.HelloWorld();
#endif

		// Exercise an statically constructed C++ object

#if defined(CONFIG_HAVE_CXXINITIALIZE) || defined(CONFIG_BINFMT_CONSTRUCTORS)
		printf("helloxx_main: Saying hello from the statically constructed instance\n");
		g_HelloWorld.HelloWorld();
#endif

		delete pHelloWorld;
		return 0;
	}
}
