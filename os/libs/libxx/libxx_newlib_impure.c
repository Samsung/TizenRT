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

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>

/************************************************************************
 * Public Variables
 ************************************************************************/

/* _impure_ptr is part of libc.a of newlib. If we try to link any other library
 * (ex: libsupc++.a) from toolchain to TizenRT we would endup with below error:
 * .../lib/libsupc++.a(vterminate.o): In function `__gnu_cxx::__verbose_terminate_handler()':
 * vterminate.cc:(....): undefined reference to `_impure_ptr'
 * This is because TizenRT uses its own libc.a instead of linking libc.a of
 * newlib from toolchain.
 * Hence we declare dummy _impure_ptr variable to have a workaround for linking
 * other libraries from toolchain
 * Though it should be part of libc.a, currently it is used while linking
 * libsupc++.a for libc++ as an ABI .Hence it is declared here and used only
 * when LIBCXX_HAVE_LIBSUPCXX is enabled.
 */
void * _impure_ptr;
