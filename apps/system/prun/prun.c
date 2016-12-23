/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * apps/system/prun/prun.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <debug.h>

#include <apps/prun.h>

#include "pexec.h"
#include "pedefs.h"
#include "prun.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: prun
 *
 * Description:
 *   Execute/interpret a P-Code file.  This function does not return until
 *   the P-code program terminates or until a fatal error occurs.
 *
 * Input Parameters:
 *   exepath - The full path to the P-Code binary.
 *   varsize - Size of the P-Code variable stack
 *   strsize - the size of the P-Code string stack.
 *
 * Returned Value:
 *   OK if the P-Code program successfully terminated; A negated errno value
 *   is returned on the event of any failure.
 *
 ****************************************************************************/

int prun(FAR char *exepath, size_t varsize, size_t strsize)
{
	FAR struct pexec_s *st;
	int errcode;
	int ret = OK;

	/* Load the POFF file into memory */

	st = pload(exepath, varsize, varsize);
	if (!st) {
		bdbg("ERROR: Could not load %s\n", exepath);
		return -ENOEXEC;
	}

	bvdbg("Loaded %s\n", exepath);

	/* Execute the P-Code program until a stopping condition occurs */

	for (;;) {
		/* Execute the instruction; Check for exceptional conditions */

		errcode = pexec(st);
		if (errcode != eNOERROR) {
			break;
		}
	}

	if (errcode != eEXIT) {
		/* REVISIT: Select a more appropriated return errocode */

		bdbg("ERROR: Runtime error 0x%02x -- Execution Stopped\n", errcode);
		ret = -ENOEXEC;
	}

	/* Clean up resources used by the interpreter */

	bvdbg("Execution terminated\n");
	pexec_release(st);
	return ret;
}
