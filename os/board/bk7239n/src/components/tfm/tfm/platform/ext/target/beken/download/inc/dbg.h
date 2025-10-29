/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __DBG_H__
#define __DBG_H__

#ifndef __ASSEMBLY__

#ifdef __cplusplus
extern "C" {
#endif

/* TODO */
#define back_trace

typedef enum _dbb_level {
    _DBG = 0,
    _INFO,
    _ERR,
} dbg_level;

#define CUR_DBG_LEVEL _INFO

#ifndef DBG_PRE_PROMPT
#warning "DBG_PRE_PROMPT is undefined!!!"
#define DBG_PRE_PROMPT     "DPP"
#endif
#define _PROMPT DBG_PRE_PROMPT": "

extern int printf (const char *__format, ...);

#define DBG(fmt, ...)                                                          \
    do {                                                                       \
        if (_DBG >= CUR_DBG_LEVEL) {                                           \
            printf(_PROMPT fmt, ##__VA_ARGS__);                                \
        }                                                                      \
    } while (0)

#define INFO(fmt, ...)                                                         \
    do {                                                                       \
        if (_INFO >= CUR_DBG_LEVEL) {                                          \
            printf(_PROMPT fmt, ##__VA_ARGS__);                                \
        }                                                                      \
    } while (0)

#define ERR(fmt, ...)                                                          \
    do {                                                                       \
        if (_ERR >= CUR_DBG_LEVEL) {                                           \
            printf(_PROMPT"ERROR " fmt, ##__VA_ARGS__);                        \
        }                                                                      \
    } while (0)

#define ASSERT_MSG(_x, fmt, ...)                                \
    do {                                                        \
        if (!(_x)) {                                            \
            ERR("ASSERT FAILURE:\n");                           \
            ERR("%s (%d): %s\n",                                \
                    __func__, __LINE__, __FUNCTION__);          \
            ERR(fmt, ##__VA_ARGS__);                            \
            back_trace;                                         \
            while (1) /* loop */;                               \
        }                                                       \
    } while (0)

#define ASSERT(_x)                                              \
    do {                                                        \
        if (!(_x)) {                                            \
            ERR("ASSERT FAILURE:\n");                           \
            ERR("%s (%d): %s\n",                                \
                    __func__, __LINE__, __FUNCTION__);          \
            back_trace;                                         \
            while (1) /* loop */;                               \
        }                                                       \
    } while (0)

#define CHECK_RET(format, ...)                                                 \
    do {                                                                       \
        if ((0) != (ret)) {                                                    \
            ERR("%s line:%d. Error number is 0x%x \n",                         \
                __func__,                                                      \
                __LINE__,                                                      \
                ret);                                                          \
            ERR(format, ##__VA_ARGS__);                                        \
            goto finish;                                                       \
        }                                                                      \
    } while (0)

#define CHECK_CONDITION(true_condition, ret_code, format, ...)                 \
    do {                                                                       \
        if (!(true_condition)) {                                               \
            ret = ret_code;                                                    \
            ERR("%s line:%d. Error number is 0x%x \n",                         \
                __func__,                                                      \
                __LINE__,                                                      \
                ret);                                                          \
            ERR(format, ##__VA_ARGS__);                                        \
            goto finish;                                                       \
        }                                                                      \
    } while (0)

static inline void dump_data(char *prefix, unsigned char *data, unsigned int size)
{
#if CONFIG_DEBUG
    unsigned int i;

    printf("%s", prefix);
    printf(" :\n");

    for (i=0; i<size; i++) {
        if ((i&0xf) == 0) {
            printf("\n");
        }
        printf("0x%02x", data[i]);
        printf(" ");
    }
    printf("\n");
#endif
}

#ifdef __cplusplus
}
#endif

#endif /* __ASSEMBLY__ */

#endif /* __DBG_H__ */
