#ifndef BINDINGS_CPP_ARTIK_NETWORK_HH_
#define BINDINGS_CPP_ARTIK_NETWORK_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_network.h>

/*! \file artik_network.hh
 *
 *  \brief C++ Wrapper to the Network module
 *
 *  This is a class encapsulation of the C
 *  Network module API \ref artik_network.h
 */

namespace artik
{
/*!
 *  \brief Network module C++ Class
 */
class Network
{
private:
  artik_network_module* m_module;
  artik_network_dhcp_client_handle m_dhcp_client_handle;
  artik_network_dhcp_server_handle m_dhcp_server_handle;

public:
  Network();
  ~Network();

  artik_error get_current_public_ip(artik_network_ip* ip);
  artik_error dhcp_client_start(artik_network_interface_t interface);
  artik_error dhcp_client_stop();
  artik_error dhcp_server_start(artik_network_dhcp_server_config* config);
  artik_error dhcp_server_stop();
  artik_error get_online_status(bool* online_status);
  artik_error add_watch_online_status(watch_online_status_handle* handle,
                                      watch_online_status_callback app_callback,
                                      void *user_data);
  artik_error remove_watch_online_status(watch_online_status_handle handle);

};
}

#endif /* BINDINGS_CPP_ARTIK_NETWORK_HH_ */
