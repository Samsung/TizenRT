// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_include.h>
#include <components/usb_types.h>

/*******************************************************************************
* Function Declarations
*******************************************************************************/
int usb_osi_funcs_init(void *osi_funcs);

bk_err_t bk_usb_register_class_driver(uint8_t usb_prot_mode, void *drvier_info);
bk_err_t bk_usb_unregister_class_driver(uint8_t usb_prot_mode, void *drvier_info);

bk_err_t bk_usb_ip_init(uint8_t usb_prot_mode);
bk_err_t bk_usb_ip_deinit (void);

void *bk_usb_init_pipe(           void *hBus, void *pUsbDevice, uint8_t *ep_descriptors_buffer);
void bk_usb_deinit_pipe(void *pipe_ptr);

bk_err_t bk_usb_transfer_request(uint8_t tranfer_mode, void *parameter);
bk_err_t bk_usb_cancel_transfer(uint8_t tranfer_mode, void *parameter);

#ifdef __cplusplus
}
#endif
