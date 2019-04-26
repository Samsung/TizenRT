/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/arch.h>
#include <tinyara/fs/fs.h>
#include <tinyara/sched.h>
#include <tinyara/lwnl/lwnl80211.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define DMA_BUFFER_MAX_SIZE 65536	/* 64K */

#define DMA_BUFFER_MIN_SIZE 4096	/* 4K */

#define MQ_REOPEN                                                                     \
	do {                                                                              \
		if (upper->usermq == NULL) {                                                  \
			struct mq_attr attr;                                                      \
			attr.mq_maxmsg = LWNL80211_MQUEUE_MAX_DATA_NUM;                           \
			attr.mq_msgsize = sizeof(lwnl80211_cb_data);                              \
			attr.mq_flags = 0;                                                        \
			attr.mq_curmsgs = 0;                                                      \
			upper->usermq = mq_open(upper->mqname, O_RDWR | O_CREAT, 0666, &attr);    \
			if (upper->usermq == NULL) {                                              \
				nldbg("Failed to open mq\n");                                 \
				ret = -ENOSYS;                                                        \
			} else {                                                                  \
				nldbg("Open mq with %s\n", upper->mqname);                    \
				ret = OK;                                                             \
			}                                                                         \
		}                                                                             \
	} while (0)

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct lwnl80211_upperhalf_s {
	uint8_t crefs;
	volatile bool started;
	sem_t exclsem;
	struct lwnl80211_lowerhalf_s *lower; /* Arch-specific operations */
	mqd_t usermq;
	char mqname[16];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int lwnl80211_open(struct file *filep);
static int lwnl80211_close(struct file *filep);
static ssize_t lwnl80211_read(struct file *filep, char *buffer, size_t len);
static ssize_t lwnl80211_write(struct file *filep, const char *buffer, size_t len);
static int lwnl80211_ioctl(struct file *filep, int cmd, unsigned long arg);
static int lwnl80211_start(struct lwnl80211_upperhalf_s *upper);
static void lwnl80211_callback(struct lwnl80211_lowerhalf_s *dev, lwnl80211_cb_status status, void *buffer);


/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_lwnl80211_fops = {
	lwnl80211_open,                                          /* open */
	lwnl80211_close,                                         /* close */
	lwnl80211_read,                                          /* read */
	lwnl80211_write,                                         /* write */
	0,                                                       /* seek */
	lwnl80211_ioctl                                          /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                                                      /* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int lwnl80211_open(struct file *filep)
{
	LWNL80211_ENTER;
	struct inode *inode = filep->f_inode;
	struct lwnl80211_upperhalf_s *upper = inode->i_private;
	int tmp_crefs;
	int ret;

	tmp_crefs = upper->crefs + 1;
	if (tmp_crefs == 0) {
		ret = -EMFILE;
		goto errout;
	}

	upper->crefs = tmp_crefs;

	if (upper->crefs == 1) {
		upper->usermq = NULL;
	}

	ret = OK;

errout:
	LWNL80211_LEAVE;
	return ret;
}

static int lwnl80211_close(struct file *filep)
{
	LWNL80211_ENTER;
	struct inode *inode = filep->f_inode;
	struct lwnl80211_upperhalf_s *upper = inode->i_private;
	int ret = OK;

	if (ret < 0) {
		ret = -errno;
		goto errout;
	}

	if (upper->crefs > 0) {
		upper->crefs--;
	} else {
		ret = -ENOSYS;
	}

errout:
	LWNL80211_LEAVE;
	return ret;
}

static ssize_t lwnl80211_read(struct file *filep, char *buffer, size_t len)
{
	LWNL80211_ENTER;

	return OK;
}

static ssize_t lwnl80211_write(struct file *filep, const char *buffer, size_t len)
{
	LWNL80211_ENTER;

	return OK;
}

static int lwnl80211_ioctl(struct file *filep, int cmd, unsigned long arg)
{
	LWNL80211_ENTER;

	int ret = -EINVAL;
	lwnl80211_result_e *res;
	struct inode *inode = filep->f_inode;
	struct lwnl80211_upperhalf_s *upper = inode->i_private;
	struct lwnl80211_lowerhalf_s *lower = upper->lower;
	lwnl80211_data *data_in;

	data_in = (lwnl80211_data *)((uintptr_t)arg);
	res = &data_in->res;
	ret = sem_wait(&upper->exclsem);
	if (ret < 0) {
		LWNL80211_ERR;
		return ret;
	}

	switch (cmd) {
	case LWNL80211_REGISTERMQ:
	{
		struct mq_attr attr;
		attr.mq_maxmsg = LWNL80211_MQUEUE_MAX_DATA_NUM;
		attr.mq_msgsize = sizeof(lwnl80211_cb_data);
		attr.mq_flags = 0;
		attr.mq_curmsgs = 0;
		memcpy(upper->mqname, (char *)(data_in->data), data_in->data_len);
		upper->usermq = mq_open(upper->mqname, O_RDWR | O_CREAT, 0666, &attr);
		if (upper->usermq == NULL) {
			nldbg("Failed to open mq\n");
			ret = -ENOSYS;
		} else {
			nldbg("Open mq with %s\n", upper->mqname);
			ret = OK;
		}
	}
	break;
	case LWNL80211_UNREGISTERMQ:
	{
		if (upper->usermq) {
			mq_close(upper->usermq);
			upper->usermq = NULL;
		}
		ret = OK;
	}
	break;
	case LWNL80211_INIT:
	{
		if (lower->ops->init != NULL) {
			*res = lower->ops->init(lower);
			if (*res == LWNL80211_SUCCESS) {
				ret = OK;
			} else {
				ret = -ENOSYS;
			}
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LWNL80211_DEINIT:
	{
		if (lower->ops->deinit != NULL) {
			*res = lower->ops->deinit();
			if (*res == LWNL80211_SUCCESS) {
				ret = OK;
			} else {
				ret = -ENOSYS;
			}
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LWNL80211_GET_INFO:
	{
		lwnl80211_info *info = (lwnl80211_info *)data_in->data;;
		if (lower->ops->get_info != NULL) {
			*res = lower->ops->get_info(info);
			if (*res == LWNL80211_SUCCESS) {
				ret = OK;
			} else {
				ret = -ENOSYS;
			}
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LWNL80211_SET_AUTOCONNECT:
	{
		uint8_t *check = (uint8_t *)data_in->data;
		if (lower->ops->set_autoconnect != NULL) {
			*res = lower->ops->set_autoconnect(*check);
			if (*res == LWNL80211_SUCCESS) {
				ret = OK;
			} else {
				ret = -ENOSYS;
			}
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LWNL80211_START_STA:
	{
		if (lower->ops->start_sta != NULL) {
			*res = lower->ops->start_sta();
			if (*res == LWNL80211_SUCCESS) {
				ret = OK;
			} else {
				ret = -ENOSYS;
			}
		} else {
			ret = -ENOSYS;
		}
		MQ_REOPEN;
	}
	break;
	case LWNL80211_CONNECT_AP:
	{
		lwnl80211_ap_config_s *config = (lwnl80211_ap_config_s *)data_in->data;
		if (lower->ops->connect_ap != NULL) {
			*res = lower->ops->connect_ap(config, NULL);
			if (*res == LWNL80211_SUCCESS) {
				ret = OK;
			} else {
				ret = -ENOSYS;
			}
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LWNL80211_DISCONNECT_AP:
	{
		if (lower->ops->disconnect_ap != NULL) {
			*res = lower->ops->disconnect_ap(NULL);
			if (*res == LWNL80211_SUCCESS) {
				ret = OK;
			} else {
				ret = -ENOSYS;
			}
		} else {
			ret = -ENOSYS;
		}
		MQ_REOPEN;
	}
	break;
	case LWNL80211_START_SOFTAP:
	{
		lwnl80211_softap_config_s *config = (lwnl80211_softap_config_s *)(data_in->data);
		if (lower->ops->start_softap != NULL) {
			*res = lower->ops->start_softap(config);
			if (*res == LWNL80211_SUCCESS) {
				ret = OK;
			} else {
				ret = -ENOSYS;
			}
		} else {
			ret = -ENOSYS;
		}
		MQ_REOPEN;
	}
	break;
	case LWNL80211_STOP_SOFTAP:
	{
		if (lower->ops->stop_softap != NULL) {
			*res = lower->ops->stop_softap();
			if (*res == LWNL80211_SUCCESS) {
				ret = OK;
			} else {
				ret = -ENOSYS;
			}
		} else {
			ret = -ENOSYS;
		}
		MQ_REOPEN;
	}
	break;
	case LWNL80211_SCAN_AP:
	{
		if (lower->ops->scan_ap != NULL) {
			*res = lower->ops->scan_ap(NULL);
			if (*res == LWNL80211_SUCCESS) {
				ret = OK;
			} else {
				ret = -ENOSYS;
			}
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	default:
	{
		if (lower->ops->drv_ioctl != NULL) {
			 nldbg("Forwarding unrecognized cmd: %d arg: %ld\n", cmd, arg);
			 ret = lower->ops->drv_ioctl(cmd, arg);
		 } else {
			 ret = -ENOSYS;
		 }
	}
	break;
	}

	sem_post(&upper->exclsem);
	LWNL80211_LEAVE;
	return ret;
}

static void lwnl80211_callback(struct lwnl80211_lowerhalf_s *dev, lwnl80211_cb_status status, void *buffer)
{
	LWNL80211_ENTER;
	struct lwnl80211_upperhalf_s *upper = dev->parent;
	int mq_ret;

	if (upper->usermq == NULL) {
		LWNL80211_ERR;
		return;
	}

	switch (status) {
	case LWNL80211_STA_CONNECTED:
	case LWNL80211_STA_CONNECT_FAILED:
	case LWNL80211_STA_DISCONNECTED:
	case LWNL80211_SOFTAP_STA_JOINED:
	case LWNL80211_SOFTAP_STA_LEFT:
	case LWNL80211_SCAN_FAILED:
	{
		lwnl80211_cb_data data_s = {status, .u.data = NULL, 0, 0};
		mq_ret = mq_send(upper->usermq, (const char *)&data_s, sizeof(data_s), LWNL80211_MQUEUE_PRIORITY);
		if (mq_ret < 0) {
			LWNL80211_ERR;
			return;
		}
		break;
	}
	case LWNL80211_SCAN_DONE:
	{
		lwnl80211_scan_list_s *scan_list = (lwnl80211_scan_list_s *)buffer;
		while (scan_list) {
			lwnl80211_cb_data data_s = {status, .u.ap_info = scan_list->ap_info, sizeof(lwnl80211_ap_scan_info_s), 1};
			if (scan_list->next == NULL) {
				data_s.md = 0;
			}
			scan_list = scan_list->next;
			mq_ret = mq_send(upper->usermq, (const char *)&data_s, sizeof(data_s), LWNL80211_MQUEUE_PRIORITY);
			if (mq_ret < 0) {
				LWNL80211_ERR;
				return;
			}
		}
		break;
	}
	case LWNL80211_UNKNOWN:
	default:
		LWNL80211_ERR;
		return;
	}

	LWNL80211_LEAVE;

}

/*
 * Public APIs
 */
int lwnl80211_register(struct lwnl80211_lowerhalf_s *dev)
{
	LWNL80211_ENTER;
	struct lwnl80211_upperhalf_s *upper;
	int ret;

	upper = (struct lwnl80211_upperhalf_s *)kmm_zalloc(sizeof(struct lwnl80211_upperhalf_s));
	if (!upper) {
		LWNL80211_ERR;
		return -ENOMEM;
	}

	sem_init(&upper->exclsem, 0, 1);

	upper->lower = dev;

	dev->parent = upper;
	dev->cbk = (lwnl80211_callback_t)lwnl80211_callback;

	ret = register_driver(LWNL80211_PATH, &g_lwnl80211_fops, 0666, upper);

	if (ret < 0) {
		LWNL80211_ERR;
		goto errout_with_priv;
	}

	LWNL80211_LEAVE;

	return OK;

errout_with_priv:
	sem_destroy(&upper->exclsem);
	kmm_free(upper);
	return ret;
}

int lwnl80211_unregister(struct lwnl80211_lowerhalf_s *dev)
{
	LWNL80211_ENTER;
	struct lwnl80211_upperhalf_s *upper = dev->parent;

	sem_destroy(&upper->exclsem);
	LWNL80211_LEAVE;
	return unregister_driver(LWNL80211_PATH);
}
