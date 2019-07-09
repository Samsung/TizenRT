#include <araui/ui_widget.h>
#include <time.h>
#include "araui_sample.h"

static void clock_intv_cb(ui_widget_t widget)
{
	static int h = 0;
	static int m = 0;
	static int s = 0;
	static bool pm = false;
	static bool colon = true;

	if (colon) {
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		h = tm.tm_hour;
		m = tm.tm_min;
		s = tm.tm_sec;

		pm = false;

		if (h >= 12) {
			h -= 12;
			pm = true;
		}
	}

	ui_text_widget_set_text_format(widget, "%02d%s%02d%s%02d %s", h, (colon) ? ":" : " ", m, (colon) ? ":" : " ", s, (pm) ? "PM" : "AM");
	colon = !colon;
}

ui_widget_t build_up_clock_page(void)
{
	ui_widget_t page = ui_widget_create(CONFIG_UI_DISPLAY_WIDTH, CONFIG_UI_DISPLAY_HEIGHT);

	ui_widget_t title = ui_text_widget_create(CONFIG_UI_DISPLAY_WIDTH, 20, g_default_font_20, "Clock 위젯");
	ui_text_widget_set_align(title, UI_ALIGN_CENTER | UI_ALIGN_MIDDLE);
	ui_widget_add_child(page, title, 0, 16);

	ui_widget_t clock_text = ui_text_widget_create(CONFIG_UI_DISPLAY_WIDTH, 100, g_digital_7_mono_48, "");
	ui_text_widget_set_align(clock_text, UI_ALIGN_CENTER | UI_ALIGN_MIDDLE);
	ui_widget_add_child(page, clock_text, 0, 60);
	ui_widget_set_interval_callback(clock_text, clock_intv_cb, 500);

	return page;
}
