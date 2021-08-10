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
SL_SS_TEST_POOL("write", SL_SS_TYPE_WRITE, sl_handle_ss_write)
SL_SS_TEST_POOL("read", SL_SS_TYPE_READ, sl_handle_ss_read)
SL_SS_TEST_POOL("delete", SL_SS_TYPE_DELETE, sl_handle_ss_delete)
