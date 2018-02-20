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

#ifndef __file_h__
#define __file_h__

#define htoll(x) (x)
#define ltohl(x) (x)

#define MKID(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

#define ID_STAT MKID('S', 'T', 'A', 'T')
#define ID_LIST MKID('L', 'I', 'S', 'T')
#define ID_ULNK MKID('U', 'L', 'N', 'K')
#define ID_SEND MKID('S', 'E', 'N', 'D')
#define ID_RECV MKID('R', 'E', 'C', 'V')
#define ID_DENT MKID('D', 'E', 'N', 'T')
#define ID_DONE MKID('D', 'O', 'N', 'E')
#define ID_DATA MKID('D', 'A', 'T', 'A')
#define ID_OKAY MKID('O', 'K', 'A', 'Y')
#define ID_FAIL MKID('F', 'A', 'I', 'L')
#define ID_QUIT MKID('Q', 'U', 'I', 'T')

typedef union syncmsg syncmsg;

union syncmsg {
	unsigned id;
	struct {
		unsigned id;
		unsigned namelen;
	} req;
	struct {
		unsigned id;
		unsigned mode;
		unsigned size;
		unsigned time;
	} stat;
	struct {
		unsigned id;
		unsigned mode;
		unsigned size;
		unsigned time;
		unsigned namelen;
	} dent;
	struct {
		unsigned id;
		unsigned size;
	} data;
	struct {
		unsigned id;
		unsigned msglen;
	} status;
};

void adb_initfile(void);
void execute_fsync(const char *data, int len);
static int adb_fsync(int argc, char **argv);
void fsync_wakeup(void);

#endif
