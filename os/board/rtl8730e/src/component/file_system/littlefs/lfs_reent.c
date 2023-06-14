#include "FreeRTOS.h"
#include "task.h"
#include <cmsis.h>
#include "task.h"
#include <stdint.h>
#include "section_config.h"
#include "lfs.h"
#include "lfs_util.h"
#include "ftl_common_api.h"
#include "lfs_reent.h"
_mutex lfs_mutex = NULL;
int lfs_system_lock(const struct lfs_config *c)
{
	if (lfs_mutex == NULL) {
		printf("lfs_mutex init\r\n");
		memset(&lfs_mutex, 0, sizeof(lfs_mutex));
		rtw_mutex_init(&lfs_mutex);
		rtw_mutex_get(&lfs_mutex);
	} else {
		rtw_mutex_get(&lfs_mutex);
	}
	return 0;
}

int lfs_system_unlock(const struct lfs_config *c)
{
	if (lfs_mutex == NULL) {
		printf("lfs_mutex init\r\n");
		memset(&lfs_mutex, 0, sizeof(lfs_mutex));
		rtw_mutex_init(&lfs_mutex);
		rtw_mutex_put(&lfs_mutex);
	} else {
		rtw_mutex_put(&lfs_mutex);
	}
	return 0;
}