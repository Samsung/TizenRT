/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/qemu/qemu_virtio_net.c
 *
 * The network device of the QEMU 'virt' machine for the network manager
 * (os/net/netmgr) and lwIP: a virtio-net device behind a virtio-mmio
 * transport.
 *
 *   qemu-system-arm -M virt ... -netdev user,id=net0 \
 *                   -device virtio-net-device,netdev=net0
 *
 * The machine has 32 virtio-mmio transports.  QEMU plugs the device in one
 * of them, so the driver scans them for a network device.  It supports the
 * legacy interface (version 1, the default of QEMU) and the interface of
 * virtio 1.0 (version 2, '-global virtio-mmio.force-legacy=false').
 *
 * The driver does not depend on a virtio framework: it has only what one
 * network device needs.  Two virtqueues of VIRTNET_QUEUE_SIZE buffers,
 * received frames are copied to lwIP by the network manager from the high
 * priority work queue, transmitted frames are copied to the buffers of the
 * transmit queue.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/kmalloc.h>
#include <tinyara/mutex.h>
#include <tinyara/signal.h>
#include <tinyara/wqueue.h>

#include "arm_internal.h"
#include "chip.h"

#ifdef CONFIG_QEMU_VIRTIO_NET
#include <netinet/in.h>
#include <tinyara/net/if/ethernet.h>
#include <tinyara/netmgr/netdev_mgr.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_QEMU_VIRTIO_NET

/* The virtio-mmio transports of the 'virt' machine */

#define VIRTIO_MMIO_BASE            0x0a000000
#define VIRTIO_MMIO_SIZE            0x200
#define VIRTIO_MMIO_NUM             32
#define VIRTIO_MMIO_IRQ(n)          (QEMU_SPI_IRQ_BASE + 16 + (n))

/* The registers of a virtio-mmio transport */

#define VIRTIO_MMIO_MAGIC           0x000 /* 'virt' */
#define VIRTIO_MMIO_VERSION         0x004 /* 1: legacy, 2: virtio 1.0 */
#define VIRTIO_MMIO_DEVICE_ID       0x008
#define VIRTIO_MMIO_HOST_FEATURES   0x010
#define VIRTIO_MMIO_HOST_FEAT_SEL   0x014
#define VIRTIO_MMIO_GUEST_FEATURES  0x020
#define VIRTIO_MMIO_GUEST_FEAT_SEL  0x024
#define VIRTIO_MMIO_GUEST_PAGE_SIZE 0x028 /* legacy */
#define VIRTIO_MMIO_QUEUE_SEL       0x030
#define VIRTIO_MMIO_QUEUE_NUM_MAX   0x034
#define VIRTIO_MMIO_QUEUE_NUM       0x038
#define VIRTIO_MMIO_QUEUE_ALIGN     0x03c /* legacy */
#define VIRTIO_MMIO_QUEUE_PFN       0x040 /* legacy */
#define VIRTIO_MMIO_QUEUE_READY     0x044
#define VIRTIO_MMIO_QUEUE_NOTIFY    0x050
#define VIRTIO_MMIO_INT_STATUS      0x060
#define VIRTIO_MMIO_INT_ACK         0x064
#define VIRTIO_MMIO_STATUS          0x070
#define VIRTIO_MMIO_QUEUE_DESC_LO   0x080
#define VIRTIO_MMIO_QUEUE_DESC_HI   0x084
#define VIRTIO_MMIO_QUEUE_AVAIL_LO  0x090
#define VIRTIO_MMIO_QUEUE_AVAIL_HI  0x094
#define VIRTIO_MMIO_QUEUE_USED_LO   0x0a0
#define VIRTIO_MMIO_QUEUE_USED_HI   0x0a4
#define VIRTIO_MMIO_CONFIG          0x100 /* virtio-net: the MAC address */

#define VIRTIO_MAGIC                0x74726976
#define VIRTIO_ID_NET               1

#define VIRTIO_STATUS_ACKNOWLEDGE   0x01
#define VIRTIO_STATUS_DRIVER        0x02
#define VIRTIO_STATUS_DRIVER_OK     0x04
#define VIRTIO_STATUS_FEATURES_OK   0x08
#define VIRTIO_STATUS_FAILED        0x80

#define VIRTIO_INT_USED_RING        0x01

/* The features: the driver takes the MAC address of the device, and the
 * interface of virtio 1.0 if the transport is not legacy.  No checksum or
 * segmentation offload, no mergeable receive buffers.
 */

#define VIRTIO_NET_F_MAC            (1 << 5)  /* Bit 5 of the bits 0-31 */
#define VIRTIO_F_VERSION_1          (1 << 0)  /* Bit 32: bit 0 of the bits 32-63 */

#define VRING_DESC_F_WRITE          2         /* The device writes the buffer */
#define VRING_AVAIL_F_NO_INTERRUPT  1

/* The virtqueues */

#define VIRTNET_RXQ                 0
#define VIRTNET_TXQ                 1
#define VIRTNET_QUEUE_SIZE          16        /* Must be a power of 2 */
#define VIRTNET_QUEUE_MASK          (VIRTNET_QUEUE_SIZE - 1)
#define VIRTNET_PAGE_SIZE           4096

/* The header of virtio-net that precedes every frame: 10 bytes with the
 * legacy interface, 12 bytes with virtio 1.0.  The driver sends it zeroed
 * (no offload).
 */

#define VIRTNET_HDR_LEGACY          10
#define VIRTNET_HDR_V1              12

/* A buffer holds the header and a frame: the Ethernet header, the payload
 * of CONFIG_NET_ETH_MTU bytes and a VLAN tag.
 */

#define VIRTNET_FRAME_MAX           (CONFIG_NET_ETH_MTU + 18)
#define VIRTNET_FRAME_MIN           60
#define VIRTNET_BUF_SIZE            ((VIRTNET_HDR_V1 + VIRTNET_FRAME_MAX + 63) & ~63)

/* How long the transmission waits for a free buffer */

#define VIRTNET_TX_RETRIES          100
#define VIRTNET_TX_RETRY_USEC       1000

#define VIRTNET_MB()                __sync_synchronize()

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct vring_desc_s {
	uint64_t addr;
	uint32_t len;
	uint16_t flags;
	uint16_t next;
};

struct vring_avail_s {
	uint16_t flags;
	uint16_t idx;
	uint16_t ring[VIRTNET_QUEUE_SIZE];
};

struct vring_used_elem_s {
	uint32_t id;
	uint32_t len;
};

struct vring_used_s {
	uint16_t flags;
	uint16_t idx;
	struct vring_used_elem_s ring[VIRTNET_QUEUE_SIZE];
};

struct virtnet_queue_s {
	FAR void *mem;						/* The memory of the rings */
	FAR struct vring_desc_s *desc;
	FAR struct vring_avail_s *avail;
	FAR volatile struct vring_used_s *used;
	FAR uint8_t *bufs;					/* VIRTNET_QUEUE_SIZE buffers */
	uint16_t last_used;					/* The used entries that are handled */
};

struct virtnet_dev_s {
	uintptr_t base;						/* The registers of the transport */
	int irq;
	uint32_t version;					/* 1: legacy, 2: virtio 1.0 */
	uint32_t hdrlen;					/* The length of the virtio-net header */
	bool running;						/* The device is initialized (DRIVER_OK) */
	struct netdev *dev;					/* The device of the network manager */
	struct virtnet_queue_s rxq;
	struct virtnet_queue_s txq;
	uint16_t tx_free[VIRTNET_QUEUE_SIZE];	/* The free transmit buffers */
	uint16_t tx_nfree;
	mutex_t txlock;
	struct work_s rxwork;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int virtnet_init(struct netdev *dev);
static int virtnet_deinit(struct netdev *dev);
static int virtnet_enable(struct netdev *dev);
static int virtnet_disable(struct netdev *dev);
static int virtnet_linkoutput(struct netdev *dev, void *data, uint16_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct virtnet_dev_s g_virtnet;

static struct ethernet_ops g_virtnet_ethops = {
	.init = virtnet_init,
	.deinit = virtnet_deinit,
	.enable = virtnet_enable,
	.disable = virtnet_disable,
};

static struct nic_io_ops g_virtnet_ioops = {
	.linkoutput = virtnet_linkoutput,
	.igmp_mac_filter = NULL,			/* The device receives all of the multicast frames */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static inline uint32_t virtnet_getreg(FAR struct virtnet_dev_s *priv, uint32_t offset)
{
	return getreg32(priv->base + offset);
}

static inline void virtnet_putreg(FAR struct virtnet_dev_s *priv, uint32_t offset, uint32_t value)
{
	putreg32(value, priv->base + offset);
}

static inline FAR uint8_t *virtnet_buf(FAR struct virtnet_queue_s *vq, uint16_t id)
{
	return &vq->bufs[(size_t)id * VIRTNET_BUF_SIZE];
}

/****************************************************************************
 * Name: virtnet_queue_alloc
 *
 * Description:
 *   Allocate the rings and the buffers of a virtqueue.  The rings have the
 *   layout of the legacy interface: the descriptors and the available ring
 *   in the first page, the used ring in the second.  The interface of
 *   virtio 1.0 takes the three addresses, so the layout fits both.
 *
 ****************************************************************************/

static int virtnet_queue_alloc(FAR struct virtnet_queue_s *vq)
{
	if (vq->mem == NULL) {
		vq->mem = kmm_memalign(VIRTNET_PAGE_SIZE, 2 * VIRTNET_PAGE_SIZE);
		vq->bufs = kmm_memalign(64, VIRTNET_QUEUE_SIZE * VIRTNET_BUF_SIZE);
		if (vq->mem == NULL || vq->bufs == NULL) {
			return -ENOMEM;
		}
	}

	memset(vq->mem, 0, 2 * VIRTNET_PAGE_SIZE);
	vq->desc = (FAR struct vring_desc_s *)vq->mem;
	vq->avail = (FAR struct vring_avail_s *)((FAR uint8_t *)vq->mem + VIRTNET_QUEUE_SIZE * sizeof(struct vring_desc_s));
	vq->used = (FAR volatile struct vring_used_s *)((FAR uint8_t *)vq->mem + VIRTNET_PAGE_SIZE);
	vq->last_used = 0;
	return OK;
}

/****************************************************************************
 * Name: virtnet_queue_setup
 *
 * Description:
 *   Give the rings of a virtqueue to the device.
 *
 ****************************************************************************/

static int virtnet_queue_setup(FAR struct virtnet_dev_s *priv, int index, FAR struct virtnet_queue_s *vq)
{
	virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_SEL, index);
	if (virtnet_getreg(priv, VIRTIO_MMIO_QUEUE_NUM_MAX) < VIRTNET_QUEUE_SIZE) {
		nerr("ERROR: queue %d is too small\n", index);
		return -EIO;
	}

	virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_NUM, VIRTNET_QUEUE_SIZE);
	if (priv->version == 1) {
		virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_ALIGN, VIRTNET_PAGE_SIZE);
		virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_PFN, (uintptr_t)vq->mem / VIRTNET_PAGE_SIZE);
	} else {
		virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_DESC_LO, (uintptr_t)vq->desc);
		virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_DESC_HI, 0);
		virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_AVAIL_LO, (uintptr_t)vq->avail);
		virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_AVAIL_HI, 0);
		virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_USED_LO, (uintptr_t)vq->used);
		virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_USED_HI, 0);
		virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_READY, 1);
	}

	return OK;
}

/****************************************************************************
 * Name: virtnet_rxwork
 *
 * Description:
 *   Pass the received frames to the network stack and give the buffers
 *   back to the device.  It runs on the high priority work queue.
 *
 ****************************************************************************/

static void virtnet_rxwork(FAR void *arg)
{
	FAR struct virtnet_dev_s *priv = (FAR struct virtnet_dev_s *)arg;
	FAR struct virtnet_queue_s *vq = &priv->rxq;

	if (!priv->running) {
		return;
	}

	for (;;) {
		bool received = false;

		while (vq->last_used != vq->used->idx) {
			uint32_t id = vq->used->ring[vq->last_used & VIRTNET_QUEUE_MASK].id;
			uint32_t len = vq->used->ring[vq->last_used & VIRTNET_QUEUE_MASK].len;

			if (id < VIRTNET_QUEUE_SIZE) {
				if (len > priv->hdrlen && len <= VIRTNET_BUF_SIZE) {
					/* The network manager copies the frame to a pbuf */

					netdev_input(priv->dev, virtnet_buf(vq, id) + priv->hdrlen, len - priv->hdrlen);
				}

				/* The buffer is available for the device again */

				vq->avail->ring[vq->avail->idx & VIRTNET_QUEUE_MASK] = id;
				VIRTNET_MB();
				vq->avail->idx++;
				VIRTNET_MB();
				received = true;
			}

			vq->last_used++;
		}

		if (received) {
			virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_NOTIFY, VIRTNET_RXQ);
		}

		/* The interrupt handler has disabled the interrupt of the queue.
		 * Enable it, and look again for the frames that were received before
		 * that.
		 */

		vq->avail->flags = 0;
		VIRTNET_MB();
		if (vq->last_used == vq->used->idx) {
			break;
		}

		vq->avail->flags = VRING_AVAIL_F_NO_INTERRUPT;
	}
}

/****************************************************************************
 * Name: virtnet_interrupt
 ****************************************************************************/

static int virtnet_interrupt(int irq, FAR void *context, FAR void *arg)
{
	FAR struct virtnet_dev_s *priv = (FAR struct virtnet_dev_s *)arg;
	uint32_t status = virtnet_getreg(priv, VIRTIO_MMIO_INT_STATUS);

	virtnet_putreg(priv, VIRTIO_MMIO_INT_ACK, status);

	if ((status & VIRTIO_INT_USED_RING) != 0 && priv->running) {
		/* No more interrupts until the work has handled the frames */

		priv->rxq.avail->flags = VRING_AVAIL_F_NO_INTERRUPT;
		work_queue(HPWORK, &priv->rxwork, virtnet_rxwork, priv, 0);
	}

	return OK;
}

/****************************************************************************
 * Name: virtnet_txreclaim
 *
 * Description:
 *   Take back the buffers of the frames that the device has sent.
 *
 ****************************************************************************/

static void virtnet_txreclaim(FAR struct virtnet_dev_s *priv)
{
	FAR struct virtnet_queue_s *vq = &priv->txq;

	while (vq->last_used != vq->used->idx) {
		uint32_t id = vq->used->ring[vq->last_used & VIRTNET_QUEUE_MASK].id;

		if (id < VIRTNET_QUEUE_SIZE && priv->tx_nfree < VIRTNET_QUEUE_SIZE) {
			priv->tx_free[priv->tx_nfree++] = id;
		}

		vq->last_used++;
	}
}

/****************************************************************************
 * Name: virtnet_linkoutput
 *
 * Description:
 *   Send a frame.  The network manager calls it with the frame of lwIP
 *   copied to one buffer.
 *
 ****************************************************************************/

static int virtnet_linkoutput(struct netdev *dev, void *data, uint16_t len)
{
	FAR struct virtnet_dev_s *priv = (FAR struct virtnet_dev_s *)dev->priv;
	FAR struct virtnet_queue_s *vq = &priv->txq;
	FAR uint8_t *buf;
	uint32_t total;
	uint16_t id;
	int retry;
	int ret;

	if (data == NULL || len == 0 || len > VIRTNET_FRAME_MAX) {
		return -1;
	}

	ret = nxmutex_lock(&priv->txlock);
	if (ret < 0) {
		return -1;
	}

	if (!priv->running) {
		nxmutex_unlock(&priv->txlock);
		return -1;
	}

	virtnet_txreclaim(priv);
	for (retry = 0; priv->tx_nfree == 0 && retry < VIRTNET_TX_RETRIES; retry++) {
		nxsig_usleep(VIRTNET_TX_RETRY_USEC);
		virtnet_txreclaim(priv);
	}

	if (priv->tx_nfree == 0) {
		nerr("ERROR: no transmit buffer\n");
		nxmutex_unlock(&priv->txlock);
		return -1;
	}

	id = priv->tx_free[--priv->tx_nfree];
	buf = virtnet_buf(vq, id);
	memset(buf, 0, priv->hdrlen);
	memcpy(buf + priv->hdrlen, data, len);
	total = len;
	if (total < VIRTNET_FRAME_MIN) {
		memset(buf + priv->hdrlen + total, 0, VIRTNET_FRAME_MIN - total);
		total = VIRTNET_FRAME_MIN;
	}

	vq->desc[id].addr = (uintptr_t)buf;
	vq->desc[id].len = priv->hdrlen + total;
	vq->desc[id].flags = 0;
	vq->desc[id].next = 0;

	vq->avail->ring[vq->avail->idx & VIRTNET_QUEUE_MASK] = id;
	VIRTNET_MB();
	vq->avail->idx++;
	VIRTNET_MB();
	virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_NOTIFY, VIRTNET_TXQ);

	nxmutex_unlock(&priv->txlock);
	return 0;
}

/****************************************************************************
 * Name: virtnet_reset
 ****************************************************************************/

static void virtnet_reset(FAR struct virtnet_dev_s *priv)
{
	priv->running = false;
	virtnet_putreg(priv, VIRTIO_MMIO_STATUS, 0);
}

/****************************************************************************
 * Name: virtnet_init
 *
 * Description:
 *   Initialize the device: the method of the network manager that is
 *   called when the interface is brought up.
 *
 ****************************************************************************/

static int virtnet_init(struct netdev *dev)
{
	FAR struct virtnet_dev_s *priv = (FAR struct virtnet_dev_s *)dev->priv;
	uint32_t features;
	uint32_t status;
	int ret;
	int i;

	if (priv->running) {
		return 0;
	}

	ret = nxmutex_lock(&priv->txlock);
	if (ret < 0) {
		return ret;
	}

	/* Reset the device and tell it that the driver is here */

	virtnet_reset(priv);
	status = VIRTIO_STATUS_ACKNOWLEDGE;
	virtnet_putreg(priv, VIRTIO_MMIO_STATUS, status);
	status |= VIRTIO_STATUS_DRIVER;
	virtnet_putreg(priv, VIRTIO_MMIO_STATUS, status);

	/* The features */

	virtnet_putreg(priv, VIRTIO_MMIO_HOST_FEAT_SEL, 0);
	features = virtnet_getreg(priv, VIRTIO_MMIO_HOST_FEATURES) & VIRTIO_NET_F_MAC;
	virtnet_putreg(priv, VIRTIO_MMIO_GUEST_FEAT_SEL, 0);
	virtnet_putreg(priv, VIRTIO_MMIO_GUEST_FEATURES, features);

	if (priv->version == 1) {
		priv->hdrlen = VIRTNET_HDR_LEGACY;
		virtnet_putreg(priv, VIRTIO_MMIO_GUEST_PAGE_SIZE, VIRTNET_PAGE_SIZE);
	} else {
		priv->hdrlen = VIRTNET_HDR_V1;
		virtnet_putreg(priv, VIRTIO_MMIO_GUEST_FEAT_SEL, 1);
		virtnet_putreg(priv, VIRTIO_MMIO_GUEST_FEATURES, VIRTIO_F_VERSION_1);

		status |= VIRTIO_STATUS_FEATURES_OK;
		virtnet_putreg(priv, VIRTIO_MMIO_STATUS, status);
		if ((virtnet_getreg(priv, VIRTIO_MMIO_STATUS) & VIRTIO_STATUS_FEATURES_OK) == 0) {
			nerr("ERROR: the device refuses the features\n");
			ret = -EIO;
			goto errout;
		}
	}

	/* The virtqueues */

	ret = virtnet_queue_alloc(&priv->rxq);
	if (ret >= 0) {
		ret = virtnet_queue_alloc(&priv->txq);
	}

	if (ret >= 0) {
		ret = virtnet_queue_setup(priv, VIRTNET_RXQ, &priv->rxq);
	}

	if (ret >= 0) {
		ret = virtnet_queue_setup(priv, VIRTNET_TXQ, &priv->txq);
	}

	if (ret < 0) {
		goto errout;
	}

	/* Every receive buffer is available for the device */

	for (i = 0; i < VIRTNET_QUEUE_SIZE; i++) {
		priv->rxq.desc[i].addr = (uintptr_t)virtnet_buf(&priv->rxq, i);
		priv->rxq.desc[i].len = VIRTNET_BUF_SIZE;
		priv->rxq.desc[i].flags = VRING_DESC_F_WRITE;
		priv->rxq.desc[i].next = 0;
		priv->rxq.avail->ring[i] = i;
	}

	priv->rxq.avail->idx = VIRTNET_QUEUE_SIZE;

	/* Every transmit buffer is free.  The buffers are taken back when the
	 * next frame is sent, without an interrupt.
	 */

	for (i = 0; i < VIRTNET_QUEUE_SIZE; i++) {
		priv->tx_free[i] = i;
	}

	priv->tx_nfree = VIRTNET_QUEUE_SIZE;
	priv->txq.avail->flags = VRING_AVAIL_F_NO_INTERRUPT;
	VIRTNET_MB();

	status |= VIRTIO_STATUS_DRIVER_OK;
	virtnet_putreg(priv, VIRTIO_MMIO_STATUS, status);
	priv->running = true;
	virtnet_putreg(priv, VIRTIO_MMIO_QUEUE_NOTIFY, VIRTNET_RXQ);

	nxmutex_unlock(&priv->txlock);
	return 0;

errout:
	virtnet_putreg(priv, VIRTIO_MMIO_STATUS, VIRTIO_STATUS_FAILED);
	nxmutex_unlock(&priv->txlock);
	return ret;
}

/****************************************************************************
 * Name: virtnet_deinit
 ****************************************************************************/

static int virtnet_deinit(struct netdev *dev)
{
	FAR struct virtnet_dev_s *priv = (FAR struct virtnet_dev_s *)dev->priv;

	up_disable_irq(priv->irq);
	nxmutex_lock(&priv->txlock);
	virtnet_reset(priv);
	nxmutex_unlock(&priv->txlock);
	work_cancel(HPWORK, &priv->rxwork);
	return 0;
}

/****************************************************************************
 * Name: virtnet_enable / virtnet_disable
 ****************************************************************************/

static int virtnet_enable(struct netdev *dev)
{
	FAR struct virtnet_dev_s *priv = (FAR struct virtnet_dev_s *)dev->priv;

	up_enable_irq(priv->irq);
	return 0;
}

static int virtnet_disable(struct netdev *dev)
{
	FAR struct virtnet_dev_s *priv = (FAR struct virtnet_dev_s *)dev->priv;

	up_disable_irq(priv->irq);
	return 0;
}

/****************************************************************************
 * Name: virtnet_probe
 *
 * Description:
 *   Look for a network device in the virtio-mmio transports.
 *
 ****************************************************************************/

static bool virtnet_probe(FAR struct virtnet_dev_s *priv)
{
	int i;

	for (i = 0; i < VIRTIO_MMIO_NUM; i++) {
		uintptr_t base = VIRTIO_MMIO_BASE + i * VIRTIO_MMIO_SIZE;
		uint32_t version;

		if (getreg32(base + VIRTIO_MMIO_MAGIC) != VIRTIO_MAGIC || getreg32(base + VIRTIO_MMIO_DEVICE_ID) != VIRTIO_ID_NET) {
			continue;
		}

		version = getreg32(base + VIRTIO_MMIO_VERSION);
		if (version != 1 && version != 2) {
			continue;
		}

		priv->base = base;
		priv->irq = VIRTIO_MMIO_IRQ(i);
		priv->version = version;
		return true;
	}

	return false;
}

/****************************************************************************
 * Name: qemu_virtio_net_initialize
 *
 * Description:
 *   Register the virtio-net device, if QEMU has one, to the network
 *   manager as an Ethernet interface (eth0).
 *
 ****************************************************************************/

static void qemu_virtio_net_initialize(void)
{
	FAR struct virtnet_dev_s *priv = &g_virtnet;
	struct netdev_config config;
	uint8_t mac[NM_MAX_HWADDR_LEN];
	int i;

	if (!virtnet_probe(priv)) {
		ninfo("No virtio-net device\n");
		return;
	}

	virtnet_reset(priv);
	nxmutex_init(&priv->txlock);

	/* The MAC address that QEMU gives to the device */

	for (i = 0; i < NM_MAX_HWADDR_LEN; i++) {
		mac[i] = getreg8(priv->base + VIRTIO_MMIO_CONFIG + i);
	}

	if (irq_attach(priv->irq, virtnet_interrupt, priv) < 0) {
		nerr("ERROR: irq_attach(%d) failed\n", priv->irq);
		return;
	}

	memset(&config, 0, sizeof(config));
	config.ops = &g_virtnet_ioops;
	config.flag = NM_FLAG_ETHARP | NM_FLAG_ETHERNET | NM_FLAG_BROADCAST | NM_FLAG_IGMP;

	/* The network manager allocates the buffer of the transmitted frame
	 * with this size: the frame with the Ethernet header.
	 */

	config.mtu = VIRTNET_FRAME_MAX;
	config.hwaddr_len = NM_MAX_HWADDR_LEN;
	memcpy(config.hwaddr, mac, NM_MAX_HWADDR_LEN);
	config.is_default = 1;
	config.type = NM_ETHERNET;
	config.t_ops.eth = &g_virtnet_ethops;
	config.d_ioctl = NULL;
	config.priv = priv;

	priv->dev = netdev_register(&config);
	if (priv->dev == NULL) {
		nerr("ERROR: netdev_register() failed\n");
		irq_detach(priv->irq);
		return;
	}

	netdev_set_hwaddr(priv->dev, mac, NM_MAX_HWADDR_LEN);
}

#endif							/* CONFIG_QEMU_VIRTIO_NET */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_netinitialize
 *
 * Description:
 *   Initialize the network devices of the chip.  It is called by
 *   up_initialize(), after net_initialize() has initialized the network
 *   stack and the network manager.
 *
 ****************************************************************************/

void arm_netinitialize(void)
{
#ifdef CONFIG_QEMU_VIRTIO_NET
	qemu_virtio_net_initialize();
#endif
}
