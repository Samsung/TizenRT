/****************************************************************************
 * drivers/input/ist415.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <poll.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#include <tinyara/i2c.h>
#include <tinyara/irq.h>
#include <tinyara/input/touchscreen.h>
#include <tinyara/input/ist415.h>

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

#define CONFIG_INPUT_NPOLLWAITERS 10

#define TOUCH_THRESHOLD 0
#define EVENT_PACKET_SIZE 16
/****************************************************************************
 * Private Types
 ****************************************************************************/

/* IST415 Device */

struct ist415_dev_s
{
  /* I2C bus and address for device. */

  struct i2c_dev_s *i2c;
  struct i2c_config_s config;
  uint8_t addr;
  struct lcd_touch_config *lower;
  /* Configuration for device. */

  sem_t sem;
  uint8_t crefs;
  bool int_pending;
#ifndef CONFIG_DISABLE_POLL
	sem_t pollsem;
	struct pollfd *fds[CONFIG_SERIAL_NPOLLWAITERS];
#endif
};


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int ist415_open(FAR struct file *filep);
static int ist415_close(FAR struct file *filep);
static ssize_t ist415_read(FAR struct file *filep, FAR char *buffer,
                            size_t buflen);
static ssize_t ist415_write(FAR struct file *filep, FAR char *buffer,
                            size_t buflen);
#ifndef CONFIG_DISABLE_POLL
static void ist415_pollnotify(FAR struct ist415_dev_s *dev, pollevent_t eventset);
static int ist415_poll(FAR struct file *filep, FAR struct pollfd *fds,
                        bool setup);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/
static FAR struct ist415_dev_s g_ist415_priv;

/* File Operations exposed to NuttX Apps */
static const struct file_operations g_ist415_fileops =
{
  ist415_open,   /* open */
  ist415_close,  /* close */
  ist415_read,   /* read */
  ist415_write,           /* write */
  NULL,           /* seek */
  NULL,          /*ioctl */
  ist415_poll,          
};

/* Last event, last ID and last valid touch coordinates */

static uint8_t  last_event = 0xff;
static uint8_t  last_id    = 0xff;
static uint16_t last_x     = 0xffff;
static uint16_t last_y     = 0xffff;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int ist415_get_touch_data(struct ist415_dev_s *dev, FAR void *buf)
{
        struct i2c_dev_s *i2c = dev->i2c;
        struct i2c_config_s config = dev->config;
	struct ts_event_coordinate *p_evt_coord;
        struct ts_coordinate coord;
	struct touch_sample_s data;
	u8 event[EVENT_PACKET_SIZE];
        u8 *left_event;
	int reg[2];
	reg[0] = 0x60;
	int ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
	//printf("ret only check points %d\n", ret);
	if(ret == 1) {
		i2c_read(i2c, &config, (uint8_t *)event, EVENT_PACKET_SIZE);
		u8 le  = event[7] & 0x1F;
		u8 eid = event[0] & 0x3;
		if (eid == EID_COORD) {
			p_evt_coord = (struct ts_event_coordinate *) event;
			coord.id = p_evt_coord->tid - 1;
			coord.status = p_evt_coord->tsta;
			coord.x = (p_evt_coord->x_11_4 << 4) | p_evt_coord->x_3_0;
			coord.y = (p_evt_coord->y_11_4 << 4) | p_evt_coord->y_3_0;
			coord.z = p_evt_coord->z;
			coord.major = p_evt_coord->major;
			coord.minor = p_evt_coord->minor;
			coord.type = (p_evt_coord->ttype_3_2 << 2) | p_evt_coord->ttype_1_0;
			memset(&data, 0, sizeof(data));
  			data.npoints     = 1;
  			data.point[0].id = coord.id;
  			data.point[0].x  = coord.x;
  			data.point[0].y  = coord.y;
			switch (coord.status) {
				case 1:
					data.point[0].flags = TOUCH_DOWN;
					break;
				case 2:
					data.point[0].flags = TOUCH_MOVE;
					break;
				case 3:
					data.point[0].flags = TOUCH_UP;
					break;
			}
			memcpy(buf, &data, sizeof(data));
			printf("corrdinates are id %d status %d type %d x : %d y : %d\n", coord.id, coord.status, coord.type, coord.x, coord.y);
			return 1;
		} else if (eid == EID_STATUS) {
			/** TO DO **/	
		}
		
	}

	return -1;
}

/************************************************************************************
 * Name: touch_semtake
 ************************************************************************************/
static inline int touch_semtake(FAR sem_t *sem, bool errout)
{
	/* Take a count from the semaphore, possibly waiting */
	if (sem_wait(sem) < 0) {
		/* EINTR is the only error that we expect */
		int errcode = get_errno();
		DEBUGASSERT(errcode == EINTR);
		if (errout) {
			return errcode;
		}
	}

	return 0;
}


void touch_semtake1(sem_t *sem)
{
	/* Take the semaphore (perhaps waiting) */

	while (sem_wait(sem) != 0) {
		/* The only case that an error should occur here is if
		 * the wait was awakened by a signal.
		 */

		ASSERT(*get_errno_ptr() == EINTR);
	}
}

/****************************************************************************
 * Name: touch_semgive
 ****************************************************************************/

void touch_semgive(sem_t *sem)
{
	sem_post(sem);
}

static int ist415_open(FAR struct file *filep)
{
	FAR struct ist415_dev_s *priv;
	priv = filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	touch_semtake(&priv->sem, false);
	if (priv->crefs == 0) {
		priv->lower->irq_enable(true);
	}
	priv->crefs++;
	DEBUGASSERT(priv->crefs > 0);
	touch_semgive(&priv->sem);
}


static int ist415_close(FAR struct file *filep)
{
	printf("fd close \n");
	FAR struct ist415_dev_s *priv;
	priv = filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	touch_semtake(&priv->sem, false);
	DEBUGASSERT(priv->crefs > 0);
	priv->crefs--;
	if (priv->crefs == 0) {
	priv->lower->irq_enable(false);
#ifndef CONFIG_DISABLE_POLL
        /* Check if this file is registered in a list of waiters for polling.
         * For example, when task A is blocked by calling poll and task B try to terminate task A,
         * a pollfd of A remains in this list. If it is, it should be cleared.
         */
        (void)touch_semtake(&priv->pollsem, false);
        for (int i = 0; i < CONFIG_SERIAL_NPOLLWAITERS; i++) {
                struct pollfd *fds = priv->fds[i];
                if (fds && (FAR struct file *)fds->filep == filep) {
                        priv->fds[i] = NULL;
                }
        }
        touch_semgive(&priv->pollsem);
#endif
	}
	touch_semgive(&priv->sem);
}

static ssize_t ist415_read(FAR struct file *filep, FAR char *buffer,
                            size_t buflen)
{
  FAR struct inode *inode;
  FAR struct ist415_dev_s *priv;
  size_t outlen;
  irqstate_t flags;
  int ret;

  DEBUGASSERT(buffer);
  DEBUGASSERT(buflen > 0);
  DEBUGASSERT(filep);
  inode = filep->f_inode;

  DEBUGASSERT(inode && inode->i_private);
  priv = inode->i_private;

  /* Wait for semaphore to prevent concurrent reads */

  touch_semtake(&priv->sem, false);

  /* Read the touch data, only if screen has been touched or if we're waiting for touch up */

  outlen = sizeof(struct touch_sample_s);
  if (priv->int_pending)
    {
	    flags = enter_critical_section();
	     priv->int_pending = false;
  	    leave_critical_section(flags);
	    ret = ist415_get_touch_data(priv, buffer);
    }
//printf("not pending \n");
  /* Clear pending flag with critical section */
  //priv->lower->irq_enable(false);
  /*flags = enter_critical_section();
  priv->int_pending = false;
  //priv->lower->irq_enable(true);
  leave_critical_section(flags);*/

  /* Release semaphore and allow next read */

  touch_semgive(&priv->sem);
  //printf("return %d\n", ret);
  return ret < 0 ? ret : outlen;
}

static ssize_t ist415_write(FAR struct file *filep, FAR char *buffer,
                            size_t buflen)
{
}

#ifndef CONFIG_DISABLE_POLL
static void ist415_pollnotify(FAR struct ist415_dev_s *dev, pollevent_t eventset)
{
	int i;
	for (i = 0; i < CONFIG_SERIAL_NPOLLWAITERS; i++) {
		struct pollfd *fds = dev->fds[i];
		if (fds) {
			fds->revents |= (fds->events & eventset);
			if (fds->revents != 0) {
				sem_post(fds->sem);
			}
		}
	}
}

static int ist415_poll(FAR struct file *filep, FAR struct pollfd *fds,
                        bool setup)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct ist415_dev_s *dev = inode->i_private;
	pollevent_t eventset;
	int ret;
	int i;
	bool pending = false;
	/* Some sanity checking */

#ifdef CONFIG_DEBUG
	if (!dev || !fds) {
		return -ENODEV;
	}
#endif

	/* Are we setting up the poll?  Or tearing it down? */
	ret = touch_semtake(&dev->pollsem, true);
	if (ret < 0) {
		/* A signal received while waiting for access to the poll data
		 * will abort the operation.
		 */

		return ret;
	}

	if (setup) {
		/* This is a request to set up the poll.  Find an available
		 * slot for the poll structure reference
		 */

		for (i = 0; i < CONFIG_SERIAL_NPOLLWAITERS; i++) {
			/* Find an available slot */

			if (!dev->fds[i]) {
				/* Bind the poll structure and this slot */
				dev->fds[i] = fds;
				fds->priv = &dev->fds[i];
				fds->filep = (void *)filep;
				break;
			}
		}

		if (i >= CONFIG_SERIAL_NPOLLWAITERS) {
			fds->priv = NULL;
			fds->filep = NULL;
			ret = -EBUSY;
			goto errout;
		}
		eventset = 0;
		touch_semtake(&dev->sem, false);

		pending = dev->int_pending;
		if (pending) {
			eventset |= (fds->events & POLLIN);
		}
		touch_semgive(&dev->sem);
		if (eventset) {
			ist415_pollnotify(dev, POLLIN);
		}

	} else if (fds->priv) {
		/* This is a request to tear down the poll. */

		struct pollfd **slot = (struct pollfd **)fds->priv;

		/* Remove all memory of the poll setup */

		*slot = NULL;
		fds->priv = NULL;
		fds->filep = NULL;
	}

errout:
	touch_semgive(&dev->pollsem);
	return ret;
}
#endif

static void touch_interrupt(int d)
{
	FAR struct ist415_dev_s *priv = &g_ist415_priv;
	irqstate_t state = enter_critical_section();

	/*static clock_t prev = 0; 
        clock_t now = clock();
	//lldbg("now %f , prev %f\n", (float)now/CLOCKS_PER_SEC, (float)prev/CLOCKS_PER_SEC);
	clock_t diff = now - prev;
        if(diff > TOUCH_THRESHOLD) {
                lldbg("Touch interruptttttt... diff : %d\n", diff);

                priv->int_pending = true;
		prev = now;
		leave_critical_section(state);
                ist415_pollnotify(priv, POLLIN);
        }
	else
	{
		prev = now;
		leave_critical_section(state);
	}*/
	lldbg("Touch interruptttttt...\n");
	priv->int_pending = true;
	leave_critical_section(state);
        ist415_pollnotify(priv, POLLIN);
	priv->lower->irq_enable(true);
}

int ist415_register(FAR const char *devpath,
                     FAR struct i2c_dev_s *i2c_dev,
                     FAR struct lcd_touch_config *config)
{
  struct ist415_dev_s *priv = &g_ist415_priv;;
  int ret = 0;

  /* Allocate device private structure. */

  //priv = (struct ist415_dev_s *)kmm_zalloc(sizeof(struct ist415_dev_s));
  if (!priv)
    {
      printf("Memory allocation failed\n");
      return -ENOMEM;
    }

  /* Setup device structure. */

  priv->lower = config;
  priv->config = config->i2c_config;
  priv->i2c = i2c_dev;
	sem_init(&priv->sem, 0, 1);
		sem_init(&priv->pollsem, 0, 1);

#ifndef CONFIG_DISABLE_POLL
	(void)touch_semtake(&priv->pollsem, false);
	for (int i = 0; i < CONFIG_SERIAL_NPOLLWAITERS; i++) {
			priv->fds[i] = NULL;
	}
	touch_semgive(&priv->pollsem);
#endif
   ret = register_driver("/dev/input0", &g_ist415_fileops, 0666, priv);
  if (ret < 0)
    {
      kmm_free(priv);
      sem_destroy(&priv->sem);
      sem_destroy(&priv->pollsem);
      printf("Driver registration failed\n");
      return ret;
    }

  priv->lower->attach(touch_interrupt, NULL);
  priv->lower->irq_enable(false);
  //irq_attach(41, touch_interrupt, NULL); 
  //irq_disable(9);
  printf("Driver registered\n");
  return 0;
}
