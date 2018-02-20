/* Copyright 2018 Byoungtae Cho <bt.cho@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __adb_h__
#define __adb_h__

#define ADB_USE_POLLWORKER

#ifdef CONFIG_SYSTEM_ADB_DEBUG
#	define ADB_DEBUG
#endif

#ifdef ADB_DEBUG
#	define D(fmt, ...)			dbgmsg(__func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#	define D(fmt, ...)
#endif

#define MAX_PAYLOAD				4096

#define A_SYNC					0x434e5953
#define A_CNXN					0x4e584e43
#define A_OPEN					0x4e45504f
#define A_OKAY					0x59414b4f
#define A_CLSE					0x45534c43
#define A_WRTE					0x45545257
#define A_AUTH					0x48545541

#define A_VERSION				0x01000000

/* for usb */
#define ADB_CLASS				0xff
#define ADB_SUBCLASS			0x42
#define ADB_PROTOCOL			0x01

/* AUTH packets first argument */
/* Request */
#define ADB_AUTH_TOKEN			1
/* Response */
#define ADB_AUTH_SIGNATURE		2
#define ADB_AUTH_RSAPUBLICKEY	3


#define ADB_SHELL_IN_PATH		"/dev/adb_shell_in"
#define ADB_SHELL_OUT_PATH		"/dev/adb_shell_out"
#define ADB_FSYNC_IN_PATH		"/dev/adb_fsync_in"
#define ADB_FSYNC_OUT_PATH		"/dev/adb_fsync_out"
#define ADB_ARBITER_PATH		"/dev/adb_arbiter"

typedef struct atransport		atransport;
typedef struct amessage			amessage;
typedef struct apacket			apacket;
typedef struct aconnection		aconnection;
typedef struct aarbiter			aarbiter;
typedef struct amsg				amsg;
typedef enum   media			media;
typedef enum   state			state;

struct amessage {
	unsigned command;		/* command identifier constant		*/
	unsigned arg0;			/* first argument					*/
	unsigned arg1;			/* second argument					*/
	unsigned data_length;	/* length of payload (0 is allowed) */
	unsigned data_check;	/* checksum of data payload			*/
	unsigned magic;			/* command ^ 0xffffffff				*/
};

struct amsg {
	uint16_t	msgId;
	void		*p;
};

struct apacket {
	amessage	msg;
	uint8_t		data[MAX_PAYLOAD];
	uint32_t	len;
};

enum media {
	kTransportNet,
};

struct atransport {
	int (*read)(int fd, uint16_t len, uint8_t *data);
	int (*write)(int fd, uint16_t len, uint8_t *data);

	int fd;
	int online;
	media type;
};

enum state {
	ADB_OFFLINE = 0,
	ADB_CLOSED,
	ADB_OPEN,
	ADB_SHELL,
	ADB_COMMAND,
	ADB_FILE,
};

struct aconnection {
	char name[32];
	uint32_t local;
	uint32_t remote;
	int auth;
	int arbiter;
	state status;
	void (*execute)(const char *data, int len);
};

struct aarbiter {
	aconnection *conn;
	atransport  *t;
};

void send_msg(aconnection *conn, atransport *t, uint8_t *data, uint32_t len);
void send_close(aconnection *conn, atransport *t);
aconnection *getconn(void);

void dbgmsg(const char *func, int line, char *fmt, ...);

#endif
