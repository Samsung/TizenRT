#ifndef __PING_GENERATOR_H__
#define __PING_GENERATOR_H__
#include "csifw/csifw.h"

CSIFW_RES ping_generator_change_interval(unsigned int pingInterval);
CSIFW_RES ping_generator_start(void);
CSIFW_RES ping_generator_stop(CSIFW_REASON reason);

#endif /* __PING_GENERATOR_H__ */
