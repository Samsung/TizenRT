#include <stdio.h>
#include <pthread.h>
#include "ss_mutex.h"
#include "things_malloc.h"
#include "things_logger.h"

#define TAG ("SS_MUTEX")

ss_mutex_t *ss_mutex_new(void)
{
	ss_mutex_t *mutex_info = (ss_mutex_t *) things_malloc(sizeof(ss_mutex_t));
	if (NULL != mutex_info) {
		// create the mutex with the attributes set
		int ret = pthread_mutex_init(&(mutex_info->mutex), PTHREAD_MUTEX_DEFAULT);
		if (0 != ret) {
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s Failed to initialize mutex : %d", __func__, ret);
			things_free(mutex_info);
			mutex_info = NULL;
		}
	} else {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s Failed to allocate mutex!", __func__);
	}
	return mutex_info;
}

bool ss_mutex_free(ss_mutex_t *mutex_info)
{
	bool bRet = false;
	if (mutex_info) {
		int ret = pthread_mutex_destroy(&mutex_info->mutex);
		if (0 == ret) {
			things_free(mutex_info);
			bRet = true;
		} else {
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s Failed to free mutex : %d", __func__, ret);
		}
	} else {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s Invalid mutex !", __func__);
	}
	return bRet;
}

void ss_mutex_lock(ss_mutex_t *mutex_info)
{
	if (mutex_info) {
		int ret = pthread_mutex_lock(&mutex_info->mutex);
		if (ret != 0) {
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Pthread Mutex lock failed: %d", ret);
		}
	} else {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s Invalid mutex !", __func__);
	}
}

void ss_mutex_unlock(ss_mutex_t *mutex_info)
{
	if (mutex_info) {
		int ret = pthread_mutex_unlock(&mutex_info->mutex);
		if (ret != 0) {
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Pthread Mutex unlock failed: %d", ret);
		}
	} else {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s: Invalid mutex !", __func__);
	}
}
