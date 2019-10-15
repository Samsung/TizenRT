/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef _RTW_SDIO_H_
#define _RTW_SDIO_H_

#include <drv_types.h> /* struct dvobj_priv and etc. */

u8 rtw_sdio_read_cmd52(struct dvobj_priv *, u32 addr, void *buf, size_t len);
u8 rtw_sdio_read_cmd53(struct dvobj_priv *, u32 addr, void *buf, size_t len);
u8 rtw_sdio_write_cmd52(struct dvobj_priv *, u32 addr, void *buf, size_t len);
u8 rtw_sdio_write_cmd53(struct dvobj_priv *, u32 addr, void *buf, size_t len);

#endif /* _RTW_SDIO_H_ */
