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

#include <stdio.h>
#include <araui/ui_asset.h>
#include <araui/ui_core.h>
#include <araui/ui_window.h>
#include <araui/ui_widget.h>
#include "dal/dal_sdl.h"

ui_window_t g_window;

static void on_create_cb(ui_window_t window)
{

}

static void on_destroy_cb(ui_window_t window)
{

}

static void on_show_cb(ui_window_t window)
{

}

static void on_hide_cb(ui_window_t window)
{

}

int main(int argc, char *argv[])
{
	ui_start();

	g_window = ui_window_create(on_create_cb, on_destroy_cb, on_show_cb, on_hide_cb);

	sdl_loop();

	ui_window_destroy(g_window);

	ui_stop();

	return 0;
}
