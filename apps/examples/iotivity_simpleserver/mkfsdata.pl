#!/usr/bin/perl
###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
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
#
# Extracted from uIP which has a license that is compatible with NuttX.
# There is no authorship, copyright, or licensing information in the
# original file.  Possibly written by Adam Dunkels.
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

open(OUTPUT, "> ./iotivity/apps/file_data.c");

chdir("./iotivity/apps/data");

opendir(DIR, ".");
@files =  grep { !/^\./ && !/(CVS|~)/ } readdir(DIR);
closedir(DIR);

print(OUTPUT "#ifndef NULL\n#define NULL 0\n#endif\n\n");

foreach $file (@files) {

    if(-d $file && $file !~ /^\./) {
        print "Processing directory $file\n";
        opendir(DIR, $file);
        @newfiles =  grep { !/^\./ && !/(CVS|~)/ } readdir(DIR);
        closedir(DIR);
        printf "Adding files @newfiles\n";
        @files = (@files, map { $_ = "$file/$_" } @newfiles);
        next;
    }
}

foreach $file (@files) {
    if(-f $file) {

        print "Adding file $file\n";

        open(FILE, $file) || die "Could not open file $file\n";

        $file =~ s-^-/-;
        $fvar = $file;
        $fvar =~ s-/-_-g;
        $fvar =~ s-\.-_-g;
        # for AVR, add PROGMEM here
        print(OUTPUT "static const unsigned char data".$fvar."[] =\n");
        print(OUTPUT "{\n  /* $file */\n\n  ");
      #  for($j = 0; $j < length($file); $j++) {
      #      printf(OUTPUT "%#02x, ", unpack("C", substr($file, $j, 1)));
      #  }
      #  printf(OUTPUT "0x00,\n  ");

        $i = 0;
        while(read(FILE, $data, 1)) {
            printf(OUTPUT "%#02x, ", unpack("C", $data));
            $i++;
            if($i == 10) {
                print(OUTPUT "\n");
                $i = 0;
                print(OUTPUT "  ");
            }
        }
#       print(OUTPUT "0x00\n};\n\n");
        print(OUTPUT "\n};\n\n");
        close(FILE);
        push(@fvars, $fvar);
        push(@pfiles, $file);
    }
}

for($i = 0; $i < @fvars; $i++) {
    $file = $pfiles[$i];
    $fvar = $fvars[$i];

    if($i == 0) {
        $prevfile = "NULL";
    } else {
        $prevfile = "file" . $fvars[$i - 1];
    }
}

# print(OUTPUT "#define HTTPD_FS_ROOT     file$fvars[$i - 1]\n\n");
# print(OUTPUT "const int g_data_numfiles = $i;\n");
