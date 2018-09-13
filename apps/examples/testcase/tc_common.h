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

#ifndef __EXAMPLES_TESTCASE_TESTCASE_COMMON_H
#define __EXAMPLES_TESTCASE_TESTCASE_COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

enum tc_op_type_e {
	TC_START,
	TC_END,
	TC_OP_TYPE_MAX
};
typedef enum tc_op_type_e tc_op_type_t;

extern int total_pass;
extern int total_fail;

#define TC_ASSERT_CLEANUP(api_name, var, freeResource) \
{\
	if (!(var)) {\
		printf("\n[%s] FAIL [Line : %d] %s : Following expression is not true: %s\n", __func__, __LINE__, api_name, #var); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT(api_name, var) TC_ASSERT_CLEANUP(api_name, var, )

#define TC_ASSERT_EQ_CLEANUP(api_name, var, ref, freeResource) \
{\
	if ((var) != (ref)) {\
		printf("\n[%s] FAIL [Line : %d] %s : Values (%s == 0x%x) and (%s == 0x%x) are not equal\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_EQ_ERROR_CLEANUP(api_name, var, ref, error, freeResource) \
{\
	if ((var) != (ref)) {\
		printf("\n[%s] FAIL [Line : %d] %s : API returned = %d, Values (%s == 0x%x) and (%s == 0x%x) are not equal\n", __func__, __LINE__, api_name, error, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_EQ_RETURN(api_name, var, ref, ret) \
{ \
	if ((var) != (ref)) { \
		printf("\n[%s] FAIL [Line : %d] %s : Values (%s == 0x%x) and (%s == 0x%x) are not equal\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		return ret; \
	} \
}

#define TC_ASSERT_EQ(api_name, var, ref) TC_ASSERT_EQ_RETURN(api_name, var, ref, )

#define TC_ASSERT_NEQ_CLEANUP(api_name, var, ref, freeResource) \
{\
	if ((var) == (ref)) {\
		printf("\n[%s] FAIL [Line : %d] %s : Values (%s == 0x%x) and (%s == 0x%x) are equal\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_NEQ(api_name, var, ref) TC_ASSERT_NEQ_CLEANUP(api_name, var, ref, )

#define TC_ASSERT_GT_CLEANUP(api_name, var, ref, freeResource) \
{\
	if ((var) <= (ref)) {\
		printf("\n[%s] FAIL [Line : %d] %s : Values (%s == 0x%x) is not greater than (%s == 0x%x)\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_GT(api_name, var, ref) TC_ASSERT_GT_CLEANUP(api_name, var, ref, )

#define TC_ASSERT_GEQ_CLEANUP(api_name, var, ref, freeResource) \
{\
	if ((var) < (ref)) {\
		printf("\n[%s] FAIL [Line : %d] %s : Values (%s == 0x%x) is not greater than or equal to (%s == 0x%x)\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_GEQ(api_name, var, ref) TC_ASSERT_GEQ_CLEANUP(api_name, var, ref, )

#define TC_ASSERT_LT_CLEANUP(api_name, var, ref, freeResource) \
{\
	if ((var) >= (ref)) {\
		printf("\n[%s] FAIL [Line : %d] %s : Values (%s == 0x%x) is not lower than (%s == 0x%x)\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_LT(api_name, var, ref) TC_ASSERT_LT_CLEANUP(api_name, var, ref, )

#define TC_ASSERT_LEQ_CLEANUP(api_name, var, ref, freeResource) \
{\
	if ((var) > (ref)) {\
		printf("\n[%s] FAIL [Line : %d] %s : Values (%s == 0x%x) is not lower than or equal to (%s == 0x%x)\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_LEQ(api_name, var, ref) TC_ASSERT_LEQ_CLEANUP(api_name, var, ref, )

#define TC_SUCCESS_RESULT() \
{\
	printf("\n[%s] PASS \n", __func__); \
	total_pass++; \
}

#define TC_FREE_MEMORY(buffer) \
{\
	if ((buffer) != NULL) {\
		free(buffer); \
		buffer = NULL; \
	} \
}

#ifdef __cplusplus
extern "C" {
#endif

int tc_handler(tc_op_type_t type, const char *tc_name);

#ifdef __cplusplus
}
#endif

#endif
