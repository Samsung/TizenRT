
/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef __MEDIA_PLAYER_APP_WIFI_H
#define __MEDIA_PLAYER_APP_WIFI_H

#include <condition_variable>
#include <chrono>
#include <mutex>

namespace MediaPlayerApp
{
class WiFiConnector
{
public:
	static int connect(void);
	static int disconnect(void);

private:
	static void WIFI_CONNECTED_RESETFLAG(void)
	{
		sg_wifiIsConnected = false;
	}
	static bool WIFI_CONNECTED_CHECKFLAG(void)
	{
		return sg_wifiIsConnected;
	}

	static void WIFI_CONNECTED_NOTIFY(void)
	{
		std::lock_guard<std::mutex> lock(sg_wifiMutex);
		sg_wifiIsConnected = true;
		sg_wifiCondv.notify_one();
	}
	static void WIFI_CONNECTED_WAIT(void)
	{
		std::unique_lock<std::mutex> lock(sg_wifiMutex);
		sg_wifiCondv.wait_for(lock, WAIT_WIFI_TIMEOUT, [&] { return sg_wifiIsConnected; });
	}
	static bool sg_wifiIsConnected;
	static std::mutex sg_wifiMutex;
	static std::condition_variable sg_wifiCondv;
	static std::chrono::seconds WAIT_WIFI_TIMEOUT;
};
}
#endif
