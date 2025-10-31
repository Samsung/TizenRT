#include "cli.h"
#include "frame_buffer.h"
#include "media_app.h"
#include <driver/flash.h>
#include <driver/psram.h>
#include <driver/lcd.h>

extern void lcd_8080_display_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_8080_display_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_8080_display_480p_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_display_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_display_jpeg(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_display_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_close(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void sdcard_read_to_psram(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void jpeg_dec_display_demo(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void sdcard_write_from_mem(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

beken_thread_t frame_push;
beken_thread_t frame_read;
static frame_buffer_t *frame = NULL;
static frame_buffer_t *lcd_show = NULL;
static uint32_t read_count = 0;
static uint32_t push_count = 0;

extern bk_err_t bk_lcd_set_yuv_mode(pixel_format_t input_data_format);
extern void bk_lcd_send_data(uint32_t command, uint16_t *data, uint32_t len);
extern void st7796s_set_display_mem_area(uint16 xs, uint16 xe, uint16 ys, uint16 ye);

static void h264_print(uint8_t *buffer, int length)
{
	for (int i = 0; i < length; i++)
	{
		os_printf("%02x ", *(buffer+i));
	}
	os_printf("\r\n");
}

static void frame_push_entry(void)
{
	while (1)
	{
		rtos_delay_milliseconds(45);
		frame->fmt = PIXEL_FMT_UYVY;
		frame->width = 320;
		frame->height = 480;
		frame_buffer_fb_display_push(frame);
		os_printf("push count is %d \r\n", push_count++);
		frame = NULL;
		frame = frame_buffer_fb_display_malloc_wait();
	}
	
}

static void st7796s_lcd_driver_partial_set(uint16_t width, uint16_t height)
{
	uint16_t x = 320;
	uint16_t y = 480;

	bk_lcd_pixel_config(width, height);
	uint16_t start_x = 1;
	uint16_t start_y = 1;
	uint16_t end_x = x;
	uint16_t end_y = y;

	if (x < width || y < height)
	{
		if (x < width)
		{
			start_x = (width - x) / 2 + 1;
			end_x = start_x + x - 1;
		}
		if (y < height)
		{
			start_y = (height - y) / 2 + 1;
			end_y = start_y + y - 1;
		}

		CLI_LOGD("%s, offset %d, %d, %d, %d\n", __func__, start_x, end_x, start_y, end_y);
	}

	bk_lcd_set_partical_display(1, start_x, end_x, start_y, end_y);
}


static void frame_read_entry(void)
{
	while (1)
	{
		rtos_delay_milliseconds(20);

		// st7796s_set_display_mem_area(0, 160, 0, 320);
		// bk_lcd_send_data(0x2C, (uint16_t *)0x60600000, 160*320);
		// st7796s_set_display_mem_area(0, 319, 0, 479);
		lcd_driver_set_display_base_addr((uint32_t)60600000);
		bk_lcd_set_yuv_mode(PIXEL_FMT_UYVY);
		bk_lcd_8080_start_transfer(1);
		bk_lcd_8080_ram_write(0x2c);
		rtos_delay_milliseconds(20);

		frame_buffer_fb_display_free(lcd_show);
		lcd_show = frame_buffer_fb_display_pop_wait();
		if (lcd_show == NULL)
		{
			os_printf("read display frame NULL\n");
			continue;
		}
		os_printf("read count is %d \r\n", read_count++);
		lcd_driver_set_display_base_addr((uint32_t)lcd_show->frame);
		bk_lcd_set_yuv_mode(lcd_show->fmt);
		st7796s_lcd_driver_partial_set(lcd_show->width, lcd_show->height);
		bk_lcd_8080_start_transfer(1);
		bk_lcd_8080_ram_write(0x2c);
	}
	
}

static void lcd_cpy_yuv(int width, int height, uint32_t fb_yuv_buffer_size)
{
	os_printf("flash copy start! \r\n");
	uint8_t *yuv_data = (uint8_t *)0x60000000;

	int length = length = width * height * 2;
	int length_k = length / 1024;
	os_printf("offset:%x ... KB:%d \r\n", length, length_k);

	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	uint8_t read_buffer[1024];

	for (int j = 0; j < 6; j++)
	{
		for (int i = 0; i < length_k; i++)
		{
			bk_flash_read_bytes(0x100000+1024*i+length*j, read_buffer, 1024);
			bk_psram_memcpy((yuv_data+1024*i), read_buffer, 1024);
		}
		h264_print(yuv_data, 8);
		yuv_data += fb_yuv_buffer_size;
	}
}

static void lcd_8080_frame_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{	
	if(os_strcmp(argv[1], "push") == 0) {
		frame_buffer_fb_display_init(PPI_640X480);
		frame = frame_buffer_fb_display_malloc_wait();
		lcd_cpy_yuv(320, 480, 800*600*2);

		rtos_create_thread(&frame_push,
							4,
							"fp",
							(beken_thread_function_t)frame_push_entry,
							1024 * 6,
							(beken_thread_arg_t)NULL);
	}
	if(os_strcmp(argv[1], "disp_task") == 0) {
		lcd_show = frame_buffer_fb_display_malloc_wait();

		rtos_create_thread(&frame_read,
							4,
							"fr",
							(beken_thread_function_t)frame_read_entry,
							1024 * 6,
							(beken_thread_arg_t)NULL);
	}
	if(os_strcmp(argv[1], "task_del") == 0) {
		rtos_delete_thread(&frame_read);
		rtos_delete_thread(&frame_push);
	}
}


#define LCD_CNT (sizeof(s_lcd_commands) / sizeof(struct cli_command))
static const struct cli_command s_lcd_commands[] = {
	{"lcd_8080", "init|fram_disp", lcd_8080_display_test},
	{"lcd_8080_yuv", "lcd_8080_yuv=4", lcd_8080_display_yuv},
	{"lcd_8080_yuv_partical", "lcd_8080_yuv_partical=4", lcd_8080_display_480p_yuv},
	{"lcd_rgb", "lcd_rgb=rgb565_display,1,display_partical", lcd_rgb_display_test},
	{"lcd_rgb_yuv", "lcd_rgb=480p,1,display_partical", lcd_rgb_display_yuv},
	{"lcd_rgb_jpeg", "lcd_rgb_jpeg =480p ,25", lcd_rgb_display_jpeg},
	{"lcd_close", "lcd_close=yuv|jpeg,", lcd_rgb_close},
	{"sdcard_write_from_mem", "file_name|pixelx|pixely|addr", sdcard_write_from_mem},
	{"sdcard_read_to_mem", "file_name,addr", sdcard_read_to_psram},
	{"lcd", " file_display,file name", jpeg_dec_display_demo},
	{"lcd_8080_local", "push|disp_task", lcd_8080_frame_test},
};

int cli_lcd_init(void)
{
	return cli_register_commands(s_lcd_commands, LCD_CNT);
}








