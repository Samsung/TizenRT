/****************************************************************************
 * include/tinyara/bluetooth/bluetooth.h
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef  __INCLUDE_TINYARA_BLUETOOTH_BLUETOOTH_H
#define  __INCLUDE_TINYARA_BLUETOOTH_BLUETOOTH_H

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <tinyara/bluetooth/bt_hci.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/** @def BT_ID_DEFAULT
 *
 *  Convenience macro for specifying the default identity. This helps
 *  make the code more readable, especially when only one identity is
 *  supported.
 */
#define BT_ID_DEFAULT 0

/* BLUETOOTH_MAX_FRAMELEN
 * Maximum amount of data that can fit in a buffer.
 *
 * The biggest foreseeable buffer size requirement right now comes from
 * the Bluetooth 4.2 SMP MTU which is 65. This then become 65 + 4 (L2CAP
 * header) + 4 (ACL header) + 1 (H4 header) = 74. This also covers the
 * biggest HCI commands and events which are a bit under the 70 byte
 * mark.
 */

#define BLUETOOTH_L2CAP_HDRLEN  4	/* Size of L2CAP header */
#define BLUETOOTH_ACL_HDRLEN    4	/* Size of ACL header */
#define BLUETOOTH_H4_HDRLEN     1	/* Size of H4 header */

#define BLUETOOTH_MAX_HDRLEN \
	(BLUETOOTH_L2CAP_HDRLEN + BLUETOOTH_ACL_HDRLEN + BLUETOOTH_H4_HDRLEN)

#define BLUETOOTH_SMP_MTU       65
#define BLUETOOTH_MAX_MTU       70

#define BLUETOOTH_MAX_FRAMELEN  (BLUETOOTH_MAX_MTU + BLUETOOTH_MAX_HDRLEN)

#define BLUETOOTH_ADDRSIZE      6
#define BLUETOOTH_ADDRCOPY(d, s) memcpy((d), (s), BLUETOOTH_ADDRSIZE)
#define BLUETOOTH_ADDRCMP(a, b)  (memcmp((a), (b), BLUETOOTH_ADDRSIZE) == 0)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This is the form of the meta-data that accompanies frames received from
 * the Bluetooth stack.
 */

struct bluetooth_frame_meta_s {
	bt_addr_t bm_raddr;			/* Connected remote address */
	uint8_t bm_channel;			/* Connection channel */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: bluetooth_input
 *
 * Description:
 *   Handle incoming Bluetooth input
 *
 *   This function is called when the radio device driver has received an
 *   frame from the network.  The frame from the device driver must be
 *   provided in by the IOB frame argument of the  function call:
 *
 *   - The frame data is in the IOB io_data[] buffer,
 *   - The length of the frame is in the IOB io_len field, and
 *   - The offset past and radio MAC header is provided in the io_offset
 *     field.
 *
 *   The frame argument may refer to a single frame (a list of length one)
 *   or may it be the head of a list of multiple frames.
 *
 *   - The io_flink field points to the next frame in the list (if enable)
 *   - The last frame in the list will have io_flink == NULL.
 *
 * Input Parameters:
 *   radio       The radio network driver interface.
 *   framelist - The head of an incoming list of frames.  Normally this
 *               would be a single frame.  A list may be provided if
 *               appropriate, however.
 *   meta      - Meta data characterizing the received frame.
 *
 *               If there are multiple frames in the list, this metadata
 *               must apply to all of the frames in the list.
 *
 * Returned Value:
 *   OK    The Bluetooth has been processed  and can be deleted
 *   ERROR Hold the Bluetooth and try again later. There is a listening
 *         socket but no recv in place to catch the Bluetooth yet.
 *         Useful when a packet arrives before a recv call is in place.
 *
 * Assumptions:
 *   Called from the network diver with the network locked.
 *
 ****************************************************************************/

struct radio_driver_s;			/* Forward reference */
struct bluetooth_data_ind_s;	/* Forward reference */
struct iob_s;					/* Forward reference */

int bluetooth_input(FAR struct radio_driver_s *radio, FAR struct iob_s *framelist, FAR struct bluetooth_frame_meta_s *meta);

/** Advertising options */
enum {
	/** Convenience value when no options are specified. */
	BT_LE_ADV_OPT_NONE = 0,

	/** Advertise as connectable. Type of advertising is determined by
	 * providing SCAN_RSP data and/or enabling local privacy support.
	 */
	BT_LE_ADV_OPT_CONNECTABLE = BIT(0),

	/** Don't try to resume connectable advertising after a connection.
	 *  This option is only meaningful when used together with
	 *  BT_LE_ADV_OPT_CONNECTABLE. If set the advertising will be stopped
	 *  when bt_le_adv_stop() is called or when an incoming (slave)
	 *  connection happens. If this option is not set the stack will
	 *  take care of keeping advertising enabled even as connections
	 *  occur.
	 */
	BT_LE_ADV_OPT_ONE_TIME = BIT(1),

	/** Advertise using the identity address as the own address.
	 *  @warning This will compromise the privacy of the device, so care
	 *           must be taken when using this option.
	 */
	BT_LE_ADV_OPT_USE_IDENTITY = BIT(2),

	/* Advertise using GAP device name */
	BT_LE_ADV_OPT_USE_NAME = BIT(3),

	/** Use low duty directed advertising mode, otherwise high duty mode
	 *  will be used. This option is only effective when used with
	 *  bt_conn_create_slave_le().
	 */
	BT_LE_ADV_OPT_DIR_MODE_LOW_DUTY = BIT(4),

	/** Enable use of Resolvable Private Address (RPA) as the target address
	 *  in directed advertisements when CONFIG_BT_PRIVACY is not enabled.
	 *  This is required if the remote device is privacy-enabled and
	 *  supports address resolution of the target address in directed
	 *  advertisement.
	 *  It is the responsibility of the application to check that the remote
	 *  device supports address resolution of directed advertisements by
	 *  reading its Central Address Resolution characteristic.
	 */
	BT_LE_ADV_OPT_DIR_ADDR_RPA = BIT(5),
};

/** Description of different data types that can be encoded into
  * advertising data. Used to form arrays that are passed to the
  * bt_le_adv_start() function.
  */
struct bt_data {
	uint8_t type;
	uint8_t data_len;
	const uint8_t *data;
};

/** LE Advertising Parameters. */
struct bt_le_adv_param {
	/** Local identity */
	uint8_t id;

	/** Bit-field of advertising options */
	uint8_t options;

	/** Minimum Advertising Interval (N * 0.625) */
	uint16_t interval_min;

	/** Maximum Advertising Interval (N * 0.625) */
	uint16_t interval_max;
};

/** LE scan parameters */
struct bt_le_scan_param {
	/** Scan type (BT_HCI_LE_SCAN_ACTIVE or BT_HCI_LE_SCAN_PASSIVE) */
	uint8_t type;

	/** Duplicate filtering (BT_HCI_LE_SCAN_FILTER_DUP_ENABLE or
	 *  BT_HCI_LE_SCAN_FILTER_DUP_DISABLE)
	 */
	uint8_t filter_dup;

	/** Scan interval (N * 0.625 ms) */
	uint16_t interval;

	/** Scan window (N * 0.625 ms) */
	uint16_t window;
};

/** @brief Helper to declare elements of bt_data arrays
 *
 *  This macro is mainly for creating an array of struct bt_data
 *  elements which is then passed to bt_le_adv_start().
 *
 *  @param _type Type of advertising data field
 *  @param _data Pointer to the data field payload
 *  @param _data_len Number of bytes behind the _data pointer
 */
#define BT_DATA(_type, _data, _data_len) \
{ \
	.type = (_type), \
	.data_len = (_data_len), \
	.data = (const unsigned char *)(_data), \
}

/** OOB data that is specific for LE SC pairing method. */
struct bt_le_oob_sc_data {
	/** Random Number. */
	uint8_t r[16];
	/** Confirm Value. */
	uint8_t c[16];
};

/** General OOB data. */
struct bt_le_oob {
	/** LE address. If local privacy is enabled this is Resolvable Private
	 *  Address.
	 */
	bt_addr_le_t addr;

	/** OOB data that are relevant for LESC pairing. */
	struct bt_le_oob_sc_data le_sc_data;
};

/** @brief BR/EDR discovery result structure */
struct bt_br_discovery_result {
	/** private */
	uint8_t _priv[4];
	/** Remote device address */
	bt_addr_t addr;
	/** RSSI from inquiry */
	int8_t rssi;
	/** Class of Device */
	uint8_t cod[3];
	/** Extended Inquiry Response */
	uint8_t eir[240];
};

/** BR/EDR discovery parameters */
struct bt_br_discovery_param {
	/** Maximum length of the discovery in units of 1.28 seconds.
	 *  Valid range is 0x01 - 0x30.
	 */
	uint8_t length;

	/** True if limited discovery procedure is to be used. */
	bool limited;
};

struct bt_br_oob {
	/** BR/EDR address. */
	bt_addr_t addr;
};

/** Information about a bond with a remote device. */
struct bt_bond_info {
	/** Address of the remote device. */
	bt_addr_le_t addr;
};

/**
 * @brief Simple network buffer representation.
 *
 * This is a simpler variant of the net_buf object (in fact net_buf uses
 * net_buf_simple internally). It doesn't provide any kind of reference
 * counting, user data, dynamic allocation, or in general the ability to
 * pass through kernel objects such as FIFOs.
 *
 * The main use of this is for scenarios where the meta-data of the normal
 * net_buf isn't needed and causes too much overhead. This could be e.g.
 * when the buffer only needs to be allocated on the stack or when the
 * access to and lifetime of the buffer is well controlled and constrained.
 */
struct net_buf_simple {
	/** Pointer to the start of data in the buffer. */
	uint8_t *data;

	/** Length of the data behind the data pointer. */
	uint8_t len;

	/** Amount of data that this buffer can store. */
	uint16_t size;

	/** Start of the data storage. Not to be accessed directly
	 *  (the data pointer should be used instead).
	 */
	uint8_t *__buf;
};

/**
 * @typedef bt_ready_cb_t
 * @brief Callback for notifying that Bluetooth has been enabled.
 *
 *  @param err zero on success or (negative) error code otherwise.
 */
typedef void (*bt_ready_cb_t)(int err);

/** @brief Enable Bluetooth
 *
 *  Enable Bluetooth. Must be the called before any calls that
 *  require communication with the local Bluetooth hardware.
 *
 *  @param cb Callback to notify completion or NULL to perform the
 *  enabling synchronously.
 *
 *  @return Zero on success or (negative) error code otherwise.
 */
int bt_enable(bt_ready_cb_t cb);

/** @brief Set Bluetooth Device Name
 *
 *  Set Bluetooth GAP Device Name.
 *
 *  @param name New name
 *
 *  @return Zero on success or (negative) error code otherwise.
 */
int bt_set_name(const char *name);

/** @brief Get Bluetooth Device Name
 *
 *  Get Bluetooth GAP Device Name.
 *
 *  @return Bluetooth Device Name
 */
const char *bt_get_name(void);

/** @brief Set the local Identity Address
 *
 *  Allows setting the local Identity Address from the application.
 *  This API must be called before calling bt_enable(). Calling it at any
 *  other time will cause it to fail. In most cases the application doesn't
 *  need to use this API, however there are a few valid cases where
 *  it can be useful (such as for testing).
 *
 *  At the moment, the given address must be a static random address. In the
 *  future support for public addresses may be added.
 *
 *  @return Zero on success or (negative) error code otherwise.
 */
int bt_set_id_addr(const bt_addr_le_t *addr);

/** @brief Get the currently configured identities.
 *
 *  Returns an array of the currently configured identity addresses. To
 *  make sure all available identities can be retrieved, the number of
 *  elements in the @a addrs array should be CONFIG_BT_ID_MAX. The identity
 *  identifier that some APIs expect (such as advertising parameters) is
 *  simply the index of the identity in the @a addrs array.
 *
 *  Note: Deleted identities may show up as BT_LE_ADDR_ANY in the returned
 *  array.
 *
 *  @param addrs Array where to store the configured identities.
 *  @param count Should be initialized to the array size. Once the function
 *               returns it will contain the number of returned identities.
 */
void bt_id_get(bt_addr_le_t *addrs, size_t *count);

/** @brief Create a new identity.
 *
 *  Create a new identity using the given address and IRK. This function
 *  can be called before calling bt_enable(), in which case it can be used
 *  to override the controller's public address (in case it has one). However,
 *  the new identity will only be stored persistently in flash when this API
 *  is used after bt_enable(). The reason is that the persistent settings
 *  are loaded after bt_enable() and would therefore cause potential conflicts
 *  with the stack blindly overwriting what's stored in flash. The identity
 *  will also not be written to flash in case a pre-defined address is
 *  provided, since in such a situation the app clearly has some place it got
 *  the address from and will be able to repeat the procedure on every power
 *  cycle, i.e. it would be redundant to also store the information in flash.
 *
 *  If the application wants to have the stack randomly generate identities
 *  and store them in flash for later recovery, the way to do it would be
 *  to first initialize the stack (using bt_enable), then call settings_load(),
 *  and after that check with bt_id_get() how many identities were recovered.
 *  If an insufficient amount of identities were recovered the app may then
 *  call bt_id_create() to create new ones.
 *
 *  @param addr Address to use for the new identity. If NULL or initialized
 *              to BT_ADDR_LE_ANY the stack will generate a new static
 *              random address for the identity and copy it to the given
 *              parameter upon return from this function (in case the
 *              parameter was non-NULL).
 *  @param irk  Identity Resolving Key (16 bytes) to be used with this
 *              identity. If set to all zeroes or NULL, the stack will
 *              generate a random IRK for the identity and copy it back
 *              to the parameter upon return from this function (in case
 *              the parameter was non-NULL). If privacy support
 *              (CONFIG_BT_PRIVACY) is not enabled this parameter must
 *              be NULL.
 *
 *  @return Identity identifier (>= 0) in case of success, or a negative
 *          error code on failure.
 */
int bt_id_create(bt_addr_le_t *addr, uint8_t *irk);

/** @brief Reset/reclaim an identity for reuse.
 *
 *  The semantics of the @a addr and @a irk parameters of this function
 *  are the same as with bt_id_create(). The difference is the first
 *  @a id parameter that needs to be an existing identity (if it doesn't
 *  exist this function will return an error). When given an existing
 *  identity this function will disconnect any connections created using it,
 *  remove any pairing keys or other data associated with it, and then create
 *  a new identity in the same slot, based on the @a addr and @a irk
 *  parameters.
 *
 *  Note: the default identity (BT_ID_DEFAULT) cannot be reset, i.e. this
 *  API will return an error if asked to do that.
 *
 *  @param id   Existing identity identifier.
 *  @param addr Address to use for the new identity. If NULL or initialized
 *              to BT_ADDR_LE_ANY the stack will generate a new static
 *              random address for the identity and copy it to the given
 *              parameter upon return from this function (in case the
 *              parameter was non-NULL).
 *  @param irk  Identity Resolving Key (16 bytes) to be used with this
 *              identity. If set to all zeroes or NULL, the stack will
 *              generate a random IRK for the identity and copy it back
 *              to the parameter upon return from this function (in case
 *              the parameter was non-NULL). If privacy support
 *              (CONFIG_BT_PRIVACY) is not enabled this parameter must
 *              be NULL.
 *
 *  @return Identity identifier (>= 0) in case of success, or a negative
 *          error code on failure.
 */
int bt_id_reset(uint8_t id, bt_addr_le_t *addr, uint8_t *irk);

/** @brief Delete an identity.
 *
 *  When given a valid identity this function will disconnect any connections
 *  created using it, remove any pairing keys or other data associated with
 *  it, and then flag is as deleted, so that it can not be used for any
 *  operations. To take back into use the slot the identity was occupying the
 *  bt_id_reset() API needs to be used.
 *
 *  Note: the default identity (BT_ID_DEFAULT) cannot be deleted, i.e. this
 *  API will return an error if asked to do that.
 *
 *  @param id   Existing identity identifier.
 *
 *  @return 0 in case of success, or a negative error code on failure.
 */
int bt_id_delete(uint8_t id);

/* Advertising API */

/** @brief Start advertising
 *
 *  Set advertisement data, scan response data, advertisement parameters
 *  and start advertising.
 *
 *  @param param Advertising parameters.
 *  @param ad Data to be used in advertisement packets.
 *  @param ad_len Number of elements in ad
 *  @param sd Data to be used in scan response packets.
 *  @param sd_len Number of elements in sd
 *
 *  @return Zero on success or (negative) error code otherwise.
 *  @return -ECONNREFUSED When connectable advertising is requested and there
 *			  is already maximum number of connections established.
 *			  This error code is only guaranteed when using Zephyr
 *			  controller, for other controllers code returned in
 *			  this case may be -EIO.
 */
int bt_le_adv_start(const struct bt_le_adv_param *param, const struct bt_data *ad, size_t ad_len, const struct bt_data *sd, size_t sd_len);

/** @brief Update advertising
 *
 *  Update advertisement and scan response data.
 *
 *  @param ad Data to be used in advertisement packets.
 *  @param ad_len Number of elements in ad
 *  @param sd Data to be used in scan response packets.
 *  @param sd_len Number of elements in sd
 *
 *  @return Zero on success or (negative) error code otherwise.
 */
int bt_le_adv_update_data(const struct bt_data *ad, size_t ad_len, const struct bt_data *sd, size_t sd_len);

/** @brief Stop advertising
 *
 *  Stops ongoing advertising.
 *
 *  @return Zero on success or (negative) error code otherwise.
 */
int bt_le_adv_stop(void);

/** @typedef bt_le_scan_cb_t
 *  @brief Callback type for reporting LE scan results.
 *
 *  A function of this type is given to the bt_le_scan_start() function
 *  and will be called for any discovered LE device.
 *
 *  @param addr Advertiser LE address and type.
 *  @param rssi Strength of advertiser signal.
 *  @param adv_type Type of advertising response from advertiser.
 *  @param buf Buffer containing advertiser data.
 */
typedef void bt_le_scan_cb_t(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *buf);

/** @brief Start (LE) scanning
 *
 *  Start LE scanning with given parameters and provide results through
 *  the specified callback.
 *
 *  @param param Scan parameters.
 *  @param cb Callback to notify scan results.
 *
 *  @return Zero on success or error code otherwise, positive in case
 *  of protocol error or negative (POSIX) in case of stack internal error
 */
int bt_le_scan_start(const struct bt_le_scan_param *param, bt_le_scan_cb_t cb);

/** @brief Stop (LE) scanning.
 *
 *  Stops ongoing LE scanning.
 *
 *  @return Zero on success or error code otherwise, positive in case
 *  of protocol error or negative (POSIX) in case of stack internal error
 */
int bt_le_scan_stop(void);

/** @brief Set (LE) channel map.
 *
 * @param chan_map Channel map.
 *
 *  @return Zero on success or error code otherwise, positive in case
 *  of protocol error or negative (POSIX) in case of stack internal error
 */
int bt_le_set_chan_map(uint8_t chan_map[5]);

/** @brief Helper for parsing advertising (or EIR or OOB) data.
 *
 *  A helper for parsing the basic data types used for Extended Inquiry
 *  Response (EIR), Advertising Data (AD), and OOB data blocks. The most
 *  common scenario is to call this helper on the advertising data
 *  received in the callback that was given to bt_le_scan_start().
 *
 *  @param ad        Advertising data as given to the bt_le_scan_cb_t callback.
 *  @param func      Callback function which will be called for each element
 *                   that's found in the data. The callback should return
 *                   true to continue parsing, or false to stop parsing.
 *  @param user_data User data to be passed to the callback.
 */
void bt_data_parse(struct net_buf_simple *ad, bool(*func)(struct bt_data *data, void *user_data), void *user_data);

/**
 * @brief Get LE local Out Of Band information
 *
 * This function allows to get local information that are useful for Out Of Band
 * pairing or connection creation process.
 *
 * If privacy is enabled this will result in generating new Resolvable Private
 * Address that is valid for CONFIG_BT_RPA_TIMEOUT seconds. This address
 * will be used for advertising, active scanning and connection creation.
 *
 * @param id  Local identity, in most cases BT_ID_DEFAULT.
 * @param oob LE related information
 *
 *  @return Zero on success or error code otherwise, positive in case
 *  of protocol error or negative (POSIX) in case of stack internal error
 */
int bt_le_oob_get_local(uint8_t id, struct bt_le_oob *oob);

/** @typedef bt_br_discovery_cb_t
 *  @brief Callback type for reporting BR/EDR discovery (inquiry)
 *         results.
 *
 *  A callback of this type is given to the bt_br_discovery_start()
 *  function and will be called at the end of the discovery with
 *  information about found devices populated in the results array.
 *
 *  @param results Storage used for discovery results
 *  @param count Number of valid discovery results.
 */
typedef void bt_br_discovery_cb_t(struct bt_br_discovery_result *results, size_t count);

/** @brief Start BR/EDR discovery
 *
 *  Start BR/EDR discovery (inquiry) and provide results through the specified
 *  callback. When bt_br_discovery_cb_t is called it indicates that discovery
 *  has completed. If more inquiry results were received during session than
 *  fits in provided result storage, only ones with highest RSSI will be
 *  reported.
 *
 *  @param param Discovery parameters.
 *  @param results Storage for discovery results.
 *  @param count Number of results in storage. Valid range: 1-255.
 *  @param cb Callback to notify discovery results.
 *
 *  @return Zero on success or error code otherwise, positive in case
 *  of protocol error or negative (POSIX) in case of stack internal error
 */
int bt_br_discovery_start(const struct bt_br_discovery_param *param, struct bt_br_discovery_result *results, size_t count, bt_br_discovery_cb_t cb);

/** @brief Stop BR/EDR discovery.
 *
 *  Stops ongoing BR/EDR discovery. If discovery was stopped by this call
 *  results won't be reported
 *
 *  @return Zero on success or error code otherwise, positive in case
 *  of protocol error or negative (POSIX) in case of stack internal error
 */
int bt_br_discovery_stop(void);

/**
 * @brief Get BR/EDR local Out Of Band information
 *
 * This function allows to get local controller information that are useful
 * for Out Of Band pairing or connection creation process.
 *
 * @param oob Out Of Band information
 */
int bt_br_oob_get_local(struct bt_br_oob *oob);

/** @brief Enable/disable set controller in discoverable state.
 *
 *  Allows make local controller to listen on INQUIRY SCAN channel and responds
 *  to devices making general inquiry. To enable this state it's mandatory
 *  to first be in connectable state.
 *
 *  @param enable Value allowing/disallowing controller to become discoverable.
 *
 *  @return Negative if fail set to requested state or requested state has been
 *  already set. Zero if done successfully.
 */
int bt_br_set_discoverable(bool enable);

/** @brief Enable/disable set controller in connectable state.
 *
 *  Allows make local controller to be connectable. It means the controller
 *  start listen to devices requests on PAGE SCAN channel. If disabled also
 *  resets discoverability if was set.
 *
 *  @param enable Value allowing/disallowing controller to be connectable.
 *
 *  @return Negative if fail set to requested state or requested state has been
 *  already set. Zero if done successfully.
 */
int bt_br_set_connectable(bool enable);

/** Clear pairing information.
  *
  * @param id    Local identity (mostly just BT_ID_DEFAULT).
  * @param addr  Remote address, NULL or BT_ADDR_LE_ANY to clear all remote
  *              devices.
  *
  * @return 0 on success or negative error value on failure.
  */
int bt_unpair(uint8_t id, const bt_addr_le_t *addr);

/** Iterate through all existing bonds.
  *
  * @param id         Local identity (mostly just BT_ID_DEFAULT).
  * @param func       Function to call for each bond.
  * @param user_data  Data to pass to the callback function.
  */
void bt_foreach_bond(uint8_t id, void (*func)(const struct bt_bond_info *info, void *user_data), void *user_data);

#endif							/*  __INCLUDE_TINYARA_BLUETOOTH_BLUETOOTH_H */
