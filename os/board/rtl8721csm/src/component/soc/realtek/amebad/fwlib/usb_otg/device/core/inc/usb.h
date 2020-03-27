#ifndef _USB_H_
#define _USB_H_

#include <stdint.h>
#include "usb_errno.h"
#include "usb_ch9.h"

enum usb_device_speed {
    USB_SPEED_UNKNOWN = 0,
    USB_SPEED_LOW,
    USB_SPEED_FULL,
    USB_SPEED_HIGH,
    USB_SPEED_VARIABL
};
    
enum usb_power_state_t {
    USB_PDN = 0,
    USB_RTK_PWRON_SEQ_DONE,
    USB_SW_INIT,
    USB_ATTACHED,
    USB_ADDRESSED,
    USB_CONFIGURATED,
    USB_SUSPEND_STATE,
    USB_ACTIVE
};

enum usb_status_t {
    USB_STATUS_INIT,
    USB_STATUS_DETACHED,
    USB_STATUS_ATTACHED
};

typedef struct {
    int usb_power_state;   // refer to usb_power_state_t
    int usb_status;        // refer to usb_status_t
} usb_info_t;

typedef struct {
    // Device speed
    u8 bSpeed;

    // Device descriptor parameters
    u8 bDeviceClass;
    u8 bDeviceSubClass;
    u8 bDeviceProtocol;
    u16 idVendor;
    u16 idProduct;
    u16 bcdDevice;

    // Config descriptor parameters
    u8 bmAttributes;
    u8 bMaxPower;

    // IRQ task priority
    u8 bIrqTaskPriority;

    // String descriptor parameters
    const char *sManufacturer;
    const char *sProduct;
    const char *sSerialNumber;
} usb_cfg_t;

// Default PID/VID
#define REALTEK_USB_VID             0x0BDA
#define REALTEK_USB_PID             0x4042 // 0x8773

// Common interrupt mask
#define OTG_COMMON_INTR_MASK        0xF1010806UL /* BIT1 | BIT2 | BIT11 | BIT16 | BIT24 | BIT28 | BIT29 | BIT30 | BIT31 */

// big enough to hold our biggest descriptor
#define USB_COMP_EP0_BUFSIZ         (1024 + 24)

// debug trace level
#define OTG_DBG_OUT_INT             (1U << 0)
#define OTG_DBG_IN_INT              (1U << 1)
#define OTG_DBG_OUT_STS             (1U << 2)    //do_setup_out_status_phase
#define OTG_DBG_EO0                 (1U << 3)    //
#define OTG_DBG_CIL_CORE_INIT       (1U << 4)    //
#define OTG_DBG_LINUX_INIT          (1U << 5)    //
#define OTG_DBG_PCD                 (1U << 6)    //
#define DBG_USB_AUDOCONFIG          (1U << 7)    //
#define DBG_USB_STAND_CMD           (1U << 8)    //
#define DBG_USB_EP_QUEUE            (1U << 8)    //
#define OTG_DBG_CIL                 (1U << 9)    //
#define DBG_USB_VENDOR              (1U << 10)
#define DBG_USB_COMPOSITE           (1U << 10)
#define OTG_DBG_PCD_INTR            (1U << 11)   //
#define DBG_USB_VENDOR_BULK         (1U << 12)
#define OTG_DBG_IMCOMP_ISOOUT_INT   (1U << 13)   //dwc_otg_pcd_handle_incomplete_isoc_out_intr
#define OTG_DBG_IMCOMP_ISOIN_INT    (1U << 14)   //dwc_otg_pcd_handle_incomplete_isoc_out_intr
#define OTG_DBG_ISO                 (1U << 15)   //dwc_otg_pcd_handle_incomplete_isoc_out_intr

/*
 * The USB records contain some unaligned little-endian word
 * components.  The U[SG]ETW macros take care of both the alignment
 * and endian problem and should always be used to access non-byte
 * values.
 */
//#define UGETW(w) ((w)[0] | ((w)[1] << 8))
#define UGETW(w) (w)

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef UNUSED
#define UNUSED(X) (void)X
#endif

void usb_set_cfg(usb_cfg_t *cfg);
usb_cfg_t *usb_get_cfg(void);

void usb_init(void);
void usb_deinit(void);
int usb_get_status(void);

#endif /* _USB_H_ */

