#ifndef INCLUDE_ARTIK_WEBSOCKET_H_
#define INCLUDE_ARTIK_WEBSOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"
#include "artik_ssl.h"

	/*! \file artik_websocket.h
	 *
	 *  \brief WEBSOCKET module definition
	 *
	 *  Definitions and functions for accessing
	 *  the WEBSOCKET module and performing
	 *  websocket client to remote servers
	 *
	 *  \example websocket_test/artik_websocket_test.c
	 *  \example websocket_test/artik_websocket_client_test.c
	 *  \example websocket_test/artik_websocket_cloud_test.c
	 */

	/*!
	 *  \brief Websocket connection state
	 *
	 *  State of the websocket
	 */
	typedef enum {
		ARTIK_WEBSOCKET_CLOSED = 1,
		ARTIK_WEBSOCKET_CONNECTED,
		ARTIK_WEBSOCKET_HANDSHAKE_ERROR
	} artik_websocket_connection_state;

	/*!
	 *  \brief WEBSOCKET handle type
	 *
	 *  Handle type used to carry instance specific
	 *  information for a WEBSOCKET object.
	 */
	typedef void *artik_websocket_handle;

	/*!
	 *  \brief websocket configuration structure
	 *
	 *  Structure containing one or more
	 *  websocket connection information fields
	 */
	typedef struct {
		/*!
		 *  \brief URI of the websocket server
		 */
		char *uri;
		/*!
		 *  \brief SSL configuration
		 */
		artik_ssl_config ssl_config;
        /*!
         *  \brief Pointer to data for internal use by the API.
         */
        void *private_data;
	} artik_websocket_config;

	/*!
	 *  \brief Websocket callback type
	 *
	 *  Callback prototype for Websocket callback functions
	 */
	typedef void (*artik_websocket_callback)(void *user_data, void *result);

	/*! \struct artik_websocket_module
	 *
	 *  \brief Websocket module operations
	 *
	 *  Structure containing all the operations exposed
	 *  by the module to perform Websocket features
	 */
	typedef struct {
		/*!
		 *  \brief Request a handle to make a websocket stream
		 *
		 *  \param[in] handle Pointer of a Handle variable
		 *  \param[in] config Configuration structure of the websocket. It should be
		 *                    set before passed to this function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*websocket_request) (artik_websocket_handle * handle,
						 artik_websocket_config * config);
		/*!
		 *  \brief Open websocket stream
		 *
		 *  \param[in] handle Handle value obtained from websocket_request function
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*websocket_open_stream) (artik_websocket_handle
						     handle);
		/*!
		 *  \brief Send a string through stream
		 *
		 *  \param[in] handle Handle value obtained from websocket_request function
		 *  \param[in] message String that you want to send
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*websocket_write_stream) (artik_websocket_handle
						      handle, char *message);
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
		artik_error(*websocket_set_connection_callback) (artik_websocket_handle
								handle, artik_websocket_callback callback, void *user_data);
		/*!
		 *  \brief Set a callback function handling data received
		 *
		 *  \param[in] handle Handle value obtained from websocket_request function
		 *  \param[in] callback \ref artik_websocket_callback type function pointer of a
		 *                       callback to be called upon data reception
		 *  \param[in] user_data Pointer of a data that you want to pass into callback
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*websocket_set_receive_callback) (artik_websocket_handle
						       handle, artik_websocket_callback callback, void *user_data);
		/*!
		 *  \brief Close websocket stream
		 *
		 *  \param[in] handle Handle value returned by \ref websocket_request function
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*websocket_close_stream) (artik_websocket_handle
						      handle);
	} artik_websocket_module;

	extern const artik_websocket_module websocket_module;

#ifdef __cplusplus
}
#endif
#endif				/* INCLUDE_ARTIK_WEBSOCKET_H_ */
