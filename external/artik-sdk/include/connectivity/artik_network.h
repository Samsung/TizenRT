#ifndef INCLUDE_ARTIK_NETWORK_H_
#define INCLUDE_ARTIK_NETWORK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

	/*! \file artik_network.h
	 *
	 *  \brief Network module definition
	 *
	 *  Definitions and functions for accessing
	 *  the Network module.
	 *
	 *  \example network_test/artik_network_test.c
	 *  \example network_test/artik_dhcp_client_test.c
	 */

	/*!
	 *  \brief Maximum length for IP Address
	 *
	 *  Maximum length allowed for IP Address
	 */
#define MAX_IP_ADDRESS_LEN			24

	/*!
	 *  \brief Maximum number of configurable DNS servers for the DHCP server
	 *
	 *  Maximum number of DNS servers that can be exposed by the DHCP server
	 */
#define MAX_DNS_ADDRESSES			2

	/*!
	 *  \brief WATCH_ONLINE_STATUS handle type
	 *
	 *  Handle type used to carry instance specific
	 *  information for a WATCH_ONLINE_STATUS object.
	 */
	typedef void* watch_online_status_handle;

	/*!
	 *  \brief IP address type
	 *
	 *  Type of an IP address.
	 */
	typedef enum {
		ARTIK_IPV4,
		ARTIK_IPV6
	} artik_network_ip_type_t;

	/*! \struct artik_network_ip
	 *
	 *  \brief IP Address definition structure
	 *
	 *  Structure containing elements for defining
	 *  an IP Address
	 */
	typedef struct {
		/*!
		 *  \brief current IP Address
		 */
		char address[MAX_IP_ADDRESS_LEN];
		/*!
		 *  \brief IP Address type
		 */
		artik_network_ip_type_t type;
	} artik_network_ip;

	/*!
	 *  \brief Watch online status callback prototype
	 *
	 *  \param[in] online_status The new online status
	 *  \param[in] user_data The user data passed from the callback function
	 */
	typedef void (*watch_online_status_callback)(bool online_status, void *user_data);

	/*!
	 * \brief DHCP client handle type
	 *
	 * Handle type used to carry instance specific
	 * information for a DHCP client object
	 */
	typedef void *artik_network_dhcp_client_handle;

	/*!
	 * \brief DHCP server handle type
	 *
	 * Handle type used to carry instance specific
	 * information for a DHCP server object
	 */
	typedef void *artik_network_dhcp_server_handle;

	/*!
	 *  \brief Network Interface
	 *
	 *  Type of a network interface
	 */
	typedef enum {
		ARTIK_WIFI,
		ARTIK_ETHERNET
	} artik_network_interface_t;

	/*!
	 *  \brief DHCP server configuration structure
	 *
	 *  Structure containing the configuration parameters
	 *  of the DHCP server.
	 */
	typedef struct {
		artik_network_interface_t interface;
		artik_network_ip ip_addr;
		artik_network_ip netmask;
		artik_network_ip gw_addr;
		artik_network_ip dns_addr[MAX_DNS_ADDRESSES];
		artik_network_ip start_addr;
		unsigned int num_leases;
	} artik_network_dhcp_server_config;

	/*! \struct artik_network_module
	 *
	 *  \brief Network module operations
	 *
	 *  Structure containing all the operations exposed
	 *  by the network module
	 */
	typedef struct {
		/*!
		 *  \brief Get current public IP
		 *
		 *  \param[out] ip Pointer to an IP structure filled up by
		 *          the function with the current public IP of
		 *          the running device
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_current_public_ip)(artik_network_ip * ip);

		/*!
		 *  \brief Start the DHCP client service
		 *
		 *  \param[out] handle Handle returned by the API for later
		 *              reference to the service
		 *  \param[in] interface The network interface onto which
		 *            to start the DHCP client
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*dhcp_client_start)(artik_network_dhcp_client_handle * handle,
				artik_network_interface_t interface);

		/*!
		 *  \brief Stop the DHCP client service
		 *
		 *  \param[in] handle Handle returned by the API when calling
		 *             \ref dhcp_client_start
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*dhcp_client_stop)(artik_network_dhcp_client_handle handle);

		/*!
		 *  \brief Start the DHCP server
		 *
		 *  \param[out] handle Handle returned by the API for later
		 *              reference to the service
		 *  \param[in] config Configuration of the DHCP server
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*dhcp_server_start)(artik_network_dhcp_server_handle * handle,
				artik_network_dhcp_server_config *config);

		/*!
		 *  \brief Stop the DHCP server
		 *
		 *  \param[in] handle Handle returned by the API when calling
		 *             \ref dhcp_server_start.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*dhcp_server_stop)(artik_network_dhcp_server_handle handle);

		/*!
		 *  \brief Get network online status
		 *
		 *  \param[out] online_status  Pointer to an integer filled up
		 *               by the function with the current online status
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_online_status)(bool *online_status);

		/*!
		 *  \brief Add a registered callback that watches online status
		 *
		 *  Call callback function when online status change.
		 *
		 *  \param[out] handle Handle reference of the registered callback
		 *  \param[in] func The callback function to register
		 *  \param[in] user_data The user data to be passed to the callback function
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*add_watch_online_status)(watch_online_status_handle* handle,
						      watch_online_status_callback func,
						      void *user_data);

		/*!
		 *  \brief Remove a registered callback
		 *
		 *  \param[in] handle Reference to the registered callback
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*remove_watch_online_status)(watch_online_status_handle handle);
	} artik_network_module;

	extern const artik_network_module network_module;

#ifdef __cplusplus
}
#endif
#endif				/* INCLUDE_ARTIK_NETWORK_H_ */
