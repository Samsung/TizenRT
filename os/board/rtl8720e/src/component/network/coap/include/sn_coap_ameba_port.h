#ifndef SN_COAP_AMEBA_PORT
#define SN_COAP_AMEBA_PORT

/** @addtogroup coap       COAP
 *  @ingroup    network
 *  @brief      mbed CoAP APIs and Ameba wrappers
 *  @{
 */

#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "platform_stdlib.h"
#include "osdep_service.h"
#include "wifi_constants.h"
#include "wifi_conf.h"
#include <lwipconf.h>
#include <stdint.h>

#include "sn_coap_protocol.h"

#define randLIB_seed_random()	NULL	/*!< Not used */

#define COAP_DBG_EN				0 		/*!< Flag to enable / disable CoAP debug message in function **tr_debug()** */
#define COAP_DBG_DETAIL			0		/*!< Flage to enable / disable detialed log in CoAP debug message in function **tr_debug()** */

/**
* \brief	print CoAP debug message
*/
#define tr_debug(fmat,...) \
do { \
    if(COAP_DBG_EN){ \
        printf(fmat "\n", ##__VA_ARGS__); \
        if(COAP_DBG_DETAIL){\
            printf("Function: %s  ::  Line: %d\n\n",__FUNCTION__,__LINE__);\
        }\
    } \
} while (0)


/**
* \brief	This function is called in **sn_coap_protocol_init()** for ramdom message ID
* \return 	A random 16bit unsigned value
*/
uint16_t randLIB_get_16bit(void);

/**
* \brief	 Ameba **malloc()** function wrapper
* \param	 size: memory size to be allocated
* \return	 NULL = if memory allocation failed
* \return	 void* pointer to allocated memory = if allocation success
*/
void *coap_malloc(uint16_t size);

/**
* \brief	Ameba **free()** function wrapper
* \param	*addr: memory address to be freed
*/
void coap_free(void *addr);

/**
* \brief	Ameba **calloc()** function wrapper, the effective result is the allocation of a zero-initialized memory block of *size* bytes
* \param	size: memory size to be allocated
* \return	NULL = if memory allocation failed
* \return	void* pointer to the allocated memory = if memory allocation success
*/
void *coap_calloc(uint32_t size);

/**
* \brief	Ameba **socket()** function wrapper, creates a **Datagrams type** i.e. *SOCK_DGRAM* socket
* \return	Non-negative Integer (socket file descriptor) = if successfully completion
* \return	-1 = if failed to create socket
*/
int coap_sock_open(void);

/**
* \brief	Ameba **close()** function wrapper, closes a socket file descriptor
* \param	sock_fd: the socket file descriptor is going to be closed.
* \return 	-1 = if close operation failed
* \return	0 = if close operation success
*/
int coap_sock_close(int socket_fd);

/**
* \brief	Ameba **sn_coap_protocol_init()** function wrapper, initializes CoAP Protocol part.
* \param	*coap_tx_callback : function callback pointer to tx function for sending coap messages
* \param	*coap_rx_callback : function callback pointer to rx function, used to return CoAP header struct with status COAP_STATUS_BUILDER_MESSAGE_SENDING_FAILED
* 			 when re-sendings exceeded. If set to NULL, no error message is returned.
* \return 	pointer to CoAP handle = if success
* \return 	NULL = if failed
*/
struct coap_s *coap_protocol_init(uint8_t (*coap_tx_callback)(uint8_t *, uint16_t, sn_nsdl_addr_s *, void *), int8_t (*coap_rx_callback)(sn_coap_hdr_s *,
								  sn_nsdl_addr_s *, void *));

/**
* \brief	Send the constructed CoAP message to designated host address on the specific port number
* \param	*to_address : the host / domain / ip address that the message is targeted to
* \param	port : port number that the host is used to receive the message
* \param	socket : socket file descriptor used to send the message
* \param	*coap_msg_hdr : constructed CoAP message pointer
* \return 	The number of bytes sent = if fuccess
* \return 	-1 = if failed
*/
int coap_send(char *to_address, uint16_t port, int socket, sn_coap_hdr_s *coap_msg_hdr);

/**
* \brief	Receive the constructed CoAP message from host
* \param 	socket : socket file descriptor used to receive the message
* \param 	*from_address : an internet address pointer, which the sender address is to be stored into
* \param 	*buf : memory buffer that used to store the received message
* \param 	buf_size : specifies the length in bytes of the buffer pointed to by the *buf* argument.
* \return 	The length of the message in bytes : if successful completion
* \return 	0 = If no messages are available to be received
* \return 	-1 = receive message failed
*/
int coap_recv(int socket, struct sockaddr_in *from_address, void *buf, uint32_t buf_size);

/**
* \brief	Print CoAP message header (for debug use)
* \param 	*parsed_hdr : pointer to constructed CoAP message header
*/
void coap_print_hdr(sn_coap_hdr_s *parsed_hdr);

/*\@}*/

#endif //SN_COAP_AMEBA_PORT
