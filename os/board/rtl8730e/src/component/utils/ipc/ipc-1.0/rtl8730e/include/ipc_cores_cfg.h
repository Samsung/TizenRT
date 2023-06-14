#ifndef _IPC_CORES_CFG_H_
#define _IPC_CORES_CFG_H_

#include "ameba_soc.h"
#include "osdep_service.h"

#include "inic_ipc.h"
#include "PinNames.h"


/**********Memory Address Configuration***********/


/** @defgroup Alignment
  * @{
  */
#define IMQ_BYTE_ALIGNMENT_MASK 			0x7F
#define IMQ_BYTE_ALIGNMENT					128

/***************************************************/


/**********Core Configuration***********************/

/** @defgroup Queue Type
  * @{
  */
#define MESSAGE_QUEUE			0x01	// packet with header, trx with ipc interrupt
#define SOCKET_QUEUE			0x02	// packet without header, trx with ipc interrpt
#define RINGBUFFER_QUEUE		0x04	// packet without header, trx without ipc interrupt

/** @defgroup Core Type
  * @{
  */
#define IMQ_CORE_NP	BIT(0)
#define IMQ_CORE_AP	BIT(1)
#define IMQ_CORE_LP	BIT(2)


#define IMQ_SEM_IDX		IPC_SEM_IMQ

#if defined(ARM_CORE_CM4)
#define IMQ_CORE_LOCK	IMQ_CORE_NP
#elif defined(ARM_CORE_CA7)
#define IMQ_CORE_LOCK	IMQ_CORE_AP
#else
#define IMQ_CORE_LOCK IMQ_CORE_LP
#endif

/** @defgroup Heap Index
  * @{
  */
#define IMQ_INFO_HEAP_IDX		0
#define IMQ_M_ENTRY_HEAP_IDX	1
#define IMQ_S_ENTRY_HEAP_IDX	2
#define SHARE_MEMORY_HEAP_IDX	3
/****************************************************/


/**********IPC Channel Configuration***********/

/** @defgroup ChanNum
  * @{
  */
#if defined(ARM_CORE_CM4)
#define IPC_DIR_IMQ_MSG_RX_0 IPC_AP_TO_NP
#define IPC_IMQ_RX_TRAN_0	IPC_A2N_IMQ_TRX_TRAN  /*!<  AP -->  NP  IPC Queue Message Exchange */
#else
#define IPC_DIR_IMQ_MSG_RX_0 IPC_NP_TO_AP
#define IPC_IMQ_RX_TRAN_0	IPC_N2A_IMQ_TRX_TRAN  /*!<  NP -->  AP  IPC Queue Message Exchange */
#endif

/****************************************************/


/**
  * @brief  IPC Message Queue Configration Structure Definition
  */
struct _IMQ_INIT_TABLE {
	uint32_t 	ID;
	uint8_t 	DIR;
	int32_t		RING_BUFFER_SIZE;
	uint8_t 	TYPE;
};
typedef struct _IMQ_INIT_TABLE IMQ_INIT_TABLE;

struct IPC_HeapRegion {
	uint32_t 	xStartAddress;
	uint32_t 	xSizeInBytes;
};
typedef struct IPC_HeapRegion IPC_HeapRegion_t;

extern const IMQ_INIT_TABLE imq_rsvd_queue_config[];
extern IPC_HeapRegion_t imq_info_region;
extern IPC_HeapRegion_t imq_heap_region;

#endif
