#!/usr/bin/env python

from __future__ import print_function
import argparse
import subprocess
import os
import re
import struct
import sys
import hashlib
import binascii

ARMINO_OBJCOPY = "arm-none-eabi-objcopy"
ARMINO_READELF = "arm-none-eabi-readelf"
ARMINO_NM = "arm-none-eabi-nm"

def replace_string_in_file(file_path, old_string, new_string):
    with open(file_path, 'r') as file:
        content = file.read()
    content = content.replace(old_string, new_string)
    with open(file_path, 'w') as file:
        file.write(content)

def RunCmd(cmd):
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate()
    print(out)
    if (p.returncode):
       print(err)
       exit(1)

def Prepost(sourceDir, outputDir):
    RunCmd("cp %s %s" % (sourceDir + "/bootloader/bootloader.bin", outputDir + "/bootloader.bin"))
    RunCmd("cp %s %s" % (sourceDir + "/configuration.json", outputDir + "/configuration.json"))
    replace_string_in_file(outputDir + "/configuration.json", "BOOTLOADER_IMAGE", outputDir + "/bootloader.bin")
    replace_string_in_file(outputDir + "/configuration.json", "APP_IMAGE", outputDir + "/app.bin")

def Concatenate(sourceDir, outputDir):
    RunCmd(ARMINO_OBJCOPY + " -O binary " + outputDir + "/tinyara.axf " + outputDir + "/app.bin")
    
    # For Test
    #RunCmd(ARMINO_OBJCOPY + " -O binary " + sourceDir + "/app_test.elf " + outputDir + "/app.bin")
    
    RunCmd(ARMINO_READELF + " -a -h -l -S -g -s " + outputDir + "/tinyara.axf " + " > " + outputDir + "/app.txt")
    RunCmd(ARMINO_READELF + " -n -l -C -a -A -g " + outputDir + "/tinyara.axf " + " > " + outputDir + "/app.nm")

    packetCmd = sourceDir + "/image_tool/cmake_Gen_image genfile -injsonfile " + outputDir + "/configuration.json"
    packetCmd += " -infile " + outputDir + "/bootloader.bin" + " " + outputDir + "/app.bin"
    packetCmd += " -outfile " + outputDir + "/all-app_original.bin"
    RunCmd(packetCmd)

    crcCmd = sourceDir + "/image_tool/cmake_encrypt_crc -crc " + outputDir + "/all-app_original.bin"
    RunCmd(crcCmd)

    RunCmd("mv %s %s" % (outputDir + "/all-app_original_crc.bin", outputDir + "/all-app.bin"))


def main():
    global toolsDir
    global buildDir
    parser = argparse.ArgumentParser(description='Beken Packet Wrapper')

    parser.add_argument('--tools-dir', help='Tools Dir')
    parser.add_argument('--build-dir', help='Build Dir')
    args = parser.parse_args()

    toolsDir = args.tools_dir
    buildDir = args.build_dir + "/output/bin"

    #print("Tools Dir %s" % toolsDir);
    #print("Build Dir %s" % buildDir);

    Prepost(toolsDir, buildDir)
    Concatenate(toolsDir, buildDir)

if __name__ == '__main__':
    main()

