//Need to be changed in tizenrt code


#define configENABLE_FPU 0
#define configENABLE_MPU 0
#define configENABLE_TRUSTZONE 0
#define configCPU_CLOCK_HZ PLATFORM_CLOCK
#define configUSE_16_BIT_TICKS 0
#define configRUN_FREERTOS_SECURE_ONLY 0
#define configTICK_RATE_HZ PLATFORM_CLOCK
#define pdFALSE FALSE
#define pdTRUE TRUE
typedef void (*TaskFunction_t)( void * );
typedef void * TaskHandle_t;
#define xTaskHandle TaskHandle_t

#define taskSCHEDULER_NOT_STARTED ( ( BaseType_t ) 1 )

#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
