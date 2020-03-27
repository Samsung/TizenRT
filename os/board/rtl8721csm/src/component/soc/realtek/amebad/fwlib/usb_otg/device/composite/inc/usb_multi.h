#ifndef USB_MULTI_H
#define USB_MULTI_H

#define EP0_DATA_OUT 0
#define EP0_DATA_IN  1

#define STRING_MANUFACTURER        1
#define STRING_PRODUCT             2
#define STRING_SERIALNUMBER        3

enum {
    VENDOR_WRITEN                = 0,
    VENDOR_READN                 = 1,
    VENDOR_DEV_ISREADY           = 2,

    // For DFU over USB
    VENDOR_GET_IMG_HEADER_FORMAT = 3,
    VENDOR_GET_BUF_SIZE          = 4,    // For Control
    VENDOR_START_TO_DFU          = 5,
    VENDOR_VALID_IMAGE           = 6,
    VENDOR_ACTIVE_RESET          = 7,
    VENDOR_DFU_DATA              = 8,

    VENDOR_MP_ENABLE             = 9,
    VENDOR_MP_CMD                = 10,

    VENDOR_RF_ENABLE             = 11,
};

// OpCode for Bulk Transfer
enum {
    BULK_HEADER_LOOPBACK         = 0,
    BULK_HEADER_DFU              = 1,
    BULK_HEADER_MP               = 2,
    BULK_HEADER_RF               = 3,
    BULK_HEADER_APP              = 4,
};
typedef struct _BULK_HEADER {
    uint8_t opcode;
    uint8_t res1;
    uint16_t length;
} BULK_HEADER;

#endif // USB_MULTI_H