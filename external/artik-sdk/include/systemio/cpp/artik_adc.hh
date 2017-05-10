#ifndef ARTIK_ADC_HH_
#define ARTIK_ADC_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_adc.h>

/*! \file artik_adc.hh
 *  \brief C++ Wrapper to the ADC module
 *
 *  This is a class encapsulation of the C
 *  ADC module API \ref artik_adc.h
 *  Example of usage see \ref artik_adc_test.cpp
 */

namespace artik
{
/*!
 *  \brief Adc Module C++ Class
 */
class Adc
{
private:
	artik_adc_module* m_module;
	artik_adc_handle  m_handle;
	artik_adc_config  m_config;

public:
	Adc(unsigned int, char*);
	Adc(artik_adc_config&);
	Adc(Adc const &);
	Adc();
	~Adc();

	Adc &operator=(Adc const &);

public:
	artik_error		release(void);
	artik_error		request(void);
	artik_error		get_value(int*);

	unsigned int  	get_pin_num(void) const;
	char*			get_name(void) const;

	void			set_pin_num(unsigned int);
	void	      	set_name(char*);

};
}
 
#endif /* ARTIK_ADC_HH_ */
