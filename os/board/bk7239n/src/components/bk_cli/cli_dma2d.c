#include "cli.h"

extern void sdcard_read_to_mem_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void sdcard_write_from_mem_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

extern void dma2d_fill_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void dma2d_memcpy_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void dma2d_pfc_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void dma2d_blend_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void dma2d_blend_offset_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_display_dma2d(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);


#define DMA2D_CNT (sizeof(s_dma2d_commands) / sizeof(struct cli_command))
static const struct cli_command s_dma2d_commands[] = {
	{"dma2d_fill_test", "fill \r\n", dma2d_fill_test},
	{"dma2d_memcpy_test", "dma2d_mem_to_mem \r\n", dma2d_memcpy_test},
	{"dma2d_pfc_test", "dma2d_pfc_test \r\n", dma2d_pfc_test},
	{"dma2d_blend_test", "dma2d_blend_test \r\n", dma2d_blend_test},
	{"dma2d_blend_offset_test", "dma2d_blend_test \r\n", dma2d_blend_offset_test},
	{"rgb_dma2d", "dma2d_blend_test \r\n", lcd_rgb_display_dma2d},

	

	{"sdcard_write", "sdcard_write_from_mem_test \r\n", sdcard_write_from_mem_test},
	{"sdcard_read", "sdcard_write_from_mem_test \r\n", sdcard_read_to_mem_test},
};

int cli_dma2d_init(void)
{
	return cli_register_commands(s_dma2d_commands, DMA2D_CNT);
}





















