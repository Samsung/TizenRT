#ifndef ARTIK_WIFI_HH_
#define ARTIK_WIFI_HH_

#include <artik_module.h>
#include <artik_wifi.h>

/*! \file artik_wifi.hh
 *  \brief C++ Wrapper to the Wi-Fi module
 *
 *  This is a class encapsulation of the C
 *  Wi-Fi module API \ref artik_wifi.h
 */

namespace artik
{
/*!
 *  \brief Wi-Fi module C++ Class
 */
class Wifi
{
private:
	artik_wifi_module* m_module;

public:
	Wifi();
	Wifi(Wifi const &);
	~Wifi();

	Wifi &operator=(Wifi const &);

public:
	artik_error disconnect();
	artik_error init(artik_wifi_mode_t mode);
	artik_error deinit();
	artik_error scan_request();
	artik_error get_scan_result(artik_wifi_ap** aps, int* num_aps);
	artik_error start_ap(const char *ssid, const char *passphrase, unsigned int channel);
	artik_error connect(const char* ssid, const char* password, bool persistent);
	artik_error set_scan_result_callback(artik_wifi_callback user_callback, void* user_data);
	artik_error set_connect_callback(artik_wifi_callback user_callback, void* user_data);
};
}
 
#endif /* ARTIK_WIFI_HH_ */
