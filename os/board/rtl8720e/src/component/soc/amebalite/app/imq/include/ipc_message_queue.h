#ifndef _IPC_MESSAGE_QUEUE_H_
#define _IPC_MESSAGE_QUEUE_H_

#include "ameba_soc.h"
#include "freertos_service.h"

#include "inic_ipc.h"
#include "PinNames.h"



/**********Memory Address Configuration***********/

#define IMQ_QUEUE_INFO_BASE_ADDR			0x60100000
#define IMQ_QUEUE_MASTER_BASE_ADDR			0x60100400
#define IMQ_QUEUE_SLAVE_BASE_ADDR			0x60140400
#define IMQ_QUEUE_END_ADDR					0x60180000-1
#define IMQ_QUEUE_MASTER_MAX_LEN			IMQ_QUEUE_SLAVE_BASE_ADDR - IMQ_QUEUE_MASTER_BASE_ADDR - 1
#define IMQ_QUEUE_SLAVE_MAX_LEN				IMQ_QUEUE_END_ADDR - IMQ_QUEUE_SLAVE_BASE_ADDR
#define IMQ_QUEUE_INFO_MAX_LEN				0x400

#define IMQ_SHARE_MEM_BASE_ADDR
#define IMQ_SHARE_MEM_MAX_LEN

#define IMQ_BYTE_ALIGNMENT_MASK 			0x1F
#define IMQ_BYTE_ALIGNMENT					32

/***************************************************/


/**********Core Configuration***********************/

#define IMQ_MASTER	0x00
#define IMQ_SLAVE	0x01

#define IMQ_CORE_KM4	BIT(0)
#define IMQ_CORE_KR4	BIT(1)
#define IMQ_CORE_DSP	BIT(2)
#define IMQ_CORE_CA7	BIT(3)

/****************************************************/



/**********Debug Configuration***********************/

#define IMQ_CHKSUM_DEBUG	0
#define IMQ_GPIO_MEAS		0

/****************************************************/


/**********IPC Message Queue IPC Channel Configuration***********/
#if defined(CONFIG_PLATFORM_AMEBALITE)
#define IPC_M2S_IMQ_TRX_TRAN	5  /*!<  Master -->  Slave  IPC Queue Message Exchange */
#define IPC_S2M_IMQ_TRX_TRAN	5  /*!<  Slave  -->  Master IPC Queue Message Exchange */
#elif defined(CONFIG_PLATFORM_AMEBAD2)
#define IPC_M2S_IMQ_TRX_TRAN	7  /*!<  Master -->  Slave  IPC Queue Message Exchange */
#define IPC_S2M_IMQ_TRX_TRAN	7  /*!<  Slave  -->  Master IPC Queue Message Exchange */
#endif
/****************************************************/


/**********IPC Message Queue ID Configuration***********/
#define IMQ_QUEUE_MAX		30

typedef enum {
	QUEUE_RSVD0 = 0,
	QUEUE_RSVD1 = 1,
	QUEUE_TEST	= 2,


	QUEUE_MAX	= IMQ_QUEUE_MAX,
} QUEUE_ID;

/*****************************************************/

/**********IPC Message Queue Structure Definitions***********/
typedef struct imq_queue_info imq_queue_info_t;

struct imq_queue_info {
	uint32_t m_init;    // master initialize flag
	uint32_t m_addr;	 // master share memory start address
	uint32_t m_len;		// master share memory length
	uint32_t m_wptr;	// master write pointer
	uint32_t m_rptr;	// master read pointer
	uint32_t s_init;    // slave initialize flag
	uint32_t s_addr;	 // slave share memory start address
	uint32_t s_len;		// slave share memory length
	uint32_t s_wptr;	// slave write pointer
	uint32_t s_rptr;	// slave read pointer
	uint32_t rsvd[6];
};

struct imq_message {
	uint32_t msg_len;
#if IMQ_CHKSUM_DEBUG
	uint32_t chksum;
#endif
};

typedef struct imq_message imq_message_t;

/*****************************************************/

/************Error Code Definitions*******************/

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
} imq_error_t;

/*****************************************************/


/**********Share Memory Structure Definitions***********/

struct imq_share_heap {
	uint32_t startBlock;   // heap start block address
	uint32_t endBlock;		// heap end block address
	uint32_t totalLen;			// heap total size
	uint32_t remainFreeBytes;	// heap remain size
	uint32_t lock;			// heap lock
	uint32_t init;
};

typedef struct imq_share_heap imq_share_heap_t;

struct imq_share_buffer_handle {
	uint32_t addr;
	uint32_t size;
	uint32_t fd;
};

struct imq_share_buffer {
	struct imq_share_buffer *pNextUsedBlock;  // block has been malloced
	struct imq_share_buffer *pNextFreeBlock;  // block is free, can be malloced
	uint32_t xBlockSize;
	uint32_t fd;
	uint32_t ref;
};

typedef struct imq_share_buffer imq_share_buffer_t;

/******************************************************/


#define IPC_BYTE_ALIGNMENT(size) \
	if( ( size & IMQ_BYTE_ALIGNMENT_MASK ) != 0x00 ) { \
		size += ( IMQ_BYTE_ALIGNMENT - ( (size) & IMQ_BYTE_ALIGNMENT_MASK ) ); \
	} \


imq_error_t IPC_Message_Queue_Master_Init(u8 ID, int sendbuf_size, int recvbuf_size);
imq_error_t IPC_Message_Queue_Slave_Init(u8 ID);
imq_error_t IPC_Message_Queue_Send(u8 ID, u8 *buf, int size);
imq_error_t IPC_Message_Queue_Send_Timeout(u8 ID, u8 *buf, int size, int timeout);
imq_error_t IPC_Message_Queue_Recv(u8 ID, u8 *buf, int *size);
imq_error_t IPC_Message_Queue_Recv_Timeout(u8 ID, u8 *buf, int *size, int timeout);

#endif
