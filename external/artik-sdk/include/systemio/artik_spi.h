#ifndef INCLUDE_ARTIK_SPI_H_
#define INCLUDE_ARTIK_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

	/*! \file artik_spi.h
	 *
	 *  \brief SPI module definition
	 *
	 *  Definitions and functions for accessing
	 *  the SPI module and performing operations
	 *  on hardware chips
	 *
	 *  \example spi_test/artik_spi_test.c
	 */

	/*!
	 *  \brief SPI handle type
	 *
	 *  Handle type used to carry instance specific
	 *  information for a SPI object
	 */
	typedef void *artik_spi_handle;

	/*!
	 *  \brief spi mode type
	 *
	 *  Type for specifying the spi mode
	 */
	typedef enum {
		SPI_MODE0 = 0,
		SPI_MODE1,
		SPI_MODE2,
		SPI_MODE3,
		SPI_MODE_INVALID
	} artik_spi_mode;

	/*!
	 *  \brief SPI configuration structure
	 *
	 *  Structure containing the configuration elements
	 *  for a single requested SPI instance
	 */
	typedef struct {
		/*!
		 *  \brief Bus number of the SPI controller to request
		 */
		unsigned int bus;
		/*!
		 *  \brief Chip select number of the SPI controller to request
		 */
		unsigned int cs;
		/*!
		 *  \brief Mode of the SPI controller to request
		 */
		artik_spi_mode mode;
		/*!
		 *  \brief bits per word of the SPI controller to request
		 */
		unsigned int bits_per_word;
		/*!
		 *  \brief bits max speed of the SPI controller to request
		 */
		unsigned int max_speed;

	} artik_spi_config;

	/*! \struct artik_spi_module
	 *
	 *  \brief SPI module operations
	 *
	 *  Structure containing all the operations exposed
	 *  by the SPI module.
	 */
	typedef struct {
		/*!
		 *  \brief Request a SPI instance
		 *
		 *  \param[out] handle Handle tied to the requested SPI instance.
		 *  \param[in] config Configuration to apply to the requested SPI instance.
		 *
		 *  \return S_OK on success, error code otherwise.
		 */
		artik_error(*request) (artik_spi_handle * handle,
				       artik_spi_config * config);
		/*!
		 *  \brief Release a SPI instance
		 *
		 *  \param[in] handle Handle tied to the requested SPI instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*release) (artik_spi_handle handle);
		/*!
		 *  \brief Perform a read transaction over the SPI bus
		 *
		 *  \param[in] handle Handle tied to the requested SPI instance to be read.
		 *                    This handle is returned by the \ref request function.
		 *  \param[out] buffer Buffer filled with the data read from the SPI bus.
		 *  \param[in] lent Length of the buffer corresponding to the number of bytes to read.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*read) (artik_spi_handle handle, char *buffer,
				    int len);
		/*!
		 *  \brief Perform a write transaction over the SPI bus
		 *
		 *  \param[in] handle Handle tied to the requested SPI instance to write to.
		 *                    This handle is returned by the \ref request function.
		 *  \param[in] buffer Buffer containing the data to write to the SPI bus.
		 *  \param[in] len Length of the buffer corresponding to the number of bytes to write.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*write) (artik_spi_handle handle, char *buffer,
				     int len);
		/*!
		 *  \brief Perform a read/write transaction over the SPI bus
		 *
		 *  \param[in] handle Handle tied to the requested SPI instance to be read and write.
		 *                    This handle is returned by the \ref request function.
		 *  \param[in] tx_buf Buffer containing the data to send over the SPI bus.
		 *  \param[out] rx_buf Preallocated buffer filled with the data read during the SPI transaction.
		 *  \param[in] len Length in bytes of the transaction to perform. The TX and RX buffers
		 *                 must be preallocated to this minimum size.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*read_write) (artik_spi_handle handle, char *tx_buf,
					  char *rx_buf, int len);
	} artik_spi_module;

	extern const artik_spi_module spi_module;

#ifdef __cplusplus
}
#endif
#endif				/* INCLUDE_ARTIK_SPI_H_ */
