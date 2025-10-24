/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __PSA_INTERFACE_REDIRECT_H__
#define __PSA_INTERFACE_REDIRECT_H__

#include "psa/framework_feature.h"

#if CONFIG_TFM_PSA_API_SUPERVISOR_CALL == 1

#define psa_framework_version    psa_framework_version_svc
#define psa_version              psa_version_svc
#define tfm_psa_call_pack        tfm_psa_call_pack_svc
#define psa_wait                 psa_wait_svc
#define psa_get                  psa_get_svc
#define psa_read                 psa_read_svc
#define psa_skip                 psa_skip_svc
#define psa_write                psa_write_svc
#define psa_reply                psa_reply_svc
#define psa_panic                psa_panic_svc
#define psa_rot_lifecycle_state  psa_rot_lifecycle_state_svc

#if CONFIG_TFM_DOORBELL_API == 1
#define psa_notify               psa_notify_svc
#define psa_clear                psa_clear_svc
#endif /* CONFIG_TFM_DOORBELL_API == 1 */

/* Following PSA APIs are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
#define psa_connect              psa_connect_svc
#define psa_close                psa_close_svc
#define psa_set_rhandle          psa_set_rhandle_svc
#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */

#if CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1
#define psa_irq_enable           psa_irq_enable_svc
#define psa_irq_disable          psa_irq_disable_svc
/* This API is only used for FLIH. */
#if CONFIG_TFM_FLIH_API == 1
#define psa_reset_signal         psa_reset_signal_svc
#endif
/* This API is only used for SLIH. */
#if CONFIG_TFM_SLIH_API == 1
#define psa_eoi                  psa_eoi_svc
#endif
#endif /* CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1 */

#elif CONFIG_TFM_PSA_API_CROSS_CALL == 1

#define psa_framework_version    psa_framework_version_cross
#define psa_version              psa_version_cross
#define tfm_psa_call_pack        tfm_psa_call_pack_cross
#define psa_wait                 psa_wait_cross
#define psa_get                  psa_get_cross
#define psa_read                 psa_read_cross
#define psa_skip                 psa_skip_cross
#define psa_write                psa_write_cross
#define psa_reply                psa_reply_cross
#define psa_panic                psa_panic_cross
#define psa_rot_lifecycle_state  psa_rot_lifecycle_state_cross

#if CONFIG_TFM_DOORBELL_API == 1
#define psa_notify               psa_notify_cross
#define psa_clear                psa_clear_cross
#endif /* CONFIG_TFM_DOORBELL_API == 1 */

/* Following PSA APIs are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
#define psa_connect              psa_connect_cross
#define psa_close                psa_close_cross
#define psa_set_rhandle          psa_set_rhandle_cross
#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */

#if CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1
#define psa_irq_enable           psa_irq_enable_cross
#define psa_irq_disable          psa_irq_disable_cross
/* This API is only used for FLIH. */
#if CONFIG_TFM_FLIH_API == 1
#define psa_reset_signal         psa_reset_signal_cross
#endif
/* This API is only used for SLIH. */
#if CONFIG_TFM_SLIH_API == 1
#define psa_eoi                  psa_eoi_cross
#endif
#endif /* CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1 */

#if PSA_FRAMEWORK_HAS_MM_IOVEC
#define psa_map_invec            psa_map_invec_cross
#define psa_unmap_invec          psa_unmap_invec_cross
#define psa_map_outvec           psa_map_outvec_cross
#define psa_unmap_outvec         psa_unmap_outvec_cross
#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC */

#elif CONFIG_TFM_PSA_API_SFN_CALL == 1

#define psa_framework_version    psa_framework_version_sfn
#define psa_version              psa_version_sfn
#define tfm_psa_call_pack        psa_call_pack_sfn
#define psa_read                 psa_read_sfn
#define psa_skip                 psa_skip_sfn
#define psa_write                psa_write_sfn
#define psa_panic                psa_panic_sfn
/* Following PSA APIs are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
#define psa_connect              psa_connect_sfn
#define psa_close                psa_close_sfn
#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */
#if CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1
#define psa_wait                 psa_wait_sfn
#define psa_irq_enable           psa_irq_enable_sfn
#define psa_irq_disable          psa_irq_disable_sfn
/* This API is only used for FLIH. */
#if CONFIG_TFM_FLIH_API == 1
#define psa_reset_signal         psa_reset_signal_sfn
#endif
/* This API is only used for SLIH. */
#if CONFIG_TFM_SLIH_API == 1
#define psa_eoi                  psa_eoi_sfn
#endif
#endif /* CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1 */

#if PSA_FRAMEWORK_HAS_MM_IOVEC
#define psa_map_invec            psa_map_invec_sfn
#define psa_unmap_invec          psa_unmap_invec_sfn
#define psa_map_outvec           psa_map_outvec_sfn
#define psa_unmap_outvec         psa_unmap_outvec_sfn
#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC */

#endif

#endif /* __PSA_INTERFACE_REDIRECT_H__ */
