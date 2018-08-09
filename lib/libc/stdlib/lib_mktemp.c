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
 *
 * Copyright © 2005-2014 Rich Felker, et al.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/stat.h>

#include "lib_internal.h"

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: mktemp
 *
 * Description:
 *   The use of mktemp is dangerous; use mkstemp instead.
 *   The mktemp() function generates a unique temporary filename from template.
 *   The last six characters of template must be XXXXXX and these are replaced
 *   with a  string  that  makes the filename unique.
 *   Since it will be modified, template must not be a string constant,
 *   but should be declared as a character array.
 *
 * Return Value:
 *   The mktemp() function always returns template.
 *   If a unique name was created,  the  last six  bytes  of template will have
 *   been modified in such a way that the resulting name is unique
 *   (i.e., does not exist already) If a unique name could not be created,
 *   template is made an empty string, and errno is set to indicate the error.
 *
 ****************************************************************************/

FAR char *mktemp(FAR char *path_template)
{
	size_t len;
	int retries = 100;
	struct stat st;

	if (!path_template) {
		set_errno(EINVAL);
		return NULL;
	}

	len = strlen(path_template);

	/* Check the number of X's at the end of the template */

	if (len < 6 || memcmp((path_template + len - 6), "XXXXXX", 6)) {
		/* No Xs?  There should always really be 6 */
		set_errno(EINVAL);
		*path_template = '\0';
		return path_template;
	}

	/* Then loop until we find a unique file name */

	do {
		/* Form the candidate file name */

		__randname((path_template + len - 6), 6);

		/* Check whether the file with name generated exist or not */

		if (stat(path_template, &st)) {
			if (get_errno() != ENOENT) {
				*path_template = '\0';
			}
			return path_template;
		}
	} while (--retries);

	/* We could not find an unique filename */

	*path_template = '\0';
	set_errno(EEXIST);

	return path_template;
}
