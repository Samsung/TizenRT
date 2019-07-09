#include <stdlib.h>
#include <araui/ui_widget.h>
#include "araui_sample.h"

#define ICON_FROM_X   (-80)
#define ICON_FROM_Y   (60)
#define ICON_TO_X     (30)
#define ICON_TO_Y     (60)
#define ICON_DURATION (700)
#define NUM_FROM_X    (320)
#define NUM_FROM_Y    (54)
#define NUM_TO_X      (155)
#define NUM_TO_Y      (54)
#define NUM_DURATION  (400)
#define LOC_FROM_X    (320)
#define LOC_FROM_Y    (110)
#define LOC_TO_X      (115)
#define LOC_TO_Y      (110)
#define LOC_DURATION  (550)

static void icon_intv_cb(ui_widget_t widget)
{
	ui_widget_set_position(widget, ICON_FROM_X, ICON_FROM_Y);
	switch (rand() % 6) {
	case 0: ui_image_widget_set_image(widget, g_weather_icon_cloudy); break;
	case 1: ui_image_widget_set_image(widget, g_weather_icon_cloudy_night); break;
	case 2: ui_image_widget_set_image(widget, g_weather_icon_hot); break;
	case 3: ui_image_widget_set_image(widget, g_weather_icon_rainy); break;
	case 4: ui_image_widget_set_image(widget, g_weather_icon_sunny); break;
	case 5: ui_image_widget_set_image(widget, g_weather_icon_thunder); break;
	}
	ui_widget_tween_moveto(widget, ICON_TO_X, ICON_TO_Y, ICON_DURATION, TWEEN_EASE_OUT_QUAD, NULL);
}

static void num_intv_cb(ui_widget_t widget)
{
	ui_widget_set_position(widget, NUM_FROM_X, NUM_FROM_Y);
	ui_text_widget_set_text_format(widget, "%d%%", (rand() % 20) + 15);
	ui_widget_tween_moveto(widget, NUM_TO_X, NUM_TO_Y, NUM_DURATION, TWEEN_EASE_OUT_QUAD, NULL);
}

static void loc_intv_cb(ui_widget_t widget)
{
	ui_widget_set_position(widget, LOC_FROM_X, LOC_FROM_Y);

	switch (rand() % 6) {
	case 0: ui_text_widget_set_text(widget, "SAN FRANCISCO"); break;
	case 1: ui_text_widget_set_text(widget, "대한민국"); break;
	case 2: ui_text_widget_set_text(widget, "BRAZIL"); break;
	case 3: ui_text_widget_set_text(widget, "NEW YORK"); break;
	case 4: ui_text_widget_set_text(widget, "PETERSBURG"); break;
	case 5: ui_text_widget_set_text(widget, "YOKOHAMA"); break;
	}

	ui_widget_tween_moveto(widget, LOC_TO_X, LOC_TO_Y, LOC_DURATION, TWEEN_EASE_OUT_QUAD, NULL);
}

ui_widget_t build_up_weather_page(void)
{
	ui_widget_t page = ui_widget_create(CONFIG_UI_DISPLAY_WIDTH, CONFIG_UI_DISPLAY_HEIGHT);

	ui_widget_t title = ui_text_widget_create(CONFIG_UI_DISPLAY_WIDTH, 20, g_default_font_20, "Weather 위젯");
	ui_text_widget_set_align(title, UI_ALIGN_CENTER | UI_ALIGN_MIDDLE);
	ui_widget_add_child(page, title, 0, 16);

	ui_widget_t icon = ui_image_widget_create(g_weather_icon_cloudy_night);
	ui_widget_add_child(page, icon, ICON_FROM_X, ICON_FROM_Y);
	ui_widget_tween_moveto(icon, ICON_TO_X, ICON_TO_Y, ICON_DURATION, TWEEN_EASE_OUT_QUAD, NULL);
	ui_widget_set_interval_callback(icon, icon_intv_cb, 7000);

	ui_widget_t num = ui_text_widget_create(140, 64, g_arial_num_64, "23%");
	ui_text_widget_set_align(num, UI_ALIGN_RIGHT | UI_ALIGN_MIDDLE);
	ui_widget_add_child(page, num, NUM_FROM_X, NUM_FROM_Y);
	ui_widget_tween_moveto(num, NUM_TO_X, NUM_TO_Y, NUM_DURATION, TWEEN_EASE_OUT_QUAD, NULL);
	ui_widget_set_interval_callback(num, num_intv_cb, 4000);

	ui_widget_t loc = ui_text_widget_create(180, 64, g_default_font_20, "SAN FRANCISCO");
	ui_text_widget_set_align(loc, UI_ALIGN_RIGHT | UI_ALIGN_MIDDLE);
	ui_widget_add_child(page, loc, LOC_FROM_X, LOC_FROM_Y);
	ui_widget_tween_moveto(loc, LOC_TO_X, LOC_TO_Y, LOC_DURATION, TWEEN_EASE_OUT_QUAD, NULL);
	ui_widget_set_interval_callback(loc, loc_intv_cb, 6000);

	return page;
}
