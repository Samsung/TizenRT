#ifndef BINDINGS_CPP_ARTIK_HTTP_HH_
#define BINDINGS_CPP_ARTIK_HTTP_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_http.h>

/*! \file artik_http.hh
 *
 *  \brief C++ Wrapper to the HTTP module
 *
 *  This is a class encapsulation of the C
 *  HTTP module API \ref artik_http.h
 */

namespace artik
{
/*!
 *  \brief Http module C++ Class
 */
class Http
{
private:
  artik_http_module* m_module;

public:
  Http();
  ~Http();

  artik_error get(const char* url, artik_http_headers* headers, char** response, int *status, artik_ssl_config *ssl);
  artik_error post(const char* url, artik_http_headers* headers, const char* body, char** response, int *, artik_ssl_config *ssl);
  artik_error put(const char* url, artik_http_headers* headers, const char* body, char** response, int *status, artik_ssl_config *ssl);
  artik_error del(const char* url, artik_http_headers* headers, char** response, int *status, artik_ssl_config *ssl);
};

}

#endif /* BINDINGS_CPP_ARTIK_HTTP_HH_ */
