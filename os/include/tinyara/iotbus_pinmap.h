#ifndef __IOTBUS_PINMAP_H__
#define __IOTBUS_PINMAP_H__
#include <stdint.h>

typedef struct {
	uint32_t u32Port;						///> 물리적 포트값
	uint32_t u32Alternative;			///> Alternative 속성 , Alternative 가 일반 GPIO 일때 High/Low Value
} PORT_PROPERTY;

#endif // __IOTBUS_PINMAP_H__
