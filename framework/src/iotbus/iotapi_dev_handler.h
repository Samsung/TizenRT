#ifndef _IOTAPI_DEV_HANDLER_H__
#define _IOTAPI_DEV_HANDLER_H__

typedef struct _iotapi_dev_ctx_s *iotapi_hnd;

typedef int evttype;
typedef void (*iotapi_cbk)(evttype evt);

int iotapi_dev_init(iotapi_hnd *hnd);
int iotapi_dev_deinit(iotapi_hnd hnd);
int iotapi_dev_register(iotapi_hnd hnd, iotapi_cbk cbk);
int iotapi_dev_unregister(iotapi_hnd hnd);

#endif // _IOTAPI_DEV_HANDLER_H__
