#ifndef __FMAC_MSG_H_
#define __FMAC_MSG_H_

#include <stdint.h>
#include <stdbool.h>
#include "co_list.h"
#include "co_bit.h"

#ifndef __INLINE
#define __INLINE static __attribute__((__always_inline__)) inline
#endif

#include "rwnx_config.h"
#include "fhost_mac.h"
typedef uint32_t evt_field_t;
#include "generated/lmac_msg.h"

#endif
