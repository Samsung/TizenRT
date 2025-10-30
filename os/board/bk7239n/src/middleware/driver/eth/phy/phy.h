/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2011 Freescale Semiconductor, Inc.
 *	Andy Fleming <afleming@gmail.com>
 *
 * This file pretty much stolen from Linux's mii.h/ethtool.h/phy.h
 */

#ifndef _PHY_H
#define _PHY_H


#include <components/log.h>
#include "common/bk_typedef.h"
#include "bk_list.h"
#include "phy_interface.h"
#include "mii.h"
#include "ethtool.h"
#include "mdio.h"

#define PHY_FIXED_ID		0xa5a55a5a
#define PHY_NCSI_ID            0xbeefcafe

/*
 * There is no actual id for this.
 * This is just a dummy id for gmii2rgmmi converter.
 */
#define PHY_GMII2RGMII_ID	0x5a5a5a5a

#define PHY_MAX_ADDR 32

#define PHY_FLAG_BROKEN_RESET	(1 << 0) /* soft reset not supported */

#define PHY_DEFAULT_FEATURES	(SUPPORTED_Autoneg | \
				 SUPPORTED_TP | \
				 SUPPORTED_MII)

#define PHY_10BT_FEATURES	(SUPPORTED_10baseT_Half | \
				 SUPPORTED_10baseT_Full)

#define PHY_100BT_FEATURES	(SUPPORTED_100baseT_Half | \
				 SUPPORTED_100baseT_Full)

#define PHY_1000BT_FEATURES	(SUPPORTED_1000baseT_Half | \
				 SUPPORTED_1000baseT_Full)

#define PHY_BASIC_FEATURES	(PHY_10BT_FEATURES | \
				 PHY_100BT_FEATURES | \
				 PHY_DEFAULT_FEATURES)

#define PHY_100BT1_FEATURES	(SUPPORTED_TP | \
				 SUPPORTED_MII | \
				 SUPPORTED_100baseT_Full)

#define PHY_GBIT_FEATURES	(PHY_BASIC_FEATURES | \
				 PHY_1000BT_FEATURES)

#define PHY_10G_FEATURES	(PHY_GBIT_FEATURES | \
				SUPPORTED_10000baseT_Full)

#ifndef PHY_ANEG_TIMEOUT
#define PHY_ANEG_TIMEOUT	4000
#endif

struct netif;
struct phy_device;

#define MDIO_NAME_LEN 32

struct mii_dev {
	struct list_head link;
	char name[MDIO_NAME_LEN];
	void *priv;
	int (*read)(struct mii_dev *bus, int addr, int devad, int reg);
	int (*write)(struct mii_dev *bus, int addr, int devad, int reg, u16 val);
	int (*reset)(struct mii_dev *bus);
	struct phy_device *phymap[PHY_MAX_ADDR];
	u32 phy_mask;
};

/* struct phy_driver: a structure which defines PHY behavior
 *
 * uid will contain a number which represents the PHY.  During
 * startup, the driver will poll the PHY to find out what its
 * UID--as defined by registers 2 and 3--is.  The 32-bit result
 * gotten from the PHY will be masked to
 * discard any bits which may change based on revision numbers
 * unimportant to functionality
 *
 */
struct phy_driver {
	char *name;
	unsigned int uid;
	unsigned int mask;
	unsigned int mmds;

	u32 features;

	/* Called to do any driver startup necessities */
	/* Will be called during phy_connect */
	int (*probe)(struct phy_device *phydev);

	/* Called to configure the PHY, and modify the controller
	 * based on the results.  Should be called after phy_connect */
	int (*config)(struct phy_device *phydev);

	/* Called when starting up the controller */
	int (*startup)(struct phy_device *phydev);

	/* Called when bringing down the controller */
	int (*shutdown)(struct phy_device *phydev);

	int (*readext)(struct phy_device *phydev, int addr, int devad, int reg);
	int (*writeext)(struct phy_device *phydev, int addr, int devad, int reg,
			u16 val);

	/* Phy specific driver override for reading a MMD register */
	int (*read_mmd)(struct phy_device *phydev, int devad, int reg);

	/* Phy specific driver override for writing a MMD register */
	int (*write_mmd)(struct phy_device *phydev, int devad, int reg,
			 u16 val);

	struct list_head list;

	/* driver private data */
	void *data;
};

struct phy_device {
	/* Information about the PHY type */
	/* And management functions */
	struct mii_dev *bus;
	struct phy_driver *drv;
	void *priv;

#ifdef CONFIG_DM_ETH
	struct udevice *dev;
	ofnode node;
#else
	struct netif *dev;
#endif

	/* forced speed & duplex (no autoneg)
	 * partner speed & duplex & pause (autoneg)
	 */
	int speed;
	int duplex;

	/* The most recently read link state */
	unsigned int link:1;
	unsigned int autoneg_complete:1;
	int port;
	phy_interface_t interface;

	u32 advertising;
	u32 supported;
	u32 mmds;

	int autoneg;
	int addr;
	int pause;
	int asym_pause;
	u32 phy_id;
	bool is_c45;
	u32 flags;
};

struct fixed_link {
	int phy_id;
	int duplex;
	int link_speed;
	int pause;
	int asym_pause;
};

/**
 * ieee8023_phy_init() - Initializes the PHY drivers
 * This function registers all available PHY drivers
 *
 * @return: 0 if OK, -ve on error
 */
int ieee8023_phy_init(void);

/**
 * phy_reset() - Resets the specified PHY
 * Issues a reset of the PHY and waits for it to complete
 *
 * @phydev:	PHY to reset
 * @return: 0 if OK, -ve on error
 */
int phy_reset(struct phy_device *phydev);

/**
 * phy_find_by_mask() - Searches for a PHY on the specified MDIO bus
 * The function checks the PHY addresses flagged in phy_mask and returns a
 * phy_device pointer if it detects a PHY.
 * This function should only be called if just one PHY is expected to be present
 * in the set of addresses flagged in phy_mask.  If multiple PHYs are present,
 * it is undefined which of these PHYs is returned.
 *
 * @bus:	MII/MDIO bus to scan
 * @phy_mask:	bitmap of PYH addresses to scan
 * @return: pointer to phy_device if a PHY is found, or NULL otherwise
 */
struct phy_device *phy_find_by_mask(struct mii_dev *bus, unsigned phy_mask);

#ifdef CONFIG_DM_ETH

/**
 * phy_connect_dev() - Associates the given pair of PHY and Ethernet devices
 * @phydev:	PHY device
 * @dev:	Ethernet device
 * @interface:	type of MAC-PHY interface
 */
void phy_connect_dev(struct phy_device *phydev, struct udevice *dev,
		     phy_interface_t interface);

/**
 * phy_connect() - Creates a PHY device for the Ethernet interface
 * Creates a PHY device for the PHY at the given address, if one doesn't exist
 * already, and associates it with the Ethernet device.
 * The function may be called with addr <= 0, in this case addr value is ignored
 * and the bus is scanned to detect a PHY.  Scanning should only be used if only
 * one PHY is expected to be present on the MDIO bus, otherwise it is undefined
 * which PHY is returned.
 *
 * @bus:	MII/MDIO bus that hosts the PHY
 * @addr:	PHY address on MDIO bus
 * @dev:	Ethernet device to associate to the PHY
 * @interface:	type of MAC-PHY interface
 * @return: pointer to phy_device if a PHY is found, or NULL otherwise
 */
struct phy_device *phy_connect(struct mii_dev *bus, int addr,
				struct udevice *dev,
				phy_interface_t interface);
/**
 * phy_device_create() - Create a PHY device
 *
 * @bus:		MII/MDIO bus that hosts the PHY
 * @addr:		PHY address on MDIO bus
 * @phy_id:		where to store the ID retrieved
 * @is_c45:		Device Identifiers if is_c45
 * @return: pointer to phy_device if a PHY is found, or NULL otherwise
 */
struct phy_device *phy_device_create(struct mii_dev *bus, int addr,
				     u32 phy_id, bool is_c45);

/**
 * phy_connect_phy_id() - Connect to phy device by reading PHY id
 *			  from phy node.
 *
 * @bus:		MII/MDIO bus that hosts the PHY
 * @dev:		Ethernet device to associate to the PHY
 * @return:		pointer to phy_device if a PHY is found,
 *			or NULL otherwise
 */
struct phy_device *phy_connect_phy_id(struct mii_dev *bus, struct udevice *dev,
				      int phyaddr);

static inline ofnode phy_get_ofnode(struct phy_device *phydev)
{
	if (ofnode_valid(phydev->node))
		return phydev->node;
	else
		return dev_ofnode(phydev->dev);
}
#else

/**
 * phy_connect_dev() - Associates the given pair of PHY and Ethernet devices
 * @phydev:	PHY device
 * @dev:	Ethernet device
 * @interface:	type of MAC-PHY interface
 */
void phy_connect_dev(struct phy_device *phydev, struct netif *dev,
		     phy_interface_t interface);

/**
 * phy_connect() - Creates a PHY device for the Ethernet interface
 * Creates a PHY device for the PHY at the given address, if one doesn't exist
 * already, and associates it with the Ethernet device.
 * The function may be called with addr <= 0, in this case addr value is ignored
 * and the bus is scanned to detect a PHY.  Scanning should only be used if only
 * one PHY is expected to be present on the MDIO bus, otherwise it is undefined
 * which PHY is returned.
 *
 * @bus:	MII/MDIO bus that hosts the PHY
 * @addr:	PHY address on MDIO bus
 * @dev:	Ethernet device to associate to the PHY
 * @interface:	type of MAC-PHY interface
 * @return: pointer to phy_device if a PHY is found, or NULL otherwise
 */
struct phy_device *phy_connect(struct mii_dev *bus, int addr,
				struct netif *dev,
				phy_interface_t interface);
#endif

int phy_read(struct phy_device *phydev, int devad, int regnum);
int phy_write(struct phy_device *phydev, int devad, int regnum, u16 val);
void phy_mmd_start_indirect(struct phy_device *phydev, int devad, int regnum);
int phy_read_mmd(struct phy_device *phydev, int devad, int regnum);
int phy_write_mmd(struct phy_device *phydev, int devad, int regnum, u16 val);
int phy_set_bits_mmd(struct phy_device *phydev, int devad, u32 regnum, u16 val);
int phy_clear_bits_mmd(struct phy_device *phydev, int devad, u32 regnum, u16 val);

int phy_startup(struct phy_device *phydev);
int phy_config(struct phy_device *phydev);
int phy_shutdown(struct phy_device *phydev);
int phy_register(struct phy_driver *drv);
int phy_set_supported(struct phy_device *phydev, u32 max_speed);
int phy_modify(struct phy_device *phydev, int devad, int regnum, u16 mask,
	       u16 set);
int genphy_config_aneg(struct phy_device *phydev);
int genphy_restart_aneg(struct phy_device *phydev);
int genphy_update_link(struct phy_device *phydev);
int genphy_parse_link(struct phy_device *phydev);
int genphy_config(struct phy_device *phydev);
int genphy_startup(struct phy_device *phydev);
int genphy_shutdown(struct phy_device *phydev);
int genphy_read_status(struct phy_device *phydev);

int phy_micrel_ksz90x1_init(void);
int phy_smsc_init(void);
int phy_realtek_init();

int board_phy_config(struct phy_device *phydev);
int get_phy_id(struct mii_dev *bus, int addr, int devad, u32 *phy_id);

/**
 * phy_interface_is_rgmii - Convenience function for testing if a PHY interface
 * is RGMII (all variants)
 * @phydev: the phy_device struct
 * @return: true if MII bus is RGMII or false if it is not
 */
static inline bool phy_interface_is_rgmii(struct phy_device *phydev)
{
	return phydev->interface >= PHY_INTERFACE_MODE_RGMII &&
		phydev->interface <= PHY_INTERFACE_MODE_RGMII_TXID;
}

/**
 * phy_interface_is_sgmii - Convenience function for testing if a PHY interface
 * is SGMII (all variants)
 * @phydev: the phy_device struct
 * @return: true if MII bus is SGMII or false if it is not
 */
static inline bool phy_interface_is_sgmii(struct phy_device *phydev)
{
	return phydev->interface >= PHY_INTERFACE_MODE_SGMII &&
		phydev->interface <= PHY_INTERFACE_MODE_QSGMII;
}

/* PHY UIDs for various PHYs that are referenced in external code */
#define PHY_UID_CS4340		0x13e51002
#define PHY_UID_CS4223		0x03e57003
#define PHY_UID_TN2020		0x00a19410
#define PHY_UID_IN112525_S03	0x02107440

#endif
