#ifndef ARTIK_ERROR_HH_
#define ARTIK_ERROR_HH_

#include <artik_error.h>
#include <stdarg.h>

#if __cplusplus__ > 199711L
#define artik_throw nowexcept
#else
#define artik_throw throw
#endif

/*! \file artik_error.hh
 *
 *  \brief C++ handling of exceptions
 *
 */

namespace artik
{
/*!
 *  \brief Exception base class
 */
class ArtikException
{
private:
	int error;

public:
	ArtikException(int nb) artik_throw() : error(nb)  { this->what(); }
	ArtikException() artik_throw() : error(E_TRY_AGAIN)  { this->what(); }

	virtual ~ArtikException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return error_msg(this->error); }
};

/*!
 *  \brief Bad arguments exception class
 */
class ArtikBadArgsException : virtual public ArtikException
{
public:
	explicit ArtikBadArgsException() artik_throw() : ArtikException(E_BAD_ARGS)  {;}
	virtual ~ArtikBadArgsException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief Busy exception class
 */
class ArtikBusyException : virtual public ArtikException
{
public:
	explicit ArtikBusyException() artik_throw() : ArtikException(E_BUSY)  {;}
	virtual ~ArtikBusyException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief Not Initialized exception class
 */
class ArtikInitException : virtual public ArtikException
{
public:
	explicit ArtikInitException() artik_throw() : ArtikException(E_NOT_INITIALIZED)  {;}
	virtual ~ArtikInitException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief Out of memory exception class
 */
class ArtikNoMemException : virtual public ArtikException
{
public:
	explicit ArtikNoMemException() artik_throw() : ArtikException(E_NO_MEM)  {;}
	virtual ~ArtikNoMemException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief Not supported exception class
 */
class ArtikSupportException : virtual public ArtikException
{
public:
	explicit ArtikSupportException() artik_throw() : ArtikException(E_NOT_SUPPORTED)  {;}
	virtual ~ArtikSupportException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief Overflow exception class
 */
class ArtikOverflowException : virtual public ArtikException
{
public:
	explicit ArtikOverflowException() artik_throw() : ArtikException(E_OVERFLOW)  {;}
	virtual ~ArtikOverflowException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief Access denied exception class
 */
class ArtikDeniedException : virtual public ArtikException
{
public:
	explicit ArtikDeniedException() artik_throw() : ArtikException(E_ACCESS_DENIED)  {;}
	virtual ~ArtikDeniedException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief Interrupted exception class
 */
class ArtikInterruptException : virtual public ArtikException
{
public:
	explicit ArtikInterruptException() artik_throw() : ArtikException(E_INTERRUPTED)  {;}
	virtual ~ArtikInterruptException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief HTTP error exception class
 */
class ArtikHTTPException : virtual public ArtikException
{
public:
	explicit ArtikHTTPException() artik_throw() : ArtikException(E_HTTP_ERROR)  {;}
	virtual ~ArtikHTTPException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief Try again exception class
 */
class ArtikAgainException : virtual public ArtikException
{
public:
	explicit ArtikAgainException() artik_throw() : ArtikException(E_TRY_AGAIN)  {;}
	virtual ~ArtikAgainException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief Timeout arguments exception class
 */
class ArtikTimeException : virtual public ArtikException
{
public:
	explicit ArtikTimeException() artik_throw() : ArtikException(E_TIMEOUT)  {;}
	virtual ~ArtikTimeException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};

/*!
 *  \brief Bad value exception class
 */
class ArtikBadValException : virtual public ArtikException
{
public:
	explicit ArtikBadValException() artik_throw() : ArtikException(E_INVALID_VALUE)  {;}
	virtual ~ArtikBadValException() artik_throw() {;}

	virtual const char * what(void) artik_throw() { return this->ArtikException::what(); }
};
}

#endif //! ARTIK_ERROR_HH_
