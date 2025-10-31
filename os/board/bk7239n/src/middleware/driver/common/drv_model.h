#ifndef _DRV_MODEL_H_
#define _DRV_MODEL_H_

#include "legacy/drv_model_pub.h"

#define DD_MAX_DEV              (DD_DEV_TYPE_END & 0xFFFF)

#define DD_MAX_DEV_MASK         (DD_MAX_DEV - 1)

#define DD_MAX_NAME_LEN         (16)

typedef enum _dd_state_ {
	DD_STATE_NODEVICE = 0,	// find no such device when you open
	DD_STATE_CLOSED,		//
	DD_STATE_OPENED,		//
	DD_STATE_BREAK,			//
	DD_STATE_SUCCESS		//
} DD_STATE;

typedef struct _drv_dev_ {
	//    char *name;
	UINT32 use_cnt;

	DD_STATE state;
	DD_OPERATIONS *op;
	DD_OPEN_METHOD method;

	void *private;
} DRV_DEV_S, *DRV_DEV_PTR;

#endif // _DRV_MODEL_H_

