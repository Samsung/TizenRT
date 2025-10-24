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

.macro PUSH_SVC_REG
	SUB     SP, SP, #18 * 4
	STMIA   SP, {R0 - R12}
	MOV     R0, SP
	MRS     R6, SPSR
	STR     LR, [R0, #15*4]
	STR     R6, [R0, #16*4]
	STR     SP, [R0, #13*4]
	STR     LR, [R0, #14*4]
	MRS     R6, CPSR
	STR     R6, [R0, #17*4]
.endm

#ifdef __cplusplus
}
#endif
