#ifndef INCLUDE_ARTIK_I2C_H_
#define INCLUDE_ARTIK_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

	/*! \file artik_i2c.h
	 *
	 *  \brief I2C module definition
	 *
	 *  Definitions and functions for accessing
	 *  the I2C module and performing operations
	 *  on hardware chips
	 *
	 *  \example i2c_test/artik_i2c_test.c
	 */

	/*!
	 *  \brief I2C handle type
	 *
	 *  Handle type used to carry instance specific
	 *  information for a I2C object
	 */
	typedef void *artik_i2c_handle;

	/*!
	 *  \brief I2C ID type
	 *
	 *  ID type used to specify a I2C controller
	 */
	typedef unsigned int artik_i2c_id;

	/*!
	 *  \brief I2C word size type
	 *
	 *  Type for specifying the word size of the I2C chip
	 */
	typedef enum {
		I2C_8BIT = 1,
		I2C_16BIT,
		I2C_WORDSIZE_INVALID
	} artik_i2c_wordsize_t;

	/*!
	 *  \brief I2C configuration structure
	 *
	 *  Structure containing the configuration elements
	 *  for a single requested I2C instance
	 */
	typedef struct {
		/*!
		 *  \brief ID of the I2C controller to request
		 */
		artik_i2c_id id;
		/*!
		 *  \brief maximum frequency allowed by the I2C chip to address
		 */
		int frequency;
		/*!
		 *  \brief Word size of the I2C chip to address
		 */
		artik_i2c_wordsize_t wordsize;
		/*!
		 *  \brief Address of the I2C chip to address
		 */
		unsigned char address;
	} artik_i2c_config;

	/*! \struct artik_i2c_module
	 *
	 *  \brief I2C module operations
	 *
	 *  Structure containing all the operations exposed
	 *  by the I2C module.
	 */
	typedef struct {
		/*!
		 *  \brief Request a I2C instance
		 *
		 *  \param[out] handle Handle tied to the requested I2C instance returned by the function.
		 *  \param[in] config Configuration to apply to the requested I2C instance.
		 *
		 *  \return S_OK on success, error code otherwise. The function returns E_BUSY if the
		 *          requested ID/address pair has already been requested
		 */
		artik_error(*request) (artik_i2c_handle * handle,
				       artik_i2c_config * config);
		/*!
		 *  \brief Release a I2C instance
		 *
		 *  \param[in] handle Handle tied to the requested I2C instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*release) (artik_i2c_handle handle);
		/*!
		 *  \brief Read from the I2C instance
		 *
		 *  \param[in] handle Handle tied to the requested I2C instance to be read.
		 *                    This handle is returned by the \ref request function.
		 *  \param[out] buffer Array to be filled with the data read from the I2C bus
		 *  \param[in] len Length of the array corresponding to the number of bytes to read
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*read) (artik_i2c_handle handle, char *buffer,
				    int len);
		/*!
		 *  \brief Write to the I2C instance
		 *
		 *  \param[in] handle Handle tied to the requested I2C instance to write to.
		 *                    This handle is returned by the \ref request function.
		 *  \param[in] buffer Array containing the data to write to the I2C bus
		 *  \param[in] len Length of the array corresponding to the number of bytes to write
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*write) (artik_i2c_handle handle, char *buffer,
				     int len);
		/*!
		 *  \brief Read a register from the I2C instance
		 *
		 *  \param[in] handle Handle tied to the requested I2C instance to be read.
		 *                    This handle is returned by the \ref request function.
		 *  \param[in] reg I2C internal register address to read the value from
		 *  \param[out] buffer Array to be filled with the data read from the register
		 *  \param[in] len Length of the array corresponding to the number of bytes to read
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*read_register) (artik_i2c_handle handle,
					     unsigned int reg, char *buffer,
					     int len);
		/*!
		 *  \brief Write to a register from the I2C instance
		 *
		 *  \param[in] handle Handle tied to the requested I2C instance to be read.
		 *                    This handle is returned by the \ref request function.
		 *  \param[in] reg I2C internal register address to write
		 *  \param[in] buffer Array containing the data to write to the register
		 *  \param[in] len Length of the array corresponding to the number of bytes to write
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*write_register) (artik_i2c_handle handle,
					      unsigned int reg, char *buffer,
					      int len);
	} artik_i2c_module;

	extern const artik_i2c_module i2c_module;

#ifdef __cplusplus
}
#endif
#endif				/* INCLUDE_ARTIK_I2C_H_ */
