#ifndef _LFS_REENT_H_
#define _LFS_REENT_H_
#include "lfs.h"
#include "lfs_util.h"
int lfs_system_lock(const struct lfs_config *c);
int lfs_system_unlock(const struct lfs_config *c);
#endif