// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define IRDA_FAILURE                (1)
#define IRDA_SUCCESS                (0)

#define IRDA_DEV_NAME                "irda"

#define IRDA_CMD_MAGIC              (0xe290000)
enum
{
	IRDA_CMD_ACTIVE = IRDA_CMD_MAGIC + 1,
	IRDA_CMD_SET_POLARITY,
	IRDA_CMD_SET_CLK,
	IRDA_CMD_SET_INT_MASK,
};

enum
{
	IR_KEY_TYPE_SHORT = 0,
	IR_KEY_TYPE_LONG,
	IR_KEY_TYPE_HOLD,
	IR_KEY_TYPE_MAX,
};

#define USERCODE_MASK 	0xffff

#define KEY_CODE_MASK	0xff0000
#define KEY_CODE_SHIFT  16

#define KEY_CODE_INVERS_MASK  0xff000000
#define KEY_CODE_INVERS_SHIFT 24

#define KEY_SHORT_CNT         3
#define KEY_LONG_CNT          8
#define KEY_HOLD_CNT          11


#define GENERATE_KEY(type,value) (((type) << 24) | (value))
#define GET_KEY_TYPE(msg) (((msg) >> 24) & 0xff)
#define GET_KEY_VALUE(msg) ((msg) & 0xff)

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void irda_init(void);
void irda_exit(void);
void irda_isr(void);
void Irda_init_app(void);
void set_irda_usrcode(UINT16 ir_usercode);
long IR_get_key(void *buffer, unsigned long  size, INT32 timeout);

#ifdef __cplusplus
}
#endif
