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
