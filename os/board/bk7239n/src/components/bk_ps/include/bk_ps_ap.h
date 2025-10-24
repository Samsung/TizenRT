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
	extern void ap_ps_enable_set ( void );
	extern void ap_ps_enable_clear ( void );
	extern UINT32 ap_ps_enable_get ( void );
	UINT32 ap_if_ap_rf_sleep ( void );
	void ap_idle_msg_process( UINT8 );
	
#ifdef __cplusplus
}
#endif
