/*
 * Copyright (C) 2020 Realtek Semiconductor Corp.  All Rights Reserved.
 *
 * Author: PSP Software Group
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "sheipa.h"
#include "platform_autoconf.h"
#include "ameba_userheapcfg.h"

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/*
 * The FreeRTOS Cortex-A port implements a full interrupt nesting model.
 *
 * Interrupts that are assigned a priority at or below
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which counter-intuitively in the ARM
 * generic interrupt controller [GIC] means a priority that has a numerical
 * value above configMAX_API_CALL_INTERRUPT_PRIORITY) can call FreeRTOS safe API
 * functions and will nest.
 *
 * Interrupts that are assigned a priority above
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which in the GIC means a numerical
 * value below configMAX_API_CALL_INTERRUPT_PRIORITY) cannot call any FreeRTOS
 * API functions, will nest, and will not be masked by FreeRTOS critical
 * sections (although it is necessary for interrupts to be globally disabled
 * extremely briefly as the interrupt mask is updated in the GIC).
 *
 * FreeRTOS functions that can be called from an interrupt are those that end in
 * "FromISR".  FreeRTOS maintains a separate interrupt safe API to enable
 * interrupt entry to be shorter, faster, simpler and smaller.
 *
 * The Sheipa implements 0x20 unique interrupt priorities.  For the purpose of
 * setting configMAX_API_CALL_INTERRUPT_PRIORITY 0xFF represents the lowest
 * priority.
 */
#define configMAX_API_CALL_INTERRUPT_PRIORITY	0x11

#if defined(CONFIG_CLINTWOOD ) && CONFIG_CLINTWOOD
#define configNUM_CORES							1
#else
#define configNUM_CORES							CONFIG_CPUS_NUM
#endif
#define configCPU_CLOCK_HZ						CPU_ClkGet()
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	0
#define configUSE_TICKLESS_IDLE					1
#define configTICK_RATE_HZ						( ( TickType_t ) 1000 )
#define configUSE_PREEMPTION					1
#define configUSE_IDLE_HOOK						0
#define configUSE_TICK_HOOK						0
#define configMAX_PRIORITIES					( 7 )	//AMEBAD2_TODO
#define configMINIMAL_STACK_SIZE				( ( unsigned short ) 1024 ) /* Large in case configUSE_TASK_FPU_SUPPORT is 2 in which case all tasks have an FPU context. */
#define configMAX_TASK_NAME_LEN					( 24 )
#define configUSE_TRACE_FACILITY				1
#define configUSE_16_BIT_TICKS					0
#define configIDLE_SHOULD_YIELD					1
#define configUSE_MUTEXES						1
#define configQUEUE_REGISTRY_SIZE				8
#define configCHECK_FOR_STACK_OVERFLOW			2
#define configUSE_RECURSIVE_MUTEXES				1
#define configUSE_MALLOC_FAILED_HOOK			1
#define configUSE_APPLICATION_TASK_TAG			1
#define configUSE_COUNTING_SEMAPHORES			1
#define configUSE_QUEUE_SETS					1
//#define configSUPPORT_STATIC_ALLOCATION			1  //AMEBAD2_TODO

#define configRECORD_STACK_HIGH_ADDRESS					1

/* Include the query-heap CLI command to query the free heap space. */
#define configINCLUDE_QUERY_HEAP_COMMAND		1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 					0
#define configMAX_CO_ROUTINE_PRIORITIES 		( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS						1
#define configTIMER_TASK_PRIORITY				( configMAX_PRIORITIES - 1 )
#if defined(CONFIG_AS_INIC_NP) || defined(CONFIG_SINGLE_CORE_WIFI)
#define configTIMER_QUEUE_LENGTH				(5 + 64)
#else
#define configTIMER_QUEUE_LENGTH				5
#endif
#define configTIMER_TASK_STACK_DEPTH			( configMINIMAL_STACK_SIZE * 2 )

/* If configUSE_TASK_FPU_SUPPORT is set to 1 (or undefined) then each task will
be created without an FPU context, and a task must call vTaskUsesFPU() before
making use of any FPU registers.  If configUSE_TASK_FPU_SUPPORT is set to 2 then
tasks are created with an FPU context by default, and calling vTaskUsesFPU() has
no effect. */
#define configUSE_TASK_FPU_SUPPORT				2

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet				1
#define INCLUDE_uxTaskPriorityGet				1
#define INCLUDE_vTaskDelete						1
#define INCLUDE_vTaskCleanUpResources			1
#define INCLUDE_vTaskSuspend					1
#define INCLUDE_vTaskDelayUntil					1
#define INCLUDE_vTaskDelay						1
#define INCLUDE_xTimerPendFunctionCall			1
#define INCLUDE_eTaskGetState					1
#define INCLUDE_xTaskAbortDelay					1
#define INCLUDE_xTaskGetTaskHandle				1
#define INCLUDE_xTaskGetHandle					1
#define INCLUDE_xSemaphoreGetMutexHolder		1

/* This demo makes use of one or more example stats formatting functions.  These
format the raw data provided by the uxTaskGetSystemState() function in to human
readable ASCII form.  See the notes in the implementation of vTaskList() within
FreeRTOS/Source/tasks.c for limitations. */
#define configUSE_STATS_FORMATTING_FUNCTIONS	1

/* The size of the global output buffer that is available for use when there
are multiple command interpreters running at once (for example, one on a UART
and one on TCP/IP).  This is done to prevent an output buffer being defined by
each implementation - which would waste RAM.  In this case, there is only one
command interpreter running. */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE		2096

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
void vAssertCalled(const char *pcFile, uint32_t ulLine);
#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ );

/* If configTASK_RETURN_ADDRESS is not defined then a task that attempts to
return from its implementing function will end up in a "task exit error"
function - which contains a call to configASSERT().  However this can give GCC
some problems when it tries to unwind the stack, as the exit error function has
nothing to return to.  To avoid this define configTASK_RETURN_ADDRESS to 0.  */
#define configTASK_RETURN_ADDRESS	NULL

/* defined by Realtek, for offseting wlan task priority */
#define PRIORITIE_OFFSET								( 4 )

#define configMINIMAL_SECURE_STACK_SIZE					( 1024 )
/****** Hardware specific settings. *******************************************/
void prvSetupHardware(void);
void prvSetupHardwareSecondary(void);

/*
 * The application must provide a function that configures a peripheral to
 * create the FreeRTOS tick interrupt, then define configSETUP_TICK_INTERRUPT()
 * in FreeRTOSConfig.h to call the function.  This file contains a function
 * that is suitable for use on the Zynq MPU.  FreeRTOS_Tick_Handler() must
 * be installed as the peripheral's interrupt handler.
 */
void vConfigureTickInterrupt(void);
#define configSETUP_TICK_INTERRUPT() vConfigureTickInterrupt()

void vClearTickInterrupt(void);
#define configCLEAR_TICK_INTERRUPT() vClearTickInterrupt()

void vDisableIPIInterrupt(void);

void vConfigureIPIInterrupt(void);
#define configSETUP_IPI_INTERRUPT() vConfigureIPIInterrupt()

void vClearIPIInterrupt(void);
#define configCLEAR_IPI_INTERRUPT() vClearIPIInterrupt()
#if ( configNUM_CORES > 1 )
void vConfigureSMPSendIPI(uint32_t ulCoreID);
#define configYieldOtherCore(x) vConfigureSMPSendIPI(x)
#endif
#define configPRE_SLEEP_PROCESSING( x )         		( freertos_pre_sleep_processing((uint32_t *)&x) )
#define configPOST_SLEEP_PROCESSING( x )        		( freertos_post_sleep_processing((uint32_t *)&x) )

extern void freertos_pre_sleep_processing(uint32_t *expected_idle_time);
extern void freertos_post_sleep_processing(uint32_t *expected_idle_time);


/* The following constant describe the hardware */
#define configINTERRUPT_CONTROLLER_BASE_ADDRESS 		GIC_DIST_BASE
#define configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET	(GIC_CPU_BASE - GIC_DIST_BASE)

#define configUNIQUE_INTERRUPT_PRIORITIES				0x20

/* Task running time and task status statistics */
#define configGENERATE_RUN_TIME_STATS 0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() do {} while (0)
#define portGET_RUN_TIME_COUNTER_VALUE() (*((volatile uint32_t*)(0x42009000 + 0x004)))  /* DEBUGTIMER_REG_BASE: 0x42009000, COUNT REGISTER offset: 0x004 */

#endif /* FREERTOS_CONFIG_H */
