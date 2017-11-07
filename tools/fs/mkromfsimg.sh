#!/bin/bash
###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################
############################################################################
# tools/fs/mkromfsimg.sh
#
#   Copyright (C) 2014 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

# Make sure we understand where we are

if [ ! -f tools/mkromfsimg.sh ]; then
  cd .. || { echo "ERROR: cd .. failed"; exit 1; }
  if [ ! -f tools/mkromfsimg.sh ]; then
    error "This script must be executed from the top-level apps/ directory"
    exit 1
  fi
fi


# Environmental stuff

topdir=$PWD
buildpath=${topdir}/../build
contentsdir=${topdir}/../tools/fs/contents
romfsimg=${buildpath}/output/bin/romfs.img

if [ $# -gt 0 ]; then
  contentsdir=${1}; shift
fi

# Sanity checks
if [ ! -d "${contentsdir}" ]; then
  echo "$0 ERROR: Directory ${contentsdir} does not exist"
  exit 1
fi

genromfs -h 1>/dev/null 2>&1 || { \
  echo "Host executable genromfs not available in PATH"; \
  echo "You may need to download in from http://romfs.sourceforge.net/"; \
  exit 1; \
}

# Now we are ready to make the ROMFS image

genromfs -f ${romfsimg} -d ${contentsdir} -x .gitignore -V "TinyAraROMVol" || { echo "genromfs failed" ; exit 1 ; }

# And, finally, create the header file
echo "${romfsimg} was made."
