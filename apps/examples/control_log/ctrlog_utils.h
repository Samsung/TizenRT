/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#pragma once

#include <tinyara/net/netlog.h>

typedef int (*cmd_handler)(void *opt);
typedef int (*cmd_parser_func)(void *opt, int argc, char **argv);

typedef struct cl_options {
  cmd_handler func;
  nl_color_e color;
  nl_options op;
  netlog_module_e mod;
  uint32_t level;
  uint32_t sub_mod;
} cl_options_s;
