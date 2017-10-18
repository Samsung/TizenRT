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
/*
 * Copyright (c) 2010, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *      Definitions and declarations for the Propositional Logic Engine.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

#ifndef LVM_H
#define LVM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdlib.h>
#include "db_options.h"

/****************************************************************************
* Pre-processor Definitions
****************************************************************************/
#define LVM_ERROR(x)    (x >= 2)

/****************************************************************************
* Public Type Definitions
****************************************************************************/
typedef unsigned char variable_id_t;

typedef int lvm_ip_t;

enum lvm_status_e {
	LVM_FALSE = 0,
	LVM_TRUE = 1,
	INVALID_IDENTIFIER = 2,
	SEMANTIC_ERROR = 3,
	MATH_ERROR = 4,
	STACK_OVERFLOW = 5,
	TYPE_ERROR = 6,
	VARIABLE_LIMIT_REACHED = 7,
	EXECUTION_ERROR = 8,
	DERIVATION_ERROR = 9
};

typedef enum lvm_status_e lvm_status_t;

enum node_type_e {
	LVM_ARITH_OP = 0x10,
	LVM_OPERAND = 0x20,
	LVM_CMP_OP = 0x40,
	LVM_CONNECTIVE = 0x80
};
typedef enum node_type_e node_type_t;

enum lvm_operator_e   {
	LVM_ADD = LVM_ARITH_OP | 1,
	LVM_SUB = LVM_ARITH_OP | 2,
	LVM_MUL = LVM_ARITH_OP | 3,
	LVM_DIV = LVM_ARITH_OP | 4,
	LVM_EQ = LVM_CMP_OP | 1,
	LVM_NEQ = LVM_CMP_OP | 2,
	LVM_GE = LVM_CMP_OP | 3,
	LVM_GEQ = LVM_CMP_OP | 4,
	LVM_LE = LVM_CMP_OP | 5,
	LVM_LEQ = LVM_CMP_OP | 6,
	LVM_AND = LVM_CONNECTIVE | 1,
	LVM_OR = LVM_CONNECTIVE | 2,
	LVM_NOT = LVM_CONNECTIVE | 3
};
typedef enum lvm_operator_e operator_t;

enum operand_type_e {
	LVM_VARIABLE,
	LVM_FLOAT,
	LVM_LONG
};
typedef enum operand_type_e operand_type_t;

union operand_value_u {
	long l;
	double d;
#if LVM_USE_FLOATS
	float f;
#endif
	variable_id_t id;
};
typedef union operand_value_u operand_value_t;

struct operand_s {
	operand_type_t type;
	operand_value_t value;
};
typedef struct operand_s operand_t;

struct operand_variable_s {
	operand_type_t type;
	operand_value_t value;
	char name[LVM_MAX_NAME_LENGTH + 1];
};
typedef struct operand_variable_s variable_t;

struct derivation_s {
	operand_value_t max;
	operand_value_t min;
	uint8_t derived;
};
typedef struct derivation_s derivation_t;

struct lvm_instance_s {
	unsigned char code[DB_VM_BYTECODE_SIZE];
	variable_t variables[LVM_MAX_VARIABLE_ID];
	derivation_t derivations[LVM_MAX_VARIABLE_ID];
	lvm_ip_t end;
	lvm_ip_t ip;
	unsigned error;
};
typedef struct lvm_instance_s lvm_instance_t;

/****************************************************************************
* Global Function Prototypes
****************************************************************************/
void lvm_reset(lvm_instance_t *p);
void lvm_clone(lvm_instance_t *dst, lvm_instance_t *src);
lvm_status_t lvm_derive(lvm_instance_t *p);
lvm_status_t lvm_get_derived_range(lvm_instance_t *p, char *name, operand_value_t *min, operand_value_t *max);
void lvm_print_derivations(lvm_instance_t *p);
lvm_status_t lvm_execute(lvm_instance_t *p);
lvm_status_t lvm_register_variable(lvm_instance_t *p, char *name, operand_type_t type);
void lvm_print_code(lvm_instance_t *p);
lvm_ip_t lvm_jump_to_operand(lvm_instance_t *p);
lvm_ip_t lvm_shift_for_operator(lvm_instance_t *p, lvm_ip_t end);
lvm_ip_t lvm_get_end(lvm_instance_t *p);
lvm_ip_t lvm_set_end(lvm_instance_t *p, lvm_ip_t end);
lvm_status_t lvm_set_op(lvm_instance_t *p, operator_t op);
lvm_status_t lvm_set_relation(lvm_instance_t *p, operator_t op);
lvm_status_t lvm_set_operand(lvm_instance_t *p, operand_t *op);
lvm_status_t lvm_set_operand_value(lvm_instance_t *p, attribute_t *attr, unsigned char *value);
lvm_status_t lvm_set_long(lvm_instance_t *p, long l);
lvm_status_t lvm_set_variable(lvm_instance_t *p, char *name);
lvm_status_t lvm_set_variable_value(lvm_instance_t *p, char *name, operand_value_t value);
#endif							/* LVM_H */
