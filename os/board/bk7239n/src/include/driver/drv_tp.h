#ifndef __DRV_TP_H
#define __DRV_TP_H

#include "driver/bk_queue.h"

#define TP_DATA_QUEUE_MAX_SIZE (30)

typedef enum {
    TP_MIRROR_NONE,
    TP_MIRROR_X_COORD,
    TP_MIRROR_Y_COORD,
    TP_MIRROR_X_Y_COORD,
} tp_mirror_type_t;

typedef struct{
    uint16_t m_x;
    uint16_t m_y;
    uint16_t m_state;              //0 release 1 press
    uint16_t m_need_continue;
}tp_point_infor_t;

typedef struct{
    queue_t                  *m_point_queue;
    beken_mutex_t            m_mutex;
}tp_dev_t;

typedef void (*tp_touch_event_notify)(void *arg);

int drv_tp_open(int hor_size, int ver_size, tp_mirror_type_t tp_mirror);
int drv_tp_close(void);
int drv_tp_read(tp_point_infor_t *point);
int drv_tp_write(uint16_t x, uint16_t y, uint16_t state);
void drv_tp_reg_touch_event(tp_touch_event_notify event_notify_func, void *arg);

#endif
