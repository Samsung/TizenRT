/*
 * Copyright (c) 2024 Syntiant Corp.  All rights reserved.
 * Contact at http://www.syntiant.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file LICENSE in the main
 * directory of this source tree, or the OpenIB.org BSD license below.  Any
 * code involving Linux software will require selection of the GNU General
 * Public License (GPL) Version 2.
 *
 * OPENIB.ORG BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 	** SDK: v112.3.6-Samsung **
*/
#include <syntiant_ilib/syntiant_portability.h>

#include <syntiant_ilib/syntiant_ndp_ilib_version.h>
#include <syntiant_ilib/syntiant_ndp_pbi_version.h>
#include <syntiant_ilib/syntiant_ndp.h>
#include <syntiant-firmware/ndp_serial.h>
#include <syntiant_packager/syntiant_package.h>

#include <syntiant_ilib/syntiant_ndp_driver.h>
#ifndef EXCLUDE_SYNTIANT_CORE_1
#include <syntiant-firmware/ndp10x_firmware.h>
#include <syntiant_ilib/syntiant_ndp10x_driver.h>
#include <syntiant_ilib/syntiant_ndp10x.h>
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
#include <syntiant-dsp-firmware/ndp120_serial.h>
#include <syntiant-dsp-firmware/ndp120_dsp_fw_state.h>
#include <syntiant_ilib/syntiant_ndp120.h>
#include <syntiant_ilib/syntiant_ndp120_driver.h>
#include <syntiant_ilib/syntiant_ndp115_driver.h>
#include "syntiant_ndp120_config.h"
#endif
#include <syntiant_ilib/syntiant_ndp_error.h>
#include "syntiant_ndp_config.h"

static struct syntiant_ndp_driver_s *syntiant_ndp_drivers[] = {
#ifndef EXCLUDE_SYNTIANT_CORE_1
    &syntiant_ndp10x_driver,
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
    &syntiant_ndp120_driver,
    &syntiant_ndp115_driver,
#endif
    NULL
};

#if defined(EXCLUDE_SYNTIANT_CORE_1) &&  defined(EXCLUDE_SYNTIANT_CORE_2)
#error EXCLUDE_SYNTIANT_CORE_1 and EXCLUDE_SYNTIANT_CORE_2 are defined
#endif

static struct syntiant_ndp_driver_s *
syntiant_ndp_get_driver(unsigned int device_id);

/**
 * @brief Validates the package's PBI version vs ilib
 *
 * @param ndp NDP state object pointer
 * @return a @c SYNTIANT_NDP_ERROR_* code, @c NONE for success,
 *         @c FAIL for unknown family
 */
static int syntiant_ndp_validate_pbi_version(struct syntiant_ndp_device_s *ndp);

char *
syntiant_ndp_ilib_version(void) {
    static char *version = SYNTIANT_NDP_ILIB_VERSION;

    return version;
}

char *
syntiant_ndp_ilib_pbi_version(struct syntiant_ndp_device_s *ndp,
    struct syntiant_pbi_version_params_t_ *pbi_ver) {
    char *version = SYNTIANT_NDP_ILIB_PBI_VERSION;

    if (!ndp || !pbi_ver) {
        version = NULL;
        goto done;
    }
    memset(pbi_ver, 0, sizeof(*pbi_ver));
#ifndef EXCLUDE_SYNTIANT_CORE_1
    if (ndp && ndp->driver == &syntiant_ndp10x_driver) {
        pbi_ver->chip_type = PBI_CHIP_TYPE_NDP10X_B0;
        pbi_ver->major = SYNTIANT_NDP_ILIB_PBI_NDP10XB0_MAJOR_VERSION;
        pbi_ver->minor = SYNTIANT_NDP_ILIB_PBI_NDP10XB0_MINOR_VERSION;
        pbi_ver->patch = SYNTIANT_NDP_ILIB_PBI_NDP10XB0_PATCH_VERSION;
        version = SYNTIANT_NDP_ILIB_PBI_VERSION_10X;
    }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
    if (ndp && ndp->driver == &syntiant_ndp120_driver) {
        pbi_ver->chip_type = PBI_CHIP_TYPE_NDP120_B0;
        pbi_ver->major = SYNTIANT_NDP_ILIB_PBI_NDP120B0_MAJOR_VERSION;
        pbi_ver->minor = SYNTIANT_NDP_ILIB_PBI_NDP120B0_MINOR_VERSION;
        pbi_ver->patch = SYNTIANT_NDP_ILIB_PBI_NDP120B0_PATCH_VERSION;
        version = SYNTIANT_NDP_ILIB_PBI_VERSION_120;
    } else if (ndp && ndp->driver == &syntiant_ndp115_driver) {
        pbi_ver->chip_type = PBI_CHIP_TYPE_NDP115_A0;
        pbi_ver->major = SYNTIANT_NDP_ILIB_PBI_NDP115A0_MAJOR_VERSION;
        pbi_ver->minor = SYNTIANT_NDP_ILIB_PBI_NDP115A0_MINOR_VERSION;
        pbi_ver->patch = SYNTIANT_NDP_ILIB_PBI_NDP115A0_PATCH_VERSION;
        version = SYNTIANT_NDP_ILIB_PBI_VERSION_115;
    }
#endif

done:
    return version;
}

char *syntiant_ndp_ilib_release_hash(void) {
    static char *version = SYNTIANT_NDP_ILIB_RELEASE_HASH;

    return version;
}

int syntiant_ndp_ilib_enumerate_family(unsigned int family,
                                       unsigned int *next_family)
{
    int s = SYNTIANT_NDP_ERROR_MORE;
    int i;
    unsigned int next_enum = SYNTIANT_NDP_FAMILY_UNKNOWN;
    struct syntiant_ndp_driver_s *driver;

    if (family != SYNTIANT_NDP_FAMILY_UNKNOWN) {
        /* Unless we find an exact match already existing, will fail */
        s = SYNTIANT_NDP_ERROR_FAIL;
    }
    for (i = 0; (driver = syntiant_ndp_drivers[i]); i++) {
        if (family < driver->device_family) {
            if (next_enum == SYNTIANT_NDP_FAMILY_UNKNOWN
               || next_enum > driver->device_family) {
                next_enum = driver->device_family;
            }
        } else if (family == driver->device_family) {
            /* Not an error condition */
            s = SYNTIANT_NDP_ERROR_MORE;
        }
    }
    if (s == SYNTIANT_NDP_ERROR_FAIL) {
        goto done;
    }

    *next_family = next_enum;
    if (next_enum == SYNTIANT_NDP_FAMILY_UNKNOWN) {
        s = SYNTIANT_NDP_ERROR_NONE;
    }

done:
    return s;
}

int
syntiant_ndp_ilib_family_version(unsigned int family, unsigned int *major,
                                 unsigned int *minor, unsigned int *patch)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct syntiant_ndp_driver_s **driverp;
    struct syntiant_ndp_driver_s *driver;

    for (driverp = syntiant_ndp_drivers; (driver = *driverp); driverp++) {
        if (driver->device_family == family) {
            break;
        }
    }

    if (!driver) {
        s = SYNTIANT_NDP_ERROR_UNSUP;
        goto error;
    }

    *major = driver->major;
    *minor = driver->minor;
    *patch = driver->patch;

error:
    return s;
}

static struct syntiant_ndp_driver_s *
syntiant_ndp_get_driver(unsigned int device_id)
{
    int i, j;
    struct syntiant_ndp_driver_s *driver;

    for (i = 0; syntiant_ndp_drivers[i]; i++) {
        driver = syntiant_ndp_drivers[i];
        for (j = 0; driver->device_types[j]; j++) {
            if (driver->device_types[j] == device_id) {
                return driver;
            }
        }
    }

    return NULL;
}

int syntiant_ndp_validate_pbi_version(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_pbi_version_params_t pbi_ver;

    syntiant_ndp_ilib_pbi_version(ndp, &pbi_ver);

    if (!ndp->pstate.package_pbi_version.chip_type) {
        /* If the PBI version tag is missing, then assume all is fine */
    } else if (pbi_ver.chip_type != ndp->pstate.package_pbi_version.chip_type) {
        s = SYNTIANT_NDP_ERROR_PACKAGE_PBI_VER;
    } else if (pbi_ver.major != ndp->pstate.package_pbi_version.major) {
        s = SYNTIANT_NDP_ERROR_PACKAGE_PBI_VER;
    }

    return s;
}

int
syntiant_ndp_init(struct syntiant_ndp_device_s **ndpp,
    struct syntiant_ndp_integration_interfaces_s *iif,
    enum syntiant_ndp_init_mode_e init_mode)
{
    int s, s0;
    unsigned int id;
    struct syntiant_ndp_device_s *ndp = *ndpp;
    int alloced = !ndp;

    if (!ndp) {
        ndp = (iif->malloc)(sizeof(*ndp));
        if (!ndp) {
            s = SYNTIANT_NDP_ERROR_NOMEM;
            goto error;
        }
        memset(ndp, 0, sizeof(*ndp));
        /* ndpp may be used by the client during a restart init */
        *ndpp = ndp;
    }

    ndp->init = 0;

    if (iif) {
        ndp->iif = *iif;
    }

    s = (ndp->iif.get_type)(ndp->iif.d, &id);
    if (s) goto error;

    ndp->device_type = id;
    ndp->driver = syntiant_ndp_get_driver(id);
    if (!ndp->driver) {
        s = SYNTIANT_NDP_ERROR_UNSUP;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->init)(ndp, init_mode);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;

error:
    if (!s) {
        ndp->init = 1;
    } else if (alloced && ndp) {
        (ndp->iif.free)(ndp);
        *ndpp = NULL;
    }

    return s;
}

int
syntiant_ndp_uninit(struct syntiant_ndp_device_s *ndp, int free,
    enum syntiant_ndp_init_mode_e init_mode)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp))
        return SYNTIANT_NDP_ERROR_UNINIT;

    ndp->init = 0;

    if (!ndp->driver && !ndp->driver->uninit)
        return SYNTIANT_NDP_ERROR_UNINIT;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->uninit)(ndp, init_mode);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;

error:
    if (free) {
        (ndp->iif.free)(ndp);
    }
    return s;
}

int
syntiant_ndp_op_size(struct syntiant_ndp_device_s *ndp, int mcu,
                     unsigned int *size)
{
    int s;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->driver->op_size)(ndp->iif.d, mcu, size);

error:
    return s;
}

int
syntiant_ndp_read_block(struct syntiant_ndp_device_s *ndp, int mcu,
    uint32_t address, void *value, unsigned int count)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->read_block)(ndp, mcu, address, value, count);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_write_block(struct syntiant_ndp_device_s *ndp, int mcu,
                         uint32_t address, const void *value, unsigned int count)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->write_block)(ndp, mcu, address, value, count);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_write(struct syntiant_ndp_device_s *ndp, int mcu, uint32_t address,
                   uint32_t value)
{
    int s;
    unsigned int size;

    s = syntiant_ndp_op_size(ndp, mcu, &size);
    if (!s) {
        s = syntiant_ndp_write_block(ndp, mcu, address, &value, size);
    }
    return s;
}

int
syntiant_ndp_read(struct syntiant_ndp_device_s *ndp, int mcu, uint32_t address,
                  void *value)
{
    int s;
    unsigned int size;

    s = syntiant_ndp_op_size(ndp, mcu, &size);
    if (!s) {
        s = syntiant_ndp_read_block(ndp, mcu, address, value, size);
    }
    return s;
}

int
syntiant_ndp_interrupts(struct syntiant_ndp_device_s *ndp, int *on)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->interrupts)(ndp, on);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_poll(
    struct syntiant_ndp_device_s *ndp, uint32_t *notifications, int clear)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->poll)(ndp, notifications, clear);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_load(struct syntiant_ndp_device_s *ndp, void *package, int len)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    if (!len) {
        memset((void *)&ndp->pstate.package_pbi_version, 0,
            sizeof(ndp->pstate.package_pbi_version));
    }
    s = (ndp->driver->load)(ndp, package, len);
    if (!s && len) {
        s = syntiant_ndp_validate_pbi_version(ndp);
    }

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_get_device_family(unsigned int device_id,
                               unsigned int *device_family)
{
    struct syntiant_ndp_driver_s *driver;

    driver = syntiant_ndp_get_driver(device_id);

    if (!driver) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }

    *device_family = driver->device_family;

    return SYNTIANT_NDP_ERROR_NONE;
}

int
syntiant_ndp_get_config(
    struct syntiant_ndp_device_s *ndp, struct syntiant_ndp_config_s *config)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    config->device_id = ndp->device_type;
    config->device_family = ndp->driver->device_family;
    s = (ndp->driver->get_config)(ndp, config);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_send_data(
    struct syntiant_ndp_device_s *ndp, uint8_t *data, unsigned int len,
    int type, uint32_t offset)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    if (len < 1) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->send_data)(ndp, data, len, type, offset);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_extract_data(struct syntiant_ndp_device_s *ndp, int type,
                          int from, uint8_t *data, unsigned int *len)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->extract_data)(ndp, type, from, data, len);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_get_match_summary(
    struct syntiant_ndp_device_s *ndp, uint32_t *summary)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->get_match_summary)(ndp, summary);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_get_match_binary(
    struct syntiant_ndp_device_s *ndp, uint8_t *matches, unsigned int len)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->get_match_binary)(ndp, matches, len);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_get_match_strength(
    struct syntiant_ndp_device_s *ndp, uint8_t *strengths, unsigned int len,
    int type)
{
    int s, s0;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->get_match_strength)(ndp, strengths, len, type);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

static int
syntiant_serial_address(
    /*
     * Note: it is the caller's responsibility to validate the
     * interface_address if the interface is not I2C.
     * This is because the caller knows which Core family
     * is running.
     */
    unsigned char interface,
    unsigned char interface_address,
    unsigned char *addressp) {
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned char address = interface_address;

    if (interface == SYNTIANT_NDP_SERIAL_INTERFACE_I2C) {
        if (0x7f < interface_address) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto out;
        }
        address |= NDP_FW_SERIAL_ADDRESS_I2C_MASK;
    } else if (SYNTIANT_NDP_SERIAL_INTERFACE_MAX < interface) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto out;
    } else {
        address |= (uint8_t)
            ((interface - SYNTIANT_NDP_SERIAL_INTERFACE_SPI_DEV0)
            << NDP_FW_SERIAL_ADDRESS_SPI_MODE_SHIFT);
    }

    *addressp = address;

out:
    return s;
}

int
syntiant_serial_transfer(struct syntiant_ndp_device_s *ndp,
    unsigned int ifc_type, unsigned int ifc_addr,
    uint8_t *out, unsigned int outlen, uint8_t *in, unsigned int inlen,
    int continue_)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_serial_transfer_no_sync(ndp, ifc_type, ifc_addr, out,
                                         outlen, in, inlen, continue_);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    return s ? s : s0;
}

int
syntiant_serial_transfer_no_sync(struct syntiant_ndp_device_s *ndp,
    unsigned int ifc_type, unsigned int ifc_addr,
    uint8_t *out, unsigned int outlen, uint8_t *in, unsigned int inlen,
    int continue_) {
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned int outn, outl, inn, inl, l;
    uint8_t * outp,*inp;
    uint32_t addr = 0;
    uint8_t serial_address;
    struct ndp_fw_serial_s serial;

    if (SYNTIANT_NDP_IS_UNINIT(ndp)) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

#ifndef EXCLUDE_SYNTIANT_CORE_1
    if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
        if (!ndp->d.ndp10x.fw_state_addr) {
            s = SYNTIANT_NDP_ERROR_UNINIT;
            goto error;
        }
        addr = ndp->d.ndp10x.fw_state_addr + (uint32_t)offsetof(
            struct ndp10x_fw_state_s, host_intf.serial);
        /* Validate serial addr value before calling syntiant_serial_address */
        if ((unsigned char)ifc_type != SYNTIANT_NDP_SERIAL_INTERFACE_I2C
           && NDP10X_FW_GPIO_MAX <= (unsigned char)ifc_addr) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
        }
    }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
    if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
        if (!ndp->d.ndp120.dsp_fw_state_addr) {
            s = SYNTIANT_NDP_ERROR_UNINIT;
            goto error;
        }
        addr = ndp->d.ndp120.dsp_fw_state_addr + (unsigned)offsetof(
            ndp120_dsp_fw_state_t, serial_sensor_mgmt.serial);
        /* Validate serial addr value before calling syntiant_serial_address */
        if ((unsigned char)ifc_type != SYNTIANT_NDP_SERIAL_INTERFACE_I2C
           && NDP120_FW_GPIO_MAX <= (unsigned char)ifc_addr) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
        }
    }
#endif

    /* Note, caller is responsible for validating ifc_addr, was done above */
    s = syntiant_serial_address(
        (unsigned char)ifc_type, (unsigned char)ifc_addr, &serial_address);
    if (s) {
        goto error;
    }

    outn = outlen;
    inn = inlen;
    outp = out;
    inp = in;
    while (outn || inn) {
        outl = 0;
        if (outn) {
            outl = NDP_FW_SERIAL_BYTE_MAX <= outn
                ? NDP_FW_SERIAL_BYTE_MAX
                : outn;
            memcpy(serial.data, outp, outl);
            outn -= outl;
            outp += outl;
        }
        inl = 0;
        if (!outn && inn) {
            inl = NDP_FW_SERIAL_BYTE_MAX <= inn
                ? NDP_FW_SERIAL_BYTE_MAX
                : inn;
            inn -= inl;
        }

        serial.control =
            (((unsigned int)serial_address)
            << NDP_FW_SERIAL_CONTROL_ADDRESS_SHIFT)
            | (((unsigned int)(outn || inn || continue_))
            << NDP_FW_SERIAL_CONTROL_CONTINUE_SHIFT)
            | (outl << NDP_FW_SERIAL_CONTROL_OUTLEN_SHIFT)
            | (inl << NDP_FW_SERIAL_CONTROL_INLEN_SHIFT)
            | NDP_FW_SERIAL_CONTROL_RUN_MASK;

        s = ndp->driver->write_block(ndp, 1, addr, &serial, sizeof(serial));
        if (s) {
            goto error;
        }

#ifndef EXCLUDE_SYNTIANT_CORE_1
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
            s = syntiant_ndp10x_do_mailbox_req_no_sync(
                ndp, NDP10X_MB_REQUEST_NOP);
        }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
            s = syntiant_ndp120_do_mailbox_req_no_sync(
                ndp, NDP120_DSP_MB_D2H_MODE_CHANGE, NULL);
        }
#endif
        if (s) {
            goto error;
        }

        l = (unsigned int)(sizeof(serial.control) + ((inl + 3U) & ~3U));
        s = ndp->driver->read_block(ndp, 1, addr, &serial, l);
        if (s) {
            goto error;
        }

        if (serial.control & NDP_FW_SERIAL_CONTROL_RUN_MASK) {
            s = SYNTIANT_NDP_ERROR_FAIL;
            goto error;
        }
        s = (serial.control & NDP_FW_SERIAL_CONTROL_STATUS_MASK)
            >> NDP_FW_SERIAL_CONTROL_STATUS_SHIFT;
        if (s) {
            s = (s == NDP_FW_SERIAL_CONTROL_STATUS_TIMEOUT)
                ? SYNTIANT_NDP_ERROR_TIMEOUT
                : SYNTIANT_NDP_ERROR_FAIL;
            goto error;
        }

        memcpy(inp, serial.data, inl);
        inp += inl;
    }

error:
    return s;
}


int
syntiant_ndp_config_get_sensor(struct syntiant_ndp_device_s *ndp,
                                  void *sconfig)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    union {
#ifndef EXCLUDE_SYNTIANT_CORE_1
        struct syntiant_ndp10x_config_s *ndp10x;
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
        syntiant_ndp120_config_sensor_t *ndp120;
#endif
    } config;
    uint32_t addr = 0;
    uint32_t addr_sensor = 0;
    uint32_t enable, control;
    struct ndp_fw_sensor_configuration_s sensors[NDP_FW_SENSOR_MAX];
    struct ndp_fw_sensor_configuration_s *sensor;
    uint8_t gpio_role[MAX(NDP10X_FW_GPIO_MAX, NDP120_FW_GPIO_MAX)];
    unsigned int gpio_count = 0;
    unsigned int i, j, saddress, smode, gpio;
    unsigned char role, id, interface, interface_address, tank, input, parameter;

#ifndef EXCLUDE_SYNTIANT_CORE_1
    config.ndp10x = NULL;
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
    config.ndp120 = NULL;
#endif
    if (!sconfig) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto out;
    }

#ifndef EXCLUDE_SYNTIANT_CORE_1
    if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
        config.ndp10x = (struct syntiant_ndp10x_config_s *)sconfig;
        gpio_count = NDP10X_FW_GPIO_MAX;
        if (!ndp->d.ndp10x.fw_state_addr) {
            memset(config.ndp10x->gpio_role, 0, sizeof(config.ndp10x->gpio_role));
            memset(config.ndp10x->sensor_id, 0, sizeof(config.ndp10x->sensor_id));
            memset(config.ndp10x->sensor_interface, 0, sizeof(config.ndp10x->sensor_interface));
            memset(config.ndp10x->sensor_interface_address, 0,
                   sizeof(config.ndp10x->sensor_interface_address));
            memset(config.ndp10x->sensor_int, 0, sizeof(config.ndp10x->sensor_int));
            memset(config.ndp10x->sensor_int_gpio, 0, sizeof(config.ndp10x->sensor_int_gpio));
            memset(config.ndp10x->sensor_axis_tank, 0, sizeof(config.ndp10x->sensor_axis_tank));
            memset(config.ndp10x->sensor_axis_input, 0, sizeof(config.ndp10x->sensor_axis_input));
            memset(config.ndp10x->sensor_parameter, 0, sizeof(config.ndp10x->sensor_parameter));
            goto out;
        } else {
            addr = ndp->d.ndp10x.fw_state_addr + (uint32_t)offsetof(
                struct ndp10x_fw_state_s, host_intf.gpio_role);
            addr_sensor = ndp->d.ndp10x.fw_state_addr + (uint32_t)offsetof(
                struct ndp10x_fw_state_s, host_intf.sensor);
        }
    }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
    if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
        config.ndp120 = (syntiant_ndp120_config_sensor_t *)sconfig;
        gpio_count = NDP120_FW_GPIO_MAX;
        if (!ndp->d.ndp120.dsp_fw_state_addr) {
            memset(config.ndp120->gpio_role, 0, sizeof(config.ndp120->gpio_role));
            memset(config.ndp120->sensor_id, 0, sizeof(config.ndp120->sensor_id));
            memset(config.ndp120->sensor_interface, 0, sizeof(config.ndp120->sensor_interface));
            memset(config.ndp120->sensor_interface_address, 0,
                   sizeof(config.ndp120->sensor_interface_address));
            memset(config.ndp120->sensor_int, 0, sizeof(config.ndp120->sensor_int));
            memset(config.ndp120->sensor_int_gpio, 0, sizeof(config.ndp120->sensor_int_gpio));
            memset(config.ndp120->sensor_axis_tank, 0, sizeof(config.ndp120->sensor_axis_tank));
            memset(config.ndp120->sensor_axis_input, 0, sizeof(config.ndp120->sensor_axis_input));
            memset(config.ndp120->sensor_parameter, 0, sizeof(config.ndp120->sensor_parameter));
            goto out;
        } else {
            addr = ndp->d.ndp120.dsp_fw_state_addr +
                   (uint32_t) offsetof(ndp120_dsp_fw_state_t,
                   serial_sensor_mgmt.gpio_role);
            addr_sensor = ndp->d.ndp120.dsp_fw_state_addr +
                   (uint32_t) offsetof(ndp120_dsp_fw_state_t,
                   serial_sensor_mgmt.sensor_config);
        }
    }
#endif

    s = ndp->driver->read_block(ndp, 1, addr, gpio_role, gpio_count * sizeof(gpio_role[0]));
    if (s)
        goto out;

    for (i = 0; i < gpio_count; i++) {
        role = SYNTIANT_CONFIG_GPIO_ROLE_NONE;
        if (i < gpio_count) {
            switch (gpio_role[i]) {
            case NDP_FW_GPIO_ROLE_IDATA:
                role = SYNTIANT_CONFIG_GPIO_ROLE_IDATA;
                break;
            case NDP_FW_GPIO_ROLE_ICLK:
                role = SYNTIANT_CONFIG_GPIO_ROLE_ICLK;
                break;
            case NDP_FW_GPIO_ROLE_MMISO:
                role = SYNTIANT_CONFIG_GPIO_ROLE_MMISO;
                break;
            case NDP_FW_GPIO_ROLE_MMOSI:
                role = SYNTIANT_CONFIG_GPIO_ROLE_MMOSI;
                break;
            case NDP_FW_GPIO_ROLE_MSCLK:
                role = SYNTIANT_CONFIG_GPIO_ROLE_MSCLK;
                break;
            case NDP_FW_GPIO_ROLE_MSSEL:
                role = SYNTIANT_CONFIG_GPIO_ROLE_MSSEL;
                break;
            case NDP_FW_GPIO_ROLE_I2SCLK:
                role = SYNTIANT_CONFIG_GPIO_ROLE_I2SCLK;
                break;
            case NDP_FW_GPIO_ROLE_INTEF:
                role = SYNTIANT_CONFIG_GPIO_ROLE_INTEF;
                break;
            case NDP_FW_GPIO_ROLE_INTER:
                role = SYNTIANT_CONFIG_GPIO_ROLE_INTER;
                break;
            case NDP_FW_GPIO_ROLE_INTLL:
                role = SYNTIANT_CONFIG_GPIO_ROLE_INTLL;
                break;
            case NDP_FW_GPIO_ROLE_INTLH:
                role = SYNTIANT_CONFIG_GPIO_ROLE_INTLH;
                break;
            default:
                break;
            }
        }
#ifndef EXCLUDE_SYNTIANT_CORE_1
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
            config.ndp10x->gpio_role[i] = role;
        }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
            config.ndp120->gpio_role[i] = role;
        }
#endif
    }

    s = ndp->driver->read_block(ndp, 1, addr_sensor, sensors, sizeof(sensors));
    if (s)
        goto out;

    for (i = 0; i < SYNTIANT_CONFIG_SENSOR_SENSORS; i++) {
        sensor = &sensors[i];

        id = SYNTIANT_CONFIG_SENSOR_ID_NONE;
        if (NDP_FW_SENSOR_MAX <= i) {
#ifndef EXCLUDE_SYNTIANT_CORE_1
            if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
                config.ndp10x->sensor_id[i] = id;
            }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
            if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
                config.ndp120->sensor_id[i] = id;
            }
#endif
            continue;
        }
        control = sensor->control;
        id = (control & NDP_FW_SENSOR_CONTROL_ID_MASK)
            >> NDP_FW_SENSOR_CONTROL_ID_SHIFT;
        switch (id) {
        case NDP_FW_SENSOR_ID_BMI160:
            id = SYNTIANT_CONFIG_SENSOR_ID_BMI160;
            break;
        case NDP_FW_SENSOR_ID_VM3011:
            id = SYNTIANT_CONFIG_SENSOR_ID_VM3011;
            break;
        case NDP_FW_SENSOR_ID_EDGE_INT:
            id = SYNTIANT_CONFIG_SENSOR_ID_EDGE_INT;
            break;
        case NDP_FW_SENSOR_ID_DA217:
            id = SYNTIANT_CONFIG_SENSOR_ID_DA217;
            break;
        case NDP_FW_SENSOR_ID_KX120:
            id = SYNTIANT_CONFIG_SENSOR_ID_KX120;
            break;
        case NDP_FW_SENSOR_ID_MC3419:
            id = SYNTIANT_CONFIG_SENSOR_ID_MC3419;
            break;
        case NDP_FW_SENSOR_ID_GPIO_DEBOUNCE:
            id = SYNTIANT_CONFIG_SENSOR_ID_GPIO_DEBOUNCE;
            break;
        case NDP_FW_SENSOR_ID_ICM42670:
            id = SYNTIANT_CONFIG_SENSOR_ID_ICM42670;
            break;
        default:
            break;
        }
#ifndef EXCLUDE_SYNTIANT_CORE_1
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
            config.ndp10x->sensor_id[i] = id;
        }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
            config.ndp120->sensor_id[i] = id;
        }
#endif
        saddress =
            (control & NDP_FW_SENSOR_CONTROL_ADDRESS_MASK)
            >> NDP_FW_SENSOR_CONTROL_ADDRESS_SHIFT;
        if (saddress & NDP_FW_SERIAL_ADDRESS_I2C_MASK) {
            interface = SYNTIANT_NDP_SERIAL_INTERFACE_I2C;
            interface_address = saddress
                & NDP_FW_SERIAL_ADDRESS_I2C_ADDRESS_MASK;
        } else {
            smode = (saddress & NDP_FW_SERIAL_ADDRESS_SPI_MODE_MASK)
                >> NDP_FW_SERIAL_ADDRESS_SPI_MODE_SHIFT;
            switch (smode) {
            case 0:
                interface = SYNTIANT_NDP_SERIAL_INTERFACE_SPI_DEV0;
                break;
            case 1:
                interface = SYNTIANT_NDP_SERIAL_INTERFACE_SPI_DEV1;
                break;
            case 2:
                interface = SYNTIANT_NDP_SERIAL_INTERFACE_SPI_DEV2;
                break;
            case 3:
                s = SYNTIANT_NDP_ERROR_UNSUP;    /* MSSB supports 0,1,2 */
                goto out;
            }
            interface_address =
                (saddress & NDP_FW_SERIAL_ADDRESS_SPI_SELECT_GPIO_MASK)
                >> NDP_FW_SERIAL_ADDRESS_SPI_SELECT_GPIO_SHIFT;
        }

        gpio = (control & NDP_FW_SENSOR_CONTROL_INT_GPIO_MASK)
                >> NDP_FW_SENSOR_CONTROL_INT_GPIO_SHIFT;
#ifndef EXCLUDE_SYNTIANT_CORE_1
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
            config.ndp10x->sensor_int[i] = !!gpio;
            config.ndp10x->sensor_int_gpio[i] =
                    (unsigned char) (gpio ? gpio - 1 : 0);
            config.ndp10x->sensor_axes[i] = (unsigned char)
                    ((control & NDP_FW_SENSOR_CONTROL_AXES_MASK)
                     >> NDP_FW_SENSOR_CONTROL_AXES_SHIFT);
            config.ndp10x->sensor_interface[i] = interface;
            config.ndp10x->sensor_interface_address[i] = interface_address;
        }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
            config.ndp120->sensor_int[i] = !!gpio;
            config.ndp120->sensor_int_gpio[i] =
                    (unsigned char) (gpio ? gpio - 1 : 0);
            config.ndp120->sensor_axes[i] = (unsigned char)
                    ((control & NDP_FW_SENSOR_CONTROL_AXES_MASK)
                     >> NDP_FW_SENSOR_CONTROL_AXES_SHIFT);
            config.ndp120->sensor_interface[i] = interface;
            config.ndp120->sensor_interface_address[i] = interface_address;
        }
#endif

        enable = sensor->enable;
        for (j = 0; j < SYNTIANT_CONFIG_SENSOR_AXES; j++) {
            tank = 0;
            input = 0;
            if (j < NDP_FW_SENSOR_AXIS_MAX) {
                tank = !!(enable
                          & (1U << (j + NDP_FW_SENSOR_ENABLE_TANK_SHIFT)));
                input = !!(enable
                           & (1U << (j + NDP_FW_SENSOR_ENABLE_INPUT_SHIFT)));
            }
#ifndef EXCLUDE_SYNTIANT_CORE_1
            if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
                config.ndp10x->sensor_axis_tank[i][j] = tank;
                config.ndp10x->sensor_axis_input[i][j] = input;
            }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
            if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
                config.ndp120->sensor_axis_tank[i][j] = tank;
                config.ndp120->sensor_axis_input[i][j] = input;
            }
#endif
        }
        for (j = 0; j < SYNTIANT_CONFIG_SENSOR_PARAMETERS; j++) {
            parameter = 0;
            if (j < sizeof(sensor->parameter)) {
                parameter = sensor->parameter[j];
            }

#ifndef EXCLUDE_SYNTIANT_CORE_1
            if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
                config.ndp10x->sensor_parameter[i][j] = parameter;
            }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
            if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
                config.ndp120->sensor_parameter[i][j] = parameter;
            }
#endif
        }
    }

 out:
    return s;
}

int
syntiant_ndp_config_set_sensor(struct syntiant_ndp_device_s *ndp,
                               void *sconfig)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    union {
#ifndef EXCLUDE_SYNTIANT_CORE_1
        struct syntiant_ndp10x_config_s *ndp10x;
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
        syntiant_ndp120_config_sensor_t *ndp120;
#endif
    } config;

    uint32_t addr = 0;
    uint32_t addr_sensor = 0;
    uint32_t enable = 0;
    uint32_t control = 0;
    struct ndp_fw_sensor_configuration_s sensors[NDP_FW_SENSOR_MAX];
    struct ndp_fw_sensor_configuration_s *sensor;
    uint8_t gpio_role[MAX(NDP10X_FW_GPIO_MAX, NDP120_FW_GPIO_MAX)];
    uint8_t cgpio_role = 0;
    uint8_t csensor_id = 0;
    uint8_t csensor_axes = 0;
    unsigned int gpio_count = 0;
    unsigned int i, j, int_gpio;
    unsigned int gint = 0;
    unsigned int gpio = 0;
    unsigned int gint1 = 0;
    unsigned int gpio1 = 0;
    unsigned char address = 0;
    unsigned char role;
    unsigned int syntiant_config_sensor_gpios =
        MAX(NDP10X_FW_GPIO_MAX, NDP120_FW_GPIO_MAX);

#ifndef EXCLUDE_SYNTIANT_CORE_1
    config.ndp10x = NULL;
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
    config.ndp120 = NULL;
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_1
    if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
        syntiant_config_sensor_gpios = NDP10X_FW_GPIO_MAX;
        if (!(((struct syntiant_ndp10x_config_s *)sconfig)->set1 &
              SYNTIANT_NDP10X_CONFIG_SET1_SENSOR)) {
            goto out;
        }
    }
#endif

    if (!sconfig) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto out;
    }

#ifndef EXCLUDE_SYNTIANT_CORE_1
    if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
        config.ndp10x = (struct syntiant_ndp10x_config_s *)sconfig;
        gpio_count = NDP10X_FW_GPIO_MAX;
        if (!ndp->d.ndp10x.fw_state_addr) {
            s = SYNTIANT_NDP_ERROR_UNINIT;
            goto out;
        } else {
            if (!(config.ndp10x->set1 & SYNTIANT_NDP10X_CONFIG_SET1_SENSOR)) {
                goto out;
            }
            addr = ndp->d.ndp10x.fw_state_addr + (uint32_t)offsetof(
                struct ndp10x_fw_state_s, host_intf.gpio_role);
            addr_sensor = ndp->d.ndp10x.fw_state_addr + (uint32_t)offsetof(
                struct ndp10x_fw_state_s, host_intf.sensor);
        }
    }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
    if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
        syntiant_config_sensor_gpios = NDP120_FW_GPIO_MAX;
        config.ndp120 = (syntiant_ndp120_config_sensor_t *)sconfig;
        gpio_count = NDP120_FW_GPIO_MAX;
        if (!ndp->d.ndp120.dsp_fw_state_addr) {
            s = SYNTIANT_NDP_ERROR_UNINIT;
            goto out;
        } else {
            if (!(config.ndp120->set)) {
                goto out;
            }
            addr = ndp->d.ndp120.dsp_fw_state_addr +
                   (uint32_t) offsetof(ndp120_dsp_fw_state_t,
                   serial_sensor_mgmt.gpio_role);
            addr_sensor = ndp->d.ndp120.dsp_fw_state_addr +
                   (uint32_t) offsetof(ndp120_dsp_fw_state_t,
                   serial_sensor_mgmt.sensor_config);
        }
    }
#endif

    memset(gpio_role, 0, sizeof(gpio_role));

    for (i = 0; i < gpio_count; i++) {
        if (i < gpio_count) {
#ifndef EXCLUDE_SYNTIANT_CORE_1
            if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
                cgpio_role = config.ndp10x->gpio_role[i];
            }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
            if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
                cgpio_role = config.ndp120->gpio_role[i];
            }
#endif
            switch (cgpio_role) {
            case SYNTIANT_CONFIG_GPIO_ROLE_NONE:
                role = NDP_FW_GPIO_ROLE_NONE;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_IDATA:
                role = NDP_FW_GPIO_ROLE_IDATA;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_ICLK:
                role = NDP_FW_GPIO_ROLE_ICLK;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_MMISO:
                role = NDP_FW_GPIO_ROLE_MMISO;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_MMOSI:
                role = NDP_FW_GPIO_ROLE_MMOSI;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_MSCLK:
                role = NDP_FW_GPIO_ROLE_MSCLK;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_MSSEL:
                role = NDP_FW_GPIO_ROLE_MSSEL;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_I2SCLK:
                role = NDP_FW_GPIO_ROLE_I2SCLK;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_INTEF:
                role = NDP_FW_GPIO_ROLE_INTEF;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_INTER:
                role = NDP_FW_GPIO_ROLE_INTER;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_INTLL:
                role = NDP_FW_GPIO_ROLE_INTLL;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_INTLH:
                role = NDP_FW_GPIO_ROLE_INTLH;
                break;
            case SYNTIANT_CONFIG_GPIO_ROLE_OUT:
                role = NDP_FW_GPIO_ROLE_OUT;
                break;
            default:
                s = SYNTIANT_NDP_ERROR_ARG;
                goto out;
            }
        }
        gpio_role[i] = role;
    }

    memset(sensors, 0, sizeof(sensors));

    for (i = 0; i < SYNTIANT_CONFIG_SENSOR_SENSORS; i++) {
#ifndef EXCLUDE_SYNTIANT_CORE_1
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
            csensor_id = config.ndp10x->sensor_id[i];
            gint = config.ndp10x->sensor_int[i];
            gpio = config.ndp10x->sensor_int_gpio[i];
            csensor_axes = config.ndp10x->sensor_axes[i];
        }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
            csensor_id = config.ndp120->sensor_id[i];
            gint = config.ndp120->sensor_int[i];
            gpio = config.ndp120->sensor_int_gpio[i];
            gint1 = config.ndp120->sensor_int1[i];
            gpio1 = config.ndp120->sensor_int_gpio1[i];
            csensor_axes = config.ndp120->sensor_axes[i];
        }
#endif
        sensor = &sensors[i];
        if (NDP_FW_SENSOR_MAX <= i) {
            continue;
        }

        if (csensor_id > SYNTIANT_CONFIG_SENSOR_ID_MAX) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto out;
        }

        control = (uint32_t)csensor_id << NDP_FW_SENSOR_CONTROL_ID_SHIFT;

#ifndef EXCLUDE_SYNTIANT_CORE_1
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
            unsigned char interface = config.ndp10x->sensor_interface[i];
            unsigned char ifaddr = config.ndp10x->sensor_interface_address[i];
            if (interface != SYNTIANT_NDP_SERIAL_INTERFACE_I2C
               && NDP10X_FW_GPIO_MAX <= ifaddr) {
                s = SYNTIANT_NDP_ERROR_ARG;
            } else {
                s = syntiant_serial_address(interface, ifaddr, &address);
            }
        }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
            unsigned char interface = config.ndp120->sensor_interface[i];
            unsigned char ifaddr = config.ndp120->sensor_interface_address[i];
            if (interface != SYNTIANT_NDP_SERIAL_INTERFACE_I2C
               && NDP120_FW_GPIO_MAX <= ifaddr) {
                s = SYNTIANT_NDP_ERROR_ARG;
            } else {
                s = syntiant_serial_address(interface, ifaddr, &address);
            }
        }
#endif
        if (s) {
            goto out;
        }

        control |= ((unsigned int) address)
            << NDP_FW_SENSOR_CONTROL_ADDRESS_SHIFT;

        if (gint && syntiant_config_sensor_gpios <= gpio) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto out;
        }

        if (gint1 && syntiant_config_sensor_gpios <= gpio1) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto out;
        }

        int_gpio = gint ? gpio + 1 : 0;
        control |= int_gpio << NDP_FW_SENSOR_CONTROL_INT_GPIO_SHIFT;

#ifndef EXCLUDE_SYNTIANT_CORE_2
        if (gint1) {
            int_gpio = gpio1 + 1;
            control |= int_gpio << NDP_FW_SENSOR_CONTROL_INT_GPIO1_SHIFT;
        }
#endif

        if (NDP_FW_SENSOR_AXIS_MAX < csensor_axes) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto out;
        }
        control |= ((unsigned int) csensor_axes)
            << NDP_FW_SENSOR_CONTROL_AXES_SHIFT;

        sensor->control = control;

        enable = 0;
        for (j = 0; j < SYNTIANT_CONFIG_SENSOR_AXES; j++) {
            if (j < NDP_FW_SENSOR_AXIS_MAX) {
#ifndef EXCLUDE_SYNTIANT_CORE_1
                if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
                    enable |=
                        (config.ndp10x->sensor_axis_tank[i][j] ? 1U : 0U)
                        << (j + NDP_FW_SENSOR_ENABLE_TANK_SHIFT);
                    enable |=
                        (config.ndp10x->sensor_axis_input[i][j] ? 1U : 0U)
                        << (j + NDP_FW_SENSOR_ENABLE_INPUT_SHIFT);
                }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
                if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
                    enable |=
                        (config.ndp120->sensor_axis_tank[i][j] ? 1U : 0U)
                        << (j + NDP_FW_SENSOR_ENABLE_TANK_SHIFT);
                    enable |=
                        (config.ndp120->sensor_axis_input[i][j] ? 1U : 0U)
                        << (j + NDP_FW_SENSOR_ENABLE_INPUT_SHIFT);
                }
#endif
            }
        }
        sensor->enable = enable;
        for (j = 0; j < SYNTIANT_CONFIG_SENSOR_PARAMETERS; j++) {
            if (j < sizeof(sensor->parameter)) {
#ifndef EXCLUDE_SYNTIANT_CORE_1
                if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
                    sensor->parameter[j] = config.ndp10x->sensor_parameter[i][j];
                }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
                if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
                    sensor->parameter[j] = config.ndp120->sensor_parameter[i][j];
                }
#endif
            }
        }
    }

    s = ndp->driver->write_block(ndp, 1, addr, gpio_role, sizeof(gpio_role));
    if (s)
        goto out;

    s = ndp->driver->write_block(ndp, 1, addr_sensor, sensors, sizeof(sensors));
    if (s)
        goto out;

 out:
    return s;
}

int
syntiant_ndp_config_sensor(struct syntiant_ndp_device_s *ndp,
                              void *sconfig)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int set = 0;

#ifndef EXCLUDE_SYNTIANT_CORE_1
    if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_1) {
        if (((struct syntiant_ndp10x_config_s *)sconfig)->set1 &
            SYNTIANT_NDP10X_CONFIG_SET1_SENSOR) {
            set = 1;
        }
    }
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
    if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
        if (((syntiant_ndp120_config_sensor_t *)sconfig)->set) {
            set = 1;
        }
    }
#endif

    if (set) {
        /* printf("syntiant_ndp_config_sensor: set=1\n"); */
        s = syntiant_ndp_config_set_sensor(ndp, sconfig);
        if (s) goto error;

#ifndef EXCLUDE_SYNTIANT_CORE_2
        if (ndp->driver->device_family == SYNTIANT_NDP_FAMILY_CORE_2) {
            uint32_t pdm_enabled;

            /* Check if pdm block is enabled before issuing sensor
               set command, and set a flag accordingly */
            s = syntiant_ndp120_get_pdm_enable_status(ndp, &pdm_enabled);
            if (s) goto error;

            ndp->d.ndp120.pdm_block_enabled_while_sensor_set = pdm_enabled;
            s = syntiant_ndp120_do_mailbox_req(ndp,
                NDP120_DSP_MB_H2D_EXT_SENSOR_SET, NULL);
            if (s) goto error;
        }
#endif
    } else {
        /* printf("syntiant_ndp_config_sensor: set=0\n"); */
        s = syntiant_ndp_config_get_sensor(ndp, sconfig);
        if (s) goto error;
    }

error:
    /* printf("syntiant_ndp_config_sensor: exit: s=%d\n", s); */
    return s;
}
