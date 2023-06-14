#ifndef __CST328_H
#define __CST328_H

#include "sys.h"

typedef enum {
	TOUCH_PRESS = 1,
	TOUCH_RELEASE = 0,
} cst328_touch_state;

typedef struct {
	u8 state;
	u16 x;
	u16 y;
} cst328_touch_data_t;

typedef void (*cst328_touch_data_callback)(cst328_touch_data_t data);

void cst328_init(void);
void cst328_register_touch_data_callback(cst328_touch_data_callback cb);

#endif