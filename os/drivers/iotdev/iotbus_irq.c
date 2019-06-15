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
#include <tinyara/iotbus_irq.h>
#include <iotbus/iotbus_debug.h>

#define IOTBUS_IRQ_THREAD		"IOTBUS_IRQ"
#define IOTBUS_IRQ_PRIORITY		224
#define IOTBUS_IRQ_STACKSIZE	2048

static int iotbus_irq_open(FAR struct file *filep);
static int iotbus_irq_close(FAR struct file *filep);
static int iotbus_irq_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static void iotbus_isr(int argc, char *argv[]);
/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations iotbus_irq_ops = {
	iotbus_irq_open,  /* open */
	iotbus_irq_close, /* close */
	NULL,			  /* read */
	NULL,			  /* write */
	NULL,			  /* seek */
	iotbus_irq_ioctl  /* ioctl */
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
static int g_irq_init = 0;
static int g_irq_pid;

static int iotbus_irq_open(FAR struct file *filep)
{
	// ToDo
	return 0;
}

static int iotbus_irq_close(FAR struct file *filep)
{
	// ToDo
	return 0;
}

static int iotbus_irq_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct iotbus_upperhalf_s *upper = inode->i_private;
	int ret;

	FAR const struct iotbus_int_info_s *info = (FAR const struct iotbus_int_info_s *)((uintptr_t)arg);

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
				itr->data->callback = info->callback;
				itr->data->pin_type = info->pin_type;
				ibvdbg("Overwrite duplicated interrupt %p callback[%p, %s]\n", info->callback, info->handle, iotbus_int_str[info->int_type]);
				sem_post(&upper->exclsem[info->int_type]);
				return 0;
			}
			priv = itr;
			itr = itr->next;
		}

		struct iotbus_int_info_s *data = (struct iotbus_int_info_s *)kmm_zalloc(sizeof(struct iotbus_int_info_s));
		memcpy(data, info, sizeof(struct iotbus_int_info_s));

		struct int_node_s *node = (struct int_node_s *)kmm_zalloc(sizeof(struct int_node_s));
		memset(node, 0, sizeof(struct int_node_s));
		node->data = data;

		if (priv == NULL) {
			upper->table[info->int_type] = node;
		} else {
			priv->next = node;
		}
		ibvdbg("Insert new interrupt %p callback[%p, %s]\n", info->callback, info->handle, iotbus_int_str[info->int_type]);
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
					free(priv->data);
					free(priv);
					priv = NULL;
				} else {
					priv->next = itr->next;
					free(itr->data);
					free(itr);
					itr = priv->next;
				}
				ibdbg("Delete interrupt %p callback[%p, %s]\n", info->callback, info->handle, iotbus_int_str[info->int_type]);
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
			ibvdbg("[Table] %s[%d] : %p\n", iotbus_int_str[i], i, upper->table[i]);
		}

		for (i = 0; i < IOTBUS_INTR_MAX; i++) {
			ibvdbg("--> %s\n", iotbus_int_str[i]);
			struct int_node_s *itr = upper->table[i];

			while (itr != NULL) {
				ibvdbg("Handler : %p\tCb : %p\tPin : %d\n", itr->data->handle, itr->data->callback, itr->data->pin_type);
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

static void iotbus_isr(int argc, char *argv[])
{
	int irq;
	int int_type;
	uint32_t int_val;
	struct iotbus_upperhalf_s *dev = &g_iot_udev;

	sigset_t set;
	siginfo_t info;

	sigemptyset(&set);
	sigaddset(&set, SIG_IOTBUS_INT);

	while (1) {
		DEBUGVERIFY(sigwaitinfo(&set, &info));

#ifdef CONFIG_CAN_PASS_STRUCTS
		int_val = info.si_value.sival_int;
#else
		int_val = info.si_value;
#endif
		int_type = int_val & 0xFFFF;
		irq = int_val >> 16;
		sem_wait(&dev->exclsem[int_type]);
		struct int_node_s *itr = dev->table[int_type];
		for (; itr != NULL; itr = itr->next) {

			switch (itr->data->pin_type) {
			case IOTBUS_GPIO:
				if (itr->data->irq == irq) {
					((iotbus_gpio_cb)itr->data->callback)((iotbus_gpio_context_h)itr->data->handle);
				}
				break;
			case IOTBUS_PWM:
				((iotbus_pwm_cb)itr->data->callback)((iotbus_pwm_context_h)itr->data->handle);
				break;
			case IOTBUS_UART:
				((iotbus_uart_cb)itr->data->callback)((iotbus_uart_context_h)itr->data->handle);
				break;
			case IOTBUS_ADC:
				((iotbus_adc_cb)itr->data->callback)((iotbus_adc_context_h)itr->data->handle);
				break;
			case IOTBUS_SPI:
				((iotbus_spi_cb)itr->data->callback)((iotbus_spi_context_h)itr->data->handle);
				break;
			case IOTBUS_I2C:
				((iotbus_i2c_cb)itr->data->callback)((iotbus_i2c_context_h)itr->data->handle);
				break;
			default:
				ibdbg("Fail to estimate pin type");
			}
		}
		sem_post(&dev->exclsem[int_type]);
	}
	return;
}

void iotbus_interrupt_trigger(int irq, iotbus_int_type_e int_type)
{
	irqstate_t flags;
	struct iotbus_upperhalf_s *dev = &g_iot_udev;
	int ret;

	flags = irqsave();

	uint32_t int_val = irq << 16 | int_type;
#ifdef CONFIG_CAN_PASS_STRUCTS
	union sigval value;
	value.sival_int = (int)int_val;
	ret = sigqueue(g_irq_pid, SIG_IOTBUS_INT, value);
#else
	ret = sigqueue(g_irq_pid, SIG_IOTBUS_INT, int_val);
#endif
	if (ret < 0) {
		ibdbg("Trigger Error!\n");
	}

	irqrestore(flags);
	return;
}

void iotbus_irq_register(void)
{
	struct iotbus_upperhalf_s *dev = &g_iot_udev;
	int ret;
	int i;

	g_irq_pid = kernel_thread(IOTBUS_IRQ_THREAD, IOTBUS_IRQ_PRIORITY, IOTBUS_IRQ_STACKSIZE, (main_t)iotbus_isr, (char *const *)NULL);
	if (g_irq_pid > 0) {
		ibvdbg("Execute loading thread with pid %d\n", g_irq_pid);
	} else {
		ibdbg("Loading Fail\n");
		g_irq_init = 0;
	}

	for (i = 0; i < IOTBUS_INTR_MAX; i++) {
		ret = sem_init(&dev->exclsem[i], 0, 1);
		if (ret == -1) {
			ibdbg("[%s] sem_init fail\n", iotbus_int_str[i]);
			return;
		}
		dev->table[i] = NULL;
	}

	(void)register_driver(IOTBUS_IRQPATH, &iotbus_irq_ops, 0666, dev);
}