/* Copyright (c) 2020 Wi-Fi Alliance                                                */

/* Permission to use, copy, modify, and/or distribute this software for any         */
/* purpose with or without fee is hereby granted, provided that the above           */
/* copyright notice and this permission notice appear in all copies.                */

/* THE SOFTWARE IS PROVIDED 'AS IS' AND THE AUTHOR DISCLAIMS ALL                    */
/* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                    */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL                     */
/* THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR                       */
/* CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING                        */
/* FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF                       */
/* CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT                       */
/* OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS                          */
/* SOFTWARE. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
//#include <netinet/in.h>
#include "vendor_specific.h"

#ifdef CONTROLAPPC_LINUX_API
#include <netinet/in.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
#include <unistd.h>
//#include <sys/ioctl.h>
//#include <net/if.h>
#include <signal.h>
#include <errno.h>
#else
#include <os/os.h>
#include <components/log.h>
#endif
#include <driver/uart.h>
#include <modules/pm.h>
#include <modules/wifi.h>
#include "quick_uart.h"
#include "controlapp_eloop.h"
#include "indigo_api.h"
#include "utils.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static beken_thread_t controlappc_thread_handle = NULL;
#ifndef BK_CONTROL_APP_C
static beken_thread_t controlappc_eloop_thread_handle = NULL;
#endif
/* Internal functions */
static void control_receive_message(int sock, void *eloop_ctx, void *sock_ctx);
#ifdef CONTROLAPPC_LINUX_API
static int parse_parameters(int argc, char *argv[]);
static void usage();
#endif

/* External variables */
extern int capture_packet; /* debug. Write the received packets to files */
extern int debug_packet;   /* used by the packet hexstring print */

#ifdef BK_CONTROL_APP_C
#define CONTROL_APPC_UART_TIMEOUT    50
#define CONTROL_APPC_UART_SETTLE_DELAY_MS 10
#define CONTROL_APPC_UART_SETTLE_RETRY 10
//#define CONTROL_APPC_UART            UART_ID_1
#define CONTROL_APPC_UART            CONFIG_QUICK_TRACK_UART_PORT

#define LOCALSERVER_PORT             8888

beken_semaphore_t control_appc_uart_rx_sema = NULL;
#define CONTROL_APPC_UART_BUF_SZ    1024
static uint8_t control_appc_uart_rx_buf[CONTROL_APPC_UART_BUF_SZ];
static int uart_rx_pos = 0;
beken_thread_t  control_appc_thread_handle = NULL;

static void control_appc_uart_rx_callback(uart_id_t uport, void *param)
{
	if (control_appc_uart_rx_sema)
		rtos_set_semaphore(&control_appc_uart_rx_sema);
}
#endif // BK_CONTROL_APP_C

/* Initiate the service port. */
static int control_socket_init(int port) {
    int s = -1;
//    char cmd[S_BUFFER_LEN];
    struct sockaddr_in addr;

    /* Open UDP socket */
    s = socket(PF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to open server socket: %s", strerror(errno));
        return -1;
    }

    /* Bind specific port */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(port);
    if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to bind server socket: %s", strerror(errno));
        //if (errno == EADDRINUSE) {
        //    sprintf(cmd, "netstat -lunatp | grep %d", port);
        //    system(cmd);
        //}
        close(s);
        return -1;
    }

    /* Register to eloop and ready for the socket event */
    if (controlapp_eloop_register_read_sock(s, control_receive_message, NULL, NULL)) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to initiate ControlAppC");
        close(s);
        return -1;
    }
    return s;
}

int controlappc_service_socket_init(void)
{
#ifdef BK_CONTROL_APP_C
	return control_socket_init(LOCALSERVER_PORT);
#else
	return control_socket_init(get_service_port());
#endif
}

struct sockaddr_in *tool_addr; // For HTTP Post

#ifdef BK_CONTROL_APP_C


/* Callback function of the QuickTrack API. */
static void control_receive_message(int sock, void *eloop_ctx, void *sock_ctx) {
    int ret;                          // return code
    int fromlen, len;                 // structure size and received length
    struct sockaddr_storage from;     // source address of the message
    char buffer[S_BUFFER_LEN]; // buffer to receive the message
    struct packet_wrapper req, resp;  // packet wrapper for the received message and response
    struct indigo_api *api = NULL;    // used for API search, validation and handler call

    /* Receive request */
    fromlen = sizeof(from);

    len = recvfrom(sock, buffer, BUFFER_LEN, 0, (struct sockaddr *) &from, (socklen_t*)&fromlen);

    if (len < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Server: Failed to receive the packet");
        return ;
    } else {
        indigo_logger(LOG_LEVEL_DEBUG, "Server: Receive the packet");
    }
    tool_addr = (struct sockaddr_in *)&from;

    /* Parse request to HDR and TLV. Response NACK if parser fails. Otherwises, ACK. */
    memset(&req, 0, sizeof(struct packet_wrapper));
    memset(&resp, 0, sizeof(struct packet_wrapper));
    ret = parse_packet(&req, buffer, len);
    if (ret == 0) {
        indigo_logger(LOG_LEVEL_DEBUG, "Server: Parsed packet successfully");
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "Server: Failed to parse the packet");
        fill_wrapper_ack(&resp, req.hdr.seq, 0x31, "Unable to parse the packet");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        quicktrack_uart_send(CONTROL_APPC_UART, buffer, len);
        goto done;
    }

    /* Find API by ID. If API is not supported, assemble NACK. */
    api = get_api_by_id(req.hdr.type);
    if (api) {
        indigo_logger(LOG_LEVEL_DEBUG, "API %s: Found handler", api->name);
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "API Unknown (0x%04x): No registered handler", req.hdr.type);
        fill_wrapper_ack(&resp, req.hdr.seq, 0x31, "Unable to find the API handler");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        quicktrack_uart_send(CONTROL_APPC_UART, buffer, len);
        goto done;
    }

    /* Verify. Optional. If validation is failed, then return NACK. */
    if (api->verify == NULL || (api->verify && api->verify(&req, &resp) == 0)) {
        indigo_logger(LOG_LEVEL_INFO, "API %s: Return ACK", api->name);
        fill_wrapper_ack(&resp, req.hdr.seq, 0x30, "ACK: Command received");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        quicktrack_uart_send(CONTROL_APPC_UART, buffer, len);
        free_packet_wrapper(&resp);
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "API %s: Failed to verify and return NACK", api->name);
        fill_wrapper_ack(&resp, req.hdr.seq, 1, "Unable to find the API handler");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        quicktrack_uart_send(CONTROL_APPC_UART, buffer, len);
        goto done;
    }

    /* Optional, use timer to handle the execution */
    /* Handle & Response. Call API handle(), assemble packet by response wrapper and send back to source address. */
    if (api->handle && api->handle(&req, &resp) == 0) {
        indigo_logger(LOG_LEVEL_INFO, "API %s: Return execution result", api->name);
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        quicktrack_uart_send(CONTROL_APPC_UART, buffer, len);
    } else {
        indigo_logger(LOG_LEVEL_DEBUG, "API %s (0x%04x): No handle function", api ? api->name : "Unknown", req.hdr.type);
    }

done:
    /* Clean up resource */
    free_packet_wrapper(&req);
    free_packet_wrapper(&resp);
    indigo_logger(LOG_LEVEL_DEBUG, "API %s: Complete", api ? api->name : "Unknown");
}

static void control_receive_uart_message(char *buffer, int len)
{
    int ret;                          // return code
    struct packet_wrapper req, resp;  // packet wrapper for the received message and response
    struct indigo_api *api = NULL;    // used for API search, validation and handler call

    memset(&req, 0, sizeof(struct packet_wrapper));
    memset(&resp, 0, sizeof(struct packet_wrapper));
    ret = parse_packet(&req, buffer, len);
    if (ret == 0) {
        indigo_logger(LOG_LEVEL_DEBUG, "Server: Parsed packet successfully");
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "Server: Failed to parse the packet");
        fill_wrapper_ack(&resp, req.hdr.seq, 0x31, "Unable to parse the packet");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        quicktrack_uart_send(CONTROL_APPC_UART, buffer, len);
        goto done;
    }

    api = get_api_by_id(req.hdr.type);
    if (api) {
        indigo_logger(LOG_LEVEL_DEBUG, "API %s: Found handler", api->name);
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "API Unknown (0x%04x): No registered handler", req.hdr.type);
        fill_wrapper_ack(&resp, req.hdr.seq, 0x31, "Unable to find the API handler");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        quicktrack_uart_send(CONTROL_APPC_UART, buffer, len);
        goto done;
    }

    if (api->verify == NULL || (api->verify && api->verify(&req, &resp) == 0)) {
        indigo_logger(LOG_LEVEL_INFO, "API %s: Return ACK", api->name);
        fill_wrapper_ack(&resp, req.hdr.seq, 0x30, "ACK: Command received");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        quicktrack_uart_send(CONTROL_APPC_UART, buffer, len);
        free_packet_wrapper(&resp);
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "API %s: Failed to verify and return NACK", api->name);
        fill_wrapper_ack(&resp, req.hdr.seq, 1, "Unable to find the API handler");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        quicktrack_uart_send(CONTROL_APPC_UART, buffer, len);
        goto done;
    }

    if (api->handle && api->handle(&req, &resp) == 0) {
        indigo_logger(LOG_LEVEL_INFO, "API %s: Return execution result", api->name);
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        quicktrack_uart_send(CONTROL_APPC_UART, buffer, len);
    } else {
        indigo_logger(LOG_LEVEL_DEBUG, "API %s (0x%04x): No handle function", api ? api->name : "Unknown", req.hdr.type);
    }

done:
    free_packet_wrapper(&req);
    free_packet_wrapper(&resp);
    indigo_logger(LOG_LEVEL_DEBUG, "API %s: Complete", api ? api->name : "Unknown");
}

/* get input from control appc uart  */
static int control_get_input(char *inbuf, int bufLen, int *bp)
{
	while (quicktrack_uart_recv(CONTROL_APPC_UART, &inbuf[*bp], 1, BEKEN_NO_WAIT) > 0) {
		(*bp)++;
		if (*bp >= bufLen) {
			os_printf("Error: input buffer overflow\r\n");
			return 0;
		}
	}

	return 0;
}

static int control_uart_packet_complete(char *inbuf, int len)
{
	int parser = sizeof(struct message_hdr);
	int tlv_len;

	if (len < sizeof(struct message_hdr)) {
		return 0;
	}

	while (parser < len) {
		if (len - parser < 3) {
			while (parser < len) {
				if (inbuf[parser++] != 0) {
					return 0;
				}
			}
			return 1;
		}

		tlv_len = inbuf[parser + 2] & 0xff;
		if (len - parser < tlv_len + 3) {
			return 0;
		}

		parser += tlv_len + 3;
	}

	return 1;
}

static void control_drain_uart_input(char *inbuf, int bufLen, int *bp)
{
	int i;
	int pos;

	for (i = 0; i < CONTROL_APPC_UART_SETTLE_RETRY; i++) {
		pos = *bp;

		rtos_delay_milliseconds(CONTROL_APPC_UART_SETTLE_DELAY_MS);
		control_get_input(inbuf, bufLen, bp);

		if (*bp == pos) {
			break;
		}
	}
}

#else // BK_CONTROL_APP_C

/* Callback function of the QuickTrack API. */
static void control_receive_message(int sock, void *eloop_ctx, void *sock_ctx) {
    int ret;                          // return code
    int fromlen, len;                 // structure size and received length
    struct sockaddr_storage from;     // source address of the message
    char buffer[BUFFER_LEN]; // buffer to receive the message
    struct packet_wrapper req, resp;  // packet wrapper for the received message and response
    struct indigo_api *api = NULL;    // used for API search, validation and handler call

    /* Receive request */
    fromlen = sizeof(from);
    len = recvfrom(sock, buffer, BUFFER_LEN, 0, (struct sockaddr *) &from, (socklen_t*)&fromlen);
    if (len < 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Server: Failed to receive the packet");
        return ;
    } else {
        indigo_logger(LOG_LEVEL_DEBUG, "Server: Receive the packet");
    }
    tool_addr = (struct sockaddr_in *)&from;

    /* Parse request to HDR and TLV. Response NACK if parser fails. Otherwises, ACK. */
    memset(&req, 0, sizeof(struct packet_wrapper));
    memset(&resp, 0, sizeof(struct packet_wrapper));
    ret = parse_packet(&req, buffer, len);
    if (ret == 0) {
        indigo_logger(LOG_LEVEL_DEBUG, "Server: Parsed packet successfully");
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "Server: Failed to parse the packet");
        fill_wrapper_ack(&resp, req.hdr.seq, 0x31, "Unable to parse the packet");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);

        sendto(sock, (const char *)buffer, len, MSG_CONFIRM, (const struct sockaddr *) &from, fromlen);
        goto done;
    }

    /* Find API by ID. If API is not supported, assemble NACK. */
    api = get_api_by_id(req.hdr.type);
    if (api) {
        indigo_logger(LOG_LEVEL_DEBUG, "API %s: Found handler", api->name);
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "API Unknown (0x%04x): No registered handler", req.hdr.type);
        fill_wrapper_ack(&resp, req.hdr.seq, 0x31, "Unable to find the API handler");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        sendto(sock, (const char *)buffer, len, MSG_CONFIRM, (const struct sockaddr *) &from, fromlen);
        goto done;
    }

    /* Verify. Optional. If validation is failed, then return NACK. */
    if (api->verify == NULL || (api->verify && api->verify(&req, &resp) == 0)) {
        indigo_logger(LOG_LEVEL_INFO, "API %s: Return ACK", api->name);
        fill_wrapper_ack(&resp, req.hdr.seq, 0x30, "ACK: Command received");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        sendto(sock, (const char *)buffer, len, MSG_CONFIRM, (const struct sockaddr *) &from, fromlen);
        free_packet_wrapper(&resp);
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "API %s: Failed to verify and return NACK", api->name);
        fill_wrapper_ack(&resp, req.hdr.seq, 1, "Unable to find the API handler");
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        sendto(sock, (const char *)buffer, len, MSG_CONFIRM, (const struct sockaddr *) &from, fromlen);
        goto done;
    }

    /* Optional, use timer to handle the execution */
    /* Handle & Response. Call API handle(), assemble packet by response wrapper and send back to source address. */
    if (api->handle && api->handle(&req, &resp) == 0) {
        indigo_logger(LOG_LEVEL_INFO, "API %s: Return execution result", api->name);
        len = assemble_packet(buffer, BUFFER_LEN, &resp);
        sendto(sock, (const char *)buffer, len, MSG_CONFIRM, (const struct sockaddr *) &from, fromlen);
    } else {
        indigo_logger(LOG_LEVEL_DEBUG, "API %s (0x%04x): No handle function", api ? api->name : "Unknown", req.hdr.type);
    }

done:
    /* Clean up resource */
    free_packet_wrapper(&req);
    free_packet_wrapper(&resp);
    indigo_logger(LOG_LEVEL_DEBUG, "API %s: Complete", api ? api->name : "Unknown");
}

/* Show the usage */
static void usage() {
    printf("usage:\n");
    printf("app [-h] [-p<port number>] [-i<wireless interface>|-i<band>:<interface>[,<band>:<interface>]] [-a<hostapd path>] [-s<wpa_supplicant path>]\n\n");
    printf("usage:\n");
    printf("  -a = specify hostapd path\n");
    printf("  -d = debug received and sent message\n");
    printf("  -i = specify the interface. E.g., -i wlan0. Or, <band>:<interface>.\n       band can be 2 for 2.4GHz, 5 for 5GHz and 6 for 6GHz. E.g., -i 2:wlan0,2:wlan1,5:wlan32,5:wlan33\n");
    printf("  -p = port number of the application\n");
    printf("  -s = specify wpa_supplicant path\n\n");
}
#endif // BK_CONTROL_APP_C

/* Show the welcome message with role and version */
static void print_welcome() {
#ifdef _DUT_
    printf("Welcome to use QuickTrack Control App DUT version");
#else
    printf("Welcome to use Quicktrack Control App Platform version");
#endif

#ifdef _VERSION_
    printf(" %s.\n", CONTROLAPP_VERSION_STRING);
#else
    printf(".\n");
#endif
}

#ifdef CONTROLAPPC_LINUX_API
/* Parse the commandline parameters */
static int parse_parameters(int argc, char *argv[]) {
    int c, ifs_configured = 0;
    char buf[128];

#ifdef _VERSION_
    while ((c = getopt(argc, argv, "a:s:i:hp:dcv")) != -1)
#else
    while ((c = getopt(argc, argv, "a:s:i:hp:dc")) != -1)
#endif
    {
        switch (c) {
        case 'a':
            set_hapd_full_exec_path(optarg);
            break;
        case 'c':
            capture_packet = 1;
            break;
        case 'd':
            debug_packet = 1;
            break;
        case 'h':
            usage();
            return 1;
        case 'i':
            if (set_wireless_interface(optarg) == 0) {
                ifs_configured = 1;
            }
            break;
        case 'p':
            set_service_port(atoi(optarg));
            break;
        case 's':
            set_wpas_full_exec_path(optarg);
            break;
#ifdef _VERSION_
        case 'v':
            return 1;
#endif
        }
    }

    if (optind < argc) {
        printf("\nInvalid option %s\n", argv[optind]);
        usage();
        return 1;
    }

    if (ifs_configured == 0) {
#ifdef DEFAULT_APP_INTERFACES_PARAMS
#ifdef _OPENWRT_
        if (detect_third_radio())
            snprintf(buf, sizeof(buf), "%s", DEFAULT_APP_6E_INTERFACES_PARAMS);
        else
#endif
            snprintf(buf, sizeof(buf), "%s", DEFAULT_APP_INTERFACES_PARAMS);
        printf("\nUse default interface parameters %s.\n", buf);
        set_wireless_interface(buf);
#else
        usage();
        printf("\nWe need to specify the interfaces with -i.\n");
        return 1;
#endif
    }

    return 0;
}
#endif // CONTROLAPPC_LINUX_API

#ifdef CONTROLAPPC_LINUX_API
static void handle_term(int sig, void *eloop_ctx, void *signal_ctx) {
    indigo_logger(LOG_LEVEL_INFO, "Signal %d received - terminating\n", sig);
    eloop_terminate();
    vendor_deinit();
}
#endif

int controlappc_main(int argc, char* argv[]) {
#ifdef BK_CONTROL_APP_C
#else
    int service_socket = -1;
#endif

    /* Welcome message */
    print_welcome();


    /* Initiate the application */
#ifdef CONTROLAPPC_LINUX_API
    set_wireless_interface(WIRELESS_INTERFACE_DEFAULT);       // Set default wireless interface information
    set_hapd_full_exec_path(HAPD_EXEC_FILE_DEFAULT);          // Set default hostapd execution file path
    set_hapd_ctrl_path(HAPD_CTRL_PATH_DEFAULT);               // Set default hostapd control interface path
    set_hapd_global_ctrl_path(HAPD_GLOBAL_CTRL_PATH_DEFAULT); // Set default hostapd global control interface path
    set_hapd_conf_file(HAPD_CONF_FILE_DEFAULT);               // Set default hostapd configuration file path
    set_wpas_full_exec_path(WPAS_EXEC_FILE_DEFAULT);          // Set default wap_supplicant execution file path
    set_wpas_ctrl_path(WPAS_CTRL_PATH_DEFAULT);               // Set default wap_supplicant control interface path
    set_wpas_global_ctrl_path(WPAS_GLOBAL_CTRL_PATH_DEFAULT); // Set default wap_supplicant global control interface path
    set_wpas_conf_file(WPAS_CONF_FILE_DEFAULT);               // Set default wap_supplicant configuration file path
#endif

#ifdef CONTROLAPPC_LINUX_API
    /* Parse the application arguments */
    if (parse_parameters(argc, argv)) {
        return 0;
    }
#endif

#if !defined(_OPENWRT_) && defined(CONTROLAPPC_LINUX_API)
    system("mkdir -p /etc/hostapd/");
#endif

    /* Print the run-time information */
#ifdef TBD
    indigo_logger(LOG_LEVEL_INFO, "QuickTrack control app running at: %d", get_service_port());
#endif
    indigo_logger(LOG_LEVEL_INFO, "Wireless Interface:" );
    //show_wireless_interface_info();
#ifdef TBD
    indigo_logger(LOG_LEVEL_INFO, "hostapd Path: %s (%s)", get_hapd_full_exec_path(), get_hapd_exec_file());
    indigo_logger(LOG_LEVEL_INFO, "wpa_supplicant Path: %s (%s)", get_wpas_full_exec_path(), get_wpas_exec_file());
#endif

    /*
     * The following information may not help anymore since
     * - we support multiple vaps
     * - remote udp port is known only when receive the control interface TLV
    indigo_logger(LOG_LEVEL_INFO, "Hostapd Global Control Interface: %s", get_hapd_global_ctrl_path());
    indigo_logger(LOG_LEVEL_INFO, "Hostapd Control Interface: %s", get_hapd_ctrl_path());
    indigo_logger(LOG_LEVEL_INFO, "WPA Supplicant Control Interface: %s", get_wpas_ctrl_path());
    */

    /* Register the callback */
    register_apis();

    /* Intiate the vendor's specific startup commands */
#ifdef TBD
    vendor_init();
#endif

#ifdef BK_CONTROL_APP_C
    // Enable BW 40 @ 2.4G
    bk_wifi_capa_config(WIFI_CAPA_ID_B40_EN, 1);
    /* Lock CPU to full speed for QuickTrack Test (SAE/4-way handshake timing) */
    bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT, PM_CPU_FRQ_240M);

    /* Disable STA power save to avoid losing SAE/EAPOL frames during handshake */
    bk_wifi_sta_pm_disable();

    /* init uart */
    rtos_init_semaphore(&control_appc_uart_rx_sema, 10);

    /* init quicktrack uart */
    quicktrack_uart_init(CONTROL_APPC_UART);

    bk_uart_register_rx_isr(CONTROL_APPC_UART, control_appc_uart_rx_callback, NULL);

    /* disable uart rx fifo */
    bk_uart_disable_sw_fifo(CONTROL_APPC_UART);

    for (;;) {
            while (1) {
            int ret = rtos_get_semaphore(&control_appc_uart_rx_sema, CONTROL_APPC_UART_TIMEOUT);

            /* get input from uart */
            control_get_input((char *)control_appc_uart_rx_buf, CONTROL_APPC_UART_BUF_SZ, &uart_rx_pos);

            /* if timedout, break */
            if (ret != kNoErr)
                break;
        }

        /* uart rx timeout, this may be a complete control message */
        if (uart_rx_pos > 0) {
            control_drain_uart_input((char *)control_appc_uart_rx_buf, CONTROL_APPC_UART_BUF_SZ, &uart_rx_pos);
            if (!control_uart_packet_complete((char *)control_appc_uart_rx_buf, uart_rx_pos)) {
                continue;
            }
            // print_hex_dump("UARTRX: ", control_appc_uart_rx_buf, uart_rx_pos);
            control_receive_uart_message((char *)control_appc_uart_rx_buf, uart_rx_pos);

            memset(control_appc_uart_rx_buf, 0, sizeof(control_appc_uart_rx_buf));
            /* reset rx pos, forward to next control message */
            uart_rx_pos = 0;
        }
    }

    controlapp_eloop_terminate();
#else // BK_CONTROL_APP_C
    /* Start eloop */
    eloop_init(NULL);
#ifdef CONTROLAPPC_LINUX_API
    /* Register SIGTERM */
    eloop_register_signal(SIGINT, handle_term, NULL);
    eloop_register_signal(SIGTERM, handle_term, NULL);
#endif
    /* Bind the service port and register to eloop */
    service_socket = controlappc_service_socket_init();
    if (service_socket >= 0) {
        eloop_run();
    } else {
        indigo_logger(LOG_LEVEL_INFO, "Failed to initiate the UDP socket");
    }

    /* Stop eloop */
    eloop_destroy();
    indigo_logger(LOG_LEVEL_INFO, "ControlAppC stops");
    if (service_socket >= 0) {
        indigo_logger(LOG_LEVEL_INFO, "Close service port: %d", get_service_port());
        close(service_socket);
    }
#endif //BK_CONTROL_APP_C

    return 0;
}

void controlappc_start(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    bk_err_t ret;

#ifndef BK_CONTROL_APP_C
    if (NULL == controlappc_eloop_thread_handle) {
        ret = rtos_create_thread(&controlappc_eloop_thread_handle,
                               BEKEN_DEFAULT_WORKER_PRIORITY,
                               "controlapp_eloop",
                               (beken_thread_function_t)controlapp_eloop_thread,
                               4096,
                               (beken_thread_arg_t)0);
        if (kNoErr != ret) {
            BK_LOGE("controlappc", "create controlapp_eloop thread failed: %d\r\n", ret);
            return;
        }
    }
#endif

    if (NULL == controlappc_thread_handle) {
        ret = rtos_create_thread(&controlappc_thread_handle,
                               100,
                               "controlappc",
                               (beken_thread_function_t)controlappc_main,
                               4096,
                               (beken_thread_arg_t)0);
        if (kNoErr != ret) {
            BK_LOGE("controlappc", "create controlappc thread failed: %d\r\n", ret);
            return;
        }
    }
}
