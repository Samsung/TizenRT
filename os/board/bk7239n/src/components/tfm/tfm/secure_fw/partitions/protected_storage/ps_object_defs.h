/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PS_OBJECT_DEFS_H__
#define __PS_OBJECT_DEFS_H__

#include <stdint.h>

#include "psa/protected_storage.h"

#ifdef PS_ENCRYPTION
#include "crypto/ps_crypto_interface.h"
#endif

/*!
 * \struct ps_object_info_t
 *
 * \brief Object information.
 */
struct ps_object_info_t {
    uint32_t current_size; /*!< Current size of the object content in bytes */
    uint32_t max_size;     /*!< Maximum size of the object content in bytes */
    psa_storage_create_flags_t create_flags; /*!< Object creation flags */
};

/*!
 * \struct ps_obj_header_t
 *
 * \brief Metadata attached as a header to object data before storage.
 */
struct ps_obj_header_t {
#ifdef PS_ENCRYPTION
    union ps_crypto_t crypto;     /*!< Crypto metadata */
#else
    uint32_t version;              /*!< Object version */
    uint32_t fid;                  /*!< File ID */
#endif
    struct ps_object_info_t info; /*!< Object information */
};


#define PS_MAX_OBJECT_DATA_SIZE  PS_MAX_ASSET_SIZE

/*!
 * \struct ps_object_t
 *
 * \brief The object to be written to the file system below. Made up of the
 *        object header and the object data.
 */
struct ps_object_t {
    struct ps_obj_header_t header;         /*!< Object header */
    uint8_t data[PS_MAX_OBJECT_DATA_SIZE]; /*!< Object data */
};


#define PS_OBJECT_HEADER_SIZE    sizeof(struct ps_obj_header_t)
#define PS_MAX_OBJECT_SIZE       sizeof(struct ps_object_t)

/*!
 * \def PS_MAX_NUM_OBJECTS
 *
 * \brief Specifies the maximum number of objects in the system, which is the
 *        number of defined assets, the object table and 2 temporary objects to
 *        store the temporary object table and temporary updated object.
 */
#define PS_MAX_NUM_OBJECTS (PS_NUM_ASSETS + 3)

#endif /* __PS_OBJECT_DEFS_H__ */
