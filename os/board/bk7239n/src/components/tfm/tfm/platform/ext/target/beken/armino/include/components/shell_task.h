#ifndef _shell_task_h_
#define _shell_task_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

int handle_shell_input(char *inbuf, int in_buf_size, char * outbuf, int out_buf_size);
void shell_task(void *pvParameters);

#ifdef CONFIG_SYS_CPU0
int shell_cmd_forward(char *cmd, u16 cmd_len);
#endif

int shell_assert_out(bool bContinue, char * format, ...);
int shell_assert_raw(bool bContinue, char * data_buff, u16 data_len);
int shell_trace_out( u32 trace_id, ... );
int shell_spy_out( u16 spy_id, u8 * data_buf, u16 data_len);
int shell_log_out(u8 level, char * mod_name, const char * format, ...);
void shell_log_out_port(int level, char * mod_name, const char * format, va_list ap);
int shell_log_raw_data(const u8 *data, u16 data_len);

int shell_echo_get(void);
void shell_echo_set(int en_flag);
void shell_set_log_level(int level);
int shell_get_log_level(void);
int shell_get_log_statist(u32 * info_list, u32 num);

void shell_log_flush(void);
void shell_set_uart_port(uint8_t uart_port);

void shell_cmd_ind_out(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _shell_task_h_ */
