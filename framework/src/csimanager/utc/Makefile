UTC_TYPE=CHECK
ROOTDIR=$(abspath ../../..)
ROOTDIR_CSIPLATFORM=$(ROOTDIR)
include utcmk.defs

OBJS += utc.c
OBJS += utc_csimanager.c
OBJS += utc_csinetworkmonitor.c

CFLAGS += -Wl,--wrap=getaddrinfo,--wrap=netlib_get_ipv4_gateway_addr,--wrap=freeaddrinfo
CFLAGS += -Wl,--wrap=socket,--wrap=setsockopt,--wrap=sendto
CFLAGS += -Wl,--wrap=ioctl,--wrap=mq_receive,--wrap=mq_open,--wrap=mq_close
CFLAGS += -Wl,--wrap=wifi_manager_register_cb,--wrap=wifi_manager_unregister_cb,--wrap=wifi_manager_get_info
CFLAGS += -Wl,--wrap=task_manager_register_task,--wrap=task_manager_start,--wrap=task_manager_unregister
CFLAGS += -Wl,--wrap=task_manager_register_pthread

TRG_SRC += $(wildcard ../src/*.c)
