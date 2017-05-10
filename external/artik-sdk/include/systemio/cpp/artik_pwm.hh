#ifndef ARTIK_PWM_HH_
#define ARTIK_PWM_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_pwm.h>

/*! \file artik_pwm.hh
 *  \brief C++ Wrapper to the PWM module
 *
 *  This is a class encapsulation of the C
 *  PWM module API \ref artik_pwm.h
 *  Example of usage see \ref artik_pwm_test.cpp
 */

namespace artik
{
/*!
 *  \brief Pwm Module C++ Class
 */
class Pwm
{
private:
	artik_pwm_module* m_module;
	artik_pwm_handle  m_handle;
	artik_pwm_config  m_config;

public:
	Pwm(unsigned int, char*, unsigned int, artik_pwm_polarity_t, unsigned int);
	Pwm(artik_pwm_config&);
	Pwm(Pwm const &);
	Pwm();
	~Pwm();

	Pwm &operator=(Pwm const &);

public:
	artik_error				release(void);
	artik_error				request(void);
	artik_error				enable(void);
	artik_error				disable(void);
	artik_error				set_period(unsigned int);
	artik_error				set_polarity(artik_pwm_polarity_t);
	artik_error				set_duty_cycle(unsigned int);

	unsigned int  			get_pin_num(void) const;
	char*					get_name(void) const;
	unsigned int			get_period(void) const;
	artik_pwm_polarity_t	get_polarity(void) const;
	unsigned int			get_duty_cycle(void) const;

	void					set_pin_num(unsigned int);
	void	      			set_name(char*);
};
}
 
#endif /* ARTIK_PWM_HH_ */
