/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_HAL_ISOLATION_H__
#define __TFM_HAL_ISOLATION_H__

#include <stddef.h>
#include <stdint.h>
#include "tfm_hal_defs.h"
#include "load/partition_defs.h"
#include "load/asset_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Memory access attributes */
#define TFM_HAL_ACCESS_EXECUTABLE       (1UL << 0)
#define TFM_HAL_ACCESS_READABLE         (1UL << 1)
#define TFM_HAL_ACCESS_WRITABLE         (1UL << 2)
#define TFM_HAL_ACCESS_UNPRIVILEGED     (1UL << 3)
#define TFM_HAL_ACCESS_DEVICE           (1UL << 4)
#define TFM_HAL_ACCESS_NS               (1UL << 5)

#ifdef TFM_FIH_PROFILE_ON
#include "fih.h"
/**
 * \brief  Sets up the static isolation boundaries which are constant throughout
 *         the runtime of the system, including the SPE/NSPE and partition
 *         boundaries.
 *
 * \return TFM_HAL_SUCCESS - the isolation boundaries have been set up.
 *         TFM_HAL_ERROR_GENERIC - failed to set up the isolation boundaries.
 */
fih_int tfm_hal_set_up_static_boundaries(void);

/**
 * \brief This function is responsible for checking all critical isolation
          configurations.
 *
 * \return TFM_HAL_SUCCESS - the verification passed.
 *         TFM_HAL_ERROR_GENERIC - the verification failed.
 */
fih_int tfm_hal_verify_static_boundaries(void);

/**
 * \brief  Update the isolation boundaries.
 *
 * \param[in]   p_ldinf         Partition load information.
 * \param[in]   p_boundaries    Platform boundary handle for partition.
 *
 * \return TFM_HAL_SUCCESS          The isolation boundaries update succeeded.
 *         TFM_HAL_ERROR_GENERIC    Failed to update the isolation boundaries.
 *
 * \note   When FIH_ENABLE_DOUBLE_VARS is enabled, the return code will be
 *         wrapped and protected in \ref fih_int structure.
 */
fih_int tfm_hal_update_boundaries(const struct partition_load_info_t *p_ldinf,
                                  void *p_boundaries);
#else /* TFM_FIH_PROFILE_ON */
/**
 * \brief  Sets up the static isolation boundaries which are constant throughout
 *         the runtime of the system, including the SPE/NSPE and partition
 *         boundaries.
 *
 * \return TFM_HAL_SUCCESS - the isolation boundaries have been set up.
 *         TFM_HAL_ERROR_GENERIC - failed to set up the isolation boundaries.
 */
enum tfm_hal_status_t tfm_hal_set_up_static_boundaries(void);

/**
 * \brief  Update the isolation boundaries.
 *
 * \param[in]   p_ldinf         Partition load information.
 * \param[in]   p_boundaries    Platform boundary handle for partition.
 *
 * \return TFM_HAL_SUCCESS          The isolation boundaries update succeeded.
 *         TFM_HAL_ERROR_GENERIC    Failed to update the isolation boundaries.
 */
enum tfm_hal_status_t tfm_hal_update_boundaries(
                            const struct partition_load_info_t *p_ldinf,
                            void *p_boundaries);
#endif /* TFM_FIH_PROFILE_ON */

/**
 * \brief  This API checks if the memory region defined by base and size
 *         matches the given attributes - attr.
 *         The attributes can include NSPE access, privileged mode, and
 *         read-write permissions.
 *
 * \param[in]   base    The base address of the region.
 * \param[in]   size    The size of the region.
 * \param[in]   attr    The memory access attributes.
 *
 * \return TFM_HAL_SUCCESS - The memory region has the access permissions.
 *         TFM_HAL_ERROR_MEM_FAULT - The memory region has not the access
 *                                   permissions.
 *         TFM_HAL_ERROR_INVALID_INPUT - Invalid inputs.
 *         TFM_HAL_ERROR_GENERIC - An error occurred.
 */
enum tfm_hal_status_t tfm_hal_memory_has_access(uintptr_t base,
                                                size_t size,
                                                uint32_t attr);

/**
 * \brief  This API binds partition boundaries with the platform. The platform
 *         maintains the platform-specific settings for SPM further
 *         usage, such as update partition hardware boundaries or
 *         check resource accessibility. The platform needs to manage
 *         the settings with internal mechanism, and return a handle
 *         to SPM. SPM delivers this handle back to platform when
 *         necessary. And SPM checks this handle to decide if the
 *         platform-specific settings need to be updated. Hence
 *         multiple partitions can have the same handle if they have
 *         the same platform-specific settings, depending on isolation level.
 *
 * \param[in]   p_ldinf           Partition load information.
 * \param[in]   pp_boundaries     Pointer of the boundary handle
 *
 * \return TFM_HAL_SUCCESS          - A platform handle binding success.
 *         TFM_HAL_ERROR_GENERIC    - Error occured while binding.
 */
enum tfm_hal_status_t tfm_hal_bind_boundaries(
                                    const struct partition_load_info_t *p_ldinf,
                                    void **pp_boundaries);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_HAL_ISOLATION_H__ */
