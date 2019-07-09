/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <araui/ui_core.h>
#include <araui/ui_view.h>
#include <araui/ui_widget.h>
#include <araui/ui_commons.h>
#include <tinyara/config.h>
#include "araui_sample.h"

/****************************************************************************
 * araui_main
 ****************************************************************************/

// Assets (Converted to c)
extern const uint8_t arial_num_64[10005];
extern const uint8_t digital_7_mono_48[6253];
extern const uint8_t samsungone20[8170];
extern const uint8_t weather_icon_cloudy_night[25656];
extern const uint8_t weather_icon_cloudy[25656];
extern const uint8_t weather_icon_hot[25656];
extern const uint8_t weather_icon_rainy[25656];
extern const uint8_t weather_icon_sunny[25656];
extern const uint8_t weather_icon_thunder[25656];

ui_asset_t g_default_font_20;
ui_asset_t g_digital_7_mono_48;
ui_asset_t g_arial_num_64;
ui_asset_t g_weather_icon_cloudy;
ui_asset_t g_weather_icon_cloudy_night;
ui_asset_t g_weather_icon_hot;
ui_asset_t g_weather_icon_rainy;
ui_asset_t g_weather_icon_sunny;
ui_asset_t g_weather_icon_thunder;

ui_widget_t g_pages[2];

ui_view_t g_view;

static void change_view(ui_widget_t widget)
{
	static int32_t index = 0;

	ui_widget_set_visible(g_pages[index], false);
	ui_widget_set_visible(g_pages[1 - index], true);
	index = 1 - index;
}

static void view_create_cb(ui_view_t view)
{
	g_default_font_20 = ui_font_asset_create_from_buffer(samsungone20);
	g_digital_7_mono_48 = ui_font_asset_create_from_buffer(digital_7_mono_48);
	g_arial_num_64 = ui_font_asset_create_from_buffer(arial_num_64);
	g_weather_icon_cloudy = ui_image_asset_create_from_buffer(weather_icon_cloudy);
	g_weather_icon_cloudy_night = ui_image_asset_create_from_buffer(weather_icon_cloudy_night);
	g_weather_icon_hot = ui_image_asset_create_from_buffer(weather_icon_hot);
	g_weather_icon_rainy = ui_image_asset_create_from_buffer(weather_icon_rainy);
	g_weather_icon_sunny = ui_image_asset_create_from_buffer(weather_icon_sunny);
	g_weather_icon_thunder = ui_image_asset_create_from_buffer(weather_icon_thunder);

	g_pages[0] = build_up_clock_page();
	g_pages[1] = build_up_weather_page();

	ui_view_add_widget(view, g_pages[0], 0, 0);
	ui_view_add_widget(view, g_pages[1], 0, 0);
	ui_widget_set_visible(g_pages[1], false);

	ui_widget_set_interval_callback(g_pages[0], change_view, 10000);
}

static void view_destroy_cb(ui_view_t view)
{
	ui_font_asset_destroy(g_default_font_20);
	ui_font_asset_destroy(g_digital_7_mono_48);
	ui_font_asset_destroy(g_arial_num_64);
	ui_image_asset_destroy(g_weather_icon_cloudy);
	ui_image_asset_destroy(g_weather_icon_cloudy_night);
	ui_image_asset_destroy(g_weather_icon_hot);
	ui_image_asset_destroy(g_weather_icon_rainy);
	ui_image_asset_destroy(g_weather_icon_sunny);
	ui_image_asset_destroy(g_weather_icon_thunder);
}

static void view_show_cb(ui_view_t view)
{
}

static void view_hide_cb(ui_view_t view)
{
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int araui_sample_main(int argc, char *argv[])
#endif
{
	ui_view_t view;

	ui_start();

	view = ui_view_create(view_create_cb, view_destroy_cb, view_show_cb, view_hide_cb);
	if (!view) {
		printf("error: failed to create view.\n");
	}

	return 0;
}
