/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <vector>
#include "cstdlib"
#include "cwchar"
#include "cerrno"
#include "limits"
#include "stdexcept"
#ifdef _LIBCPP_MSVCRT
#include "support/win32/support.h"
#endif // _LIBCPP_MSVCRT
#include <stdio.h>

_LIBCPP_BEGIN_NAMESPACE_STD
   template class _LIBCPP_CLASS_TEMPLATE_INSTANTIATION_VIS __vector_base_common<true>;
_LIBCPP_END_NAMESPACE_STD
