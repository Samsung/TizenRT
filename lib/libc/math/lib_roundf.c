/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
/************************************************************************
 * lib/math/lib_roundf.c
 *
 * This file is a part of NuttX:
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *             (C) 2012 Petteri Aimonen <jpa@nx.mail.kapsi.fi>
 *
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <math.h>

/************************************************************************
 * Public Functions
 ************************************************************************/

float roundf(float x)
{
	float f = modff(x, &x);
	if (x <= 0.0f && f <= -0.5f) {
		x -= 1.0f;
	}

	if (x >= 0.0f && f >= 0.5f) {
		x += 1.0f;
	}

	return x;
}
