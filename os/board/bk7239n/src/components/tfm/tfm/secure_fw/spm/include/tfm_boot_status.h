/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_BOOT_STATUS_H__
#define __TFM_BOOT_STATUS_H__

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Major numbers (4 bit) to identify
 * the consumer of shared data in runtime SW
 */
#define TLV_MAJOR_CORE     0x0
#define TLV_MAJOR_IAS      0x1
#define TLV_MAJOR_FWU      0x2

/**
 * The shared data between boot loader and runtime SW is TLV encoded. The
 * shared data is stored in a well known location in secure memory and this is
 * a contract between boot loader and runtime SW.
 *
 * The structure of shared data must be the following:
 *  - At the beginning there must be a header: struct shared_data_tlv_header
 *    This contains a magic number and a size field which covers the entire
 *    size of the shared data area including this header.
 *  - After the header there come the entries which are composed from an entry
 *    header structure: struct shared_data_tlv_entry and the data. In the entry
 *    header is a type field (tly_type) which identify the consumer of the
 *    entry in the runtime SW and specify the subtype of that data item. There
 *    is a size field (tlv_len) which covers the length of the data
 *    (not including the entry header structure). After this structure comes
 *    the actual data.
 *  - Arbitrary number and size of data entry can be in the shared memory area.
 *
 * This table gives of overview about the tlv_type field in the entry header.
 * The tlv_type always composed from a major and minor number. Major number
 * identifies the addressee in runtime SW, who should process the data entry.
 * Minor number used to encode more info about the data entry. The actual
 * definition of minor number could change per major number. In case of boot
 * status data, which is going to be processed by initial attestation service
 * the minor number is split further to two part: sw_module and claim. The
 * sw_module identifies the SW component in the system which the data item
 * belongs to and the claim part identifies the exact type of the data.
 *
 * |---------------------------------------|
 * |            tlv_type (16)              |
 * |---------------------------------------|
 * | tlv_major(4)|      tlv_minor(12)      |
 * |---------------------------------------|
 * | MAJOR_IAS   | sw_module(6) | claim(6) |
 * |---------------------------------------|
 * | MAJOR_CORE  |          TBD            |
 * |---------------------------------------|
 */

/* Initial attestation: SW components / SW modules
 * This list is intended to be adjusted per device. It contains more SW
 * components than currently available in TF-M project. It serves as an example,
 * what kind of SW components might be available.
 */
#define SW_GENERAL     0x00
#define SW_BL2         0x01
#define SW_PROT        0x02
#define SW_AROT        0x03
#define SW_SPE         0x04
#define SW_NSPE        0x05
#define SW_S_NS        0x06
#define SW_MAX         0x07

/* Initial attestation: Claim per SW components / SW modules */
/* Bits: 0-2 */
#define SW_VERSION       0x00
#define SW_SIGNER_ID     0x01
/* Reserved              0x02 */
#define SW_TYPE          0x03
/* Bits: 3-5 */
#define SW_MEASURE_VALUE 0x08
#define SW_MEASURE_TYPE  0x09
#define SW_BOOT_RECORD   0x3F

/* Initial attestation: General claim does not belong any particular SW
 * component. But they might be part of the boot status.
 */
#define BOOT_SEED          0x00
#define HW_VERSION         0x01
#define SECURITY_LIFECYCLE 0x02

/* General macros to handle TLV type */
#define MAJOR_MASK 0xF     /* 4  bit */
#define MAJOR_POS  12      /* 12 bit */
#define MINOR_MASK 0xFFF   /* 12 bit */

#define SET_TLV_TYPE(major, minor) \
        ((((major) & MAJOR_MASK) << MAJOR_POS) | ((minor) & MINOR_MASK))
#define GET_MAJOR(tlv_type) ((tlv_type) >> MAJOR_POS)
#define GET_MINOR(tlv_type) ((tlv_type) &  MINOR_MASK)

/* Initial attenstation and Firmware Update common macros */
#define MODULE_POS 6               /* 6 bit */
#define MODULE_MASK 0x3F           /* 6 bit */
#define CLAIM_MASK 0x3F            /* 6 bit */

/* Initial attestation specific macros */
#define MEASUREMENT_CLAIM_POS 3    /* 3 bit */

#define GET_IAS_MODULE(tlv_type) (GET_MINOR(tlv_type) >> MODULE_POS)
#define GET_IAS_CLAIM(tlv_type)  (GET_MINOR(tlv_type) & CLAIM_MASK)
#define SET_IAS_MINOR(sw_module, claim) (((sw_module) << 6) | (claim))
#define GET_IAS_MEASUREMENT_CLAIM(ias_claim) ((ias_claim) >> \
                                              MEASUREMENT_CLAIM_POS)
/* Firmware Update specific macros */
#define SET_FWU_MINOR(sw_module, claim)  \
                 ((uint16_t)((sw_module & MODULE_MASK) << 6) | \
                  (uint16_t)(claim & CLAIM_MASK))
#define GET_FWU_CLAIM(tlv_type)  ((uint16_t)GET_MINOR(tlv_type) & CLAIM_MASK)
#define GET_FWU_MODULE(tlv_type)  ((uint16_t)GET_MINOR(tlv_type) >> MODULE_POS)

/* Magic value which marks the beginning of shared data area in memory */
#define SHARED_DATA_TLV_INFO_MAGIC    0x2016

/**
 * Shared data TLV header.  All fields in little endian.
 *
 *    -----------------------------------
 *    | tlv_magic(16) | tlv_tot_len(16) |
 *    -----------------------------------
 */
struct shared_data_tlv_header {
    uint16_t tlv_magic;
    uint16_t tlv_tot_len; /* size of whole TLV area (including this header) */
};

#define SHARED_DATA_HEADER_SIZE sizeof(struct shared_data_tlv_header)

/**
 * Shared data TLV entry header format. All fields in little endian.
 *
 *    -------------------------------
 *    | tlv_type(16) |  tlv_len(16) |
 *    -------------------------------
 *    |         Raw data            |
 *    -------------------------------
 */
struct shared_data_tlv_entry {
    uint16_t tlv_type;
    uint16_t tlv_len; /* size of single TLV entry (not including this header) */
};

/**
 * \struct tfm_boot_data
 *
 * \brief Store the data for the runtime SW
 */
struct tfm_boot_data {
    struct shared_data_tlv_header header;
    uint8_t data[];
};

#define SHARED_DATA_ENTRY_HEADER_SIZE sizeof(struct shared_data_tlv_entry)
#define SHARED_DATA_ENTRY_SIZE(size) (size + SHARED_DATA_ENTRY_HEADER_SIZE)

#ifdef __cplusplus
}
#endif

#endif /* __TFM_BOOT_STATUS_H__ */
