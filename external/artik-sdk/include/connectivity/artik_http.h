#ifndef INCLUDE_ARTIK_HTTP_H_
#define INCLUDE_ARTIK_HTTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"
#include "artik_ssl.h"

	/*! \file artik_http.h
	 *
	 *  \brief HTTP module definition
	 *
	 *  Definitions and functions for accessing
	 *  the HTTP module and performing HTTP/HTTPS
	 *  requests to remote servers
	 *
	 *  \example http_test/artik_http_test.c
	 *  \example http_test/artik_http_openssl_test.c
	 */

	/*!
	 *  \brief Maximum length for headers
	 *
	 *  Maximum length allowed for string
	 *  containing the name and the date
	 *  of a HTTP header
	 */
#define MAX_HEADER_SIZE									256

	/*!
	 *  \brief HTTP header field structure
	 *
	 *  Structure containing the elements
	 *  for a HTTP header field
	 */
	typedef struct {
		/*!
		 *  \brief Name of the HTTP header field (e.g. "user-agent")
		 */
		char *name;
		/*!
		 *  \brief Content of the HTTP header field
		 */
		char *data;
	} artik_http_header_field;

	/*!
	 *  \brief HTTP headers structure
	 *
	 *  Structure containing one or more
	 *  HTTP header fields
	 */
	typedef struct {
		/*!
		 *  \brief Number of header fields pointed to by "fields"
		 */
		int num_fields;
		/*!
		 *  \brief Pointer to an array containing the header fields
		 */
		artik_http_header_field *fields;
	} artik_http_headers;

	/*!
	 *  \brief Stream data callback prototype
	 *
	 *  \param[in] data Received data
	 *  \param[in] len Length of the received data
	 *  \param[in] user_data The user data passed from the callback function
	 */
	typedef int (*artik_http_stream_callback)(char *data, unsigned int len, void *user_data);

	/*! \struct artik_http_module
	 *
	 *  \brief HTTP module operations
	 *
	 *  Structure containing all the operations exposed
	 *  by the module to perform HTTP requests
	 */
	typedef struct {
		/*!
		 *  \brief Perform a GET request on streaming data
		 *
		 *  \param[in] url URL to request
		 *  \param[in] headers Pointer to the structure object containing the HTTP headers to send
		 *  \param[out] status Pointer to the status filled up by the function with the server's
		 *          response status
		 *  \param[in] callback Function called upon receiving new data from the stream
		 *  \param[in] user_data Pointer to user data that will be passed as a parameter
		 *          to the callback function
		 *  \param[in] ssl SSL configuration to use when targeting https urls. Can be NULL.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error (*get_stream) (const char *url, artik_http_headers *headers,
					   int *status,
					   artik_http_stream_callback callback, void *user_data,
					   artik_ssl_config *ssl);
		/*!
		 *  \brief Perform a GET request
		 *
		 *  \param[in] url URL to request
		 *  \param[in] headers Pointer to the structure object containing the HTTP headers to send
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response body returned by the server. It should be freed by the calling
		 *          function after use.
		 *  \param[out] status Pointer to the status filled up by the function with the server's
		 *          response status
		 *  \param[in] ssl SSL configuration to use when targeting https urls. Can be NULL.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get) (const char *url,
				   artik_http_headers * headers,
				   char **response, int *status, artik_ssl_config *ssl);
		/*!
		 *  \brief Perform a POST request
		 *
		 *  \param[in] url URL to request
		 *  \param[in] headers Pointer to the structure object containing the HTTP headers to send
		 *  \param[in] body String containing the body data to send along the POST request
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response body returned by the server. It should be freed by the calling
		 *          function after use.
		 *  \param[out] status Pointer to the status filled up by the function with the server's
		 *          response status
		 *  \param[in] ssl SSL configuration to use when targeting https urls. Can be NULL.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*post) (const char *url,
				    artik_http_headers * headers,
				    const char *body, char **response,
				    int *status, artik_ssl_config *ssl);
		/*!
		 *  \brief Perform a PUT request
		 *
		 *  \param[in] url URL to request
		 *  \param[in] headers Pointer to the structure object containing the HTTP headers to send
		 *  \param[in] body String containing the body data to send along the PUT request
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response body returned by the server. It should be freed by the calling
		 *          function after use.
		 *  \param[out] status Pointer to the status filled up by the function with the server's
		 *                  response status
		 *  \param[in] ssl SSL configuration to use when targeting https urls. Can be NULL.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*put) (const char *url,
				   artik_http_headers * headers,
				   const char *body, char **response,
				   int *status, artik_ssl_config *ssl);
		/*!
		 *  \brief Perform a DELETE request
		 *
		 *  \param[in] url URL to request
		 *  \param[in] headers Pointer to the structure object containing the HTTP headers to send
		 *  \param[out] response Pointer to a string allocated and filled up by the function with the
		 *          response body returned by the server. It should be freed by the calling
		 *          function after use.
		 *  \param[out] status Pointer to the status filled up by the function with the server's
		 *          response status
		 *  \param[in] ssl SSL configuration to use when targeting https urls. Can be NULL.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*del) (const char *url,
				   artik_http_headers * headers,
				   char **response, int *status, artik_ssl_config *ssl);

	} artik_http_module;

	extern const artik_http_module http_module;

#ifdef __cplusplus
}
#endif
#endif				/* INCLUDE_ARTIK_HTTP_H_ */
