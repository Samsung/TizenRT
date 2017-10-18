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
 */

/**
 * \file
 *      Logic engine used for quickly evaluating data constraints in relations.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aql.h"
#include "db_debug.h"
#include "lvm.h"
#include "db_options.h"

/****************************************************************************
* Pre-processor Definitions
****************************************************************************/
/*
 * The logic engine determines whether a logical  predicate is true for
 * each tuple in a relation. It uses a stack-based execution model of
 * operations that are arranged in prefix (Polish) notation.
 */
#define IS_CONNECTIVE(op) ((op) & LVM_CONNECTIVE)

/****************************************************************************
* Public Functions
****************************************************************************/
static void print_derivations(variable_t *variables, derivation_t *d)
{
	int i;
	for (i = 0; i < LVM_MAX_VARIABLE_ID; i++) {
		if (d[i].derived) {
			DB_LOG_V("%s is constrained to (%ld,%ld)\n", variables[i].name, d[i].min.l, d[i].max.l);
		}
	}
}

static variable_id_t lookup(lvm_instance_t *p, char *name)
{
	variable_t *var;

	for (var = p->variables; var <= &p->variables[LVM_MAX_VARIABLE_ID - 1] && var->name[0] != '\0'; var++) {
		if (strcmp(var->name, name) == 0) {
			break;
		}
	}

	return (variable_id_t)(var - &p->variables[0]);
}

static operator_t *get_operator(lvm_instance_t *p)
{
	operator_t *operator;

	operator = (operator_t *)&p->code[p->ip];
	p->ip += sizeof(operator_t);
	return operator;
}

static void get_operand(lvm_instance_t *p, operand_t *operand)
{
	memcpy(operand, &p->code[p->ip], sizeof(*operand));
	p->ip += sizeof(*operand);
}

static node_type_t get_type(lvm_instance_t *p)
{
	node_type_t node_type;

	node_type = *(node_type_t *)(p->code + p->ip);
	p->ip += sizeof(node_type);

	return node_type;
}

static long operand_to_long(lvm_instance_t *p, operand_t *operand)
{
	switch (operand->type) {
	case LVM_LONG:
		return operand->value.l;
#if LVM_USE_FLOATS
	case LVM_FLOAT:
		return (long)operand->value.f;
		break;
#endif							/* LVM_USE_FLOATS */
	case LVM_VARIABLE:
		return p->variables[operand->value.id].value.l;
	default:
		return 0;
	}
}

static lvm_status_t eval_expr(lvm_instance_t *p, operator_t op, operand_t *result)
{
	int i;
	node_type_t type;
	operator_t *operator;
	operand_t operand[2];
	long value[2];
	long result_value;
	lvm_status_t r;

	for (i = 0; i < 2; i++) {
		type = get_type(p);
		switch (type) {
		case LVM_ARITH_OP:
			operator = get_operator(p);
			r = eval_expr(p, *operator, &operand[i]);
			if (LVM_ERROR(r)) {
				return r;
			}
			break;
		case LVM_OPERAND:
			get_operand(p, &operand[i]);
			break;
		default:
			return SEMANTIC_ERROR;
		}
		value[i] = operand_to_long(p, &operand[i]);
	}

	switch (op) {
	case LVM_ADD:
		result_value = value[0] + value[1];
		break;
	case LVM_SUB:
		result_value = value[0] - value[1];
		break;
	case LVM_MUL:
		result_value = value[0] * value[1];
		break;
	case LVM_DIV:
		if (value[1] == 0) {
			return MATH_ERROR;
		}
		result_value = value[0] / value[1];
		break;
	default:
		return EXECUTION_ERROR;
	}

	result->type = LVM_LONG;
	result->value.l = result_value;

	return LVM_TRUE;
}

static int eval_logic(lvm_instance_t *p, operator_t *op)
{
	int i;
	int r;
	operand_t operand;
	long result[2];
	node_type_t type;
	operator_t *operator;
	long l1, l2;
	int logic_result[2];
	unsigned arguments;

	if (IS_CONNECTIVE(*op)) {
		arguments = *op == LVM_NOT ? 1 : 2;
		for (i = 0; i < arguments; i++) {
			type = get_type(p);
			if (type != LVM_CMP_OP) {
				return SEMANTIC_ERROR;
			}
			operator = get_operator(p);
			logic_result[i] = eval_logic(p, operator);
			if (LVM_ERROR(logic_result[i])) {
				return logic_result[i];
			}
		}

		if (*op == LVM_NOT) {
			return !logic_result[0];
		} else if (*op == LVM_AND) {
			return logic_result[0] == LVM_TRUE && logic_result[1] == LVM_TRUE;
		} else {
			return logic_result[0] == LVM_TRUE || logic_result[1] == LVM_TRUE;
		}
	}

	for (i = 0; i < 2; i++) {
		type = get_type(p);
		switch (type) {
		case LVM_ARITH_OP:
			operator = get_operator(p);
			r = eval_expr(p, *operator, &operand);
			if (LVM_ERROR(r)) {
				return r;
			}
			break;
		case LVM_OPERAND:
			get_operand(p, &operand);
			break;
		default:
			return SEMANTIC_ERROR;
		}
		result[i] = operand_to_long(p, &operand);
	}

	l1 = result[0];
	l2 = result[1];
	DB_LOG_D("Result1: %ld\nResult2: %ld\n", l1, l2);

	switch (*op) {
	case LVM_EQ:
		return l1 == l2;
	case LVM_NEQ:
		return l1 != l2;
	case LVM_GE:
		return l1 > l2;
	case LVM_GEQ:
		return l1 >= l2;
	case LVM_LE:
		return l1 < l2;
	case LVM_LEQ:
		return l1 <= l2;
	default:
		break;
	}

	return EXECUTION_ERROR;
}

void lvm_reset(lvm_instance_t *p)
{
	p->end = 0;
	p->ip = 0;
	p->error = 0;
	memset(p->code, 0, sizeof(p->code));
	memset(p->variables, 0, sizeof(p->variables));
	memset(p->derivations, 0, sizeof(p->derivations));
}

lvm_ip_t lvm_jump_to_operand(lvm_instance_t *p)
{
	lvm_ip_t old_end;

	old_end = p->end;
	p->end += sizeof(operator_t) + sizeof(node_type_t);
	if (p->end >= DB_VM_BYTECODE_SIZE) {
		p->error = __LINE__;
		p->end = old_end;
	}

	return old_end;
}

lvm_ip_t lvm_shift_for_operator(lvm_instance_t *p, lvm_ip_t end)
{
	unsigned char *ptr;
	lvm_ip_t old_end;

	old_end = p->end;

	if (p->end + sizeof(operator_t) > DB_VM_BYTECODE_SIZE || end >= old_end) {
		p->error = __LINE__;
		return 0;
	}

	ptr = p->code + end;

	memmove(ptr + sizeof(operator_t) + sizeof(node_type_t), ptr, old_end - end);
	p->end = end;

	return old_end + sizeof(operator_t) + sizeof(node_type_t);
}

lvm_ip_t lvm_get_end(lvm_instance_t *p)
{
	return p->end;
}

lvm_ip_t lvm_set_end(lvm_instance_t *p, lvm_ip_t end)
{
	lvm_ip_t old_end;

	if (end >= DB_VM_BYTECODE_SIZE) {
		p->error = __LINE__;
		return p->end;
	}

	old_end = p->end;
	p->end = end;

	return old_end;
}

lvm_status_t lvm_set_type(lvm_instance_t *p, node_type_t type)
{
	if (p->end >= DB_VM_BYTECODE_SIZE) {
		DB_LOG_E("lvm_set_type failed because of overflow\n");
		return STACK_OVERFLOW;
	}

	*(node_type_t *)(p->code + p->end) = type;
	p->end += sizeof(type);

	return LVM_TRUE;
}

lvm_status_t lvm_execute(lvm_instance_t *p)
{
	node_type_t type;
	operator_t *operator;
	lvm_status_t status;

	p->ip = 0;
	status = EXECUTION_ERROR;
	type = get_type(p);
	switch (type) {
	case LVM_CMP_OP:
		operator = get_operator(p);
		status = eval_logic(p, operator);
		if (!LVM_ERROR(status)) {
			DB_LOG_D("The statement is %s\n", status == LVM_TRUE ? "true" : "false");
		} else {
			DB_LOG_E("Execution error: %d\n", (int)status);
		}
		break;
	default:
		DB_LOG_E("Error: The code must start with a relational operator\n");
	}

	return status;
}

lvm_status_t lvm_set_op(lvm_instance_t *p, operator_t op)
{
	lvm_status_t result;

	result = lvm_set_type(p, LVM_ARITH_OP);

	if (LVM_ERROR(result) || p->end + sizeof(op) >= DB_VM_BYTECODE_SIZE) {
		DB_LOG_E("lvm_set_op failed because of overflow\n");
		return STACK_OVERFLOW;
	}

	memcpy(&p->code[p->end], &op, sizeof(op));
	p->end += sizeof(op);

	return LVM_TRUE;
}

lvm_status_t lvm_set_relation(lvm_instance_t *p, operator_t op)
{
	lvm_status_t result;

	result = lvm_set_type(p, LVM_CMP_OP);

	if (LVM_ERROR(result) || p->end + sizeof(op) >= DB_VM_BYTECODE_SIZE) {
		DB_LOG_E("lvm_set_relation failed because of overflow\n");
		return STACK_OVERFLOW;
	}

	memcpy(&p->code[p->end], &op, sizeof(op));
	p->end += sizeof(op);

	return LVM_TRUE;
}

lvm_status_t lvm_set_operand(lvm_instance_t *p, operand_t *op)
{
	lvm_status_t result;

	result = lvm_set_type(p, LVM_OPERAND);

	if (LVM_ERROR(result) || p->end + sizeof(*op) >= DB_VM_BYTECODE_SIZE) {
		DB_LOG_E("lvm_set_operand failed because of overflow\n");
		return STACK_OVERFLOW;
	}

	memcpy(&p->code[p->end], op, sizeof(*op));
	p->end += sizeof(*op);

	return LVM_TRUE;
}

lvm_status_t lvm_set_operand_value(lvm_instance_t *p, attribute_t *attr, unsigned char *value)
{
	operand_value_t operand_value;

	/* Update the internal state of the PLE. */
	if (attr->domain == DOMAIN_INT) {
		operand_value.l = value[0] << 8 | value[1];
	} else if (attr->domain == DOMAIN_LONG) {
		operand_value.l = (uint32_t)value[0] << 24 | (uint32_t)value[1] << 16 | (uint32_t)value[2] << 8 | value[3];
	}

	return lvm_set_variable_value(p, attr->name, operand_value);
}

lvm_status_t lvm_set_long(lvm_instance_t *p, long l)
{
	operand_t op;

	op.type = LVM_LONG;
	op.value.l = l;

	return lvm_set_operand(p, &op);
}

lvm_status_t lvm_register_variable(lvm_instance_t *p, char *name, operand_type_t type)
{
	variable_id_t id;
	variable_t *var;

	id = lookup(p, name);
	if (id == LVM_MAX_VARIABLE_ID) {
		return INVALID_IDENTIFIER;
	}

	var = &p->variables[id];
	if (var->name[0] == '\0') {
		strncpy(var->name, name, sizeof(var->name) - 1);
		var->name[sizeof(var->name) - 1] = '\0';
		var->type = type;
	}

	return LVM_TRUE;
}

lvm_status_t lvm_set_variable_value(lvm_instance_t *p, char *name, operand_value_t value)
{
	variable_id_t id;

	id = lookup(p, name);
	if (id == LVM_MAX_VARIABLE_ID) {
		return INVALID_IDENTIFIER;
	}
	p->variables[id].value = value;
	return LVM_TRUE;
}

lvm_status_t lvm_set_variable(lvm_instance_t *p, char *name)
{
	operand_t op;
	variable_id_t id;

	id = lookup(p, name);
	if (id == LVM_MAX_VARIABLE_ID) {
		return INVALID_IDENTIFIER;
	}

	DB_LOG_D("var id = %d\n", id);
	op.type = LVM_VARIABLE;
	op.value.id = id;

	return lvm_set_operand(p, &op);
}

static void create_intersection(derivation_t *result, derivation_t *d1, derivation_t *d2)
{
	int i;

	for (i = 0; i < LVM_MAX_VARIABLE_ID; i++) {
		if (!d1[i].derived && !d2[i].derived) {
			continue;
		} else if (d1[i].derived && !d2[i].derived) {
			result[i].min.l = d1[i].min.l;
			result[i].max.l = d1[i].max.l;
		} else if (!d1[i].derived && d2[i].derived) {
			result[i].min.l = d2[i].min.l;
			result[i].max.l = d2[i].max.l;
		} else {
			/* Both derivations have been made; create an
			   intersection of the ranges. */
			if (d1[i].min.l > d2[i].min.l) {
				result[i].min.l = d1[i].min.l;
			} else {
				result[i].min.l = d2[i].min.l;
			}

			if (d1[i].max.l < d2[i].max.l) {
				result[i].max.l = d1[i].max.l;
			} else {
				result[i].max.l = d2[i].max.l;
			}
		}
		result[i].derived = 1;
	}
	/* DEBUG */
}

static void create_union(derivation_t *result, derivation_t *d1, derivation_t *d2)
{
	int i;

	for (i = 0; i < LVM_MAX_VARIABLE_ID; i++) {
		if (!d1[i].derived && !d2[i].derived) {
			continue;
		} else if (d1[i].derived && !d2[i].derived) {
			result[i].min.l = d1[i].min.l;
			result[i].max.l = d1[i].max.l;
		} else if (!d1[i].derived && d2[i].derived) {
			result[i].min.l = d2[i].min.l;
			result[i].max.l = d2[i].max.l;
		} else {
			/* Both derivations have been made; create a
			   union of the ranges. */
			if (d1[i].min.l > d2[i].min.l) {
				result[i].min.l = d2[i].min.l;
			} else {
				result[i].min.l = d1[i].min.l;
			}

			if (d1[i].max.l < d2[i].max.l) {
				result[i].max.l = d2[i].max.l;
			} else {
				result[i].max.l = d1[i].max.l;
			}
		}
		result[i].derived = 1;
	}

	/* DEBUG */
}

static int derive_relation(lvm_instance_t *p, derivation_t *local_derivations)
{
	operator_t *operator;
	node_type_t type;
	operand_t operand[2];
	int i;
	int variable_id;
	operand_value_t *value;
	derivation_t *derivation;

	type = get_type(p);
	operator = get_operator(p);

	if (IS_CONNECTIVE(*operator)) {
		derivation_t d1[LVM_MAX_VARIABLE_ID];
		derivation_t d2[LVM_MAX_VARIABLE_ID];

		if (*operator != LVM_AND && *operator != LVM_OR) {
			return DERIVATION_ERROR;
		}

		DB_LOG_D("Attempting to infer ranges from a logical connective\n");

		memset(d1, 0, sizeof(d1));
		memset(d2, 0, sizeof(d2));

		if (LVM_ERROR(derive_relation(p, d1)) || LVM_ERROR(derive_relation(p, d2))) {
			return DERIVATION_ERROR;
		}

		if (*operator == LVM_AND) {
			create_intersection(local_derivations, d1, d2);
			DB_LOG_V("Created an intersection of D1 and D2\n");
		} else if (*operator == LVM_OR) {
			create_union(local_derivations, d1, d2);
			DB_LOG_V("Created a union of D1 and D2\n");
		}
		DB_LOG_V("D1: \n");
		print_derivations(p->variables, d1);
		DB_LOG_V("D2: \n");
		print_derivations(p->variables, d2);
		DB_LOG_V("Result: \n");
		print_derivations(p->variables, local_derivations);
		return LVM_TRUE;
	}

	for (i = 0; i < 2; i++) {
		type = get_type(p);
		switch (type) {
		case LVM_OPERAND:
			get_operand(p, &operand[i]);
			break;
		default:
			return DERIVATION_ERROR;
		}
	}

	if (operand[0].type == LVM_VARIABLE && operand[1].type == LVM_VARIABLE) {
		return DERIVATION_ERROR;
	}

	/* Determine which of the operands that is the variable. */
	if (operand[0].type == LVM_VARIABLE) {
		if (operand[1].type == LVM_VARIABLE) {
			return DERIVATION_ERROR;
		}
		variable_id = operand[0].value.id;
		value = &operand[1].value;
	} else {
		variable_id = operand[1].value.id;
		value = &operand[0].value;
	}

	if (variable_id >= LVM_MAX_VARIABLE_ID) {
		return DERIVATION_ERROR;
	}

	DB_LOG_D("variable id %d, value %ld\n", variable_id, *(long *)value);

	derivation = local_derivations + variable_id;
	/* Default values. */
	derivation->max.l = DB_LONG_MAX;
	derivation->min.l = DB_LONG_MIN;

	switch (*operator) {
	case LVM_EQ:
		derivation->max = *value;
		derivation->min = *value;
		break;
	case LVM_GE:
		derivation->min.l = value->l + 1;
		break;
	case LVM_GEQ:
		derivation->min.l = value->l;
		break;
	case LVM_LE:
		derivation->max.l = value->l - 1;
		break;
	case LVM_LEQ:
		derivation->max.l = value->l;
		break;
	default:
		return DERIVATION_ERROR;
	}

	DB_LOG_D("derivation max = %ld, min = %ld\n", derivation->max.l, derivation->min.l);
	derivation->derived = 1;

	return LVM_TRUE;
}

lvm_status_t lvm_derive(lvm_instance_t *p)
{
	return derive_relation(p, p->derivations);
}

lvm_status_t lvm_get_derived_range(lvm_instance_t *p, char *name, operand_value_t *min, operand_value_t *max)
{
	int i;

	for (i = 0; i < LVM_MAX_VARIABLE_ID; i++) {
		if (strcmp(name, p->variables[i].name) == 0) {
			if (p->derivations[i].derived) {
				*min = p->derivations[i].min;
				*max = p->derivations[i].max;
				return LVM_TRUE;
			}
			return DERIVATION_ERROR;
		}
	}
	return INVALID_IDENTIFIER;
}

#if DEBUG
static lvm_ip_t print_operator(lvm_instance_t *p, lvm_ip_t index)
{
	operator_t operator;
	struct operator_map {
		operator_t op;
		char *representation;
	};
	struct operator_map operator_map[] = {
		{LVM_ADD, "+"},
		{LVM_SUB, "-"},
		{LVM_MUL, "*"},
		{LVM_DIV, "/"},
		{LVM_GE, ">"},
		{LVM_GEQ, ">="},
		{LVM_LE, "<"},
		{LVM_LEQ, "<="},
		{LVM_EQ, "="},
		{LVM_NEQ, "<>"},
		{LVM_AND, "/\\"},
		{LVM_OR, "\\/"},
		{LVM_NOT, "!"}
	};
	int i;

	memcpy(&operator, p->code + index, sizeof(operator));

	for (i = 0; i < sizeof(operator_map) / sizeof(operator_map[0]); i++) {
		if (operator_map[i].op == operator) {
			DB_LOG_V("%s ", operator_map[i].representation);
			break;
		}
	}

	return index + sizeof(operator_t);
}

static lvm_ip_t print_operand(lvm_instance_t *p, lvm_ip_t index)
{
	operand_t operand;

	memcpy(&operand, p->code + index, sizeof(operand));

	switch (operand.type) {
	case LVM_VARIABLE:
		if (operand.value.id >= LVM_MAX_VARIABLE_ID || p->variables[operand.value.id].name == NULL) {
			DB_LOG_D("var(id:%d):?? ", operand.value.id);
		} else {
			DB_LOG_D("var(%s):%ld ", p->variables[operand.value.id].name, p->variables[operand.value.id].value.l);
		}
		break;
	case LVM_LONG:
		DB_LOG_D("long:%ld ", operand.value.l);
		break;
	default:
		DB_LOG_D("?? ");
		break;
	}

	return index + sizeof(operand_t);
}

static lvm_ip_t print_relation(lvm_instance_t *p, lvm_ip_t index)
{
	/* Relational operators are stored as ordinary operators. */
	return print_operator(p, index);
}
#endif							/* DEBUG */

void lvm_print_code(lvm_instance_t *p)
{
#if DEBUG
	lvm_ip_t ip;

	DB_LOG_D("Code: ");

	for (ip = 0; ip < p->end;) {
		switch (*(node_type_t *)(p->code + ip)) {
		case LVM_CMP_OP:
			ip = print_relation(p, ip + sizeof(node_type_t));
			break;
		case LVM_ARITH_OP:
			ip = print_operator(p, ip + sizeof(node_type_t));
			break;
		case LVM_OPERAND:
			ip = print_operand(p, ip + sizeof(node_type_t));
			break;
		default:
			DB_LOG_E("Invalid opcode: 0x%x ", p->code[ip]);
			ip = p->end;
			break;
		}
	}
	putchar('\n');
#endif
}

void lvm_print_derivations(lvm_instance_t *p)
{
#if DEBUG
	print_derivations(p->variables, p->derivations);
#endif							/* DEBUG */
}

#ifdef TEST
int main(void)
{
	lvm_instance_t p;
	unsigned char code[256];

	lvm_reset(&p, code, sizeof(code));

	lvm_register_variable("z", LVM_LONG);
	lvm_set_variable_value("z", (operand_value_t)15L);

	lvm_register_variable("y", LVM_LONG);
	lvm_set_variable_value("y", (operand_value_t)109L);

	/* Infix: 109 = y /\ 20 > 70 - (6 + z * 3) => 109 = 109 /\ 20 > 19 => true */
	lvm_set_relation(&p, LVM_AND);
	lvm_set_relation(&p, LVM_EQ);
	lvm_set_long(&p, 109);
	lvm_set_variable(&p, "y");
	lvm_set_relation(&p, LVM_GE);
	lvm_set_long(&p, 20);
	lvm_set_op(&p, LVM_SUB);
	lvm_set_long(&p, 70);
	lvm_set_op(&p, LVM_ADD);
	lvm_set_long(&p, 6);
	lvm_set_op(&p, LVM_MUL);
	lvm_set_variable(&p, "z");
	lvm_set_long(&p, 3);

	lvm_print_code(&p);

	lvm_execute(&p);

	/* Infix: !(9999 + 1 < -1 + 10001) => !(10000 < 10000) => true */
	lvm_reset(&p, code, sizeof(code));
	lvm_set_relation(&p, LVM_NOT);
	lvm_set_relation(&p, LVM_LE);
	lvm_set_op(&p, LVM_ADD);
	lvm_set_long(&p, 9999);
	lvm_set_long(&p, 1);
	lvm_set_op(&p, LVM_ADD);
	lvm_set_long(&p, -1);
	lvm_set_long(&p, 10001);

	lvm_print_code(&p);

	lvm_execute(&p);

	/* Derivation tests */

	/* Infix: a = 5 => a:(5,5) */
	lvm_reset(&p, code, sizeof(code));
	lvm_register_variable("a", LVM_LONG);
	lvm_set_relation(&p, LVM_EQ);
	lvm_set_variable(&p, "a");
	lvm_set_long(&p, 5);

	lvm_derive(&p);
	lvm_print_derivations(&p);

	/* Infix: a < 10 => a:(-oo,9) */
	lvm_reset(&p, code, sizeof(code));
	lvm_register_variable("a", LVM_LONG);
	lvm_set_relation(&p, LVM_LE);
	lvm_set_variable(&p, "a");
	lvm_set_long(&p, 10);

	lvm_derive(&p);
	lvm_print_derivations(&p);

	/* Infix: a < 100 /\ 10 < a => a:(11,99) */
	lvm_reset(&p, code, sizeof(code));
	lvm_register_variable("a", LVM_LONG);
	lvm_set_relation(&p, LVM_AND);
	lvm_set_relation(&p, LVM_LE);
	lvm_set_variable(&p, "a");
	lvm_set_long(&p, 100);
	lvm_set_relation(&p, LVM_GE);
	lvm_set_long(&p, 10);
	lvm_set_variable(&p, "a");

	lvm_derive(&p);
	lvm_print_derivations(&p);

	/* Infix: a < 100 /\ b > 100 => a:(-oo,99), b:(101,oo) */
	lvm_reset(&p, code, sizeof(code));
	lvm_register_variable("a", LVM_LONG);
	lvm_register_variable("b", LVM_LONG);
	lvm_set_relation(&p, LVM_AND);
	lvm_set_relation(&p, LVM_LE);
	lvm_set_variable(&p, "a");
	lvm_set_long(&p, 100);
	lvm_set_relation(&p, LVM_GE);
	lvm_set_variable(&p, "b");
	lvm_set_long(&p, 100);

	lvm_derive(&p);
	lvm_print_derivations(&p);

	/* Infix: a < 100 \/ a < 1000 \/ a < 1902 => a:(-oo,1901) */
	lvm_reset(&p, code, sizeof(code));
	lvm_register_variable("a", LVM_LONG);
	lvm_set_relation(&p, LVM_OR);
	lvm_set_relation(&p, LVM_LE);
	lvm_set_variable(&p, "a");
	lvm_set_long(&p, 100);
	lvm_set_relation(&p, LVM_OR);
	lvm_set_relation(&p, LVM_LE);
	lvm_set_long(&p, 1000);
	lvm_set_variable(&p, "a");
	lvm_set_relation(&p, LVM_LE);
	lvm_set_variable(&p, "a");
	lvm_set_long(&p, 1902);

	lvm_derive(&p);
	lvm_print_derivations(&p);

	/* Infix: (a < 100 /\ a < 90 /\ a > 80 /\ a < 105) \/ b > 10000 =>
	   a:(81,89), b:(10001:oo) */
	lvm_reset(&p, code, sizeof(code));
	lvm_register_variable("a", LVM_LONG);
	lvm_register_variable("b", LVM_LONG);

	lvm_set_relation(&p, LVM_OR);
	lvm_set_relation(&p, LVM_GE);
	lvm_set_variable(&p, "b");
	lvm_set_long(&p, 10000);

	lvm_set_relation(&p, LVM_AND);
	lvm_set_relation(&p, LVM_LE);
	lvm_set_variable(&p, "a");
	lvm_set_long(&p, 100);
	lvm_set_relation(&p, LVM_AND);
	lvm_set_relation(&p, LVM_LE);
	lvm_set_variable(&p, "a");
	lvm_set_long(&p, 90);
	lvm_set_relation(&p, LVM_AND);
	lvm_set_relation(&p, LVM_GE);
	lvm_set_variable(&p, "a");
	lvm_set_long(&p, 80);
	lvm_set_relation(&p, LVM_LE);
	lvm_set_variable(&p, "a");
	lvm_set_long(&p, 105);

	lvm_derive(&p);
	lvm_print_derivations(&p);

	DB_LOG_D("Done\n");

	return 0;
}
#endif
