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

#ifndef __EXTERNAL_LIBCXX_TEST_UTC_LIBCXX_H
#define __EXTERNAL_LIBCXX_TEST_UTC_LIBCXX_H

#include "libcxx_tc_common.h"

// Main test functions
int tc_algorithms_main(void);
int tc_numerics_main(void);
int tc_utilities_main(void);
int tc_thread_main(void);
int tc_diagnostics_main(void);
int tc_atomics_main(void);
int tc_language_support_main(void);

// Menu functions with argc/argv for command-line based test selection
#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS
void tc_algorithms_menu(int argc, char *argv[]);
#endif

#ifdef CONFIG_LIBCXX_UTC_CONTAINERS
void tc_containers_menu(int argc, char *argv[]);
#endif

#ifdef CONFIG_LIBCXX_UTC_STRINGS
void tc_strings_menu(int argc, char *argv[]);
#endif

#ifdef CONFIG_LIBCXX_UTC_ITERATORS
void tc_iterators_menu(int argc, char *argv[]);
#endif

#ifdef CONFIG_LIBCXX_UTC_UTILITIES
void tc_utilities_menu(int argc, char *argv[]);
#endif

#ifdef CONFIG_LIBCXX_UTC_LANGUAGE_SUPPORT
void tc_language_support_menu(int argc, char *argv[]);
#endif

#endif /* __EXTERNAL_LIBCXX_TEST_UTC_LIBCXX_H */
