#ifndef INCLUDE_ARTIK_CLOUD_H_
#define INCLUDE_ARTIK_CLOUD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"
#include "artik_http.h"
#include "artik_websocket.h"

	/*! \file artik_cloud.h
	 *
	 *  \brief Cloud module definition
	 *
	 *  Definitions and functions for accessing
	 *  the Cloud module and communicating with
	 *  the Artik Cloud over its RESTful API.
	 *
	 *  Detailed API specifications: https://developer.samsungsami.io/sami/api-spec.html
	 *
	 *  \example cloud_test/artik_cloud_test.c
	 */

	/*!
	 *  \brief Maximum length for token strings
	 *
	 *  Maximum length allowed for string
	 *  containing an authorization token
	 *  for the Artik Cloud API
	 */
#define	MAX_TOKEN_LEN	64
#define	WEBSOCKET_CONNECTION_TIMEOUT_MS (10*1000)

	/*! \struct artik_cloud_module
	 *
	 *  \brief Cloud module operations
	 *
	 *  Structure containing all the operations exposed
	 *  by the module to perform Cloud requests
	 */
	typedef struct {
		/*!
		 *  \brief Send a message to the Cloud
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] device_id ID of the source device from which the message is sent
		 *  \param[in] message Content of the message to send in a JSON formatted string
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*send_message) (const char *access_token,
					    const char *device_id,
					    const char *message,
					    char **response);
		/*!
		 *  \brief Send actions to a device
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] device_id ID of the destination device to send the actions to
		 *  \param[in] action JSON formatted string containing the list of actions to send with/without parameters
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*send_action) (const char *access_token,
					   const char *device_id,
					   const char *action, char **response);
		/*!
		 *  \brief Get current user profile
		 *
		 *  \param[in] access_token Authorization token. It must be a user token associated to the user whose profile is requested
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_current_user_profile) (const char
							*access_token,
							char **response);
		/*!
		 *  \brief Get user devices
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] count Number of entries to return
		 *  \param[in] properties Returns the properties of the devices if true
		 *  \param[in] offset Offset in the complete device list to return 'count' entries from
		 *  \param[in] user_id ID of the user to get the device list from
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_user_devices) (const char *access_token,
						int count, bool properties,
						int offset, const char *user_id,
						char **response);
		/*!
		 *  \brief Get user device types
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] count Number of entries to return
		 *  \param[in] shared Return also the shared public devices if true
		 *  \param[in] offset Offset in the complete device list to return 'count' entries from
		 *  \param[in] user_id ID of the user to get the device type list from
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_user_device_types) (const char *access_token,
						     int count, bool shared,
						     int offset,
						     const char *user_id,
						     char **response);
		/*!
		 *  \brief Get user application properties
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] user_id ID of the user owning the application to get the properties from
		 *  \param[in] app_id ID of the application to get the properties from
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_user_application_properties) (const char
							       *access_token,
							       const char
							       *user_id,
							       const char
							       *app_id,
							       char **response);
		/*!
		 *  \brief Get device
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] device_id ID of the device from which to read the information
		 *  \param[in] properties Return also the device properties if true
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_device) (const char *access_token,
					  const char *device_id,
					  bool properties, char **response);
		/*!
		 *  \brief Get device token
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] device_id ID of the device to get the token from
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_device_token) (const char *access_token,
						const char *device_id,
						char **response);
		/*!
		 *  \brief Add device
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] user_id ID of the user to assign the new device
		 *  \param[in] dt_id Device type ID of the device to create
		 *  \param[in] name Friendly name to give to the new device
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*add_device) (const char *access_token,
					  const char *user_id, const char *dt_id,
					  const char *name, char **response);
		/*!
		 *  \brief Create device token. If exists, update it.
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] device_id ID of the device to create the token from
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*update_device_token) (const char *access_token,
						   const char *device_id,
						   char **response);
		/*!
		 *  \brief Delete device token
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] device_id ID of the device to delete the token from
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*delete_device_token) (const char *access_token,
						   const char *device_id,
						   char **response);
		/*!
		 *  \brief Delete device
		 *
		 *  \param[in] access_token Authorization token
		 *  \param[in] device_id ID of the device to delete
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*delete_device) (const char *access_token,
						   const char *device_id,
						   char **response);

		/*!
		 *  \brief Start Secure Device Registration process
		 *
		 *  \param[in] device_type_id Device Type ID of the device to register
		 *  \param[in] vendor_id Vendor ID of the device to register
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use. In the SDR process, this response returns a JSON object
		 *          containing a registration ID (rid), a nonce, and a PIN. These values should
		 *          be used for the rest of the SDR process.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*sdr_start_registration) (const char
						      *device_type_id,
						      const char *vendor_id,
						      char **response);
		/*!
		 *  \brief Get Secure Device Registration process status
		 *
		 *  \param[in] reg_id Registration ID (rid) returned by \ref sdr_start_registration
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use. In the SDR process, this response returns a JSON object
		 *          containing the current status of the SDR process.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*sdr_registration_status) (const char *reg_id,
						       char **response);
		/*!
		 *  \brief Complete Secure Device Registration process
		 *
		 *  \param[in] reg_id Registration ID (rid) returned by \ref sdr_start_registration
		 *  \param[in] reg_nonce Registration nonce returned by \ref sdr_start_registration
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response JSON data returned by the Cloud. It should be freed by the calling
		 *          function after use.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*sdr_complete_registration) (const char *reg_id,
							 const char *reg_nonce,
							 char **response);
		/*!
		 *  \brief Open websocket stream
		 *
		 *  \param[in] handle Pointer of the handle that is used for controlling the stream
		 *  \param[in] access_token Authorization token
		 *  \param[in] device_id ID of the device to be connected on the Cloud
		 *  \param[in] use_se Whether you want to use SE or not
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*websocket_open_stream) (artik_websocket_handle*handle, const char *access_token, const char *device_id, bool use_se);
		/*!
		 *  \brief Send a message through websocket stream
		 *
		 *  \param[in] handle Handle value that is used for controlling the stream
		 *  \param[in] message Content of the message to send in a JSON formatted string
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*websocket_send_message) (artik_websocket_handle
						      handle, char *message);
		/*!
		 *  \brief Set a callback function handling received data
		 *
		 *  \param[in] handle Handle value that is used for controlling the stream
		 *  \param[in] callback \ref artik_websocket_callback type function pointer of a callback
		 *  \param[in] user_data Pointer of a data that you want to pass into callback
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*websocket_set_receive_callback)(artik_websocket_handle handle, artik_websocket_callback callback, void *user_data);

		/*!
		 *  \brief Set a callback function handling changes in connection state
		 *
		 *  \param[in] handle Handle value obtained from websocket_request function
		 *  \param[in] callback \ref artik_websocket_callback type function pointer of a
		 *                       callback to be called upon connection
		 *  \param[in] user_data Pointer of a data that you want to pass into callback
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*websocket_set_connection_callback)(artik_websocket_handle handle, artik_websocket_callback callback, void *user_data);

		/*!
		 *  \brief Close websocket stream
		 *
		 *  \param[in] handle Handle value of the stream that you want to close
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*websocket_close_stream) (artik_websocket_handle handle);
	} artik_cloud_module;

	extern const artik_cloud_module cloud_module;

#ifdef __cplusplus
}
#endif
#endif				/* INCLUDE_ARTIK_CLOUD_H_ */
