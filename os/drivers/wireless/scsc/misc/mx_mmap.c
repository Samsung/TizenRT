/*****************************************************************************
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

#include <scsc/scsc_mx.h>
#include "scsc_mif_abs.h"
#include "mxman.h"
#include "scsc_mx_impl.h"
#include "gdb_transport.h"
#include "utils_misc.h"
#include <semaphore.h>
#include <tinyara/fs/fs.h>
#include <poll.h>
#include "debug_scsc.h"

#define DRV_NAME                "mx_mmap"
#define DEVICE_NAME             "maxwell_mmap"

#ifndef VM_RESERVED
#define VM_RESERVED (VM_DONTEXPAND | VM_DONTDUMP)
#endif

#define VER_MAJOR               0
#define VER_MINOR               0

#define SCSC_MMAP_NODE          1
#define SCSC_GDB_NODE           1

#define SCSC_MAX_INTERFACES     (5 * (SCSC_MMAP_NODE + SCSC_GDB_NODE))

//#define MAX_MEMORY              4194304UL /* 4M maximum memory */
#define MAX_MEMORY              4096UL
#define NPOLLWAITERS            2
unsigned long bitmap_minor[(((SCSC_MAX_INTERFACES) + (64) - 1) / (64))];
struct inode;					/* Forward reference */

/* keep track of how many times it is mmapped */
struct mmap_info {
	char *data;					/* the data */
	int reference;				/* how many times it is mmapped */
};

struct mx_mmap_dev {
	/* file pointer */
	struct file *filp;
	/* Index in mem to save next byte written */
	uint16_t write_idx;
	/* Index in mem to return the next byte read */
	uint16_t read_idx;
	/* References counts on pipe (limited to 255) */
	uint8_t refs;
	/* Number of reference counts for write access */
	uint8_t nwriters;
	/* Number of reference counts for read access */
	uint8_t nreaders;
	/*mif_abs pointer */
	struct scsc_mif_abs *mif_abs;
	/*mif_abs pointer */
	struct scsc_mx *mx;
	/*mif_abs pointer */
	struct gdb_transport *gdb_transport;
	/*memory buffer cache */
	uint8_t *mem;
	/* Associated read_wait semaphore for read access */
	sem_t read_wait;
	/* Associated write_wait semaphore for write access. */
	sem_t write_wait;
	/* Used to serialize access to mem and indices */
	sem_t data_wait;
	/* The following is a list if poll structures of threads waiting for
	 * driver events. The 'struct pollfd' reference for each open is also
	 * retained in the f_priv field of the 'struct file'.
	 */

//#ifndef CONFIG_DISABLE_POLL
	struct pollfd *d_fds[NPOLLWAITERS];
//#endif
};

/****************************************************************************
 * Name: mx_gdb_semtake
 ****************************************************************************/
static void mx_gdb_semtake(sem_t *sem)
{
	SLSI_INFO_NODEV("Entering\n");
	while (sem_wait(sem) != 0) {
		/* The only case that an error should occur here is if the wait was
		 * awakened by a signal.
		 */
		sem_post(sem);
		ASSERT(get_errno() == EINTR);
	}
}

/****************************************************************************
 * Name: mx_gdb_pollnotify
 ****************************************************************************/

#ifndef CONFIG_DISABLE_POLL
static void mx_gdb_pollnotify(FAR struct mx_mmap_dev *mx_dev, pollevent_t eventset)
{
	int i;

	SLSI_INFO_NODEV("Entering\n");

	if (eventset & POLLERR) {
		eventset &= ~(POLLOUT | POLLIN);
	}

	for (i = 0; i < NPOLLWAITERS; i++) {
		struct pollfd *fds = mx_dev->d_fds[i];
		if (fds) {
			fds->revents |= eventset & (fds->events | POLLERR | POLLHUP);

			if ((fds->revents & (POLLOUT | POLLHUP)) == (POLLOUT | POLLHUP)) {
				/* POLLOUT and POLLHUP are mutually exclusive. */
				fds->revents &= ~POLLOUT;
			}

			if (fds->revents != 0) {
				fvdbg("Report events: %02x\n", fds->revents);
				sem_post(fds->sem);
			}
		}
	}
}
#else
#define mx_gdb_pollnotify(dev, event)
#endif

/****************************************************************************
 * Name: mx_gdb_allocdev
 ****************************************************************************/
FAR struct mx_mmap_dev *mx_gdb_allocdev(void)
{
	FAR struct mx_mmap_dev *dev;
	SLSI_INFO_NODEV("Entering \n");

	/* Allocate a private structure to manage the device */

	dev = (struct mx_mmap_dev *)kmm_malloc(sizeof(struct mx_mmap_dev));
	if (dev) {
		/* Initialize the private structure */

		//memset(dev, 0, sizeof(struct mx_mmap_dev));
		sem_init(&dev->data_wait, 0, 1);
		sem_init(&dev->read_wait, 0, 0);
		sem_init(&dev->write_wait, 0, 0);
	}

	return dev;
}

/****************************************************************************
 * Name: mx_gdb_freedev
 ****************************************************************************/
void mx_gdb_freedev(FAR struct mx_mmap_dev *dev)
{
	SLSI_INFO_NODEV("Entering\n");
	sem_destroy(&dev->data_wait);
	sem_destroy(&dev->read_wait);
	sem_destroy(&dev->write_wait);
	kmm_free(dev);
}

/****************************************************************************
 * Name: mx_gdb_open
 ****************************************************************************/
int mx_gdb_open(struct file *filp)
{
	struct mx_mmap_dev *mx_dev;
	int ret = 0;
	int sval = 0;

	FAR struct inode *inode = filp->f_inode;
	mx_dev = inode->i_private;

	SLSI_INFO_NODEV("Entering\n");

	DEBUGASSERT(mx_dev != NULL);

	filp->f_priv = mx_dev;
	mx_dev->filp = filp;

	/* Make sure that we have exclusive access to the device structure.  The
	 * sem_wait() call should fail only if we are awakened by a signal.
	 */

	ret = sem_wait(&mx_dev->data_wait);
	if (ret != OK) {
		fdbg("sem_wait failed: %d\n", get_errno());
		DEBUGASSERT(get_errno() > 0);
		return -get_errno();
	}

	if (mx_dev->refs == 0 && mx_dev->mem == NULL) {
		mx_dev->mem = (FAR uint8_t *)kmm_malloc(MAX_MEMORY);
		if (!mx_dev->mem) {
			(void)sem_post(&mx_dev->data_wait);
			return -ENOMEM;
		}
	}
	/* increment reference count */
	mx_dev->refs++;
	if ((filp->f_oflags & O_WROK) != 0) {
		mx_dev->nwriters++;

		/* If this this is the first writer, then the read semaphore indicates the
		 * number of readers waiting for the first writer.  Wake them all up.
		 */
		if (mx_dev->nwriters == 1) {
			while (sem_getvalue(&mx_dev->read_wait, &sval) == 0 && sval < 0) {
				sem_post(&mx_dev->read_wait);
			}
		}
	}

	/* If opened for reading, increment the count of reader on on the instance */
	if ((filp->f_oflags & O_RDOK) != 0) {
		mx_dev->nreaders++;
	}

	return ret;
}

/****************************************************************************
 * Name: mx_gdb_write
 ****************************************************************************/
static ssize_t mx_gdb_write(struct file *filp, const char *buf, size_t len)
{
	struct inode *inode = filp->f_inode;
	struct mx_mmap_dev *mx_dev = inode->i_private;

	SLSI_INFO_NODEV("Entering\n");
	SLSI_INFO_NODEV("writing %d bytes with data: %s\n", len, buf);
	gdb_transport_send(mx_dev->gdb_transport, (void *)buf, len);

	return len;
}

/****************************************************************************
 * Name: mx_gdb_read
 ****************************************************************************/
static ssize_t mx_gdb_read(struct file *filp, FAR char *buf, size_t len)
{
#ifdef SCSC_GDB_TRANSPORT_ENABLE
	struct inode *inode = filp->f_inode;
	struct mx_mmap_dev *mx_dev = inode->i_private;
	int ret = 0;
	ssize_t nread = 0;
	int sval;

	SLSI_INFO_NODEV("\n");

	/* Make sure that we have exclusive access to the device structure */
	if (sem_wait(&mx_dev->data_wait) < 0) {
		return ERROR;
	}

	/* If the buffer is empty, then wait for something to be written to it */
	while (mx_dev->write_idx == mx_dev->read_idx) {
		/* If O_NONBLOCK was set, then return EGAIN */
		if (filp->f_oflags & O_NONBLOCK) {
			sem_post(&mx_dev->data_wait);
			return -EAGAIN;
		}

		/* If there are no writers on the device, then return end of file */
		if (mx_dev->nwriters <= 0) {
			sem_post(&mx_dev->data_wait);
			return 0;
		}

		/* Otherwise, wait for something to be written to the pipe */
		sched_lock();
		sem_post(&mx_dev->data_wait);
		ret = sem_wait(&mx_dev->read_wait);
		sched_unlock();

		if (ret < 0) {
			return ERROR;
		}
	}

	/* return whatever is available in the buffer (which is at least one byte) */
	nread = 0;
	while (nread < len && mx_dev->write_idx != mx_dev->read_idx) {
		*buf++ = mx_dev->mem[mx_dev->read_idx];
		if (++mx_dev->read_idx >= MAX_MEMORY) {
			mx_dev->read_idx = 0;
		}
		nread++;
	}

	/* Notify all waiting writers that bytes have been removed from the buffer */
	while (sem_getvalue(&mx_dev->write_wait, &sval) == 0 && sval < 0) {
		sem_post(&mx_dev->write_wait);
	}

	/* Notify all poll/select waiters that they can write to the "FIFO" */
	mx_gdb_pollnotify(mx_dev, POLLOUT);

	sem_post(&mx_dev->data_wait);
	return nread;
#else
	return 0;
#endif
}

void gdb_read_callback(const void *message, size_t len, void *data)
{
	struct mx_mmap_dev *mx_dev = (struct mx_mmap_dev *)data;

	int nwritten = 0;
	int sval = 0;
	ssize_t last;
	int nxtwrndx;

	DEBUGASSERT(mx_dev);
	uint8_t *buffer = (FAR uint8_t *)message;

	SLSI_INFO_NODEV("\n");

	if (len == 0) {
		SLSI_ERR_NODEV("received callback with zero length\n");
		return;
	}

	DEBUGASSERT(up_interrupt_context() == false);

	if (mx_dev->filp) {
		/* Make sure that we have exclusive access to the device structure */
		if (sem_wait(&mx_dev->data_wait) < 0) {
			SLSI_ERR_NODEV("could not get exclusive access to dev structure\n");
			return;
		}

		/* Loop until all of the bytes have been written */
		last = 0;
		for (;;) {
			/* Calculate the write index AFTER the next byte is written */
			nxtwrndx = mx_dev->write_idx + 1;
			if (nxtwrndx >= MAX_MEMORY) {
				nxtwrndx = 0;
			}

			/* Would the next write overflow the circular buffer? */
			if (nxtwrndx != mx_dev->read_idx) {
				/* No... copy the byte */
				mx_dev->mem[mx_dev->write_idx] = *buffer++;
				mx_dev->write_idx = nxtwrndx;

				/* Is the write complete? */
				if (++nwritten >= len) {
					/* Yes.. Notify all of the waiting readers that more data is available */
					while (sem_getvalue(&mx_dev->read_wait, &sval) == 0 && sval < 0) {
						sem_post(&mx_dev->read_wait);
					}

					/* Notify all poll/select waiters that they can write to the FIFO */
					mx_gdb_pollnotify(mx_dev, POLLIN);

					/* Return the number of bytes written */
					sem_post(&mx_dev->data_wait);
					SLSI_INFO_NODEV("num bytes (all) pushed  %zu\n", len);
					return;
				}
			} else {
				/* There is not enough room for the next byte.  Was anything written in this pass? */
				if (last < nwritten) {
					/* Yes.. Notify all of the waiting readers that more data is available */
					while (sem_getvalue(&mx_dev->read_wait, &sval) == 0 && sval < 0) {
						sem_post(&mx_dev->read_wait);
					}
				}
				last = nwritten;

				/* There is more to be written.. wait for data to be removed from the pipe */
				sched_lock();
				sem_post(&mx_dev->data_wait);
				mx_gdb_semtake(&mx_dev->write_wait);
				sched_unlock();
				mx_gdb_semtake(&mx_dev->data_wait);
			}
		}
	} else {
		SLSI_ERR_NODEV("Device is closed. Dropping %zu octets", len);
	}
}

int mx_gdb_poll(FAR struct file *filp, FAR struct pollfd *fds, bool setup)
{
	FAR struct inode *inode = filp->f_inode;
	FAR struct mx_mmap_dev *mx_dev = inode->i_private;
	pollevent_t eventset;
	uint16_t nbytes;
	int ret = OK;
	int i;

	SLSI_INFO_NODEV("Entering\n");

	if (setup) {
		/* This is a request to set up the poll.  Find an available
		 * slot for the poll structure reference
		 */
		for (i = 0; i < NPOLLWAITERS; i++) {
			/* Find an available slot */
			if (!mx_dev->d_fds[i]) {
				/* Bind the poll structure and this slot */
				mx_dev->d_fds[i] = fds;
				fds->priv = &mx_dev->d_fds[i];
				break;
			}
		}

		if (i >= NPOLLWAITERS) {
			fds->priv = NULL;
			ret = -EBUSY;
			goto errout;
		}

		/* Should immediately notify on any of the requested events?
		 * First, determine how many bytes are in the buffer
		 */
		if (mx_dev->write_idx >= mx_dev->read_idx) {
			nbytes = mx_dev->write_idx - mx_dev->read_idx;
		} else {
			nbytes = (MAX_MEMORY - 1) + mx_dev->write_idx - mx_dev->read_idx;
		}

		/* Notify the POLLOUT event if the pipe is not full, but only if
		 * there is readers. */
		eventset = 0;
		if (nbytes < (MAX_MEMORY - 1)) {
			eventset |= POLLOUT | POLLWRNORM;
		}

		/* Notify the POLLIN event if the pipe is not empty */
		if (nbytes > 0) {
			eventset |= POLLIN | POLLRDNORM;
		}

		/* Notify the POLLHUP event if the pipe is empty and no writers */
#if 0
		if (nbytes == 0 && mx_dev->nwriters <= 0) {
			eventset |= POLLHUP;
		}
#endif
		/* Change POLLOUT to POLLERR, if no readers and policy 0. */
		if (eventset) {
			mx_gdb_pollnotify(mx_dev, eventset);
		}
	} else {
		/* This is a request to tear down the poll. */
		struct pollfd **slot = (struct pollfd **)fds->priv;

#ifdef CONFIG_DEBUG
		if (!slot) {
			ret = -EIO;
			goto errout;
		}
#endif

		/* Remove all memory of the poll setup */
		*slot = NULL;
		fds->priv = NULL;
	}

errout:
	sem_post(&mx_dev->data_wait);
	return ret;
}

int mx_gdb_close(struct file *filp)
{
	FAR struct inode *inode = filp->f_inode;
	struct mx_mmap_dev *mx_dev = inode->i_private;
	int sval;

	SLSI_INFO_NODEV("Entering %s\n", __func__);

	DEBUGASSERT(mx_dev != NULL);
	if (mx_dev->filp == NULL) {
		SLSI_ERR_NODEV("Device already closed\n");
		return -EIO;
	}

	if (mx_dev != filp->f_priv) {
		SLSI_ERR_NODEV("Data mismatch\n");
		return -EIO;
	}

	/* Make sure that we have exclusive access to the device structure.
	 * NOTE: close() is supposed to return EINTR if interrupted, however
	 * I've never seen anyone check that.
	 */
	mx_gdb_semtake(&mx_dev->data_wait);

	/* Decrement the number of references on the dev.  Check if there are
	 * still outstanding references to the dev.
	 */
	/* Check if the decremented reference count would go to zero */
	if (--mx_dev->refs > 0) {
		/* No more references.. If opened for writing, decrement the count of
		 * writers on the pipe instance.
		 */
		if ((filp->f_oflags & O_WROK) != 0) {
			/* If there are no longer any writers on the pipe, then notify all of the
			 * waiting readers that they must return end-of-file.
			 */
			if (--mx_dev->nwriters <= 0) {
				while (sem_getvalue(&mx_dev->read_wait, &sval) == 0 && sval < 0) {
					sem_post(&mx_dev->read_wait);
				}

				/* Inform poll readers that other end closed. */
				mx_gdb_pollnotify(mx_dev, POLLHUP);
			}
		}

		/* If opened for reading, decrement the count of readers on the pipe
		 * instance.
		 */
		if ((filp->f_oflags & O_RDOK) != 0) {
			if (--mx_dev->nreaders <= 0) {
				/* Inform poll writers that other end closed. */
				mx_gdb_pollnotify(mx_dev, POLLERR);
			}
		}
	} else if (mx_dev->write_idx == mx_dev->read_idx) {
		/* deallocate the buffer now. */

		kmm_free(mx_dev->mem);
		mx_dev->mem = NULL;

		/* And reset all counts and indices */
		mx_dev->write_idx = 0;
		mx_dev->read_idx = 0;
		mx_dev->refs = 0;
		mx_dev->nwriters = 0;
		mx_dev->nreaders = 0;
	}

	sem_post(&mx_dev->data_wait);
	filp->f_priv = NULL;
	mx_dev->filp = NULL;
	return OK;
}

static const struct file_operations mx_gdb_fops = {
	mx_gdb_open,				/* open */
	mx_gdb_close,				/*close */
	mx_gdb_read,				/* read */
	mx_gdb_write,				/* write */
	0,							/* seek */
	0,							/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	mx_gdb_poll,				/* poll */
#endif
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	0							// mx_gdb_unlink     /*unlink*/
#endif
};

/*
 * Receive handler for messages from the FW along the maxwell management transport
 */
void client_gdb_probe(struct gdb_transport_client *gdb_client, struct gdb_transport *gdb_transport, char *dev_uid)
{
	int ret;
	char dev_name[20];
	struct mx_mmap_dev *mx_dev;
	long uid = 0;

	SLSI_INFO_NODEV("\n");
	/************/
	/* GDB node */
	/************/
	/* Search for free minors */
	if (gdb_transport->type == GDB_TRANSPORT_M4) {
		snprintf(dev_name, sizeof(dev_name), "%/dev/s_%d_%s", "mx", (int)uid, "m4_gdb");
	} else {
		snprintf(dev_name, sizeof(dev_name), "/dev/%s_%d_%s", "mx", (int)uid, "r4_gdb");
	}

	// register the driver in nuttx as chardev
	mx_dev = mx_gdb_allocdev();
	ret = register_driver(dev_name, &mx_gdb_fops, 0666, (void *)mx_dev);
	if (ret == EINVAL) {
		SLSI_ERR_NODEV("register driver failed for path %s\n", dev_name);
	} else if (ret == EEXIST) {
		SLSI_ERR_NODEV("register driver failed. %s already exists\n", dev_name);
	} else if (ret == ENOMEM) {
		SLSI_ERR_NODEV("register driver failed to allocate enough memory\n");
	}
	gdb_transport_register_channel_handler(gdb_transport, gdb_read_callback, (void *)mx_dev);
}

void client_gdb_remove(struct gdb_transport_client *gdb_client, struct gdb_transport *gdb_transport)
{
	char dev_name[20];
	long uid = 0;

	SLSI_INFO_NODEV("Entering\n");

	if (gdb_transport->type == GDB_TRANSPORT_M4) {
		snprintf(dev_name, sizeof(dev_name), "%/dev/s_%d_%s", "mx", (int)uid, "m4_gdb");
	} else {
		snprintf(dev_name, sizeof(dev_name), "/dev/%s_%d_%s", "mx", (int)uid, "r4_gdb");
	}

	/* unregister the driver for the chardev
	 * This could probably need some more cleanup etc.
	 */
	mx_gdb_freedev(gdb_transport->channel_handler_data);
	unregister_driver(dev_name);
}

/* Test client driver registration */
struct gdb_transport_client client_gdb_driver = {
	.name = "GDB client driver",
	.probe = client_gdb_probe,
	.remove = client_gdb_remove,
};

void scsc_mx_mmap_module_probe(struct scsc_mif_mmap_driver *abs_driver, struct scsc_mif_abs *mif_abs)
{
	int minor = 0;

	/* Search for free minors */
	minor = find_first_zero_bit(bitmap_minor, SCSC_MAX_INTERFACES);

	if (minor == SCSC_MAX_INTERFACES) {
		SLSI_ERR_NODEV("minor %d > SCSC_TTY_MINORS\n", minor);
		return;
	}
}

static struct scsc_mif_mmap_driver mx_module_mmap_if = {
	.name = "Maxwell mmap Driver",
	.probe = scsc_mx_mmap_module_probe,
	.remove = 0 /*scsc_mx_mmap_module_remove */ ,
};

int mx_mmap_init(void)
{
	int ret;

	SLSI_INFO_NODEV("%s: mx_mmap INIT; version: %d.%d\n", VER_MAJOR, VER_MINOR);

	scsc_mif_mmap_register(&mx_module_mmap_if);

	ret = gdb_transport_register_client(&client_gdb_driver);
	if (ret) {
		SLSI_ERR_NODEV("scsc_mx_module_register_client_module failed: r=%d\n", ret);
	}

	return 0;
}

void mx_mmap_deinit(void)
{
	gdb_transport_unregister_client(&client_gdb_driver);

	/* Notify lower layers that we are unloading */
	scsc_mif_mmap_unregister(&mx_module_mmap_if);
}
