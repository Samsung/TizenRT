#ifndef WSCLIENT_H
#define WSCLIENT_H
#include <websocket/libwsclient.h>

/****************Define if using the polarssl*******************/
#define USING_SSL

/******************Define the function used*********************/
#ifdef USING_SSL
int wss_set_fun_ops(wsclient_context *wsclient);
#define wsclient_set_fun_ops(wsclient) wss_set_fun_ops(wsclient)
#else
int ws_set_fun_ops(wsclient_context *wsclient);
#define wsclient_set_fun_ops(wsclient) ws_set_fun_ops(wsclient)
#endif
/***************************************************************/

/*************************************************************************************************
** Function Name  : create_wsclient
** Description    : Creating the websocket client context structure
** Input          : url:websocket server's url
**					port:websocket server's port, if not given, default 80 for "ws", 443 for "wss"
**					origin: the address or url of your self
**					buf_len: the length of tx/rx/received buffer. It determine the maximum bytes of data send and receive.
**					max_queue_size: max size of queue to buffer messages which are going to send to webserver.
** Return         : Created: websocket client context structure
**					Failed:  NULL
**************************************************************************************************/
wsclient_context *create_wsclient(char *url, int port, char *path, char *origin, int buf_len, int max_queue_size);

/*************************************************************************************************
** Function Name  : ws_connect_url
** Description    : Connecting to the websocket server
** Input          : wsclient: the websocket client context created by create_wsclientfunction
** Return         : Connected: the socket value
**					Failed:    -1
**************************************************************************************************/
int ws_connect_url(wsclient_context *wsclient);

/*************************************************************************************************
** Function Name  : ws_setsockopt_keepalive
** Description    : Set global value for KeepAlive socket option. These options will be set within ws_connect_url(), so the api must be invoked before ws_connect_url() or it would not take effect.
** Input          : keepalive_idle: value for TCP_KEEPIDLE option
**                  keepalive_interval: value for TCP_KEEPINTVL option
**                  keepalive_count: value for TCP_KEEPCNT option
** Return         : None
**************************************************************************************************/
void ws_setsockopt_keepalive(uint32_t keepalive_idle, uint32_t keepalive_interval, uint32_t keepalive_count);

/*************************************************************************************************
** Function Name  : ws_setsockopt_connect_timeout
** Description    : Set global value for RCVTO/SNDTO socket option. These options will be set within ws_connect_url() and take effect in connect process, so the api must be invoked before ws_connect_url() or it would not take effect.
** Input          : recv_timeout: value for SO_RCVTIMEO option
**                  send_timeout: value for SO_SNDTIMEO option
** Return         : None
**************************************************************************************************/
void ws_setsockopt_timeout(uint32_t recv_timeout, uint32_t send_timeout);

/*************************************************************************************************
** Function Name  : ws_send
** Description    : Create the sending string data and copy to queue
** Input          : message: the string that send to server(cannot exceeding the MAX_DATA_LEN
**					message_len: the length of the string
**					use_mask: 0/1; 1 means using mask for bynary
**					wsclient: the websocket client context
** Return         : 0:send message to queue successfully
					-1:fail to send message to queue
**************************************************************************************************/
int ws_send(char *message, int message_len, int use_mask, wsclient_context *wsclient);

/*************************************************************************************************
** Function Name  : ws_sendBinary
** Description    : Create the sending binary data and copy to queue
** Input          : message: the binary that send to server(cannot exceeding the MAX_DATA_LEN
**					message_len: the length of the binary
**					use_mask: 0/1; 1 means using mask for bynary
**					wsclient: the websocket client context
** Return         : 0:send message to queue successfully
					-1:fail to send message to queue
**************************************************************************************************/
int ws_sendBinary(uint8_t *message, int message_len, int use_mask, wsclient_context *wsclient);

/*************************************************************************************************
** Function Name  : ws_sendPing
** Description    : Sending Ping to websocket server
** Input          : use_mask: 0/1; 1 means using mask for bynary
**					wsclient: the websocket client context
** Return         : 0:send message to queue successfully
					-1:fail to send message to queue
**************************************************************************************************/
int ws_sendPing(int use_mask, wsclient_context *wsclient);

/*************************************************************************************************
** Function Name  : ws_poll
** Description    : Receicing data from server and send the data in tx_buf
** Input          : timeout(in milliseconds)
					wsclient: the websocket client context
** Return         : None
**************************************************************************************************/
void ws_poll(int timeout, wsclient_context **wsclient);

/*************************************************************************************************
** Function Name  : ws_dispatch
** Description    : callback function when getting message from server
** Input          : function that resolve the message received and the message length
** Return         : None
**************************************************************************************************/
void ws_dispatch(void (*callback)(wsclient_context **, int)) ;

/*************************************************************************************************
** Function Name  : ws_getReadyState
** Description    : Getting the connection status
** Input          : wsclient: the websocket client context
** Return         : readyStateValues(3 types:CLOSING, CLOSED, OPEN)
**************************************************************************************************/
readyStateValues ws_getReadyState(wsclient_context *wsclient);

/*************************************************************************************************
** Function Name  : ws_close
** Description    : Closing the connection with websocket server
** Input          : wsclient: the websocket client context
** Return         : None
**************************************************************************************************/
void ws_close(wsclient_context **wsclient);

/*************************************************************************************************
** Function Name  : ws_handshake_header_set_protocol
** Description    : set Sec-WebSocket-Protocol of handshake header
** Input          : wsclient: the websocket client context
**					pro: the protocol, eg: "chat, superchat"
**					len: length of string pro
** Return         : result(0:ok, -1:fail)
**************************************************************************************************/
int ws_handshake_header_set_protocol(wsclient_context *wsclient, char *pro, int len);

/*************************************************************************************************
** Function Name  : ws_handshake_header_set_version
** Description    : set Sec-WebSocket-Version of handshake header
** Input          : wsclient: the websocket client context
**					ver: the version, eg: "13"
**					len: length of string ver
** Return         : result(0:ok, -1:fail)
**************************************************************************************************/
int ws_handshake_header_set_version(wsclient_context *wsclient, char *ver, int len);

/*************************************************************************************************
** Function Name  : ws_handshake_header_custom_token
** Description    : add custom token to handshake header
** Input          : wsclient: the websocket client context
**					cus: the custom token, eg: "custom_token: value\r\n"
**					len: length of string cus
** Return         : result(0:ok, -1:fail)
**************************************************************************************************/
int ws_handshake_header_custom_token(wsclient_context *wsclient, char *cus, int len);

/*************************************************************************************************
** Function Name  : ws_multisend_opts
** Description    : set multi-send queue options. Called once after create_wsclient() and before ws_poll() started.
** Input          : wsclient: the websocket client context
**					stable_buf_num: the stable buffer number in send queue. This should be a positive value and
					                        should not exceed max_queue_size in create_wsclient(). If free buffers exceed
					                        the stable_buf_num, it will be dynamically free.
** Return         : result(0:ok, -1:fail)
**************************************************************************************************/
int ws_multisend_opts(wsclient_context *wsclient, int stable_buf_num);
#endif