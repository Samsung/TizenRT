#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdint.h>
#include "download_types.h"

typedef enum {
    FLASH_LINE_1=0,
    FLASH_LINE_2,
    FLASH_LINE_4
} FLASH_LINE_MODE;

enum
{	
	STATUS_OK = 0,
	FLASH_STATUS_BUSY = 1,
	SPI_OP_T_OUT = 2,
	FLASH_OP_T_OUT = 3,
	PACK_LEN_ERROR = 4,
	PACK_PAYLOAD_LACK = 5,
	PARAM_ERROR = 6,
	UNKNOW_CMD = 7,
	INTERNAL_ERR = 8,
	SPI_FLASH_WRITE_ERR = 9,
	OPERATE_PROTECTED_AREA =10,
};
#ifndef SOC_AON_GPIO_REG_BASE_ADDR
#define SOC_AON_GPIO_REG_BASE_ADDR 0x44000400
#endif
#define GPIO_UP(id) *(volatile uint32_t*) (SOC_AON_GPIO_REG_BASE_ADDR + ((id) << 2)) = 2
#define GPIO_DOWN(id) *(volatile uint32_t*) (SOC_AON_GPIO_REG_BASE_ADDR + ((id) << 2)) = 0

#define GPIO_UP_DOWN(id) do {\
        volatile uint32_t cnt;\
        for (int i = 0; i < 10; i++) {\
                if ((i % 2) == 0) GPIO_UP((id));\
                else GPIO_DOWN(id);\
                cnt = 1000;\
                while(--cnt > 0);\
        }\
} while(0)

#endif
// EOF
