/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
// command, type, handler
SL_KEY_TEST_POOL("set_asym_rw", SL_KEY_TYPE_SET_ASYM_RW, sl_handle_key_set_asym_rw)
SL_KEY_TEST_POOL("set_ecc_rw", SL_KEY_TYPE_SET_ECC_RW, sl_handle_key_set_ecc_rw)
SL_KEY_TEST_POOL("gen_asym_rw", SL_KEY_TYPE_GEN_ASYM_RW, sl_handle_key_gen_asym_rw)
