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
// lib/libxx/libxx_cxa_guard.cxx
//
//   Copyright (C) 2015 Omni Hoverboards Inc. All rights reserved.
//   Author: Paul Alexander Patience <paul-a.patience@polymtl.ca>
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

#include "libxx_cxa_guard.hxx"
#include <pthread.h>

//***************************************************************************
// Private Data
//***************************************************************************

static pthread_mutex_t __cxa_guard_mutex = PTHREAD_MUTEX_INITIALIZER;

//***************************************************************************
// Public Functions
//***************************************************************************

extern "C"
{
  //*************************************************************************
  // Name: __cxa_guard_acquire
  //*************************************************************************

  int __cxa_guard_acquire(FAR __guard *g)
  {
    pthread_mutex_lock(&__cxa_guard_mutex);
#ifdef __ARM_EABI__
    int result = !(__atomic_load_n(g, __ATOMIC_ACQUIRE) & 1);
#else
    int result = !*(char *)g;
#endif
    if (!result) {
      pthread_mutex_unlock(&__cxa_guard_mutex);
    }
    return result;
  }

  //*************************************************************************
  // Name: __cxa_guard_release
  //*************************************************************************

  void __cxa_guard_release(FAR __guard *g)
  {
#ifdef __ARM_EABI__
    __atomic_fetch_or(g, 1, __ATOMIC_RELEASE);
#else
    *(char *)g = 1;
    __sync_synchronize();
#endif
    pthread_mutex_unlock(&__cxa_guard_mutex);
  }

  //*************************************************************************
  // Name: __cxa_guard_abort
  //*************************************************************************

  void __cxa_guard_abort(FAR __guard *)
  {
    pthread_mutex_unlock(&__cxa_guard_mutex);
  }
}
