#ifndef	__ARTIK_SERIAL_H__
#define	__ARTIK_SERIAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

	/*! \file artik_serial.h
	 *
	 *  \brief SERIAL module definition
	 *
	 *  Definitions and functions for accessing
	 *  to the SERIAL module and performing operations
	 *  on hardware IOs
	 *
	 *  \example serial_test/artik_serial_test.c
	 */

	/*!
	 *  \brief Maximum length for SERIAL names
	 *
	 *  Maximum length allowed for user-friendly
	 *  names assigned when requesting a SERIAL.
	 */
#define MAX_NAME_LEN		64

	/*!
	 *  \brief SERIAL handle type
	 *
	 *  Handle type used to carry instance specific
	 *  information for a SERIAL object.
	 */
	typedef void *artik_serial_handle;


	/*!
	 *  \brief SERIAL callback type
	 *
	 *  Callback prototype for SERIAL received data
	 *
	 *  \param[in] user_data The user data passed from the \ref set_received_callback function
	 *  \param[in] buff Buffer filled with the received data.
	 *  \param[in] len Number of bytes read.
	 */
	typedef void (*artik_serial_callback)(void *user_data, unsigned char *buf, int len);

	/*!
	 *  \brief SERIAL baudrate type
	 *
	 *  Type for specifying the baudrate of the SERIAL module.
	 */
	typedef enum {
		ARTIK_SERIAL_BAUD_4800,
		ARTIK_SERIAL_BAUD_9600,
		ARTIK_SERIAL_BAUD_14400,
		ARTIK_SERIAL_BAUD_19200,
		ARTIK_SERIAL_BAUD_38400,
		ARTIK_SERIAL_BAUD_57600,
		ARTIK_SERIAL_BAUD_115200
	} artik_serial_baudrate_t;

	/*!
	 *  \brief SERIAL parity type
	 *
	 *  Type for specifying the parity of the SERIAL module.
	 */
	typedef enum {
		ARTIK_SERIAL_PARITY_NONE,
		ARTIK_SERIAL_PARITY_ODD,
		ARTIK_SERIAL_PARITY_EVEN
	} artik_serial_parity_t;

	/*!
	 *  \brief SERIAL data bit length type
	 *
	 *  Type for specifying the data bit length of the SERIAL module.
	 */
	typedef enum {
		ARTIK_SERIAL_DATA_7BIT,
		ARTIK_SERIAL_DATA_8BIT
	} artik_serial_data_bits_t;

	/*!
	 *  \brief SERIAL bit stop type
	 *
	 *  Type for specifying the kind of bit stop of the SERIAL module.
	 */
	typedef enum {
		ARTIK_SERIAL_STOP_1BIT,
		ARTIK_SERIAL_STOP_2BIT
	} artik_serial_stop_bits_t;

	/*!
	 *  \brief SERIAL control type
	 *
	 *  Type for specifying the kind of control of the SERIAL module.
	 */
	typedef enum {
		ARTIK_SERIAL_FLOWCTRL_NONE,
		ARTIK_SERIAL_FLOWCTRL_HARD,
		ARTIK_SERIAL_FLOWCTRL_SOFT
	} artik_serial_flowcontrol_t;

	/*! \struct artik_serial_config
	 *  \brief SERIAL configuration structure
	 *
	 *  Structure containing the configuration elements
	 *  for a single requested SERIAL
	 */
	typedef struct {
		/*!
		 *  \brief Port number of the SERIAL in use.
		 */
		unsigned int port_num;
		/*!
		 *  \brief Friendly name for the SERIAL module.
		 */
		char *name;
		/*!
		 *  \brief Baudrate of the SERIAL module.
		 */
		artik_serial_baudrate_t baudrate;
		/*!
		 *  \brief Parity of the SERIAL module.
		 */
		artik_serial_parity_t parity;
		/*!
		 *  \brief Data bit length of the SERIAL module.
		 */
		artik_serial_data_bits_t data_bits;
		/*!
		 *  \brief Stop bit of the SERIAL module.
		 */
		artik_serial_stop_bits_t stop_bits;
		/*!
		 *  \brief Control of the SERIAL module.
		 */
		artik_serial_flowcontrol_t flowctrl;
		/*!
		 *  \brief Pointer to data for internal use by the API.
		 */
		void *data_user;

	} artik_serial_config;

	/*! \struct artik_serial_module
	 *
	 *  \brief SERIAL module operations
	 *
	 *  Structure containing all the exposed operations exposed
	 *  by the module to operate on a SERIAL instance
	 */
	typedef struct {
		/*!
		 *  \brief Request a SERIAL instance
		 *
		 *  \param[out] handle Handle tied to the requested Serial instance.
		 *  \param[in] config Configuration to apply to the requested SERIAL.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*request) (artik_serial_handle * handle,
				       artik_serial_config * config);
		/*!
		 *  \brief Release a SERIAL instance
		 *
		 *  \param[in] handle Handle tied to the requested Serial instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*release) (artik_serial_handle handle);
		/*!
		 *  \brief Read a message from a serial port through a SERIAL instance
		 *
		 *  \param[in] handle Handle tied to the requested Serial instance.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \param[out] buff Preallocated buffer filled by the function with the received data.
		 *  \param[in/out] len Maximum number of bytes to read. The function then
		 *                     fills up the variable with the number of bytes
		 *                     that were actually read.
		 *
		 *  \return S_OK on success, error code otherwise.
		 */
		artik_error(*read) (artik_serial_handle handle, unsigned char *buf,
				    int *len);
		/*!
		 *  \brief Write a message to a serial port through a SERIAL instance
		 *
		 *  \param[in] handle Handle tied to the requested Serial instance.
		 *                    This handle is returned by the \ref request function.
		 *  \param[in] msg Buffer containing data to write to the serial port.
		 *  \param[in/out] len Length of the data to write. The function
		 *                     then fills up the variable with the number of bytes
		 *                     that were actually written.
		 *
		 *  \return S_OK on success, error code otherwise.
		 */
		artik_error(*write) (artik_serial_handle handle, unsigned char *const msg,
				     int *len);
		/*!
		 *  \brief Wait for incoming data on a SERIAL instance
		 *
		 *  This function is blocking and will return only after data has been received
		 *  on the serial line
		 *
		 *  \param[in] handle Handle tied to the requested Serial instance to wait on.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \return S_OK if data is ready to be read, E_INTERRUPTED if the function has been
		 *                  interrupted manually by the \ref cancel_wait function or by a signal
		 *                  sent to the process.
		 */
		artik_error(*set_received_callback) (artik_serial_handle handle, artik_serial_callback callback, void *user_data);
		/*!
		 *  \brief Cancel a blocking wait on a SERIAL instance
		 *
		 *  This function cancels any pending blocking function call operated on the
		 *  SERIAL instance. The concerned functions will return E_INTERRUPTED when affected.
		 *
		 *  \param[in] handle Handle tied to the requested Serial instance.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \return S_OK on success, error code otherwise.
		 */
		artik_error(*unset_received_callback) (artik_serial_handle handle);

	} artik_serial_module;

	extern artik_serial_module serial_module;

#ifdef __cplusplus
}
#endif
#endif				/* __ARTIK_SERIAL_H__ */
