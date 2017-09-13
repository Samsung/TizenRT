#ifndef _OICCOMMON_H_
#define _OICCOMMON_H_

#include "ocstack.h"

// #ifdef __cplusplus
// extern "C" {
// #endif // __cplusplus

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
//typedef unsigned char byte;


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

// 1. EntityHandler Callback (Used in the oicresource.c)
/* EntityHandler Callback */
typedef OCEntityHandlerResult (*RequestHandlerCB) (OCEntityHandlerFlag flag,
                        OCEntityHandlerRequest *entityHandlerRequest,
                        void* cb);

typedef enum {

    RST_COMPLETE                        = 0,

    RST_EASY_SETUP_DISABLE              = 1,

    RST_CLOUD_MODULE_DISABLE            = (1 << 1),

    RST_CONTROL_MODULE_DISABLE          = (1 << 2),

    RST_CONTROL_QUEUE_EMPTY             = (1 << 3),

    RST_NOTI_MODULE_DISABLE             = (1 << 4),

    RST_DELETE_CLOUD_DEVICE_SUCCESS     = (1 << 5),

    RST_DELETE_CLOUD_DEVICE_FAILURE     = (1 << 6),

    RST_USER_CONFIRM_COMPLETED          = (1 << 7),

    RST_ALL_FLAG                        = 0xFFFFFFFF

} RST_STATE;


//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
bool OICGetResetMask(RST_STATE value);

void OICControlQueueEmpty(void);

// #ifdef __cplusplus
// }
// #endif // __cplusplus

#endif // _OICCOMMON_H_