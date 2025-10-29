/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_STRNLEN_H__
#define __TFM_STRNLEN_H__

/**
 * \brief Return the length of a given string, up to a maximum of maxlen bytes.
 *
 * \param[in]  s           Points to the string to be examined.
 * \param[in]  maxlen      The maximum number of characters to examine.
 */
size_t tfm_strnlen(const char *s, size_t maxlen);

#endif /* __TFM_STRNLEN_H__ */
