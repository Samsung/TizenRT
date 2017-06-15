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

int total_pass;
int total_fail;

#define TC_ASSERT_CLEANUP(api_name, var, error, freeResource) \
{\
	if (!(var)) {\
		printf("\n[%s][Line : %d] FAIL, %s : API error returned = %s [%s]\n", __func__, __LINE__, api_name, error, #var); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT(api_name, var) \
{\
	if (!(var)) {\
		printf("\n[%s][Line : %d] FAIL, %s : Following expression is not true: %s\n", __func__, __LINE__, api_name, #var); \
		total_fail++; \
		return; \
	} \
}

#define TC_ASSERT_EQ_CLEANUP(api_name, var, ref, error, freeResource) \
{\
	if ((var) != (ref)) {\
		printf("\n[%s][Line : %d] FAIL, %s : API error returned = %s [%d] ", __func__, __LINE__, api_name, error, #var); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_EQ(api_name, var, ref) \
{\
	if ((var) != (ref)) {\
		printf("\n[%s][Line : %d] FAIL, %s : Values (%s == 0x%x) and (%s == 0x%x) are not equal\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		return; \
	} \
}

#define TC_ASSERT_NEQ_CLEANUP(api_name, var, ref, error, freeResource) \
{\
	if ((var) == (ref)) {\
		printf("\n[%s][Line : %d] FAIL, %s : API error returned = %s [%d] ", __func__, __LINE__, api_name, error, #var); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_NEQ(api_name, var, ref) \
{\
	if ((var) == (ref)) {\
		printf("\n[%s][Line : %d] FAIL, %s : Values (%s == 0x%x) and (%s == 0x%x) are equal\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		return; \
	} \
}

#define TC_ASSERT_GT_CLEANUP(api_name, var, ref, error, freeResource) \
{\
	if ((var) <= (ref)) {\
		printf("\n[%s][Line : %d] FAIL, %s : Values (%s == 0x%x) is not greater than (%s == 0x%x)\n", __func__, __LINE__, api_name, error, #var); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_GT(api_name, var, ref) \
{\
	if ((var) <= (ref))    {\
		printf("\n[%s][Line : %d] FAIL, %s : Values (%s == 0x%x) is not greater than (%s == 0x%x)\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		return; \
	} \
}

#define TC_ASSERT_GEQ_CLEANUP(api_name, var, ref, error, freeResource) \
{\
	if ((var) < (ref)) {\
		printf("\n[%s][Line : %d] FAIL, %s : Values (%s == 0x%x) is not greater than or euqal to (%s == 0x%x)\n", __func__, __LINE__, api_name, error, #var); \
		total_fail++; \
		freeResource; \
		return; \
	} \
}

#define TC_ASSERT_GEQ(api_name, var, ref) \
{\
	if ((var) < (ref)) {\
		printf("\n[%s][Line : %d] FAIL, %s : Values (%s == 0x%x) is not greater than or equal to (%s == 0x%x)\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		return; \
	} \
}

#define TC_ASSERT_LT(api_name, var, ref) \
{\
	if ((var) >= (ref)) {\
		printf("\n[%s][Line : %d] FAIL, %s : Values (%s == 0x%x) is not lower than (%s == 0x%x)\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		return; \
	} \
}

#define TC_ASSERT_LEQ(api_name, var, ref) \
{\
	if ((var) > (ref)) {\
		printf("\n[%s][Line : %d] FAIL, %s : Values (%s == 0x%x) is not lower than or equal to (%s == 0x%x)\n", __func__, __LINE__, api_name, #var, (int)(var), #ref, (int)(ref)); \
		total_fail++; \
		return; \
	} \
}

#define TC_ASSERT_NOT_NULL(api_name, handle) \
{\
	if ((handle) == NULL) {\
		printf("\n[%s][Line : %d] FAIL , %s : API returned NULL ", __func__, __LINE__, api_name); \
		total_fail++; \
		return; \
	} \
}

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

#endif
