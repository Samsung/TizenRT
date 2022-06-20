#ifndef _HTTPD_UTIL_H_
#define _HTTPD_UTIL_H_

#include "httpd.h"

extern uint8_t httpd_debug;

#define httpd_log(...) \
	do { \
		if(httpd_debug) { \
			taskENTER_CRITICAL(); \
			printf("\n\r[HTTPD] "); \
			printf(__VA_ARGS__); \
			printf("\n\r"); \
			taskEXIT_CRITICAL(); \
		} \
	} while(0)

#define httpd_log_verbose(...) \
	do { \
		if(httpd_debug == HTTPD_DEBUG_VERBOSE) { \
			taskENTER_CRITICAL(); \
			printf("\n\r[HTTPD] "); \
			printf(__VA_ARGS__); \
			printf("\n\r"); \
			taskEXIT_CRITICAL(); \
		} \
	} while(0)

struct httpd_page {
	struct httpd_page *next;
	char *path;
	void (*callback)(struct httpd_conn *conn);
};

void *httpd_malloc(size_t size);
int httpd_page_add(char *path, void (*callback)(struct httpd_conn *conn));
void httpd_page_remove(struct httpd_page *page);
void httpd_page_clear(void);
struct httpd_conn *httpd_conn_add(int sock);
void httpd_conn_remove(struct httpd_conn *conn);
void httpd_conn_detach(struct httpd_conn *conn);
void httpd_conn_clear(void);
void httpd_query_remove_special(uint8_t *input, size_t input_len, uint8_t *output, size_t output_len);
int httpd_write(struct httpd_conn *conn, uint8_t *buf, size_t buf_len);
int httpd_read(struct httpd_conn *conn, uint8_t *buf, size_t buf_len);
int httpd_read_with_timeout(struct httpd_conn *conn, uint8_t *buf, uint16_t buf_len, int recv_timeout);
void httpd_buf_tolower(uint8_t *buf, size_t buf_len);

int httpd_tls_setup_init(const char *server_cert, const  char *server_key, const char *ca_certs);
void httpd_tls_setup_free(void);
void *httpd_tls_new_handshake(int *sock, uint8_t secure);
void httpd_tls_free(void *tls_in);
void httpd_tls_close(void *tls_in);
int httpd_tls_read(void *tls_in, uint8_t *buf, size_t buf_len);
int httpd_tls_write(void *tls_in, uint8_t *buf, size_t buf_len);
int httpd_base64_encode(uint8_t *data, size_t data_len, char *base64_buf, size_t buf_len);

#endif /* _HTTPD_UTIL_H_ */
