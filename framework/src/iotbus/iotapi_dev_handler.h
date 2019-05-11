#ifndef _IOTAPI_DEV_HANDLER_H__
#define _IOTAPI_DEV_HANDLER_H__

#include <tinyara/iotdev.h>

typedef struct _iotapi_dev_ctx_s *iotapi_hnd;

typedef id_evt_type evttype;
typedef void (*iotapi_cbk)(evttype evt);

int iotapi_dev_init(iotapi_hnd *hnd);
int iotapi_dev_deinit(iotapi_hnd hnd);
int iotapi_dev_register(iotapi_hnd hnd, evttype evt, iotapi_cbk cbk);
int iotapi_dev_unregister(iotapi_hnd hnd);

#endif // _IOTAPI_DEV_HANDLER_H__
