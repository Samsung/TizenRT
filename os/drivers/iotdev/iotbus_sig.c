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
#include <semaphore.h>
#include <errno.h>
#include <debug.h>
#include <unistd.h>
#include <assert.h>
#include <tinyara/fs/fs.h>
#include <tinyara/kmalloc.h>
#include <tinyara/iotbus_sig.h>

static int iotbus_sig_open(FAR struct file *filep);
static int iotbus_sig_close(FAR struct file *filep);
static int iotbus_sig_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations iotbus_sig_ops = {
	iotbus_sig_open,  /* open */
	iotbus_sig_close, /* close */
	NULL,			  /* read */
	NULL,			  /* write */
	NULL,			  /* seek */
	iotbus_sig_ioctl  /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	,
	NULL /* poll */
#endif
};

struct int_node_s {
	struct iotbus_int_info_s *data;
	struct int_node_s *next;
};

struct iotbus_upperhalf_s {
	struct int_node_s *table[IOTBUS_INTR_MAX];
	sem_t exclsem[IOTBUS_INTR_MAX];
};

static struct iotbus_upperhalf_s g_iot_udev = {
	0,
};

static int iotbus_sig_open(FAR struct file *filep)
{
	// ToDo
	return 0;
}

static int iotbus_sig_close(FAR struct file *filep)
{
	// ToDo
	return 0;
}

static int iotbus_sig_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct iotbus_upperhalf_s *upper = inode->i_private;
	int ret;

	FAR const struct iotbus_int_info_s *info = (FAR const struct iotbus_int_info_s *)((uintptr_t)arg);
	if (info == NULL) {
		return -EINVAL;
	}
	
	switch (cmd) {
	case IOTBUS_INTR_REGISTER: {
		ret = sem_wait(&upper->exclsem[info->int_type]);
		if (ret < 0) {
			ibdbg("Fail to register interrupt callback\n");
			return ret;
		}
		struct int_node_s *itr = upper->table[info->int_type];
		struct int_node_s *priv = NULL;

		// Check duplicated interrupt in same handler.
		while (itr != NULL) {
			if (itr->data->handle == info->handle && itr->data->int_type == info->int_type) {
				itr->data->pin_type = info->pin_type;
				ibvdbg("Overwrite duplicated interrupt [%p, %d]\n", info->handle, info->int_type);
				sem_post(&upper->exclsem[info->int_type]);
				return 0;
			}
			priv = itr;
			itr = itr->next;
		}

		struct iotbus_int_info_s *data = (struct iotbus_int_info_s *)kmm_zalloc(sizeof(struct iotbus_int_info_s));
		if (data == NULL) {
			return -ENOMEM;
		}
		memcpy(data, info, sizeof(struct iotbus_int_info_s));

		struct int_node_s *node = (struct int_node_s *)kmm_zalloc(sizeof(struct int_node_s));
		if (node == NULL) {
			kmm_free(data);
			return -ENOMEM;
		}
		memset(node, 0, sizeof(struct int_node_s));
		node->data = data;

		if (priv == NULL) {
			upper->table[info->int_type] = node;
		} else {
			priv->next = node;
		}
		ibvdbg("Insert new interrupt [%p, %d]\n", info->handle, info->int_type);
		sem_post(&upper->exclsem[info->int_type]);
	} break;

	case IOTBUS_INTR_UNREGISTER: {
		ret = sem_wait(&upper->exclsem[info->int_type]);
		if (ret < 0) {
			ibdbg("Fail to unregister interrupt callback\n");
			return ret;
		}

		struct int_node_s *itr = upper->table[info->int_type];
		struct int_node_s *priv = NULL;

		while (itr != NULL) {
			if (itr->data->handle == info->handle && itr->data->int_type == info->int_type) {
				if (priv == NULL) {
					priv = itr;
					itr = upper->table[info->int_type] = itr->next;
					kmm_free(priv->data);
					kmm_free(priv);
					priv = NULL;
				} else {
					priv->next = itr->next;
					kmm_free(itr->data);
					kmm_free(itr);
					itr = priv->next;
				}
				ibdbg("Delete interrupt [%p, %d]\n", info->handle, info->int_type);
				continue;
			}
			priv = itr;
			itr = itr->next;
		}
		sem_post(&upper->exclsem[info->int_type]);
	} break;

	case IOTBUS_INTR_INFO: {
		int i;
		ibvdbg("** Iotbus Interrupt Info **\n");

		for (i = 0; i < IOTBUS_INTR_MAX; i++) {
			ibvdbg("[Table][%d] : %p\n", i, upper->table[i]);
		}

		for (i = 0; i < IOTBUS_INTR_MAX; i++) {
			ibvdbg("--> %d\n", i);
			struct int_node_s *itr = upper->table[i];

			while (itr != NULL) {
				ibvdbg("Handler : %p\tPin : %d\n", itr->data->handle, itr->data->pin_type);
				itr = itr->next;
			}
		}
	} break;
	default: {
		ibvdbg("Forwarding unrecognized cmd: %d arg: %ld\n", cmd, arg);
	} break;
	}

	return 0;
}

void iotbus_interrupt_trigger(iotbus_int_type_e int_type)
{
	irqstate_t flags;
	struct iotbus_upperhalf_s *dev = &g_iot_udev;
	int ret;
	int signal;
	pid_t pid;

	switch (int_type) {
	case IOTBUS_GPIO_FALLING:
		signal = SIG_IOTBUS_GPIO_FALLING;
		break;
	case IOTBUS_GPIO_RISING:
		signal = SIG_IOTBUS_GPIO_RISING;
		break;
	case IOTBUS_UART_TX_EMPTY:
		signal = SIG_IOTBUS_UART_TX_EMPTY;
		break;
	case IOTBUS_UART_TX_RDY:
		signal = SIG_IOTBUS_UART_TX_RDY;
		break;
	case IOTBUS_UART_RX_AVAIL:
		signal = SIG_IOTBUS_UART_RX_AVAIL;
		break;
	case IOTBUS_UART_RECEIVED:
		signal = SIG_IOTBUS_UART_RECEIVED;
		break;
	default:
		return;
	}

	ibdbg("interrupt triggered int : %d, sig : %d\n", int_type, signal);

	flags = enter_critical_section();
	struct int_node_s *itr = dev->table[int_type];
	for (; itr != NULL; itr = itr->next) {
		pid = itr->data->pid;
		ibdbg("target pid : %d\n", pid);
		if (sched_gettcb(pid) == NULL) {
			ibdbg("pid[%d] is null", pid);
			continue;
		}
#ifdef CONFIG_CAN_PASS_STRUCTS
		union sigval value;
		value.sival_ptr = (void *)itr->data->handle;
		ret = sigqueue(pid, signal, value);
#else
		ret = sigqueue(pid, signal, (void *)itr->data->handle);
#endif
		if (ret < 0) {
			ibdbg("Trigger Error!\n");
		}
	}
	leave_critical_section(flags);
	return;
}

void iotbus_sig_register(void)
{
	struct iotbus_upperhalf_s *dev = &g_iot_udev;
	int ret;
	int i;

	for (i = 0; i < IOTBUS_INTR_MAX; i++) {
		ret = sem_init(&dev->exclsem[i], 0, 1);
		if (ret == -1) {
			ibdbg("[%d] sem_init fail\n", i);
			return;
		}
		dev->table[i] = NULL;
	}

	(void)register_driver(IOTBUS_SIGPATH, &iotbus_sig_ops, 0666, dev);
}
