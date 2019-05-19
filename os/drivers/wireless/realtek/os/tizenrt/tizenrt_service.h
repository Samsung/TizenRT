#ifndef _CUSTOMER_RTOS_SERVICE_H_
#define _CUSTOMER_RTOS_SERVICE_H_

//----- ------------------------------------------------------------------
// Include Files
//----- ------------------------------------------------------------------

#include "../../platform/dlist.h"



#include <sys/types.h>
#include <stdio.h>
#include <threads.h>
#include <signal.h>
#include <sched.h>
#include <queue.h>
#include <semaphore.h>
#include <tinyara/wqueue.h>
#include <tinyara/clock.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/time.h>
#include <arch/irq.h>
#include <tinyara/kmalloc.h>
#include <string.h>



//#include <os/arch/arm/src/imxrt/imxrt_config.h>

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

#include "../../rtk/src/osdep/wireless.h"
#include "../../rtk/include/autoconf_tizenrt.h"




#if (defined CONFIG_GSPI_HCI || defined CONFIG_SDIO_HCI) || defined(CONFIG_LX_HCI)
/* For SPI interface transfer and us delay implementation */
#if !defined(CONFIG_PLATFORM_8195A) && !defined(CONFIG_PLATFORM_8711B) && !defined(CONFIG_PLATFORM_8721D) && !defined(CONFIG_PLATFORM_8195BHP) && !defined(CONFIG_PLATFORM_8710C) && !defined(CONFIG_PLATFORM_TIZENRT)
#include <rtwlan_bsp.h>	
#endif
#endif

#ifdef CONFIG_USB_HCI
typedef struct urb *  PURB;
#endif

//----- ------------------------------------------------------------------
// Move from basic_types.h
//----- ------------------------------------------------------------------

//#define PLATFORM_FREERTOS
#include <stdint.h>

#define PLATFORM_LITTLE_ENDIAN                  0
#define PLATFORM_BIG_ENDIAN                     1

#define SYSTEM_ENDIAN                           PLATFORM_LITTLE_ENDIAN


#undef _FAIL
#define _FAIL		0

#undef _SUCCESS
#define _SUCCESS	1


#ifdef CONFIG_PLATFORM_8195A
#define SUCCESS	0
#define FAIL	(-1)

#undef _SUCCESS
#define _SUCCESS	1
#endif



#ifndef FALSE		
    #define FALSE   0
#endif

#ifndef TRUE
    #define TRUE    (!FALSE)
#endif
		
#define _TRUE        TRUE	
#define _FALSE	     FALSE	

#ifndef NULL
#define NULL 0
#endif

#ifdef __GNUC__
#define __weak                  __attribute__((weak))
#define likely(x)               __builtin_expect ((x), 1)
#define unlikely(x)             __builtin_expect ((x), 0)
#endif

typedef unsigned int	        uint;
typedef	signed int		        sint;

#ifdef __ICCARM__
typedef signed long long        __int64_t;
typedef unsigned long long      __uint64_t;
#endif

#define s8                      int8_t
#define u8                      uint8_t
#define s16                     int16_t
#define u16                     uint16_t
#define s32                     int32_t
#define u32                     uint32_t
#define s64                     int64_t
#define u64                     uint64_t

#ifdef CONFIG_MBED_ENABLED
typedef unsigned int            BOOL;
#else
#ifndef BOOL
typedef unsigned char           BOOL;
#endif
#ifndef bool
typedef unsigned char    bool;
#endif
#ifndef __cplusplus
#ifndef bool
typedef unsigned char           bool;
#endif
#endif
#endif

#define UCHAR                   uint8_t
#define USHORT                  uint16_t
#define UINT                    uint32_t
#define ULONG                   uint32_t	

//typedef struct { volatile int counter; } atomic_t;

typedef enum _RTK_STATUS_ {
    _EXIT_SUCCESS = 0,
    _EXIT_FAILURE = 1
}RTK_STATUS, *PRTK_STATUS;

#define IN
#define OUT
#define VOID void
#define INOUT
#define NDIS_OID uint
#define NDIS_STATUS uint

#ifndef	PVOID
typedef void * PVOID;
#endif

typedef     u32 dma_addr_t;

typedef     void (*proc_t)(void*);


typedef     int __kernel_ssize_t;



typedef 	__kernel_size_t	SIZE_T;	
typedef	    __kernel_ssize_t	SSIZE_T;
#define     FIELD_OFFSET(s,field)	((SSIZE_T)&((s*)(0))->field)

#define MEM_ALIGNMENT_OFFSET	(sizeof (SIZE_T))
#define MEM_ALIGNMENT_PADDING	(sizeof(SIZE_T) - 1)

#define SIZE_PTR SIZE_T
#define SSIZE_PTR SSIZE_T

#ifndef ON
#define   ON        1
#endif

#ifndef OFF
#define   OFF       0
#endif

#ifdef CONFIG_PLATFORM_8195A
#ifndef ENABLE
#define   ENABLE    1
#endif

#ifndef DISABLE
#define   DISABLE   0
#endif
#endif

#define BIT0	0x0001
#define BIT1	0x0002
#define BIT2	0x0004
#define BIT3	0x0008
#define BIT4	0x0010
#define BIT5	0x0020
#define BIT6	0x0040
#define BIT7	0x0080
#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000
#define BIT16	0x00010000
#define BIT17	0x00020000
#define BIT18	0x00040000
#define BIT19	0x00080000
#define BIT20	0x00100000
#define BIT21	0x00200000
#define BIT22	0x00400000
#define BIT23	0x00800000
#define BIT24	0x01000000
#define BIT25	0x02000000
#define BIT26	0x04000000
#define BIT27	0x08000000
#define BIT28	0x10000000
#define BIT29	0x20000000
#define BIT30	0x40000000
#define BIT31	0x80000000

#define BIT_(__n)       (1<<(__n))

#ifndef BIT
#define BIT(__n)       (1<<(__n))
#endif

#if defined (__ICCARM__)
#define STRINGIFY(s) #s
#define SECTION(_name) _Pragma( STRINGIFY(location=_name))
#define ALIGNMTO(_bound) _Pragma( STRINGIFY(data_alignment=_bound))
#define _PACKED_       __packed
#define _LONG_CALL_
#define _LONG_CALL_ROM_  
#define _WEAK          __weak
#else
#define SECTION(_name) __attribute__ ((__section__(_name)))
#define ALIGNMTO(_bound) __attribute__ ((aligned (_bound)))
#define _PACKED_       __attribute__ ((packed))
#ifdef CONFIG_RELEASE_BUILD_LIBRARIES
#define _LONG_CALL_
#define _LONG_CALL_ROM_     __attribute__ ((long_call))
#ifdef E_CUT_ROM_DOMAIN
#undef _LONG_CALL_ROM_
#define _LONG_CALL_ROM_
#endif
#else
#define _LONG_CALL_     __attribute__ ((long_call))
#define _LONG_CALL_ROM_     _LONG_CALL_
#endif
#define _WEAK           __attribute__ ((weak))
#endif

//port from fw by thomas
// TODO: Belows are Sync from SD7-Driver. It is necessary to check correctness

#define SWAP32(x) ((u32)(                         \
 			(((u32)(x) & (u32)0x000000ff) << 24) |            \
 			(((u32)(x) & (u32)0x0000ff00) <<  8) |            \
 			(((u32)(x) & (u32)0x00ff0000) >>  8) |            \
 			(((u32)(x) & (u32)0xff000000) >> 24)))
 
#define WAP16(x) ((u16)(                         \
 			(((u16)(x) & (u16)0x00ff) <<  8) |            \
 			(((u16)(x) & (u16)0xff00) >>  8)))

#if SYSTEM_ENDIAN == PLATFORM_LITTLE_ENDIAN
#ifndef rtk_le16_to_cpu
#define rtk_cpu_to_le32(x)		((u32)(x))
#define rtk_le32_to_cpu(x)		((u32)(x))
#define rtk_cpu_to_le16(x)		((u16)(x))
#define rtk_le16_to_cpu(x)		((u16)(x))
#define rtk_cpu_to_be32(x)		SWAP32((x))
#define rtk_be32_to_cpu(x)		SWAP32((x))
#define rtk_cpu_to_be16(x)		WAP16((x))
#define rtk_be16_to_cpu(x)		WAP16((x))
#endif

#elif SYSTEM_ENDIAN == PLATFORM_BIG_ENDIAN
#ifndef rtk_le16_to_cpu
#define rtk_cpu_to_le32(x)		SWAP32((x))
#define rtk_le32_to_cpu(x)		SWAP32((x))
#define rtk_cpu_to_le16(x)		WAP16((x))
#define rtk_le16_to_cpu(x)		WAP16((x))
#define rtk_cpu_to_be32(x)		((__u32)(x))
#define rtk_be32_to_cpu(x)		((__u32)(x))
#define rtk_cpu_to_be16(x)		((__u16)(x))
#define rtk_be16_to_cpu(x)		((__u16)(x))
#endif
#endif


/*
 *	Call endian free function when
 *		1. Read/write packet content.
 *		2. Before write integer to IO.
 *		3. After read integer from IO.
*/

//
// Byte Swapping routine.
//
#define EF1Byte	(u8)
#define EF2Byte 	le16_to_cpu
#define EF4Byte	le32_to_cpu

//
// Read LE format data from memory
//
#define ReadEF1Byte(_ptr)		EF1Byte(*((u8 *)(_ptr)))
#define ReadEF2Byte(_ptr)		EF2Byte(*((u16 *)(_ptr)))
#define ReadEF4Byte(_ptr)		EF4Byte(*((u32 *)(_ptr)))

//
// Write LE data to memory
//
#define WriteEF1Byte(_ptr, _val)	(*((u8 *)(_ptr)))=EF1Byte(_val)
#define WriteEF2Byte(_ptr, _val)	(*((u16 *)(_ptr)))=EF2Byte(_val)
#define WriteEF4Byte(_ptr, _val)	(*((u32 *)(_ptr)))=EF4Byte(_val)									

//
//	Example:
//		BIT_LEN_MASK_32(0) => 0x00000000
//		BIT_LEN_MASK_32(1) => 0x00000001
//		BIT_LEN_MASK_32(2) => 0x00000003
//		BIT_LEN_MASK_32(32) => 0xFFFFFFFF
//
#define BIT_LEN_MASK_32(__BitLen) \
	(0xFFFFFFFF >> (32 - (__BitLen)))
//
//	Example:
//		BIT_OFFSET_LEN_MASK_32(0, 2) => 0x00000003
//		BIT_OFFSET_LEN_MASK_32(16, 2) => 0x00030000
//
#define BIT_OFFSET_LEN_MASK_32(__BitOffset, __BitLen) \
	(BIT_LEN_MASK_32(__BitLen) << (__BitOffset)) 

//
//	Description:
//		Return 4-byte value in host byte ordering from
//		4-byte pointer in litten-endian system.
//
#define LE_P4BYTE_TO_HOST_4BYTE(__pStart) \
	(EF4Byte(*((u32 *)(__pStart))))

//
//	Description:
//		Translate subfield (continuous bits in little-endian) of 4-byte value in litten byte to
//		4-byte value in host byte ordering.
//
#define LE_BITS_TO_4BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		( LE_P4BYTE_TO_HOST_4BYTE(__pStart) >> (__BitOffset) ) \
		& \
		BIT_LEN_MASK_32(__BitLen) \
	)

//
//	Description:
//		Mask subfield (continuous bits in little-endian) of 4-byte value in litten byte oredering  
//		and return the result in 4-byte value in host byte ordering.
//
#define LE_BITS_CLEARED_TO_4BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		LE_P4BYTE_TO_HOST_4BYTE(__pStart) \
		& \
		( ~ BIT_OFFSET_LEN_MASK_32(__BitOffset, __BitLen) ) \
	)

//
//	Description:
//		Set subfield of little-endian 4-byte value to specified value.	
//
#define SET_BITS_TO_LE_4BYTE(__pStart, __BitOffset, __BitLen, __Value) \
	*((u32 *)(__pStart)) = \
		EF4Byte( \
			LE_BITS_CLEARED_TO_4BYTE(__pStart, __BitOffset, __BitLen) \
			| \
			( (((u32)__Value) & BIT_LEN_MASK_32(__BitLen)) << (__BitOffset) ) \
		);

		
#define BIT_LEN_MASK_16(__BitLen) \
		(0xFFFF >> (16 - (__BitLen)))
		
#define BIT_OFFSET_LEN_MASK_16(__BitOffset, __BitLen) \
	(BIT_LEN_MASK_16(__BitLen) << (__BitOffset))
	
#define LE_P2BYTE_TO_HOST_2BYTE(__pStart) \
	(EF2Byte(*((u16 *)(__pStart))))
	
#define LE_BITS_TO_2BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		( LE_P2BYTE_TO_HOST_2BYTE(__pStart) >> (__BitOffset) ) \
		& \
		BIT_LEN_MASK_16(__BitLen) \
	)
	
#define LE_BITS_CLEARED_TO_2BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		LE_P2BYTE_TO_HOST_2BYTE(__pStart) \
		& \
		( ~ BIT_OFFSET_LEN_MASK_16(__BitOffset, __BitLen) ) \
	)

#define SET_BITS_TO_LE_2BYTE(__pStart, __BitOffset, __BitLen, __Value) \
	*((u16 *)(__pStart)) = \
		EF2Byte( \
			LE_BITS_CLEARED_TO_2BYTE(__pStart, __BitOffset, __BitLen) \
			| \
			( (((u16)__Value) & BIT_LEN_MASK_16(__BitLen)) << (__BitOffset) ) \
		);
			
#define BIT_LEN_MASK_8(__BitLen) \
		(0xFF >> (8 - (__BitLen)))

#define BIT_OFFSET_LEN_MASK_8(__BitOffset, __BitLen) \
	(BIT_LEN_MASK_8(__BitLen) << (__BitOffset))

#define LE_P1BYTE_TO_HOST_1BYTE(__pStart) \
	(EF1Byte(*((u8 *)(__pStart))))

#define LE_BITS_TO_1BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		( LE_P1BYTE_TO_HOST_1BYTE(__pStart) >> (__BitOffset) ) \
		& \
		BIT_LEN_MASK_8(__BitLen) \
	)

#define LE_BITS_CLEARED_TO_1BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		LE_P1BYTE_TO_HOST_1BYTE(__pStart) \
		& \
		( ~BIT_OFFSET_LEN_MASK_8(__BitOffset, __BitLen) ) \
	)

#define SET_BITS_TO_LE_1BYTE(__pStart, __BitOffset, __BitLen, __Value) \
	*((u8 *)(__pStart)) = \
		EF1Byte( \
			LE_BITS_CLEARED_TO_1BYTE(__pStart, __BitOffset, __BitLen) \
			| \
			( (((u8)__Value) & BIT_LEN_MASK_8(__BitLen)) << (__BitOffset) ) \
		);

//pclint
#define LE_BITS_CLEARED_TO_1BYTE_8BIT(__pStart, __BitOffset, __BitLen) \
	( \
		LE_P1BYTE_TO_HOST_1BYTE(__pStart) \
	)

//pclint
#define SET_BITS_TO_LE_1BYTE_8BIT(__pStart, __BitOffset, __BitLen, __Value) \
{ \
	*((pu1Byte)(__pStart)) = \
		EF1Byte( \
			LE_BITS_CLEARED_TO_1BYTE_8BIT(__pStart, __BitOffset, __BitLen) \
			| \
			((u1Byte)__Value) \
		); \
}

// Get the N-bytes aligment offset from the current length
#define N_BYTE_ALIGMENT(__Value, __Aligment) ((__Aligment == 1) ? (__Value) : (((__Value + __Aligment - 1) / __Aligment) * __Aligment))

typedef unsigned char	BOOLEAN,*PBOOLEAN,boolean;

#define TEST_FLAG(__Flag,__testFlag)		(((__Flag) & (__testFlag)) != 0)
#define SET_FLAG(__Flag, __setFlag)			((__Flag) |= __setFlag)
#define CLEAR_FLAG(__Flag, __clearFlag)		((__Flag) &= ~(__clearFlag))
#define CLEAR_FLAGS(__Flag)					((__Flag) = 0)
#define TEST_FLAGS(__Flag, __testFlags)		(((__Flag) & (__testFlags)) == (__testFlags))

/* Define compilor specific symbol */
//
// inline function
//

#if defined ( __ICCARM__ )
#define __inline__                      inline
#define __inline                        inline
#define __inline_definition			//In dialect C99, inline means that a function's definition is provided 
								//only for inlining, and that there is another definition 
								//(without inline) somewhere else in the program. 
								//That means that this program is incomplete, because if 
								//add isn't inlined (for example, when compiling without optimization), 
								//then main will have an unresolved reference to that other definition.

								// Do not inline function is the function body is defined .c file and this 
								// function will be called somewhere else, otherwise there is compile error
#elif defined ( __CC_ARM   )
#define __inline__			__inline	//__linine__ is not supported in keil compilor, use __inline instead
#define inline				__inline
#define __inline_definition			// for dialect C99
#elif defined   (  __GNUC__  )
#define __inline__                      inline
#define __inline                        inline
#define __inline_definition		inline
#endif

//
// pack
//

#if defined (__ICCARM__)

#define RTW_PACK_STRUCT_BEGIN _Pragma( STRINGIFY(pack(1)))
#define RTW_PACK_STRUCT_STRUCT 
#define RTW_PACK_STRUCT_END _Pragma( STRINGIFY(pack()))
//#define RTW_PACK_STRUCT_USE_INCLUDES

#elif defined (__CC_ARM)

#define RTW_PACK_STRUCT_BEGIN __packed
#define RTW_PACK_STRUCT_STRUCT 
#define RTW_PACK_STRUCT_END

#elif defined (__GNUC__)

#define RTW_PACK_STRUCT_BEGIN
#define RTW_PACK_STRUCT_STRUCT __attribute__ ((__packed__))
#define RTW_PACK_STRUCT_END

#elif defined(PLATFORM_WINDOWS)

#define RTW_PACK_STRUCT_BEGIN
#define RTW_PACK_STRUCT_STRUCT 
#define RTW_PACK_STRUCT_END
#define RTW_PACK_STRUCT_USE_INCLUDES
#endif

// for standard library
#ifdef __ICCARM__
#define __extension__		/* Ignore */
#define	__restrict			/* Ignore */
#endif

typedef struct _RAM_START_FUNCTION_ {
    VOID (*RamStartFun) (VOID);
}RAM_START_FUNCTION, *PRAM_START_FUNCTION;

typedef struct _RAM_FUNCTION_START_TABLE_ {
    VOID (*RamStartFun) (VOID);
    VOID (*RamWakeupFun) (VOID);
    VOID (*RamPatchFun0) (VOID);
    VOID (*RamPatchFun1) (VOID);
    VOID (*RamPatchFun2) (VOID);
}RAM_FUNCTION_START_TABLE, *PRAM_FUNCTION_START_TABLE;


// --------------------------------------------
//	Platform dependent type define
// --------------------------------------------

#define FIELD_OFFSET(s,field)	((SSIZE_T)&((s*)(0))->field)

// os types
typedef char			    osdepCHAR;
typedef float			    osdepFLOAT;
typedef double			    osdepDOUBLE;
typedef long			    osdepLONG;
typedef short			    osdepSHORT;
typedef unsigned long	    osdepSTACK_TYPE;
typedef long			    osdepBASE_TYPE;
typedef unsigned long	    osdepTickType;

//lock and semaphore
 typedef pthread_mutex_t* _lock;
 typedef sem_t* _sema;
 typedef pthread_mutex_t* _mutex;


typedef void*	            _timerHandle;

typedef void*	            _queueHandle;
typedef void*	            _xqueue;
typedef struct timer_list	_timer;

typedef  int (*task_fun)(int argc, char *argv[]);



#define _SUCCESS	1
#define _FAIL		0
#define VOID void

typedef	struct sk_buff		_pkt;
typedef unsigned char		_buffer;
typedef unsigned int        systime;

#ifndef __LIST_H
#warning "DLIST_NOT_DEFINE!!!!!!"
struct list_head {
	struct list_head *next, *prev;
};
#endif

struct	__queue	{
	struct	list_head	queue;
	_lock			lock;
};

typedef struct	__queue		_queue;
typedef struct	list_head	_list;
typedef unsigned long		_irqL;

typedef void*			    _thread_hdl_;
typedef void			    thread_return;
typedef void*			    thread_context;

#if !defined(CONFIG_PLATFORM_8710C)
typedef struct { volatile int counter; } atomic_t;
#endif


#define ATOMIC_T atomic_t

int timer_used_num;
int max_timer_used_num;

struct timer_entry {
	struct list_head list;
	struct timer_list *timer;
#if TIMEOUT_LEAK_DBG
	void (*traced_func)(void *);
	int timeout;
#endif
};


#define HZ TICK_PER_SEC
#define rtw_timer_list timer_list



#define   KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
/* emulate a modern version */
#define LINUX_VERSION_CODE KERNEL_VERSION(2, 6, 17)

#define PRIORITIE_OFFSET				( 120 )


#define INIT_LINK_HEAD( _list_ )              \
		  do {								 \
		  (_list_)->flink = (_list_)->blink = (_list_);	\
		  } while(0)


static __inline _list *get_next(_list	*list)
{
	return list->next;
}	

static __inline _list	*get_list_head(_queue	*queue)
{
	return (&(queue->queue));
}

#define container_of(ptr, type, member) \
			((type *)((char *)(ptr)-(SIZE_T)(&((type *)0)->member)))
#define TASK_PRORITY_LOW  				1
#define TASK_PRORITY_MIDDLE   			2
#define TASK_PRORITY_HIGH    			3
#define TASK_PRORITY_SUPER    			4

#define TIMER_MAX_DELAY    				0xFFFFFFFF

#ifndef PRIORITIE_OFFSET
#define PRIORITIE_OFFSET				(0)
#endif

void save_and_cli(void);
void restore_flags(void);
void cli(void);


#ifndef mdelay
#define mdelay(t)					usleep(t*1000)
#endif

#ifndef udelay
#define udelay(t)					usleep(t)
#endif


//----- ------------------------------------------------------------------
// Common Definition
//----- ------------------------------------------------------------------

#define __init
#define __exit
#define __devinit
#define __devexit

#define KERN_ERR
#define KERN_INFO
#define KERN_NOTICE

#undef GFP_KERNEL
#define GFP_KERNEL			1
#define GFP_ATOMIC			1

#define SET_MODULE_OWNER(some_struct)	do { } while (0)
#define SET_NETDEV_DEV(dev, obj)	do { } while (0)
#define register_netdev(dev)		(0)
#define unregister_netdev(dev)		do { } while (0)
#define netif_queue_stopped(dev)	(0)
#define netif_wake_queue(dev)		do { } while (0)
#define printk				printf

#define DBG_ERR(fmt, args...)		printf("\n\r[%s] " fmt, __FUNCTION__, ## args)
#if WLAN_INTF_DBG
#define DBG_TRACE(fmt, args...)		printf("\n\r[%s] " fmt, __FUNCTION__, ## args)
#define DBG_INFO(fmt, args...)		printf("\n\r[%s] " fmt, __FUNCTION__, ## args)
#else
#define DBG_TRACE(fmt, args...)
#define DBG_INFO(fmt, args...)
#endif
#define HALT()				do { cli(); for(;;);} while(0)
#undef ASSERT
#define ASSERT(x)			do { \
						if((x) == 0){\
							printf("\n\rAssert(" #x ") failed on line %d in file %s", __LINE__, __FILE__); \
						HALT();}\
					} while(0)

#undef DBG_ASSERT
#define DBG_ASSERT(x, msg)		do { \
						if((x) == 0) \
							printf("\n\r%s, Assert(" #x ") failed on line %d in file %s", msg, __LINE__, __FILE__); \
					} while(0)

//----- ------------------------------------------------------------------
// Atomic Operation
//----- ------------------------------------------------------------------

/*
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#undef atomic_read
#define atomic_read(v)  ((v)->counter)

/*
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#undef atomic_set
#define atomic_set(v,i) ((v)->counter = (i))



/****************************Work Queue******************************************************/
/**
 * @ingroup workqueue
 * Initialize a work.
 *
 */
#ifdef CONFIG_THREAD_COMM_SIGNAL
typedef struct work_s _workitem;
typedef struct work_s work_struct;
#define INIT_WORK(_work, _func)          \
    do {      \
        INIT_LINK_HEAD(&((_work)->dq));    \
        (_work)->worker = (_func);            \
        (_work)->arg = (FAR void *)(0);     \
        (_work)->qtime = 0;            \
        (_work)->delay = 0;            \
    } while (0)

static inline void _init_workitem(_workitem *pwork, void *pfunc, void *cntx)
{
	INIT_WORK(pwork, pfunc);
}

inline static void _set_workitem(_workitem *pwork)
{
	work_queue(HPWORK, pwork, pwork->worker, pwork->arg, pwork->delay);
}

inline static void _cancel_workitem_sync(_workitem *pwork)
{
	work_cancel(HPWORK,pwork);
}
#endif
/****************************Work Queue End*************************************************/




extern void	rtw_init_listhead(_list *list);
extern u32	rtw_is_list_empty(_list *phead);
extern void	rtw_list_insert_head(_list *plist, _list *phead);
extern void	rtw_list_insert_tail(_list *plist, _list *phead);
extern void	rtw_list_delete(_list *plist);

#if (defined CONFIG_PLATFORM_8711B) || (defined CONFIG_PLATFORM_8721D)
extern u32 random_seed;
#endif

#endif /* _CUSTOMER_RTOS_SERVICE_H_ */
