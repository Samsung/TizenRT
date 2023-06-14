#ifndef UI_LVGL_LV_DRIVERS_DISPLAY
#define UI_LVGL_LV_DRIVERS_DISPLAY

#include "lvgl.h"

void display_init();
void display_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

#endif /* UI_LVGL_LV_DRIVERS_DISPLAY */
