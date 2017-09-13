/*
 * cloud_ping.h
 *
 *  Created on: 2017. 1. 16.
 *      Author: eunseok
 */

#ifndef OIC_CLOUD_PING_H_
#define OIC_CLOUD_PING_H_


typedef enum __PING_STATE__
{
    PING_ST_INIT        = 0,            // Ping initialization
    PING_ST_ISCLOUD     = 1,            // remote client is Cloud CIserver
    PING_ST_SIGNIN      = (1 << 1),     // Sign-IN complete
    PING_ST_TCPCONNECT  = (1 << 2),     // TCP session connected
    PING_ST_STARTTHREAD = (1 << 3),     // Ping-Thread created
    PING_ST_DISCOVERY   = (1 << 4),     // Doing discovery /oic/ping resource in Cloud
    PING_ST_REQUEST     = (1 << 5),     // Doing common-ping request to Cloud
    PING_ST_INTUPDATE   = (1 << 6),     // Doing interval-update request to Cloud
    PING_ST_INTUPDATED  = (1 << 7),     // Success interval-update request to Cloud
    PING_ST_TIMEOUT     = (1 << 8),     // Occurred Time-out with regard to any request
    PING_ALL_FLAG       = 0xFFFFFFFF
}PING_STATE;


bool OICPingInit(void);

bool OICPingTerminate(void);

bool OICPingSetMask(const char* remoteAddr, uint16_t port, PING_STATE state);

int OICPingGetMask(const char* remoteAddr, PING_STATE state);

bool OICPingUNSetMask(const char* remoteAddr, PING_STATE state);

bool OICPingSetCloudInterval( int64_t* intervals, int intEA );

bool OICPingSetInterval( const char* remoteAddr, int64_t* intervals, int intEA );

void CommandPing();	//SKKIM for ping command
#endif /* OIC_CLOUD_PING_H_ */
