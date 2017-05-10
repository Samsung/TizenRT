#ifndef BINDINGS_SPI_ARTIK_SPI_HH_
#define BINDINGS_SPI_ARTIK_SPI_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_spi.h>

/*! \file artik_spi.hh
 *
 *  \brief C++ Wrapper to the SPI module
 *
 *  This is a class encapsulation of the C
 *  SPI module API \ref artik_spi.h
 */

namespace artik
{
/*!
 *  \brief Spi Module C++ Class
 */
class Spi
{
private:
	artik_spi_module* m_module;
	artik_spi_handle  m_handle;
	artik_spi_config  m_config;

public:
	Spi(unsigned int bus, unsigned int cs, artik_spi_mode mode,
	unsigned int bits_per_word, unsigned int speed);
	~Spi();

	artik_error request(void);
	artik_error release(void);
	artik_error read(char*, int);
	artik_error write(char*, int);
	artik_error read_write(char*, char*, int);
};
}

#endif /* BINDINGS_SPI_ARTIK_SPI_HH_ */
