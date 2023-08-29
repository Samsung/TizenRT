

#include "ameba_soc.h"
#include "FreeRTOS.h"
#include "task.h"

#include "lvgl.h"
#include "lv_hal.h"

static void scr_event_cb(lv_event_t *event)
{
	lv_event_code_t eventCode = lv_event_get_code(event);
	lv_indev_t *indev = lv_indev_get_act();
	lv_point_t p;
	lv_indev_get_point(indev, &p);

	if (eventCode == LV_EVENT_PRESSING) {
		printf("x:%d, y:%d, pressing\n", p.x, p.y);
	} else if (eventCode == LV_EVENT_RELEASED) {
		printf("x:%d, y:%d, release\n", p.x, p.y);
	} else {
		//todo
	}
}

void touch_test_task(void)
{
	printf("touch test start\n");

	lv_hal_init();

	lv_obj_t *label = lv_label_create(lv_scr_act());
	lv_label_set_text(label, "Hello, world!");

	lv_obj_add_event_cb(lv_scr_act(), scr_event_cb, LV_EVENT_PRESSING, NULL);
	lv_obj_add_event_cb(lv_scr_act(), scr_event_cb, LV_EVENT_RELEASED, NULL);

	while (1) {
		lv_task_handler();
	}
}

void touch_test(void)
{
	xTaskCreate((TaskFunction_t)touch_test_task,
				(const char *)"touch_test_task",
				(uint16_t)10240,
				(void *)NULL,
				(UBaseType_t)tskIDLE_PRIORITY + 1,
				(TaskHandle_t *)NULL);
}

u32 example_touch_test(IN  u16 argc, IN  u8  *argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	touch_test();
	return _TRUE;
}

CMD_TABLE_DATA_SECTION
const COMMAND_TABLE  gui_test_cmd_table[] = {
	{
		(const u8 *)"TouchTest",  1, example_touch_test, (const u8 *)"\tTouch Test \n"
	},
};
