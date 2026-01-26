#ifndef PSA_SYSTEM_CLOCK_H
#define PSA_SYSTEM_CLOCK_H

#include <stddef.h>
#include <stdint.h>

#include "psa/error.h"
#include "tfm_system_clock_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

psa_status_t psa_system_power_ctrl(uint32_t module_id, uint32_t power_state);

psa_status_t psa_system_clock_set(uint32_t module_id, uint32_t clk_src, uint32_t clk_div);

psa_status_t psa_system_core_set(uint32_t system_operation_id);

#ifdef __cplusplus
}
#endif

#endif /* PSA_SYSTEM_CLOCK_H */
