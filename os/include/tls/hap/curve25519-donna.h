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
//
//  curve25519-donna.h
//
//  Created by Wolfgang on 1/1/14.
//  Copyright (c) 2014 Wolfgang. All rights reserved.
//

#ifndef curve25519_donna_h
#define curve25519_donna_h

typedef uint8_t u8;
extern int curve25519_donna(u8 *, const u8 *, const u8 *);

#endif
