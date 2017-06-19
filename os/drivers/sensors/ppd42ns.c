/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <semaphore.h>
#include <poll.h>
#include <tinyara/gpio.h>
#include <tinyara/sensors/sensor.h>
#include <tinyara/sensors/ppd42ns.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define GPIO_SIGNAL_LOW				0
#define GPIO_SIGNAL_HIGH			1

/****************************************************************************
 * Structures
 ****************************************************************************/
typedef struct {
	int initialized;
	int activated;
	sem_t exclsem;
	int fd;
	char devpath[16];
	int old_signal;
	int lowpulseoccupancy;
	struct timeval start_time;
	struct timeval lowpulse_start_time;
	int pipe_evt_task[2];
	int evt_task_loop;
} ppd42ns_priv;

/****************************************************************************
 * Static Function Prototype
 ****************************************************************************/
static int ppd42ns_init(sensor_device_t *sensor);
static int ppd42ns_deinit(sensor_device_t *sensor);
static int ppd42ns_activate(sensor_device_t *sensor);
static int ppd42ns_deactivate(sensor_device_t *sensor);
static int ppd42ns_ioctl(sensor_device_t *sensor, int id, sensor_ioctl_value_t *val);
static int ppd42ns_get_data(sensor_device_t *sensor, sensor_data_t *data);

/****************************************************************************
 * Variables
 ****************************************************************************/
static sensor_operations_t g_ops = {
	.init = ppd42ns_init,
	.deinit = ppd42ns_deinit,
	.activate = ppd42ns_activate,
	.deactivate = ppd42ns_deactivate,
	.ioctl = ppd42ns_ioctl,
	.set_trigger = NULL,
	.get_data = ppd42ns_get_data,
};

static ppd42ns_priv g_priv;

/****************************************************************************
 * Static Function
 ****************************************************************************/

static void ppd42ns_event_handler(sensor_device_t *sensor)
{
	/* this is both edge interrupt handler on ppd42ns's digital signal pin */

	struct timeval time;
	ppd42ns_priv *priv;

	if (sensor && sensor->priv) {
		priv = (ppd42ns_priv *) sensor->priv;
	} else {
		return;
	}

	if (priv->old_signal == GPIO_SIGNAL_LOW) {
		/* signal LOW -> HIGH */
		SENSOR_TIME_GET(time);
		priv->lowpulseoccupancy += SENSOR_TIME_DIFF_USEC(priv->lowpulse_start_time, time);
		priv->old_signal = GPIO_SIGNAL_HIGH;
#if PPD42NS_DEBUG_ON
		SENSOR_DEBUG("LOW -> HIGH : tv_sec=%d, tv_usec=%d, lowpulse_time=%d\n", time.tv_sec, time.tv_usec, SENSOR_TIME_DIFF_USEC(priv->lowpulse_start_time, time));
#endif
	} else {
		/* signal HIGH -> LOW */
		SENSOR_TIME_GET(priv->lowpulse_start_time);
		priv->old_signal = GPIO_SIGNAL_LOW;
#if PPD42NS_DEBUG_ON
		SENSOR_DEBUG("HIGH -> LOW : tv_sec=%d, tv_usec=%d\n", priv->lowpulse_start_time.tv_sec, priv->lowpulse_start_time.tv_usec);
#endif
	}
}

static void ppd42ns_event_task(sensor_device_t *sensor)
{
	int ret;
	ppd42ns_priv *priv;
	struct pollfd fds[2];
	int size;
	int timeout;

#if PPD42NS_DEBUG_ON
	SENSOR_DEBUG("start event task \n");
#endif

	if (sensor && sensor->priv) {
		priv = (ppd42ns_priv *) sensor->priv;
	} else {
		SENSOR_DEBUG("ERROR: %s is NULL.\n", sensor == NULL ? "sensor" : "sensor->priv");
		return;
	}

	/* initializes pollfd */
	memset(fds, 0, sizeof(fds));
	fds[0].fd = priv->pipe_evt_task[1];
	fds[0].events = POLLIN;
	fds[1].fd = priv->fd;
	fds[1].events = POLLIN;

	timeout = 1000;
	size = sizeof(fds) / sizeof(struct pollfd);
	priv->evt_task_loop = 1;
	while (priv->evt_task_loop) {
		ret = poll(fds, size, timeout);
		if (ret < 0) {
			SENSOR_DEBUG("ERROR: %s is NULL.\n");
			return;
		} else if (ret == 0) {
			/* poll timeout */
			continue;
		}

		/* stop message via pipe */
		if (fds[0].revents & POLLIN) {
#if PPD42NS_DEBUG_ON
			SENSOR_DEBUG("received stop message via pipe.\n");
#endif
			priv->evt_task_loop = 0;
			break;
		}

		/* gpio interrupt */
		if (fds[1].revents & POLLIN) {
#if PPD42NS_DEBUG_ON
			SENSOR_DEBUG("gpio interrupt occurred.\n");
#endif
			ppd42ns_event_handler(sensor);
		}
	}

#if PPD42NS_DEBUG_ON
	SENSOR_DEBUG("stop event task \n");
#endif
}

static int ppd42ns_event_task_start(sensor_device_t *sensor)
{
	int result = -1;
	int ret;
	pthread_t tid;
	ppd42ns_priv *priv;

	if (sensor && sensor->priv) {
		priv = (ppd42ns_priv *) sensor->priv;
	} else {
		SENSOR_DEBUG("ERROR: %s is NULL.\n", sensor == NULL ? "sensor" : "sensor->priv");
		return -1;
	}

	/* create pipe which is connected to event task */
	priv->pipe_evt_task[0] = -1;
	priv->pipe_evt_task[1] = -1;
	ret = pipe(priv->pipe_evt_task);
	if (ret == -1) {
		SENSOR_DEBUG("ERROR: pipe() failed.\n");
		goto done;
	}

	/* create event task */
	ret = pthread_create(&tid, NULL, (pthread_startroutine_t) ppd42ns_event_task, (void *)sensor);
	if (ret != 0) {
		SENSOR_DEBUG("ERROR: pthread_create() failed. (ret=%d)\n", ret);
		goto done;
	}
	ret = pthread_detach(tid);
	if (ret != 0) {
		SENSOR_DEBUG("ERROR: pthread_detach() failed. (ret=%d)\n", ret);
	}

	/* result is success */
	result = 0;

done:
	if (result != 0) {
		if (priv->pipe_evt_task[0] != -1) {
			close(priv->pipe_evt_task[0]);
			priv->pipe_evt_task[0] = -1;
		}

		if (priv->pipe_evt_task[1] != -1) {
			close(priv->pipe_evt_task[1]);
			priv->pipe_evt_task[1] = -1;
		}

		SENSOR_DEBUG("ERROR: fail to start event task\n");
	}

	return result;

}

static int ppd42ns_event_task_stop(sensor_device_t *sensor)
{
	ppd42ns_priv *priv;

	if (sensor && sensor->priv) {
		priv = (ppd42ns_priv *) sensor->priv;
	} else {
		SENSOR_DEBUG("ERROR: %s is NULL.\n", sensor == NULL ? "sensor" : "sensor->priv");
		return -1;
	}

	if (priv->evt_task_loop) {
		if (write(priv->pipe_evt_task[1], "#", 1) < 0) {
			SENSOR_DEBUG("ERROR: pipe write() failed. (errno=%d)\n", errno);
		}

		while (priv->evt_task_loop) {
			usleep(100 * 1000);
		}
	}

	if (priv->pipe_evt_task[0] != -1) {
		close(priv->pipe_evt_task[0]);
		priv->pipe_evt_task[0] = -1;
	}

	if (priv->pipe_evt_task[1] != -1) {
		close(priv->pipe_evt_task[1]);
		priv->pipe_evt_task[1] = -1;
	}

	return 0;
}

static int ppd42ns_init(sensor_device_t *sensor)
{
	int result = -1;
	ppd42ns_priv *priv;

	if (sensor == NULL || sensor->priv == NULL) {
		SENSOR_DEBUG("ERROR: %s is NULL.\n", sensor == NULL ? "sensor" : "sensor->priv");
		goto done;
	}
	priv = sensor->priv;

	if (priv->initialized) {
		SENSOR_DEBUG("ERROR: ppd42ns driver has already been initialized.\n");
		goto done;
	}

	/* set gpio device */
	snprintf(priv->devpath, 16, "/dev/gpio%d", CONFIG_SENSOR_PPD42NS_GPIO_NUM);

	/* init semaphore */
	sem_init(&priv->exclsem, 0, 1);

	/* init private variables */
	priv->activated = 0;
	priv->evt_task_loop = 0;
	priv->pipe_evt_task[0] = -1;
	priv->pipe_evt_task[1] = -1;

	/* initialized flag is set */
	priv->initialized = 1;

	/* result is success */
	result = 0;

done:
	return result;
}

static int ppd42ns_deinit(sensor_device_t *sensor)
{
	int result = -1;
	ppd42ns_priv *priv;

	if (sensor == NULL || sensor->priv == NULL) {
		SENSOR_DEBUG("ERROR: %s is NULL.\n", sensor == NULL ? "sensor" : "sensor->priv");
		goto done;
	}
	priv = sensor->priv;

	/* deactivate if activated currently */
	if (priv->activated) {
		ppd42ns_deactivate(sensor);
	}

	/* deinit semaphore */
	sem_destroy(&priv->exclsem);

	/* clear priv data */
	memset(priv, 0, sizeof(*priv));

	/* result is success */
	result = 0;

done:
	return result;
}

static int ppd42ns_activate(sensor_device_t *sensor)
{
	int result = -1;
	int ret;
	int signal;
	ppd42ns_priv *priv;
	struct gpio_pollevents_s pollevents;

	if (sensor == NULL || sensor->priv == NULL) {
		SENSOR_DEBUG("ERROR: %s is NULL.\n", sensor == NULL ? "sensor" : "sensor->priv");
		goto done_without_sem;
	}

	if (sensor == NULL || sensor->priv == NULL) {
		SENSOR_DEBUG("ERROR: %s is NULL.\n", sensor == NULL ? "sensor" : "sensor->priv");
		goto done_without_sem;
	}
	priv = sensor->priv;

	if (priv->activated) {
		SENSOR_DEBUG("ERROR: ppd42ns driver has already been activated.\n");
		goto done_without_sem;
	}

	do {
		ret = sem_wait(&priv->exclsem);
	} while (ret != 0 && errno == EINTR);

	if (ret != 0) {
		SENSOR_DEBUG("ERROR: sem_wait() failed.\n");
		goto done_without_sem;
	}

	/* open gpio pin */
	priv->fd = open(priv->devpath, O_RDWR);
	if (priv->fd < 0) {
		SENSOR_DEBUG("ERROR: open() failed. (dev: %s)\n", priv->devpath);
		goto done;
	}

	/* set gpio direction */
	ret = ioctl(priv->fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_IN);
	if (ret < 0) {
		SENSOR_DEBUG("ERROR: ioctl() failed. (dev: %s, ioctl_id: %d, errno: %d)\n", priv->devpath, GPIOIOC_SET_DIRECTION, errno);
		goto done;
	}

	/* set start time */
	SENSOR_TIME_GET(priv->start_time);

	/* get initial signal */
	ret = read(priv->fd, (void *)&signal, sizeof(int));
	if (ret < 0) {
		SENSOR_DEBUG("ERROR: read() failed. (fd: %d, errno: %d)\n", priv->fd, errno);
		goto done;
	}

	if (signal == GPIO_SIGNAL_LOW) {
		SENSOR_TIME_GET(priv->lowpulse_start_time);
	}
	priv->lowpulseoccupancy = 0;
	priv->old_signal = signal;

	/* set interrupt on both edge and poll event */
	pollevents.gp_rising = true;
	pollevents.gp_falling = true;
	ret = ioctl(priv->fd, GPIOIOC_POLLEVENTS, (unsigned long)&pollevents);
	if (ret < 0) {
		SENSOR_DEBUG("ERROR: ioctl() failed. (dev: %s, ioctl_id: %d, errno: %d)\n", priv->devpath, GPIOIOC_POLLEVENTS, errno);
		goto done;
	}

	if (ppd42ns_event_task_start(sensor) != 0) {
		SENSOR_DEBUG("ERROR: ppd42ns_event_task_start() failed. (dev: %s)\n", priv->devpath);
		goto done;
	}

	/* activated flag is set */
	priv->activated = 1;

	/* result is success */
	result = 0;

done:
	sem_post(&priv->exclsem);

done_without_sem:
	return result;
}

static int ppd42ns_deactivate(sensor_device_t *sensor)
{
	int result = -1;
	int ret;
	ppd42ns_priv *priv;

	if (sensor == NULL || sensor->priv == NULL) {
		SENSOR_DEBUG("ERROR: %s is NULL.\n", sensor == NULL ? "sensor" : "sensor->priv");
		goto done_without_sem;
	}
	priv = sensor->priv;

	do {
		ret = sem_wait(&priv->exclsem);
	} while (ret != 0 && errno == EINTR);

	if (ret != 0) {
		SENSOR_DEBUG("ERROR: sem_wait() failed.\n");
		goto done_without_sem;
	}

	ppd42ns_event_task_stop(sensor);

	/* close gpio */
	close(priv->fd);

	/* activated flag is unset */
	priv->activated = 0;

	/* result is success */
	result = 0;

	sem_post(&priv->exclsem);

done_without_sem:
	return result;
}

static int ppd42ns_ioctl(sensor_device_t *sensor, int id, sensor_ioctl_value_t *val)
{
	int result = -1;
	int ret;
	ppd42ns_priv *priv;

	if (sensor == NULL || sensor->priv == NULL) {
		SENSOR_DEBUG("ERROR: %s is NULL.\n", sensor == NULL ? "sensor" : "sensor->priv");
		goto done_without_sem;
	}
	priv = sensor->priv;

	if (!priv->initialized) {
		SENSOR_DEBUG("ERROR: ppd42ns driver is not initialized.\n");
		goto done_without_sem;
	}

	if (val == NULL) {
		SENSOR_DEBUG("ERROR: ioctl val is null.\n");
		goto done_without_sem;
	}

	do {
		ret = sem_wait(&priv->exclsem);
	} while (ret != 0 && errno == EINTR);

	if (ret != 0) {
		SENSOR_DEBUG("ERROR: sem_wait() failed.\n");
		goto done_without_sem;
	}

	switch (id) {
	case PPD42NS_IOCTL_ID_GET_GPIO_DEVPATH:
		val->p_str = priv->devpath;
		break;
	default:
		SENSOR_DEBUG("ERROR: unknown ioctl id. (id: %d)\n", id);
		goto done;
	}

	/* result is success */
	result = 0;

done:
	sem_post(&priv->exclsem);

done_without_sem:
	return result;
}

static int ppd42ns_get_data(sensor_device_t *sensor, sensor_data_t *data)
{
	int result = -1;
	float ratio = 0.0;
	float concentration;
	struct timeval time;
	int ret;
	int signal;
	ppd42ns_priv *priv;

	if (sensor == NULL || sensor->priv == NULL) {
		SENSOR_DEBUG("ERROR: %s is NULL.\n", sensor == NULL ? "sensor" : "sensor->priv");
		goto done_without_sem;
	}
	priv = sensor->priv;

	if (!priv->activated) {
		SENSOR_DEBUG("ERROR: ppd42ns driver is not activated.\n");
		goto done_without_sem;
	}

	do {
		ret = sem_wait(&priv->exclsem);
	} while (ret != 0 && errno == EINTR);

	if (ret != 0) {
		SENSOR_DEBUG("ERROR: sem_wait() failed.\n");
		goto done_without_sem;
	}

	lseek(priv->fd, 0, SEEK_SET);
	ret = read(priv->fd, (void *)&signal, sizeof(int));
	if (ret < 0) {
		SENSOR_DEBUG("ERROR: read() failed. (fd: %d, errno: %d)\n", priv->fd, errno);
		goto done;
	}

	SENSOR_TIME_GET(time);
	if (signal == GPIO_SIGNAL_LOW) {
		priv->lowpulseoccupancy += SENSOR_TIME_DIFF_USEC(priv->lowpulse_start_time, time);
		priv->lowpulse_start_time = time;
	}

	if (priv->lowpulseoccupancy != 0) {
		ratio = (float)priv->lowpulseoccupancy / (float)SENSOR_TIME_DIFF_USEC(priv->start_time, time) * 100.0;	/* Integer percentage 0=>100 */
		concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62;	/* using spec sheet curve */
	} else {
		concentration = 0.0;
	}
	data->fval = concentration;

#if PPD42NS_DEBUG_ON
	SENSOR_DEBUG("lowpulse_time=%d, total_time=%d, ratio=%.2f \n", priv->lowpulseoccupancy, SENSOR_TIME_DIFF_USEC(priv->start_time, time), ratio);
#endif

	priv->start_time = time;
	priv->lowpulseoccupancy = 0;
	priv->old_signal = signal;

	/* result is success */
	result = 0;

done:
	sem_post(&priv->exclsem);

done_without_sem:
	return result;
}

/****************************************************************************
 * Create Sensor Instance
 ****************************************************************************/
SENSOR_CREATE_INSTANCE(SENSOR_NAME_PPD42NS, SENSOR_DEVICE_TYPE_DUST, &g_ops, &g_priv);
