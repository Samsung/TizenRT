/*
 * SYNTIANT CONFIDENTIAL
 * _____________________
 *
 *   Copyright (c) 2019-2020 Syntiant Corporation
 *   All Rights Reserved.
 *
 *  NOTICE:  All information contained herein is, and remains the property of
 *  Syntiant Corporation and its suppliers, if any.  The intellectual and
 *  technical concepts contained herein are proprietary to Syntiant Corporation
 *  and its suppliers and may be covered by U.S. and Foreign Patents, patents
 *  in process, and are protected by trade secret or copyright law.
 *  Dissemination of this information or reproduction of this material is
 *  strictly forbidden unless prior written permission is obtained from
 *  Syntiant Corporation.
 	** SDK: v112.3.6-Samsung **
*/
#ifndef SYNTIANT_EVB_IO_H
#define SYNTIANT_EVB_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief handle/pointer to integration-specific I/O state
 *
 * An @c syntiant_io_handle_t typically points to a structure containing
 * all information required to perform the I/O to a single device
 * instance in a particular environment.  Or, for example, it may
 * contain a file descriptor, if that is all that is required to perform
 * the I/O.
 */
typedef void *syntiant_evb_io_handle_t;

/**
 * @brief wait for interrupt interface
 *
 * A provider of @c syntiant_evb_io_wait must not return until
 * the NDP device has signalled an interrupt, or the timeout has expired.
 *
 * @param d handle to the I/O provider state
 * @param timeout time to wait for an interrupt, in milliseconds, -1 -> forever
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
typedef int (*syntiant_evb_io_wait_f)(syntiant_evb_io_handle_t d, int timeout);

/**
 * @brief exchange bytes with NDP device interface
 *
 * A provider of @c syntiant_ndp_io_transfer will send @p count bytes
 * to the device and retrieve @p count bytes.
 *
 * This interface supports an underlying underlying SPI bus connection
 * to NDP where equal numbers of bytes are sent and received for cases
 * where full duplex transfer is relevant.  Simplex transfer is accomplished
 * by setting either @p out or @p in to NULL as appropriate.
 *
 * Transfers to the MCU space are generally expected to be simplex and
 * out == NULL -> read operation, in == NULL -> write operation.
 *
 * @param d handle to the I/O provider state
 * @param mcu target MCU space if true (SPI bus space if false)
 * @param addr starting address
 * @param out bytes to send, or NULL if '0' bytes should be sent
 * @param in bytes to receive, or NULL if received bytes should be ignored
 * @param count number of bytes to exchange
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
typedef int (*syntiant_evb_io_transfer_f)(syntiant_evb_io_handle_t d, int mcu,
                                          uint32_t addr, const void *out, void *in,
                                          int count);

/**
 * @brief uninitialize an I/O provider
 *
 * @param d handle to the I/O provider state
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
typedef int (*syntiant_evb_io_uninit_f)(syntiant_evb_io_handle_t d);

#ifdef __cplusplus
}
#endif

#endif
