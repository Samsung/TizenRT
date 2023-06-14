#
# Copyright (c) 2017-2019, ARM Limited and Contributors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# Neither the name of ARM nor the names of its contributors may be used
# to endorse or promote products derived from this software without specific
# prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

COMPILER_RT_SRCS	:=	lib/compiler-rt/builtins/popcountdi2.c		\
				lib/compiler-rt/builtins/popcountsi2.c

ifeq (${ARCH},aarch32)
COMPILER_RT_SRCS	+=	lib/compiler-rt/builtins/arm/aeabi_ldivmod.S	\
				lib/compiler-rt/builtins/arm/aeabi_uldivmod.S	\
				lib/compiler-rt/builtins/ctzdi2.c		\
				lib/compiler-rt/builtins/divdi3.c		\
				lib/compiler-rt/builtins/divmoddi4.c		\
				lib/compiler-rt/builtins/lshrdi3.c		\
				lib/compiler-rt/builtins/udivmoddi4.c
endif

ifeq (${ARCH},mips)
COMPILER_RT_SRCS        :=      lib/compiler-rt/builtins/ctzsi2.c               \
                                lib/compiler-rt/builtins/clzsi2.c               \
                                lib/compiler-rt/builtins/udivmoddi4.c           \
                                lib/compiler-rt/builtins/udivdi3.c              \
                                lib/compiler-rt/builtins/umoddi3.c
endif
