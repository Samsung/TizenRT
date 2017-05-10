#ifndef BINDINGS_CPP_ARTIK_CLOUD_HH_
#define BINDINGS_CPP_ARTIK_CLOUD_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_cloud.h>

/*! \file artik_cloud.hh
 *
 *  \brief C++ Wrapper to the Cloud module
 *
 *  This is a class encapsulation of the C
 *  Cloud module API \ref artik_cloud.h
 */

namespace artik
{
/*!
 *  \brief Cloud module C++ Class
 */
class Cloud
{
private:
  artik_cloud_module*		m_module;
  char*						m_token;
  artik_websocket_handle	m_ws_handle;

public:
  Cloud(const char* token);
  ~Cloud();

  artik_error send_message(const char* device_id, const char* message, char **response);
  artik_error send_action(const char* device_id, const char* action, char **response);
  artik_error get_current_user_profile(char **response);
  artik_error get_user_devices(int count, bool properties, int offset, const char *user_id, char **response);
  artik_error get_user_device_types(int count, bool shared, int offset, const char *user_id, char **response);
  artik_error get_user_application_properties(const char *user_id, const char *app_id, char **response);
  artik_error get_device(const char *device_id, bool properties, char **response);
  artik_error get_device_token(const char *device_id, char **response);
  artik_error add_device(const char *user_id, const char *dt_id, const char *name, char **response);
  artik_error update_device_token(const char *device_id, char **response);
  artik_error delete_device_token(const char *device_id, char **response);
  artik_error delete_device(const char *device_id, char **response);
  artik_error sdr_start_registration(const char* device_type_id, const char* vendor_id, char **response);
  artik_error sdr_registration_status(const char* reg_id, char **response);
  artik_error sdr_complete_registration(const char* reg_id, const char* reg_nonce, char **response);
  artik_error websocket_open_stream(const char *access_token, const char *device_id, bool use_se);
  artik_error websocket_send_message(char *message);
  artik_error websocket_set_connection_callback(artik_websocket_callback callback, void *user_data);
  artik_error websocket_set_receive_callback(artik_websocket_callback callback, void *user_data);
  artik_error websocket_close_stream();
};
}

#endif /* BINDINGS_CPP_ARTIK_CLOUD_HH_ */
