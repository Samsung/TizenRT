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
 * arch/arm/src/mps/mps_lan9118.c
 *
 * The Ethernet controller of the MPS2 boards for the network manager
 * (os/net/netmgr) and lwIP: a SMSC LAN9118 (the boards have a LAN9220,
 * which is compatible with it), as QEMU models it.
 *
 *   qemu-system-arm -M mps2-an500 ... -nic user,model=lan9118
 *   qemu-system-arm -M mps2-an521 ... -nic user,model=lan9118
 *
 * Received frames are read from the receive FIFO on the high priority work
 * queue and copied to lwIP by the network manager.  A transmitted frame is
 * written to the transmit FIFO by the thread that sends it.
 *
 * The driver does not manage the PHY: it does not negotiate the link or
 * follow its state.
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
#include <tinyara/mutex.h>
#include <tinyara/signal.h>
#include <tinyara/wqueue.h>

#include "arm_internal.h"
#include "chip.h"

#ifdef CONFIG_MPS_LAN9118
#include <netinet/in.h>
#include <tinyara/net/if/ethernet.h>
#include <tinyara/netmgr/netdev_mgr.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_MPS_LAN9118

/* The controller of the board: the address and the interrupt of the NVIC.
 * The AN521 runs in the secure state and uses the secure alias.
 */

#if defined(CONFIG_ARCH_CHIP_MPS2_AN521)
#  define LAN9118_BASE          0x52000000
#  define LAN9118_IRQ           (16 + 48)
#elif defined(CONFIG_ARCH_CHIP_MPS2_AN500)
#  define LAN9118_BASE          0xa0000000
#  define LAN9118_IRQ           (16 + 13)
#else
#  error "No LAN9118 for this chip"
#endif

/* The registers */

#define LAN9118_RX_DATA_FIFO    0x00
#define LAN9118_TX_DATA_FIFO    0x20
#define LAN9118_RX_STATUS_FIFO  0x40
#define LAN9118_TX_STATUS_FIFO  0x48
#define LAN9118_ID_REV          0x50
#define LAN9118_IRQ_CFG         0x54
#define LAN9118_INT_STS         0x58
#define LAN9118_INT_EN          0x5c
#define LAN9118_BYTE_TEST       0x64
#define LAN9118_FIFO_INT        0x68
#define LAN9118_RX_CFG          0x6c
#define LAN9118_TX_CFG          0x70
#define LAN9118_HW_CFG          0x74
#define LAN9118_RX_FIFO_INF     0x7c
#define LAN9118_TX_FIFO_INF     0x80
#define LAN9118_PMT_CTRL        0x84
#define LAN9118_MAC_CSR_CMD     0xa4
#define LAN9118_MAC_CSR_DATA    0xa8

#define ID_REV_ID_MASK          0xffff0000
#define ID_REV_LAN9118          0x01180000
#define ID_REV_LAN9220          0x92200000
#define BYTE_TEST_VALUE         0x87654321

#define IRQ_CFG_IRQ_TYPE        (1 << 0)  /* Push-pull */
#define IRQ_CFG_IRQ_POL         (1 << 4)  /* Active high */
#define IRQ_CFG_IRQ_EN          (1 << 8)

#define INT_RSFL                (1 << 3)  /* Receive status FIFO level */

#define FIFO_INT_DEFAULT        0x48000000 /* Receive status level 0: one frame */

#define TX_CFG_TX_ON            (1 << 1)
#define TX_CFG_STOP_TX          (1 << 0)

#define HW_CFG_SRST             (1 << 0)
#define HW_CFG_TX_FIF_SZ_5K     (5 << 16)
#define HW_CFG_MBO              (1 << 20)

#define RX_FIFO_INF_STATUS(v)   (((v) >> 16) & 0xff) /* Frames in the receive FIFO */
#define TX_FIFO_INF_STATUS(v)   (((v) >> 16) & 0xff) /* Status words of sent frames */
#define TX_FIFO_INF_FREE(v)     ((v) & 0xffff)       /* Free bytes of the transmit FIFO */

#define PMT_CTRL_READY          (1 << 0)

#define RX_STATUS_LEN(v)        (((v) >> 16) & 0x3fff) /* With the CRC */
#define RX_STATUS_ERROR         (1 << 15)

#define TX_CMD_A_FIRST          (1 << 13)
#define TX_CMD_A_LAST           (1 << 12)

#define MAC_CSR_CMD_BUSY        (1u << 31)
#define MAC_CSR_CMD_READ        (1 << 30)

/* The registers of the MAC, by MAC_CSR_CMD and MAC_CSR_DATA */

#define MAC_CR                  1
#define MAC_ADDRH               2
#define MAC_ADDRL               3

#define MAC_CR_RXEN             (1 << 2)
#define MAC_CR_TXEN             (1 << 3)
#define MAC_CR_MCPAS            (1 << 19) /* Pass all of the multicast frames */
#define MAC_CR_FDPX             (1 << 20)

/* A frame: the Ethernet header, the payload of CONFIG_NET_ETH_MTU bytes and
 * a VLAN tag.  The receive FIFO has the CRC too, in words.
 */

#define LAN9118_FRAME_MAX       (CONFIG_NET_ETH_MTU + 18)
#define LAN9118_FRAME_MIN       60
#define LAN9118_CRC_LEN         4
#define LAN9118_RXBUF_WORDS     ((LAN9118_FRAME_MAX + LAN9118_CRC_LEN + 3) / 4)

/* How long the driver waits for the controller */

#define LAN9118_RETRIES         100
#define LAN9118_RETRY_USEC      1000

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct lan9118_dev_s {
	uintptr_t base;
	int irq;
	bool running;						/* The controller is initialized */
	struct netdev *dev;					/* The device of the network manager */
	uint8_t mac[NM_MAX_HWADDR_LEN];
	mutex_t txlock;
	struct work_s rxwork;
	uint32_t rxbuf[LAN9118_RXBUF_WORDS];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int lan9118_init(struct netdev *dev);
static int lan9118_deinit(struct netdev *dev);
static int lan9118_enable(struct netdev *dev);
static int lan9118_disable(struct netdev *dev);
static int lan9118_linkoutput(struct netdev *dev, void *data, uint16_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct lan9118_dev_s g_lan9118;

static struct ethernet_ops g_lan9118_ethops = {
	.init = lan9118_init,
	.deinit = lan9118_deinit,
	.enable = lan9118_enable,
	.disable = lan9118_disable,
};

static struct nic_io_ops g_lan9118_ioops = {
	.linkoutput = lan9118_linkoutput,
	.igmp_mac_filter = NULL,			/* The MAC passes all of the multicast frames */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static inline uint32_t lan9118_getreg(FAR struct lan9118_dev_s *priv, uint32_t offset)
{
	return getreg32(priv->base + offset);
}

static inline void lan9118_putreg(FAR struct lan9118_dev_s *priv, uint32_t offset, uint32_t value)
{
	putreg32(value, priv->base + offset);
}

/****************************************************************************
 * Name: lan9118_macwait / lan9118_macread / lan9118_macwrite
 *
 * Description:
 *   The registers of the MAC are accessed by a command register.
 *
 ****************************************************************************/

static int lan9118_macwait(FAR struct lan9118_dev_s *priv)
{
	int i;

	for (i = 0; i < LAN9118_RETRIES; i++) {
		if ((lan9118_getreg(priv, LAN9118_MAC_CSR_CMD) & MAC_CSR_CMD_BUSY) == 0) {
			return OK;
		}

		up_udelay(LAN9118_RETRY_USEC);
	}

	return -ETIMEDOUT;
}

static uint32_t lan9118_macread(FAR struct lan9118_dev_s *priv, uint32_t index)
{
	if (lan9118_macwait(priv) < 0) {
		return 0;
	}

	lan9118_putreg(priv, LAN9118_MAC_CSR_CMD, MAC_CSR_CMD_BUSY | MAC_CSR_CMD_READ | index);
	if (lan9118_macwait(priv) < 0) {
		return 0;
	}

	return lan9118_getreg(priv, LAN9118_MAC_CSR_DATA);
}

static int lan9118_macwrite(FAR struct lan9118_dev_s *priv, uint32_t index, uint32_t value)
{
	int ret = lan9118_macwait(priv);

	if (ret < 0) {
		return ret;
	}

	lan9118_putreg(priv, LAN9118_MAC_CSR_DATA, value);
	lan9118_putreg(priv, LAN9118_MAC_CSR_CMD, MAC_CSR_CMD_BUSY | index);
	return lan9118_macwait(priv);
}

/****************************************************************************
 * Name: lan9118_receive
 *
 * Description:
 *   Read one frame from the receive FIFO and pass it to the network stack.
 *
 ****************************************************************************/

static void lan9118_receive(FAR struct lan9118_dev_s *priv)
{
	uint32_t status = lan9118_getreg(priv, LAN9118_RX_STATUS_FIFO);
	uint32_t len = RX_STATUS_LEN(status);
	uint32_t words = (len + 3) / 4;
	uint32_t i;

	if ((status & RX_STATUS_ERROR) != 0 || len <= LAN9118_CRC_LEN || words > LAN9118_RXBUF_WORDS) {
		/* Drop the frame: read it out of the FIFO */

		for (i = 0; i < words; i++) {
			(void)lan9118_getreg(priv, LAN9118_RX_DATA_FIFO);
		}

		return;
	}

	for (i = 0; i < words; i++) {
		priv->rxbuf[i] = lan9118_getreg(priv, LAN9118_RX_DATA_FIFO);
	}

	/* The network manager copies the frame, without the CRC, to a pbuf */

	netdev_input(priv->dev, priv->rxbuf, len - LAN9118_CRC_LEN);
}

/****************************************************************************
 * Name: lan9118_rxwork
 *
 * Description:
 *   Receive the frames of the receive FIFO.  It runs on the high priority
 *   work queue.
 *
 ****************************************************************************/

static void lan9118_rxwork(FAR void *arg)
{
	FAR struct lan9118_dev_s *priv = (FAR struct lan9118_dev_s *)arg;

	if (!priv->running) {
		return;
	}

	/* Clear the interrupt, then empty the FIFO: a frame that is received
	 * after that sets the interrupt again.
	 */

	lan9118_putreg(priv, LAN9118_INT_STS, INT_RSFL);
	while (RX_FIFO_INF_STATUS(lan9118_getreg(priv, LAN9118_RX_FIFO_INF)) != 0) {
		lan9118_receive(priv);
	}

	/* The interrupt handler has disabled the interrupt */

	lan9118_putreg(priv, LAN9118_INT_EN, INT_RSFL);
}

/****************************************************************************
 * Name: lan9118_interrupt
 ****************************************************************************/

static int lan9118_interrupt(int irq, FAR void *context, FAR void *arg)
{
	FAR struct lan9118_dev_s *priv = (FAR struct lan9118_dev_s *)arg;

	/* The interrupt is a level: no more interrupts until the work has
	 * received the frames.
	 */

	lan9118_putreg(priv, LAN9118_INT_EN, 0);
	if (priv->running) {
		work_queue(HPWORK, &priv->rxwork, lan9118_rxwork, priv, 0);
	}

	return OK;
}

/****************************************************************************
 * Name: lan9118_linkoutput
 *
 * Description:
 *   Send a frame.  The network manager calls it with the frame of lwIP
 *   copied to one buffer.
 *
 ****************************************************************************/

static int lan9118_linkoutput(struct netdev *dev, void *data, uint16_t len)
{
	FAR struct lan9118_dev_s *priv = (FAR struct lan9118_dev_s *)dev->priv;
	FAR const uint8_t *src = (FAR const uint8_t *)data;
	uint32_t total = len < LAN9118_FRAME_MIN ? LAN9118_FRAME_MIN : len;
	uint32_t words = (total + 3) / 4;
	uint32_t inf;
	uint32_t i;
	int retry;

	if (data == NULL || len == 0 || len > LAN9118_FRAME_MAX) {
		return -1;
	}

	if (nxmutex_lock(&priv->txlock) < 0) {
		return -1;
	}

	if (!priv->running) {
		nxmutex_unlock(&priv->txlock);
		return -1;
	}

	/* The frame and the two command words must fit in the transmit FIFO */

	for (retry = 0; retry < LAN9118_RETRIES; retry++) {
		if (TX_FIFO_INF_FREE(lan9118_getreg(priv, LAN9118_TX_FIFO_INF)) >= words * 4 + 8) {
			break;
		}

		nxsig_usleep(LAN9118_RETRY_USEC);
	}

	if (retry == LAN9118_RETRIES) {
		nerr("ERROR: no room in the transmit FIFO\n");
		nxmutex_unlock(&priv->txlock);
		return -1;
	}

	/* One buffer for the frame.  A short frame is padded with zeros. */

	lan9118_putreg(priv, LAN9118_TX_DATA_FIFO, TX_CMD_A_FIRST | TX_CMD_A_LAST | total);
	lan9118_putreg(priv, LAN9118_TX_DATA_FIFO, total);
	for (i = 0; i < words; i++) {
		uint32_t word = 0;
		uint32_t offset = i * 4;

		if (offset < len) {
			memcpy(&word, src + offset, len - offset < 4 ? len - offset : 4);
		}

		lan9118_putreg(priv, LAN9118_TX_DATA_FIFO, word);
	}

	/* Drop the status of the frames that were sent */

	inf = lan9118_getreg(priv, LAN9118_TX_FIFO_INF);
	for (i = TX_FIFO_INF_STATUS(inf); i > 0; i--) {
		(void)lan9118_getreg(priv, LAN9118_TX_STATUS_FIFO);
	}

	nxmutex_unlock(&priv->txlock);
	return 0;
}

/****************************************************************************
 * Name: lan9118_stop
 ****************************************************************************/

static void lan9118_stop(FAR struct lan9118_dev_s *priv)
{
	priv->running = false;
	lan9118_putreg(priv, LAN9118_INT_EN, 0);
	lan9118_macwrite(priv, MAC_CR, 0);
	lan9118_putreg(priv, LAN9118_TX_CFG, TX_CFG_STOP_TX);
}

/****************************************************************************
 * Name: lan9118_init
 *
 * Description:
 *   Initialize the controller: the method of the network manager that is
 *   called when the interface is brought up.
 *
 ****************************************************************************/

static int lan9118_init(struct netdev *dev)
{
	FAR struct lan9118_dev_s *priv = (FAR struct lan9118_dev_s *)dev->priv;
	int ret;
	int i;

	if (priv->running) {
		return 0;
	}

	ret = nxmutex_lock(&priv->txlock);
	if (ret < 0) {
		return ret;
	}

	/* Reset the controller */

	lan9118_putreg(priv, LAN9118_INT_EN, 0);
	lan9118_putreg(priv, LAN9118_HW_CFG, HW_CFG_SRST);
	ret = -ETIMEDOUT;
	for (i = 0; i < LAN9118_RETRIES; i++) {
		if ((lan9118_getreg(priv, LAN9118_HW_CFG) & HW_CFG_SRST) == 0 &&
			(lan9118_getreg(priv, LAN9118_PMT_CTRL) & PMT_CTRL_READY) != 0) {
			ret = OK;
			break;
		}

		up_udelay(LAN9118_RETRY_USEC);
	}

	if (ret < 0 || lan9118_getreg(priv, LAN9118_BYTE_TEST) != BYTE_TEST_VALUE) {
		nerr("ERROR: the controller does not come out of the reset\n");
		nxmutex_unlock(&priv->txlock);
		return -EIO;
	}

	lan9118_putreg(priv, LAN9118_HW_CFG, HW_CFG_TX_FIF_SZ_5K | HW_CFG_MBO);

	/* The MAC address */

	lan9118_macwrite(priv, MAC_ADDRH, priv->mac[4] | (priv->mac[5] << 8));
	lan9118_macwrite(priv, MAC_ADDRL, priv->mac[0] | (priv->mac[1] << 8) | (priv->mac[2] << 16) | ((uint32_t)priv->mac[3] << 24));

	/* The interrupt: one for every received frame, active high */

	lan9118_putreg(priv, LAN9118_INT_STS, 0xffffffff);
	lan9118_putreg(priv, LAN9118_FIFO_INT, FIFO_INT_DEFAULT);
	lan9118_putreg(priv, LAN9118_IRQ_CFG, IRQ_CFG_IRQ_EN | IRQ_CFG_IRQ_POL | IRQ_CFG_IRQ_TYPE);

	/* Start the transmitter and the receiver */

	lan9118_putreg(priv, LAN9118_TX_CFG, TX_CFG_TX_ON);
	ret = lan9118_macwrite(priv, MAC_CR, MAC_CR_RXEN | MAC_CR_TXEN | MAC_CR_FDPX | MAC_CR_MCPAS);
	if (ret < 0) {
		nxmutex_unlock(&priv->txlock);
		return ret;
	}

	priv->running = true;
	lan9118_putreg(priv, LAN9118_INT_EN, INT_RSFL);

	nxmutex_unlock(&priv->txlock);
	return 0;
}

/****************************************************************************
 * Name: lan9118_deinit
 ****************************************************************************/

static int lan9118_deinit(struct netdev *dev)
{
	FAR struct lan9118_dev_s *priv = (FAR struct lan9118_dev_s *)dev->priv;

	up_disable_irq(priv->irq);
	nxmutex_lock(&priv->txlock);
	lan9118_stop(priv);
	nxmutex_unlock(&priv->txlock);
	work_cancel(HPWORK, &priv->rxwork);
	return 0;
}

/****************************************************************************
 * Name: lan9118_enable / lan9118_disable
 ****************************************************************************/

static int lan9118_enable(struct netdev *dev)
{
	FAR struct lan9118_dev_s *priv = (FAR struct lan9118_dev_s *)dev->priv;

	up_enable_irq(priv->irq);
	return 0;
}

static int lan9118_disable(struct netdev *dev)
{
	FAR struct lan9118_dev_s *priv = (FAR struct lan9118_dev_s *)dev->priv;

	up_disable_irq(priv->irq);
	return 0;
}

/****************************************************************************
 * Name: mps_lan9118_initialize
 *
 * Description:
 *   Register the controller to the network manager as an Ethernet
 *   interface (eth0).
 *
 ****************************************************************************/

static void mps_lan9118_initialize(void)
{
	FAR struct lan9118_dev_s *priv = &g_lan9118;
	struct netdev_config config;
	uint32_t id;
	uint32_t addrh;
	uint32_t addrl;

	priv->base = LAN9118_BASE;
	priv->irq = LAN9118_IRQ;

	id = lan9118_getreg(priv, LAN9118_ID_REV) & ID_REV_ID_MASK;
	if (lan9118_getreg(priv, LAN9118_BYTE_TEST) != BYTE_TEST_VALUE || (id != ID_REV_LAN9118 && id != ID_REV_LAN9220)) {
		nerr("ERROR: no LAN9118 at %p\n", (FAR void *)priv->base);
		return;
	}

	nxmutex_init(&priv->txlock);
	lan9118_putreg(priv, LAN9118_INT_EN, 0);

	/* The MAC address that the controller has loaded (QEMU gives it) */

	addrh = lan9118_macread(priv, MAC_ADDRH);
	addrl = lan9118_macread(priv, MAC_ADDRL);
	priv->mac[0] = addrl & 0xff;
	priv->mac[1] = (addrl >> 8) & 0xff;
	priv->mac[2] = (addrl >> 16) & 0xff;
	priv->mac[3] = (addrl >> 24) & 0xff;
	priv->mac[4] = addrh & 0xff;
	priv->mac[5] = (addrh >> 8) & 0xff;

	if (irq_attach(priv->irq, lan9118_interrupt, priv) < 0) {
		nerr("ERROR: irq_attach(%d) failed\n", priv->irq);
		return;
	}

	memset(&config, 0, sizeof(config));
	config.ops = &g_lan9118_ioops;
	config.flag = NM_FLAG_ETHARP | NM_FLAG_ETHERNET | NM_FLAG_BROADCAST | NM_FLAG_IGMP;

	/* The network manager allocates the buffer of the transmitted frame
	 * with this size: the frame with the Ethernet header.
	 */

	config.mtu = LAN9118_FRAME_MAX;
	config.hwaddr_len = NM_MAX_HWADDR_LEN;
	memcpy(config.hwaddr, priv->mac, NM_MAX_HWADDR_LEN);
	config.is_default = 1;
	config.type = NM_ETHERNET;
	config.t_ops.eth = &g_lan9118_ethops;
	config.d_ioctl = NULL;
	config.priv = priv;

	priv->dev = netdev_register(&config);
	if (priv->dev == NULL) {
		nerr("ERROR: netdev_register() failed\n");
		irq_detach(priv->irq);
		return;
	}

	netdev_set_hwaddr(priv->dev, priv->mac, NM_MAX_HWADDR_LEN);
}

#endif							/* CONFIG_MPS_LAN9118 */

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
#ifdef CONFIG_MPS_LAN9118
	mps_lan9118_initialize();
#endif
}
