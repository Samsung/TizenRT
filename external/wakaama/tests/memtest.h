/*******************************************************************************
 *
 * Copyright (c) 2015 Bosch Software Innovations GmbH, Germany.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Bosch Software Innovations GmbH - Please refer to git log
 *
 *******************************************************************************/

#ifndef MEMTEST_H_
#define MEMTEST_H_

#ifdef MEMORY_TRACE
#include "memtrace.h"
static int blocks_before = 0;
static size_t size_before = 0;
static int blocks_after = 0;
static size_t size_after = 0;
#define MEMORY_TRACE_BEFORE trace_status(&blocks_before, &size_before)
#define MEMORY_TRACE_AFTER(OP) { trace_status(&blocks_after, &size_after); CU_ASSERT(blocks_before OP blocks_after); CU_ASSERT(size_before OP size_after); }
#define MEMORY_TRACE_AFTER_EQ { trace_status(&blocks_after, &size_after); CU_ASSERT_EQUAL(blocks_before, blocks_after); CU_ASSERT_EQUAL(size_before, size_after); }
#else
#define MEMORY_TRACE_BEFORE
#define MEMORY_TRACE_AFTER(OP)
#define MEMORY_TRACE_AFTER_EQ
#endif

#endif /* MEMTEST_H_ */
