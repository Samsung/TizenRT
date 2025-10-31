/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SPM_FUNC_H__
#define __SPM_FUNC_H__

#include <stdint.h>
#include "fih.h"
#include "spm_partition_defs.h"
#include "tfm_arch.h"
#include "psa/client.h"
#include "tfm_api.h"

#define SPM_PARTITION_STATE_UNINIT       0
#define SPM_PARTITION_STATE_IDLE         1
#define SPM_PARTITION_STATE_RUNNING      2
#define SPM_PARTITION_STATE_HANDLING_IRQ 3
#define SPM_PARTITION_STATE_SUSPENDED    4
#define SPM_PARTITION_STATE_BLOCKED      5
#define SPM_PARTITION_STATE_CLOSED       6

#define EXC_NUM_THREAD_MODE             (0)
#define EXC_NUM_SVCALL                  (11)
#define EXC_NUM_PENDSV                  (14)
#define EXC_NUM_SYSTICK                 (15)

#define SPM_INVALID_PARTITION_IDX       (~0U)

/* Privileged definitions for partition thread mode */
#define TFM_PARTITION_UNPRIVILEGED_MODE 0
#define TFM_PARTITION_PRIVILEGED_MODE   1

#define SPM_PART_FLAG_APP_ROT           0x01
#define SPM_PART_FLAG_PSA_ROT           0x02
#define SPM_PART_FLAG_IPC               0x04

#define TFM_PRIORITY_HIGH               0
#define TFM_PRIORITY_NORMAL             0x7F
#define TFM_PRIORITY_LOW                0xFF
#define TFM_PRIORITY(LEVEL)             TFM_PRIORITY_##LEVEL

enum spm_err_t {
    SPM_ERR_OK = 0,
    SPM_ERR_PARTITION_DB_NOT_INIT = 0x3A5C,
    SPM_ERR_PARTITION_ALREADY_ACTIVE = 0x5C3A,
    SPM_ERR_PARTITION_NOT_AVAILABLE = 0xA35C,
    SPM_ERR_INVALID_PARAMETER = 0xCA35,
    SPM_ERR_INVALID_CONFIG = 0x35A3C,
    SPM_ERR_GENERIC_ERR = 0x5C3A5,
};

/**
 * \brief Holds the iovec parameters that are passed to a service
 *
 * \note The size of the structure is (and have to be) multiple of 8 bytes
 */
struct iovec_args_t {
    psa_invec in_vec[PSA_MAX_IOVEC];   /*!< Array of psa_invec objects */
    size_t in_len;                     /*!< Number psa_invec objects in in_vec
                                        */
    psa_outvec out_vec[PSA_MAX_IOVEC]; /*!< Array of psa_outvec objects */
    size_t out_len;                    /*!< Number psa_outvec objects in out_vec
                                        */
};

/* The size of this struct must be multiple of 4 bytes as it is stacked to an
 * uint32_t[] array
 */
struct interrupted_ctx_stack_frame_t {
    uint32_t partition_state;
};

/* The size of this struct must be multiple of 4 bytes as it is stacked to an
 * uint32_t[] array
 */
struct handler_ctx_stack_frame_t {
    uint32_t partition_state;
    uint32_t caller_partition_idx;
};

/**
 * \brief Runtime context information of a partition
 */
struct spm_partition_runtime_data_t {
    uint32_t partition_state;
    uint32_t caller_partition_idx;
    int32_t caller_client_id;
    uint32_t stack_ptr;
    uint32_t lr;
    struct iovec_args_t iovec_args;
    psa_outvec *orig_outvec;
    uint32_t *ctx_stack_ptr;
    uint32_t signal_mask;               /*
                                         * Service signal mask passed by
                                         * psa_wait()
                                         */
};

/**
 * Holds the fields of the partition DB used by the SPM code. The values of
 * these fields are calculated at compile time, and set during initialisation
 * phase.
 */
struct spm_partition_static_data_t {
    uint32_t partition_id;
    uint32_t partition_flags;
    uint32_t partition_priority;
    sp_entry_point partition_init;
    uint32_t dependencies_num;
    int32_t *p_dependencies;
};

/**
 * Holds the fields that define a partition for SPM. The fields are further
 * divided to structures, to keep the related fields close to each other.
 */
struct spm_partition_desc_t {
    struct spm_partition_runtime_data_t runtime_data;
    const struct spm_partition_static_data_t *static_data;
    /** A list of platform_data pointers */
    const struct platform_data_t **platform_data_list;
};

struct spm_partition_db_t {
    uint32_t is_init;
    uint32_t partition_count;
    uint32_t running_partition_idx;
    struct spm_partition_desc_t *partitions;
};

/**
 * \brief Save interrupted partition context on ctx stack
 *
 * \param[in] partition_idx  Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function doesn't whether the ctx stack overflows.
 */
void tfm_spm_partition_push_interrupted_ctx(uint32_t partition_idx);

/**
 * \brief Restores interrupted partition context on ctx stack
 *
 * \param[in] partition_idx  Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function doesn't whether the ctx stack underflows.
 */
void tfm_spm_partition_pop_interrupted_ctx(uint32_t partition_idx);

/**
 * \brief Save handler partition context on ctx stack
 *
 * \param[in] partition_idx  Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function doesn't whether the ctx stack overflows.
 */
void tfm_spm_partition_push_handler_ctx(uint32_t partition_idx);

/**
 * \brief Restores handler partition context on ctx stack
 *
 * \param[in] partition_idx  Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function doesn't whether the ctx stack underflows.
 */
void tfm_spm_partition_pop_handler_ctx(uint32_t partition_idx);

/**
 * \brief Get the current runtime data of a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return The runtime data of the specified partition
 *
 * \note This function doesn't check if partition_idx is valid.
 */
const struct spm_partition_runtime_data_t *
             tfm_spm_partition_get_runtime_data(uint32_t partition_idx);

/**
 * \brief Returns the index of the partition that has running state
 *
 * \return The index of the partition with the running state, if there is any
 *         set. 0 otherwise.
 */
uint32_t tfm_spm_partition_get_running_partition_idx(void);

/**
 * \brief Save stack pointer and link register for partition in database
 *
 * \param[in] partition_idx  Partition index
 * \param[in] stack_ptr      Stack pointer to be stored
 * \param[in] lr             Link register to be stored
 *
 * \note This function doesn't check if partition_idx is valid.
 */
void tfm_spm_partition_store_context(uint32_t partition_idx, uint32_t stack_ptr,
                                     uint32_t lr);

/**
 * \brief Set the current state of a partition
 *
 * \param[in] partition_idx  Partition index
 * \param[in] state          The state to be set
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note The state has to have the value set of \ref spm_part_state_t.
 */
void tfm_spm_partition_set_state(uint32_t partition_idx, uint32_t state);

/**
 * \brief Set the caller partition index for a given partition
 *
 * \param[in] partition_idx        Partition index
 * \param[in] caller_partition_idx The index of the caller partition
 *
 * \note This function doesn't check if any of the partition_idxs are valid.
 */
void tfm_spm_partition_set_caller_partition_idx(uint32_t partition_idx,
                                                uint32_t caller_partition_idx);

/**
 * \brief Set the caller client ID for a given partition
 *
 * \param[in] partition_idx        Partition index
 * \param[in] caller_client_id     The ID of the calling client
 *
 * \note This function doesn't check if any of the partition_idxs are valid.
 */
void tfm_spm_partition_set_caller_client_id(uint32_t partition_idx,
                                            int32_t caller_client_id);

/**
 * \brief Execute partition init function
 *
 * \return Error code \ref spm_err_t.
 *         When FIH_ENABLE_DOUBLE_VARS is enabled, the return code will be
 *         wrapped and protected in \ref fih_int structure.
 */
fih_int tfm_spm_partition_init(void);

/**
 * \brief Clears the context info from the database for a partition.
 *
 * \param[in] partition_idx     Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 */
void tfm_spm_partition_cleanup_context(uint32_t partition_idx);

/**
 * \brief Set the signal mask for a given partition
 *
 * \param[in] partition_idx        Partition index
 * \param[in] signal_mask          The signal mask to be set for the partition
 *
 * \note This function doesn't check if any of the partition_idxs are valid.
 */
void tfm_spm_partition_set_signal_mask(uint32_t partition_idx,
                                       uint32_t signal_mask);

/**
 * \brief Signal that secure partition initialisation is finished
 */
void tfm_spm_secure_api_init_done(void);

/**
 * \brief Called for requests or returns from partition
 */
void tfm_spm_partition_request_return_handler(
        const uint32_t *svc_args, uint32_t exc_return, uint32_t *msp);

/**
 * \brief Called when SPM has completed a partition request or return
 */
void tfm_spm_partition_completion_handler(enum tfm_status_e res,
                                          uint32_t exc_return,
                                          uint32_t *msp);
/**
 * \brief Stores caller's client id in state context
 */
void tfm_spm_get_caller_client_id_handler(uint32_t *svc_args);

/**
 * \brief Checks if a secure service's access to a memory location is permitted
 */
void tfm_spm_memory_permission_check_handler(uint32_t *svc_args);

/**
 * \brief Check whether a buffer is ok for writing to by the privileged API
 *        function.
 *
 * This function checks whether the caller partition owns the buffer, can write
 * to it, and the buffer has proper alignment.
 *
 * \param[in] partition_idx     Partition index
 * \param[in] start_addr        The start address of the buffer
 * \param[in] len               The length of the buffer
 * \param[in] alignment         The expected alignment (in bits)
 *
 * \return TFM_SUCCESS on successful return, an error code otherwise
 *
 * \note For a zero length buffer the check fails.
 */
int32_t tfm_spm_check_buffer_access(uint32_t  partition_idx,
                                    void     *start_addr,
                                    size_t    len,
                                    uint32_t  alignment);

/**
 * \brief Handle deprivileged request
 */
extern uint32_t tfm_spm_depriv_req_handler(uint32_t *svc_args,
                                           uint32_t excReturn);

/**
 * \brief Handle request to return to privileged
 */
uint32_t tfm_spm_depriv_return_handler(uint32_t *irq_svc_args, uint32_t lr);

/**
 * \brief Handle IRQ enable request
 */
void tfm_spm_enable_irq_handler(uint32_t *svc_args);

/**
 * \brief Handle IRQ disable request
 */
void tfm_spm_disable_irq_handler(uint32_t *svc_args);

/**
 * \brief Handle signal wait request
 */
void tfm_spm_psa_wait(uint32_t *svc_args);

/**
 * \brief Handle request to record IRQ processed
 */
void tfm_spm_psa_eoi(uint32_t *svc_args);

/**
 * \brief Get the id of the partition for its index from the db
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Partition ID for that partition
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_partition_id(uint32_t partition_idx);

/**
 * \brief Initialize partition database
 *
 * \return Error code \ref spm_err_t
 */
enum spm_err_t tfm_spm_db_init(void);

/**
 * \brief                   Get the current partition mode.
 *
 * \param[in] partition_flags               Flags of current partition
 *
 * \retval TFM_PARTITION_PRIVILEGED_MODE    Privileged mode
 * \retval TFM_PARTITION_UNPRIVILEGED_MODE  Unprivileged mode
 */
uint32_t tfm_spm_partition_get_privileged_mode(uint32_t partition_flags);

/**
 * \brief                   Function to seal the PSP stacks for Function mode.
 */
void tfm_spm_seal_psp_stacks(void);

/**
 * \brief Get the flags associated with a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Flags associated with the partition
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_flags(uint32_t partition_idx);

#endif /* __SPM_FUNC_H__ */
