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

#ifndef __INCLUDE_COMPRESSION_H
#define __INCLUDE_COMPRESSION_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

enum compression_formats {
	COMPRESSION_TYPE_NONE = 0,
	COMPRESSION_TYPE_LZMA,
	COMPRESSION_TYPE_MINIZ,
	COMPRESSION_TYPE_MAX = COMPRESSION_TYPE_MINIZ,
};

/* Compression header struct */
struct s_header {
	int size_header;			/* Size of Compression header */
	int compression_format;			/* Compression Format */
	int blocksize;				/* Block Size used for file compression */
	int sections;				/* No. of compressed blocks for this file */
	int binary_size;			/* Uncompressed file size */
	int secoff[];				/* Offsets to Compressed blocks */
};
#endif						/* __INCLUDE_COMPRESSION_H */
