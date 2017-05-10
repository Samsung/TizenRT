#ifndef	__ARTIK_WIFI_H__
#define	__ARTIK_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

	/*! \file artik_wifi.h
	 *
	 *  \brief Wi-Fi module definition
	 *
	 *  Definitions and functions for accessing
	 *  the Wi-Fi module for scanning and configuring
	 *  wireless networks
	 *
	 *  \example wifi_test/artik_wifi_test.c
	 */

	/*!
	 *  \brief Maximum length for Access Points names
	 *
	 *  Maximum length allowed for Access Points
	 *  names
	 */
#define MAX_AP_NAME_LEN			64

	/*!
	 *  \brief Maximum length for BSSID
	 *
	 *  Maximum length allowed for strings
	 *  holding the SSID (MAC address) of
	 *  an Access Point
	 */
#define MAX_AP_BSSID_LEN		18

	/*!
	 *  \brief The AP does not support authentication
	 */
#define WIFI_ENCRYPTION_OPEN			(1 << 0)
	/*!
	 *  \brief The AP supports WEP authentication
	 */
#define WIFI_ENCRYPTION_WEP				(1 << 1)
	/*!
	 *  \brief The AP supports WPA authentication
	 */
#define WIFI_ENCRYPTION_WPA				(1 << 2)
	/*!
	 *  \brief The AP supports WPA2 authentication
	 */
#define WIFI_ENCRYPTION_WPA2			(1 << 3)
	/*!
	 *  \brief The AP supports WPA2 Personal authentication
	 */
#define WIFI_ENCRYPTION_WPA2_PERSONAL	(1 << 16)
	/*!
	 *  \brief The AP supports WPA2 Enterprise authentication
	 */
#define WIFI_ENCRYPTION_WPA2_ENTERPRISE	(1 << 17)

    /*!
     *  \brief Wi-Fi modes
     */
    typedef enum {
        /*!
         *  \brief Station (client) mode
         */
        ARTIK_WIFI_MODE_STATION,
        /*!
         *  \brief Access Point mode
         */
        ARTIK_WIFI_MODE_AP,
        /*!
         *  \brief Invalid mode
         */
        ARTIK_WIFI_MODE_NONE
    } artik_wifi_mode_t;

    /*!
     *  \brief Wi-Fi callback type
     *
     *  Callback prototype for Wi-Fi callback functions. When passed
     *  to \ref set_connect_callback, \ref result is mapped to a
     *  pointer to \ref artik_wifi_connection_info. If passed to
     *  \ref set_scan_result_callback, \ref result is mapped to a pointer
     *  to \ref artik_error.
     *
     */
	typedef void (*artik_wifi_callback) (void *result, void *user_data);

	/*! \struct artik_wifi_ap
	 *
	 *  \brief Access Point definition structure
	 *
	 *  Structure containing elements for defining
	 *  an Access Point and its configuration
	 */
	typedef struct {
		/*!
		 *  \brief Name (SSID) of the Access Point
		 */
		char name[MAX_AP_NAME_LEN];
		/*!
		 *  \brief BSSID (MAC address) of the Access Point
		 */
		char bssid[MAX_AP_BSSID_LEN];
		/*!
		 *  \brief Frequency of the Access Point
		 */
		unsigned int frequency;
		/*!
		 *  \brief Signal strength (in dBm) between the device
		 *  and the Access Point
		 */
		int signal_level;
		/*!
		 *  \brief Flags specifying the Encryption/Authentication
		 *  methods supported by the Access Point
		 */
		unsigned int encryption_flags;
	} artik_wifi_ap;

    /*! \struct artik_wifi_connection_info
     *
     *  \brief Connection information structure
     *
     *  Structure containing information on the
     *  state of the Wi-Fi connection
     */
	typedef struct {
        /*!
         *  \brief Error code associated to the latest change of state
         *         of the connection
         */
	    artik_error error;
        /*!
         *  \brief Wi-Fi connection state
         */
	    bool connected;
	} artik_wifi_connection_info;

	/*! \struct artik_wifi_module
	 *
	 *  \brief Wi-Fi module operations
	 *
	 *  Structure containing all the exposed operations exposed
	 *  by the module to operate on a GPIO instance
	 */
	typedef struct {
		/*!
		 * \brief Initialize Wi-Fi API
		 *
		 * \param[in] mode Initialize and configure Wi-Fi into a specific mode.
		 *
		 * \return S_OK on success, otherwise a negative error value.
		 */
		artik_error(*init) (artik_wifi_mode_t mode);
		/*!
		 * \brief Deinitialize Wi-Fi API
		 */
		artik_error(*deinit) (void);
		/*!
		 * \brief Issue a scan command
		 *
		 * \return S_OK on success, otherwise a negative error value.
		 */
		artik_error(*scan_request) (void);
		/*!
		 * \brief Read a scan result buffer
		 *
		 * \param[out] aps Information about the detected access points. The
		 *                 returned pointer points to an array allocated by the
		 *                 function. It is up to the caller to free it after use.
		 * \param[out] num_aps Number of access points stored in the array.
		 *
		 * \return S_OK on success, otherwise a negative error value.
		 */
		artik_error(*get_scan_result) (artik_wifi_ap**aps, int *num_aps);
        /*!
         * \brief Start and Access Point.
         *
         * Wi-Fi must have been initialized with ARTIK_WIFI_MODE_AP.
         *
         * \param[in] ssid Name of the SSID to use for the Access Point.
         * \param[in] passphrase Passphrase to use for authenticating on the AP.
         *                       If NULL, then the AP is created in Open mode.
         * \param[in] channel Channel onto which the AP must broadcast.
         *
         * \return S_OK on success, otherwise a negative error value.
         */
		artik_error (*start_ap)(const char *ssid, const char *passphrase, unsigned int channel);
		/*!
		 * \brief Connect to an access point
		 *
         * Wi-Fi must have been initialized with ARTIK_WIFI_MODE_STATION.
         *
		 * \param[in] ssid SSID of the access point to connect to.
		 * \param[in] password Password of the access point. NULL for Open authentication.
		 * \param[in] persistent If true, the profile is saved for auto connection upon reboot.
		 *
		 * \return S_OK on success, otherwise a negative error value.
		 */
		artik_error(*connect) (const char *ssid, const char *password,
				       bool persistent);
		/*!
		 * \brief Disconnect from the current access point
		 *
		 * Wi-Fi must have been initialized with ARTIK_WIFI_MODE_STATION.
		 *
		 * \return S_OK on success, otherwise a negative error value.
		 */
		artik_error(*disconnect) (void);
		/*!
		 * \brief Register scan result callback function
		 *
		 * \param[in] callback Function to register for receiving scan results.
		 * \param[in] user_data Pointer to the client data that will be passed to the callback.
		 *			            This parameter can be set to NULL.
		 * \return S_OK on success, otherwise a negative error value.
		 */
		artik_error(*set_scan_result_callback) (artik_wifi_callback
							user_callback,
							void *user_data);
		/*!
		 * \brief Register connection result callback function
		 *
		 * \param[in] user_callback Callback function to register for receiving connection result.
		 * \param[in] user_data A pointer to the client data that will be passed to the callback.
		 *                      This parameter can be set to NULL.
		 *
		 * \return S_OK on success, otherwise a negative error value.
		 */
		artik_error(*set_connect_callback) (artik_wifi_callback user_callback, void *user_data);
	} artik_wifi_module;

	extern artik_wifi_module wifi_module;

#ifdef __cplusplus
}
#endif
#endif				/* ARTIK_WIFI_H_ */
