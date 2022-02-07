#ifndef _TIZENRT_ETHERNET_H__
#define _TIZENRT_ETHERNET_H__

struct netdev;
struct ethernet_ops {
	int (*init)(struct netdev *dev);
	int (*deinit)(struct netdev *dev);
	int (*enable)(struct netdev *dev);
	int (*disable)(struct netdev *dev);
};

#endif // _TIZENRT_ETHERNET_H__
