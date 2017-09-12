/* *****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#ifndef SVRDBEDITOR_COMMON_H_
#define SVRDBEDITOR_COMMON_H_

#include "iotivity_config.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <stdio.h>

#include "experimental/securevirtualresourcetypes.h"

#define TAG  "OIC_SVR_DB_EDITOR"

#define BOLD_BEGIN    "\033[1m"
#define RED_BEGIN      "\033[1;31m"
#define YELLOW_BEGIN  "\033[1;33m"
#define CYAN_BEGIN  "\033[1;36m"
#define GREEN_BEGIN  "\033[1;92m"
#define COLOR_END      "\033[0m"
#define COLOR_END_NL      "\033[0m\n"

#define SVR_MAX_ENTITY (16)

#define SVR_DB_PATH_LENGTH (1024)
#define PRINT_ERR(fmt,...) printf(RED_BEGIN "error: " fmt COLOR_END_NL, ##__VA_ARGS__)
#define PRINT_WARN(fmt,...) printf(YELLOW_BEGIN "warning : " fmt COLOR_END_NL, ##__VA_ARGS__)
#define PRINT_INFO(fmt,...) printf(YELLOW_BEGIN fmt COLOR_END_NL, ##__VA_ARGS__)
#define PRINT_PROG(fmt,...) printf(BOLD_BEGIN fmt COLOR_END, ##__VA_ARGS__)
#define PRINT_DATA(fmt,...) printf(CYAN_BEGIN fmt COLOR_END, ##__VA_ARGS__)
#define PRINT_NORMAL(fmt,...) printf(fmt, ##__VA_ARGS__)
#define PRINT_NL() printf("\n");

typedef enum SubOperationType
{
    SVR_PRINT = 1,
    SVR_ADD = 2,
    SVR_REMOVE = 3,
    SVR_MODIFY = 4,
    SVR_EDIT_IDX_SIZE = 5,
    BACK = 99
} SubOperationType_t;

void PrintUuid(const OicUuid_t *uuid);
void PrintIntArray(const int *array, size_t length);
void PrintStringArray(const char **array, size_t length);
void PrintInt(int value);
void PrintString(const char *text);
void PrintBuffer(const uint8_t *buf, size_t bufLen);

int InputNumber(const char *infoText);
char *InputString(const char *infoText);
int InputUuid(OicUuid_t *uuid);

#endif /* SVRDBEDITOR_COMMON_H_ */
