#ifndef	__ARTIK_SECURITY_H__
#define	__ARTIK_SECURITY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

/*! \file artik_security.h
 *
 *  \brief Security module definition
 *
 *  Definitions and functions for accessing
 *  the Security module and make use of the
 *  hardware Secure Element for cryptographic
 *  and signing features.
 *
 * \example security_test/artik_security_test.c
 */

	/*!
	 *  \brief Maximum length for the serial number strings
	 *
	 *  Maximum length allowed for string
	 *  containing an serial number
	 *  from a certificate of artik secure element
	 */
#define ARTIK_CERT_SN_MAXLEN 20

	/*!
	 *  \brief Security handle type
	 *
	 *  Handle type used to carry instance specific
	 *  information for a security object
	 */
	typedef void *artik_security_handle;

	/*! \struct artik_security_module
	 *
	 *  \brief Security module operations
	 *
	 *  Structure containing all the exposed operations exposed
	 *  by the Security module
	 */
	typedef struct {
		/*!
		 *  \brief Request a security instance
		 *
		 *  \param[out] handle Handle tied to the requested security instance returned by the function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*request) (artik_security_handle * handle);
		/*!
		 *  \brief Release a security instance
		 *
		 *  \param[in] handle Handle tied to the requested security instance to be released.
		 *          This handle is returned by the request function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*release) (artik_security_handle handle);
		/*!
		 *  \brief Get the certificate stored in the SE
		 *
		 *  \param[in] handle Handle tied to a requested security instance.
		 *          This handle is returned by the request function.
		 *  \param[out] cert Pointer to a string that will be allocated by the function
		 *          and filled with the content of the certificate. This string must
		 *          be freed by the calling function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_certificate) (artik_security_handle handle,
					       char **cert);
		/*!
		 *  \brief Get private key from the certificate stored in the SE
		 *
		 *  \param[in] handle Handle tied to a requested security instance.
		 *          This handle is returned by the request function.
		 *  \param[in] cert Pointer to a string containing the certificate to generate
		 *          the private key from
		 *  \param[out] key Pointer to a string that will be allocated by the function
		 *          and filled with the content of the key. This string must
		 *          be freed by the calling function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_key_from_cert) (artik_security_handle handle,
						 const char *cert, char **key);

		/*!
		 *  \brief Get root CA stored in the SE
		 *
		 *  \param[in] handle Handle tied to a requested security instance.
		 *          This handle is returned by the request function.
		 *  \param[out] root_ca Pointer to a string that will be allocated by the function
		 *          and filled with the content of the CA. This string must
		 *          be freed by the calling function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error (*get_root_ca) (artik_security_handle handle, char **root_ca);

		/*!
		 *  \brief Generate true random bytes
		 *
		 *  \param[in] handle Handle tied to a requested security instance.
		 *          This handle is returned by the request function.
		 *  \param[out] rand Pointer to a preallocated array that will be filled with the
		 *          generated random bytes
		 *  \param[in] len Number of random bytes to generate
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_random_bytes) (artik_security_handle handle,
						unsigned char *rand, int len);

		/*!
		 *  \brief Get the serial number from the certificate
		 *
		 *  \param[in] handle Handle tied to a requested security instance.
		 *          This handle is returned by the request function.
		 *  \param[out] SN preallocated array provided by the user
		 *  \param[in,out] lenSN size of the pointer preallocated and set after the pointer was filled.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_certificate_sn) (artik_security_handle handle, unsigned char *sn, unsigned int *len);

	} artik_security_module;

	extern const artik_security_module security_module;

#ifdef __cplusplus
}
#endif
#endif				/* __ARTIK_SECURITY_H__ */
