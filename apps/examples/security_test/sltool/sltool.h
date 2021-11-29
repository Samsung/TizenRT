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

typedef enum {
#ifdef SLTOOL_OPS_TABLE
#undef SLTOOL_OPS_TABLE
#endif
#define SLTOOL_OPS_TABLE(cmd, type) type,
#include "sltool_ops_table.h"
	SLOPS_UNKNOWN,
} sltool_ops;

typedef struct sl_options sl_options_s;
typedef int (*sl_handler)(sl_options_s *opt);
typedef sl_handler (*sl_parser_func)(sl_options_s *opt);

typedef struct sl_options {
	int idx;
	char ss_data;
	int ss_write_size;
	sltool_ops ops;
	sl_parser_func func;
} sl_options_s;
