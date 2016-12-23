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

#ifndef __INCLUDE_DMA_H
#define __INCLUDE_DMA_H

#ifndef CONFIG_S5J_DMA
#error DMA IS NOT ENABLED
#endif

int dma_memcpy(void *dst, void *src, unsigned int size, unsigned int ch_num);
s16 dma_allocate_m2m_channel(void);
void dma_free_m2m_channel(s16 ch);

#endif
