/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * This is a simple implementaton for reference to assign NSID from NS side.
 * NSIDs of specific threads are statically pre-assigned.
 * Other threads use default NSID value -1.
 *
 * Developers can design the assignment according to RTOS and usage scenarios.
 * The assignment can be static or dynamic.
 */

#include <stdlib.h>
#include "tfm_nsid_manager.h"
#include "tfm_nsid_map_table.h"

/* Translation table pair between OS threads and NSIDs */
struct thread_test_nsid_pair {
    const char  *t_name;     /* Task/Thread name */
    int32_t     nsid;       /* NSID */
};

/* -1 is reserved for NSID as a default value */
static const struct thread_test_nsid_pair test_ns_nsid_table[] =
{
    {"Thread_A", -2},
    {"Thread_B", -3},
    {"Thread_C", -4},
    {"Thread_D", -5},
    {"seq_task", -6},
    {"mid_task", -7},
    {"pri_task", -8},
#ifdef PSA_API_TEST_NS
    {"psa_api_test", -9}
#endif
};

/*
 * Workaround: strcmp func in string.h would come into a runtime error
 * on AN521 with ARMCLANG compiler.
 */
static int str_cmp(const char* str_a, const char* str_b)
{
    int result = 0;
    uint32_t i = 0;

    while ((str_a[i] != '\0') || (str_b[i] != '\0')) {
        if (str_a[i] != str_b[i]) {
            result = 1;
            break;
        }
        i++;
    }

    return result;
}

int32_t nsid_mgr_get_thread_nsid(const char* t_name)
{
    uint32_t i;

    if (t_name == NULL) {
        return TFM_DEFAULT_NSID;
    }

    for (i = 0; i < ARRAY_SIZE(test_ns_nsid_table); i++) {
        if (str_cmp(test_ns_nsid_table[i].t_name, t_name) == 0) {
            return test_ns_nsid_table[i].nsid;
        }
    }

    /* Thread name not specified in the table, return default NSID */
    return TFM_DEFAULT_NSID;
}
