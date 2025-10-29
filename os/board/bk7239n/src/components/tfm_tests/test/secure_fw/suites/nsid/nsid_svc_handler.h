/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __NSID_SVC_HANDLER_H__
#define __NSID_SVC_HANDLER_H__

#include <stdint.h>
// #include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief SVC func to initialize ns client extension.
 *
 * \param [in] ctx_requested  Number of ns context to be requested.
 *
 * \return Returns number of context actually allocated by ns client ext.
 */
uint32_t nsid_ext_init_svc(uint32_t ctx_requested);

/**
 * \brief SVC func to acquire context from Secure side.
 *
 * \param [in] group_id  Group ID of NS client.
 * \param [in] thread_id Thread ID of NS client.
 *
 * \return Returns token gerenated by SPM.
 */
uint32_t nsid_acquire_ctx_svc(uint8_t group_id, uint8_t thread_id);

/**
 * \brief SVC func to release context from Secure side.
 *
 * \param [in] token  Token of NS client.
 *
 * \return Returns TF-M NSID Error code.
 */
uint32_t nsid_release_ctx_svc(uint32_t token);

/**
 * \brief SVC func to release context from Secure side.
 *
 * \param [in] token  Token of NS client.
 * \param [in] nsid   NSID of NS client.
 *
 * \return Returns TF-M NSID Error code.
 */
uint32_t nsid_load_ctx_svc(uint32_t token, int32_t nsid);

/**
 * \brief SVC func to save context from Secure side.
 *
 * \param [in] token  Token of NS client.
 *
 * \return Returns TF-M NSID Error code.
 */
uint32_t nsid_save_ctx_svc(uint32_t token);

#ifdef __cplusplus
}
#endif

#endif /* __NSID_SVC_HANDLER_H__ */
