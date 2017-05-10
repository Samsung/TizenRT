#ifndef  ARTIK_SECURITY_HH_
# define ARTIK_SECURITY_HH_

#include <string>

#include <artik_module.h>
#include <artik_security.h>
#include "artik_error.hh"

/*! \file artik_security.hh
 *
 *  \brief C++ Wrapper to the Security module
 *
 *  This is a class encapsulation of the C
 *  Security module API \ref artik_security.h
 */
namespace artik
{
	/*!
	 * \brief Security module C++ class
	 */
	class Security
	{
	private:
		artik_security_module	*m_module;
		artik_security_handle	m_handle;

	public:
		Security();
		~Security();

		artik_error get_certificate(char **);
		artik_error get_key_from_cert(const char *, char **);
		artik_error get_random_bytes(unsigned char*, int);
		artik_error get_certificate_sn(unsigned char*, unsigned int *);
	};

}

#endif	/*!ARTIK_SECURITY_HH_*/
