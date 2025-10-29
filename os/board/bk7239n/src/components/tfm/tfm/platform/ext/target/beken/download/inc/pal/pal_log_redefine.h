#include "pal_log.h"

//Important notice:
//This header redefine the PAL_LOG_ERR to reduce code size,
//don't include it in any other headers and use it carefully!

#if CONFIG_REDUCE_CODE_SIZE
#include <stdio.h>
#undef PAL_LOG_ERR
#define PAL_LOG_ERR(...) printf("%s %d\n", __FILE__, __LINE__)
#endif
