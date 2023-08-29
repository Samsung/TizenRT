#ifndef _IPC_MESSAGE_QUEUE_H_
#define _IPC_MESSAGE_QUEUE_H_

#include "ipc_cores_cfg.h"


#define IMQ_WAIT_FOREVER	0xFFFFFFFF
/**********IPC Message Queue ID Configuration***********/

/** @defgroup ID
  * @{
  */
#define IMQ_RSVD_QUEUE_MAX		20
/**
  * @brief  IPC Message Queue Type Definition
  * @note	RSVD QUEUE ID are used for realtek
  */
typedef enum {
	QUEUE_KM4_TO_KR4 = 0,  // KM4 -> KR4
	QUEUE_KR4_TO_KM4 = 1,  // KR4 -> KM4
	QUEUE_KM4_TO_DSP = 2,  // KM4 -> DSP
	QUEUE_DSP_TO_KM4 = 3,  // DSP -> KM4
	QUEUE_KR4_TO_DSP = 4,  // KR4 -> DSP
	QUEUE_DSP_TO_KR4 = 5,  // DSP -> KR4

	QUEUE_RSVD_MAX	= IMQ_RSVD_QUEUE_MAX,
} RSVD_QUEUE_ID;


/**********IPC Message Queue Structure Definitions***********/

/**
  * @brief  IPC Message Queue Heap Structure Definition
  */
struct imq_queue_heap {
	u32 startBlock;   // heap start block address
	u32 remainFreeBytes;	// heap remain size
	u32 init;
	u32 rsvd;
};
typedef struct imq_queue_heap imq_queue_heap_t;

/**
  * @brief  IPC Message Queue Information Structure Definition
  */
struct imq_queue_info {
	u32 init;    // initialize flag
	u32 addr;	 // ringbuffer start address
	u32 len;	// ringbuffer length
	u32 dir;
	u32 type;
	u32 rsvd0[27];
	u32 wptr;	// write pointer
	u32 rsvd1[31];
	u32 rptr;	// read pointer
	u32 rsvd2[31];
};
typedef struct imq_queue_info imq_queue_info_t;


/************IPC Message Queue Error Code Enumeration*******************/

typedef enum {
	IMQ_OK	  =  0,
	IMQ_ERROR = -1,
	IMQ_INVALID_CH	= -2,
	IMQ_INVALID_SIZE = -3,
	IMQ_NO_MEMORY	= -4,
	IMQ_NOT_INIT	= -5,
	IMQ_TX_TIMEOUT	= -6,
	IMQ_RX_TIMEOUT	= -7,
	IMQ_TX_ERROR	= -8,
	IMQ_RX_ERROR	= -9,
	IMQ_INVALID_CALL	= -10,
	IMQ_NO_ENOUGH_BUF	= -11,
} imq_error_t;

/*****************************************************/

#define IPC_BYTE_ALIGNMENT(size) \
	if( ( size & IMQ_BYTE_ALIGNMENT_MASK ) != 0x00 ) { \
		size += ( IMQ_BYTE_ALIGNMENT - ( (size) & IMQ_BYTE_ALIGNMENT_MASK ) ); \
	} \

#define IPC_ADDR_ALIGNMENT(addr) \
	if( ( addr & IMQ_BYTE_ALIGNMENT_MASK ) != 0x00 ) { \
		addr = (addr >> 7) << 7;    \
	} \


#define _INFO_		2
#define _ERR_		1
#define _ALWAYS_	0

#define IMQ_DEBUG_ENABLE	1
#define IMQ_DEBUG_LEVEL		_ERR_
#if IMQ_DEBUG_ENABLE
#define imq_printf(level, fmt, arg...)     \
do {\
	if(level <= IMQ_DEBUG_LEVEL)	\
		printf(fmt, ##arg);\
}while(0)
#else
#define imq_printf(level, fmt, arg...)
#endif

//#define DEBUG_ASSERT

#ifdef  DEBUG_ASSERT
#define imq_assert_param(expr) ((expr) ? (void)0 : io_assert_failed((uint8_t *)__FUNCTION__, __LINE__))
#else
#define imq_assert_param(expr) ((void)0)
#endif
void ipc_app_init(void);

/* MESSAGE_QUEUE API */
/**
 * @brief  Initialize the ringbuffer message queue based on imq_rsvd_queue_config.
 * @param[in] None.
 * @return  IMQ_OK.
 */
imq_error_t IPC_Message_Queue_Init(void);

/**
 * @brief  Send buffer by ringbuffer message queue.
 * @param[in] ID: Ringbuffer message queue id, should be specified.
 * @param[in] buf: Send buffer.
 * @param[in] size: Send buffer size.
 * @param[in] timeout: Value IMQ_WAIT_FOREVER means wait forver.
 * @return  IMQ_OK will be returned if send success otherwise the error code is returned.
 */
imq_error_t IPC_Message_Queue_Send(u32 ID, u8 *buf, u32 size, int timeout);

/**
 * @brief  Receive buffer from ringbuffer message queue.
 * @param[in] ID: Ringbuffer message queue id, should be specified.
 * @param[in] buf: Receive buffer.
 * @param[in] size: IN: want receive buffer size, should be specified. OUT: if readable less than size, return readable size,
 * @return  IMQ_OK will be returned if receive success otherwise the error code is returned.
 */
imq_error_t IPC_Message_Queue_Recv(u32 ID, u8 *buf, u32 *size);

#endif
