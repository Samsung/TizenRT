#ifndef _SECLINK_DRV_H__
#define _SECLINK_DRV_H__

#include <stdint.h>
#include <semaphore.h>

#define SECLINK_PATH "/dev/seclink"

struct sec_lowerhalf_s;
struct sec_upperhalf_s {
	struct sec_lowerhalf_s *lower;
	char *path;
	int32_t refcnt;
	sem_t su_lock;
};

struct sec_ops_s;
struct sec_lowerhalf_s {
	struct sec_ops_s *ops;
	struct sec_upperhalf_s *parent;
};

int se_register(const char *path, struct sec_lowerhalf_s *lower);

int se_unregister(struct sec_lowerhalf_s *lower);

#endif // _SECLINK_DRV_H__
