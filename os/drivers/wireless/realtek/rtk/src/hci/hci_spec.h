/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __HCI_SPEC_H__
#define __HCI_SPEC_H__

#if defined(CONFIG_GSPI_HCI)
#include "gspi/gspi_spec.h"

// SPI Header Files
#ifdef PLATFORM_LINUX
#include <linux/spi/spi.h>
#endif

#define GSPI_CMD_LEN 4
#define HAL_INTERFACE_CMD_LEN GSPI_CMD_LEN
#define GSPI_STATUS_LEN 8
#define HAL_INTERFACE_CMD_STATUS_LEN GSPI_STATUS_LEN
#define HAL_INTERFACE_OVERHEAD (HAL_INTERFACE_CMD_LEN + HAL_INTERFACE_OVERHEAD)
//reserve tx headroom in case of softap forwarding unicase packet
#define RX_RESERV_HEADROOM (SKB_WLAN_TX_EXTRA_LEN > RX_DRIVER_INFO + RXDESC_SIZE) ? (SKB_WLAN_TX_EXTRA_LEN - RX_DRIVER_INFO - RXDESC_SIZE) : 0
typedef struct gspi_data {
	u16 block_transfer_len; //u32 block_transfer_len;

#ifdef PLATFORM_LINUX
	struct spi_device *func;

	struct workqueue_struct *priv_wq;
	struct delayed_work irq_work;
#endif

	_mutex spi_mutex;
} GSPI_DATA, *PGSPI_DATA;

#define INTF_DATA GSPI_DATA

//extern void spi_set_intf_ops(struct _io_ops *pops);
extern void spi_int_hdl(PADAPTER padapter);
#define rtw_hci_interrupt_handler(__adapter) spi_int_hdl(__adapter)

#elif defined(CONFIG_SDIO_HCI)
#include "sdio/sdio_spec.h"

#define GSPI_CMD_LEN 0
#define HAL_INTERFACE_CMD_LEN GSPI_CMD_LEN
#define GSPI_STATUS_LEN 8
#define HAL_INTERFACE_CMD_STATUS_LEN GSPI_STATUS_LEN
#define HAL_INTERFACE_OVERHEAD (HAL_INTERFACE_CMD_LEN + HAL_INTERFACE_OVERHEAD)
#define RX_RESERV_HEADROOM (SKB_WLAN_TX_EXTRA_LEN > RX_DRIVER_INFO + RXDESC_SIZE) ? (SKB_WLAN_TX_EXTRA_LEN - RX_DRIVER_INFO - RXDESC_SIZE) : 0

typedef struct gspi_data {
	u16 block_transfer_len; //u32 block_transfer_len;

#ifdef PLATFORM_LINUX
	struct spi_device *func;

	struct workqueue_struct *priv_wq;
	struct delayed_work irq_work;
#endif

	struct sdio_func *func;

	_mutex spi_mutex;
} GSPI_DATA, *PGSPI_DATA;
typedef struct rtw_if_operations {
	int (*read)(struct dvobj_priv *d, int addr, void *buf,
				size_t len, bool fixed);
	int (*write)(struct dvobj_priv *d, int addr, void *buf,
				 size_t len, bool fixed);
} RTW_IF_OPS, *PRTW_IF_OPS;

#define INTF_DATA GSPI_DATA
#define INTF_OPS PRTW_IF_OPS

//extern void spi_set_intf_ops(struct _io_ops *pops);
extern void spi_int_hdl(PADAPTER padapter);
#define rtw_hci_interrupt_handler(__adapter) spi_int_hdl(__adapter)

#elif defined(CONFIG_USB_HCI)
#include <usb/usb_spec.h>
#include "usb_io_realtek.h"
#define GSPI_STATUS_LEN 8 //  xhl
#define HAL_INTERFACE_CMD_LEN 0
#define HAL_INTERFACE_CMD_STATUS_LEN 0
#define RX_RESERV_HEADROOM (SKB_WLAN_TX_EXTRA_LEN > RX_DRIVER_INFO + RXDESC_SIZE) ? (SKB_WLAN_TX_EXTRA_LEN - RX_DRIVER_INFO - RXDESC_SIZE) : 0

#define INTF_DATA USB_DATA

#elif defined(CONFIG_PCI_HCI)
#include <pci_osintf.h>
#ifdef PLATFORM_LINUX
#include <linux/pci.h>
#endif

#define INTF_CMD_LEN 0

#define INTEL_VENDOR_ID 0x8086
#define SIS_VENDOR_ID 0x1039
#define ATI_VENDOR_ID 0x1002
#define ATI_DEVICE_ID 0x7914
#define AMD_VENDOR_ID 0x1022

#define PCI_MAX_BRIDGE_NUMBER 255
#define PCI_MAX_DEVICES 32
#define PCI_MAX_FUNCTION 8

#define PCI_CONF_ADDRESS 0x0CF8 // PCI Configuration Space Address
#define PCI_CONF_DATA 0x0CFC	// PCI Configuration Space Data

#define PCI_CLASS_BRIDGE_DEV 0x06
#define PCI_SUBCLASS_BR_PCI_TO_PCI 0x04

#define PCI_CAPABILITY_ID_PCI_EXPRESS 0x10

#define U1DONTCARE 0xFF
#define U2DONTCARE 0xFFFF
#define U4DONTCARE 0xFFFFFFFF

#define PCI_VENDER_ID_REALTEK 0x10ec

#define HAL_HW_PCI_8180_DEVICE_ID 0x8180
#define HAL_HW_PCI_8185_DEVICE_ID 0x8185   //8185 or 8185b
#define HAL_HW_PCI_8188_DEVICE_ID 0x8188   //8185b
#define HAL_HW_PCI_8198_DEVICE_ID 0x8198   //8185b
#define HAL_HW_PCI_8190_DEVICE_ID 0x8190   //8190
#define HAL_HW_PCI_8723E_DEVICE_ID 0x8723  //8723E
#define HAL_HW_PCI_8192_DEVICE_ID 0x8192   //8192 PCI-E
#define HAL_HW_PCI_8192SE_DEVICE_ID 0x8192 //8192 SE
#define HAL_HW_PCI_8174_DEVICE_ID 0x8174   //8192 SE
#define HAL_HW_PCI_8173_DEVICE_ID 0x8173   //8191 SE Crab
#define HAL_HW_PCI_8172_DEVICE_ID 0x8172   //8191 SE RE
#define HAL_HW_PCI_8171_DEVICE_ID 0x8171   //8191 SE Unicron
#define HAL_HW_PCI_0045_DEVICE_ID 0x0045   //8190 PCI for Ceraga
#define HAL_HW_PCI_0046_DEVICE_ID 0x0046   //8190 Cardbus for Ceraga
#define HAL_HW_PCI_0044_DEVICE_ID 0x0044   //8192e PCIE for Ceraga
#define HAL_HW_PCI_0047_DEVICE_ID 0x0047   //8192e Express Card for Ceraga
#define HAL_HW_PCI_700F_DEVICE_ID 0x700F
#define HAL_HW_PCI_701F_DEVICE_ID 0x701F
#define HAL_HW_PCI_DLINK_DEVICE_ID 0x3304
#define HAL_HW_PCI_8192CET_DEVICE_ID 0x8191 //8192ce
#define HAL_HW_PCI_8192CE_DEVICE_ID 0x8178  //8192ce
#define HAL_HW_PCI_8191CE_DEVICE_ID 0x8177  //8192ce
#define HAL_HW_PCI_8188CE_DEVICE_ID 0x8176  //8192ce
#define HAL_HW_PCI_8192CU_DEVICE_ID 0x8191  //8192ce
#define HAL_HW_PCI_8192DE_DEVICE_ID 0x8193  //8192de
#define HAL_HW_PCI_002B_DEVICE_ID 0x002B	//8192de, provided by HW SD
#define HAL_HW_PCI_8188EE_DEVICE_ID 0x8179

#define HAL_MEMORY_MAPPED_IO_RANGE_8190PCI 0x1000 //8190 support 16 pages of IO registers
#define HAL_HW_PCI_REVISION_ID_8190PCI 0x00
#define HAL_MEMORY_MAPPED_IO_RANGE_8192PCIE 0x4000 //8192 support 16 pages of IO registers
#define HAL_HW_PCI_REVISION_ID_8192PCIE 0x01
#define HAL_MEMORY_MAPPED_IO_RANGE_8192SE 0x4000 //8192 support 16 pages of IO registers
#define HAL_HW_PCI_REVISION_ID_8192SE 0x10
#define HAL_HW_PCI_REVISION_ID_8192CE 0x1
#define HAL_MEMORY_MAPPED_IO_RANGE_8192CE 0x4000 //8192 support 16 pages of IO registers
#define HAL_HW_PCI_REVISION_ID_8192DE 0x0
#define HAL_MEMORY_MAPPED_IO_RANGE_8192DE 0x4000 //8192 support 16 pages of IO registers

enum pci_bridge_vendor {
	PCI_BRIDGE_VENDOR_INTEL = 0x0, //0b'0000,0001
	PCI_BRIDGE_VENDOR_ATI,		   //= 0x02,//0b'0000,0010
	PCI_BRIDGE_VENDOR_AMD,		   //= 0x04,//0b'0000,0100
	PCI_BRIDGE_VENDOR_SIS,		   //= 0x08,//0b'0000,1000
	PCI_BRIDGE_VENDOR_UNKNOWN,	 //= 0x40,//0b'0100,0000
	PCI_BRIDGE_VENDOR_MAX,		   //= 0x80
};

// copy this data structor defination from MSDN SDK
typedef struct _PCI_COMMON_CONFIG {
	u16 VendorID;
	u16 DeviceID;
	u16 Command;
	u16 Status;
	u8 RevisionID;
	u8 ProgIf;
	u8 SubClass;
	u8 BaseClass;
	u8 CacheLineSize;
	u8 LatencyTimer;
	u8 HeaderType;
	u8 BIST;

	union {
		struct _PCI_HEADER_TYPE_0 {
			u32 BaseAddresses[6];
			u32 CIS;
			u16 SubVendorID;
			u16 SubSystemID;
			u32 ROMBaseAddress;
			u8 CapabilitiesPtr;
			u8 Reserved1[3];
			u32 Reserved2;

			u8 InterruptLine;
			u8 InterruptPin;
			u8 MinimumGrant;
			u8 MaximumLatency;
		} type0;
	} u;

	u8 DeviceSpecific[108];
} PCI_COMMON_CONFIG, *PPCI_COMMON_CONFIG;

typedef struct _RT_PCI_CAPABILITIES_HEADER {
	u8 CapabilityID;
	u8 Next;
} RT_PCI_CAPABILITIES_HEADER, *PRT_PCI_CAPABILITIES_HEADER;

struct pci_priv {
	BOOLEAN pci_clk_req;

	u8 pciehdr_offset;
	//  PCIeCap is only differece between B-cut and C-cut.
	//  Configuration Space offset 72[7:4]
	//  0: A/B cut
	//  1: C cut and later.
	u8 pcie_cap;
	u8 linkctrl_reg;

	u8 busnumber;
	u8 devnumber;
	u8 funcnumber;

	u8 pcibridge_busnum;
	u8 pcibridge_devnum;
	u8 pcibridge_funcnum;
	u8 pcibridge_vendor;
	u16 pcibridge_vendorid;
	u16 pcibridge_deviceid;
	u8 pcibridge_pciehdr_offset;
	u8 pcibridge_linkctrlreg;

	u8 amd_l1_patch;
};

typedef struct _RT_ISR_CONTENT {
	union {
		u32 IntArray[2];
		u32 IntReg4Byte;
		u16 IntReg2Byte;
	};
} RT_ISR_CONTENT, *PRT_ISR_CONTENT;

//#define RegAddr(addr)           (addr + 0xB2000000UL)
//some platform macros will def here
static inline void NdisRawWritePortUlong(u32 port, u32 val)
{
	outl(val, port);
	//writel(val, (u8 *)RegAddr(port));
}

static inline void NdisRawWritePortUchar(u32 port, u8 val)
{
	outb(val, port);
	//writeb(val, (u8 *)RegAddr(port));
}

static inline void NdisRawReadPortUchar(u32 port, u8 *pval)
{
	*pval = inb(port);
	//*pval = readb((u8 *)RegAddr(port));
}

static inline void NdisRawReadPortUshort(u32 port, u16 *pval)
{
	*pval = inw(port);
	//*pval = readw((u8 *)RegAddr(port));
}

static inline void NdisRawReadPortUlong(u32 port, u32 *pval)
{
	*pval = inl(port);
	//*pval = readl((u8 *)RegAddr(port));
}
#elif defined(CONFIG_LX_HCI)
#define GSPI_CMD_LEN 0
#define GSPI_STATUS_LEN 0
#include "lxbus/lxbus_spec.h"

#elif defined(CONFIG_AXI_HCI)
#define GSPI_CMD_LEN 0
#define GSPI_STATUS_LEN 0
#include "axi/axi_spec.h"

#endif // interface define

#endif //__HCI_SPEC_H__
