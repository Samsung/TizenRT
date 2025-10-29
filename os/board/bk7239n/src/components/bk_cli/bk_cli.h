// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/bk_include.h>
#include <driver/uart.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CLI_UART                      CONFIG_UART_CLI_BKREG_PORT


#define RX_WAIT                       BEKEN_WAIT_FOREVER
#define SEND_WAIT                     BEKEN_WAIT_FOREVER

#define RET_CHAR                      '\n'
#define END_CHAR		              '\r'
#define PROMPT			              "\r\n# "
#define EXIT_MSG		              "exit"

#define NUM_BUFFERS		              1

#ifndef CONFIG_MAX_COMMANDS
#define MAX_COMMANDS	              255
#else
#define MAX_COMMANDS     CONFIG_MAX_COMMANDS
#endif

#ifdef CONFIG_KEYVALUE
#define INBUF_SIZE                    1024
#else
#define INBUF_SIZE                    128
#endif
#define OUTBUF_SIZE                   2048

#define LOG_SERVICE_BUFLEN            100

#ifndef CFG_CLI_DEBUG
#define CFG_CLI_DEBUG                 1
#endif

#if CFG_CLI_DEBUG
#define CLI_GETCHAR_TIMEOUT           (120000)
#define CLI_COMMAND_IS_RUNNING        (1)
#else
#define CLI_GETCHAR_TIMEOUT           BEKEN_NEVER_TIMEOUT
#endif

/** Structure for registering CLI commands */
struct cli_command
{
    /** The name of the CLI command */
    const char *name;
    /** The help text associated with the command */
    const char *help;
    /** The function that should be invoked for this command. */
    void (*function) (char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
};

struct cli_st
{
    int initialized;
    const struct cli_command *commands[MAX_COMMANDS];
    unsigned int num_commands;
#if 1//((!defined(CONFIG_SHELL_ASYNCLOG)) || defined(CONFIG_ATE_TEST))
    int echo_disabled;

    unsigned int bp;	/* buffer pointer */
    unsigned char inbuf[INBUF_SIZE];

    unsigned char outbuf[OUTBUF_SIZE];
#endif
} ;

#define cmd_printf(...) do{\
                                if (xWriteBufferLen > 0) {\
                                    snprintf(pcWriteBuffer, xWriteBufferLen, __VA_ARGS__);\
                                    xWriteBufferLen-= os_strlen(pcWriteBuffer);\
                                    pcWriteBuffer+= os_strlen(pcWriteBuffer);\
                                }\
                             }while(0)


#define CLI_ARGS char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv

#define is_print(c)           ((uint8_t)c >= 0x20 && (uint8_t)c <= 0x7f)


/** Register a CLI command
 *
 * This function registers a command with the command-line interface.
 *
 * \param[in] command The structure to register one CLI command
 * \return 0 on success
 * \return 1 on failure
 */
int cli_register_command(const struct cli_command *command);

/** Unregister a CLI command
 *
 * This function unregisters a command from the command-line interface.
 *
 * \param[in] command The structure to unregister one CLI command
 * \return 0 on success
 * \return 1 on failure
 */
int cli_unregister_command(const struct cli_command *command);



/** Stop the CLI thread and carry out the cleanup
 *
 * \return kNoErr on success
 * \return error code otherwise.
 *
 */
int cli_stop(void);

/** Register a batch of CLI commands
 *
 * Often, a module will want to register several commands.
 *
 * \param[in] commands Pointer to an array of commands.
 * \param[in] num_commands Number of commands in the array.
 * \return 0 on success
 * \return 1 on failure
 */
int cli_register_commands(const struct cli_command *commands, int num_commands);

/** Unregister a batch of CLI commands
 *
 * \param[in] commands Pointer to an array of commands.
 * \param[in] num_commands Number of commands in the array.
 * \return 0 on success
 * \return 1 on failure
 */
int cli_unregister_commands(const struct cli_command *commands,
                            int num_commands);

/* Get a CLI msg
 *
 * If an external input task wants to use the CLI, it can use
 * cli_get_cmd_buffer() to get a command buffer that it can then
 * submit to the CLI later using cli_submit_cmd_buffer().
 *
 * \param buff Pointer to a char * to place the buffer pointer in.
 * \return 0 on success
 * \return error code otherwise.
 */
int cli_getchar(char *inbuf);
int cli_getchars(char *inbuf, int len);
int cli_get_all_chars_len(void);
int cli_getchars_prefetch(char *inbuf, int len);

/* Send CLI output msg
 *
 * \param buff Pointer to a char * buffer.
 * \return 0 on success
 * \return error code otherwise.
 */
int cli_printf(const char *buff, ...);


int cli_init(void);

#ifdef CONFIG_KEYVALUE
#define INBUF_SIZE                    1024
#else
#define INBUF_SIZE                    128
#endif
#define OUTBUF_SIZE                   2048

#ifdef __cplusplus
}
#endif
