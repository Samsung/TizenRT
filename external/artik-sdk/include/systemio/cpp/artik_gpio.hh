#ifndef BINDINGS_CPP_ARTIK_GPIO_HH_
#define BINDINGS_CPP_ARTIK_GPIO_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_gpio.h>

/*! \file artik_gpio.hh
 *
 *  \brief C++ Wrapper to the GPIO module
 *
 *  This is a class encapsulation of the C
 *  GPIO module API \ref artik_gpio.h
 */

namespace artik
{
/*!
 *  \brief Gpio Module C++ Class
 */
class Gpio
{
private:
  artik_gpio_module* m_module;
  artik_gpio_handle  m_handle;
  artik_gpio_config  m_config;

public:
  Gpio(artik_gpio_id id, char* name, artik_gpio_dir_t dir, artik_gpio_edge_t edge, int initial_value);
  ~Gpio();

  artik_error request(void);
  artik_error release(void);
  int read(void);
  artik_error write(int value);
  char* get_name(void);
  artik_gpio_dir_t get_direction(void);
  artik_gpio_id get_id(void);
  artik_error set_change_callback(artik_gpio_callback, void*);
  void unset_change_callback();
};
}

#endif /* BINDINGS_CPP_ARTIK_GPIO_HH_ */
