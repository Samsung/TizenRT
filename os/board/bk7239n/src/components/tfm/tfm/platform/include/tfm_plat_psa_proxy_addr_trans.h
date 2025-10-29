/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLAT_PSA_PROXY_H__
#define __TFM_PLAT_PSA_PROXY_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Translates an address from Host to Secure Enclave
 *
 * \param[in]  addr    The address to be translated
 *
 * \return Returns the translated pointer
 */
void* translate_addr_from_host_to_se(void *addr);

/**
 * \brief Translates an address from Secure Enclave to Host
 *
 * \param[in]  addr    The address to be translated
 *
 * \return Returns the translated pointer
 */
void* translate_addr_from_se_to_host(void *addr);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLAT_PSA_PROXY_H__ */
