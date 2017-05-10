#ifndef BINDINGS_I2C_ARTIK_GPIO_HH_
#define BINDINGS_I2C_ARTIK_GPIO_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_i2c.h>

/*! \file artik_i2c.hh
 *
 *  \brief C++ Wrapper to the I2C module
 *
 *  This is a class encapsulation of the C
 *  I2C module API \ref artik_i2c.h
 */

namespace artik
{
/*!
 *  \brief I2c Module C++ Class
 */
class I2c
{
private:
  artik_i2c_module* m_module;
  artik_i2c_handle  m_handle;
  artik_i2c_config  m_config;

public:
  I2c(artik_i2c_id, int, artik_i2c_wordsize_t, unsigned int);
  ~I2c();

  artik_error request(void);
  artik_error release(void);
  artik_error read(char*, int);
  artik_error write(char*, int);
  artik_error read_register(unsigned int, char*, int);
  artik_error write_register(unsigned int, char*, int);
};
}

#endif /* BINDINGS_I2C_ARTIK_GPIO_HH_ */
