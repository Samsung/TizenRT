#ifndef ARTIK_SERIAL_HH_
#define ARTIK_SERIAL_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_serial.h>

/*! \file artik_serial.hh
 *  \brief C++ Wrapper to the SERIAL module
 *
 *  This is a class encapsulation of the C
 *  SERIAL module API \ref artik_serial.h
 */

namespace artik
{
/*!
 *  \brief Serial Module C++ Class
 */
class Serial
{
private:
	artik_serial_module* m_module;
	artik_serial_handle  m_handle;
	artik_serial_config  m_config;

public:
	Serial(unsigned int, char*, artik_serial_baudrate_t, artik_serial_parity_t,
		artik_serial_data_bits_t, artik_serial_stop_bits_t,
		artik_serial_flowcontrol_t );
	Serial(artik_serial_config&);
	Serial(Serial const &);
	Serial();
	~Serial();

	Serial &operator=(Serial const &);

public:
	artik_error					release(void);
	artik_error					request(void);
	artik_error					read(unsigned char*, int*);
	artik_error					write(unsigned char*, int*);
	artik_error					set_received_callback(artik_serial_callback, void *);
	artik_error					unset_received_callback(void);

	unsigned int  				get_port_num(void) const;
	char*						get_name(void) const;
	artik_serial_baudrate_t		get_baudrate(void) const;
	artik_serial_parity_t		get_parity(void) const;
	artik_serial_data_bits_t			get_data_bits(void) const;
	artik_serial_stop_bits_t			get_stop_bits(void) const;
	artik_serial_flowcontrol_t	get_flowctrl(void) const;

	void						set_port_num(unsigned int);
	void	      				set_name(char*);
	void       					set_baudrate(artik_serial_baudrate_t);
	void       	     			set_parity(artik_serial_parity_t);
	void       	       			set_data_bits(artik_serial_data_bits_t);
	void       	       			set_stop_bits(artik_serial_stop_bits_t);
	void	      	       		set_flowctrl(artik_serial_flowcontrol_t);
};
}

#endif /* ARTIK_SERIAL_HH_ */
