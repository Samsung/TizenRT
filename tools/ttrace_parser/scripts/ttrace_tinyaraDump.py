#!/usr/bin/python
###########################################################################
#
# Copyright (c) 2017 The Chromium Authors. All rights reserved.
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

from operator import itemgetter
from distutils import util
import os
import sys
import time
import glob
import select
import optparse
import subprocess


def get_os_cmd(cmdARGS):
    fd_popen = subprocess.Popen(cmdARGS.split(),
            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    ready = select.select([fd_popen.stdout, fd_popen.stderr],
            [], [fd_popen.stdout, fd_popen.stderr])
    if fd_popen.stdout in ready[0]:
        out = os.read(fd_popen.stdout.fileno(), 4096)
        return out
    else:
        return False


class GdbManager:
    def __init__(self):
        self.targetName = None
        self.openocdPath = None
        self.gdbCommandsPath = None
        self.binaryPath = None
        self.addrBegin = None
        self.addrEnd = None
        self.addrDump = None
        self.dumpFile = None
        self.traceFile = None
        return None

    def setTargetName(self, name):
        self.targetName = name

    def setopenocdPath(self, path):
        self.openocdPath = path

    def setBinaryPath(self, path):
        self.binaryPath = path

    def setDumpFile(self, name):
        self.dumpFile = name

    def setTraceFile(self, name):
        self.traceFile = name

    def checkopenocdPath(self):
        if self.openocdPath == None:
            print "You should set openocd path"
            return False
        print "openocdPath: " + self.openocdPath
        return True

    def checkBinaryPath(self):
        if self.binaryPath == None:
            print "You should set binary path that contains tinyara,\
            System.map files"
            return False
        print "binaryPath: " + self.binaryPath
        return True

    def checkRamdump(self):
        if os.access("ramdump", os.F_OK):
            print "Ramdump generated"
            return True
        print "Failed to generate ramdump"
        return False

    def connectTarget_artik053(self):
        if self.checkopenocdPath() == False:
            return False
        print("%s -f %s &"
                % (os.path.join(self.openocdPath, 'linux64/openocd'),
                os.path.join(self.openocdPath, "artik053.cfg")))
        print os.system("%s -f %s &"
                % (os.path.join(self.openocdPath, 'linux64/openocd'),
                os.path.join(self.openocdPath, "artik053.cfg")))
        time.sleep(3)
        print os.system("telnet localhost 4444 &")
        time.sleep(3)
        return True

    def connectTarget(self):
        if (self.targetName == "artik053"):
            return self.connectTarget_artik053()
        else:
            print "Could not found target name: " + self.targetName
        return False

    def checkELF(self):
        if os.access("%s/tinyara" % self.binaryPath, os.F_OK):
            return True
        print "ELF file(tinyara) doesn't exist"
        return False

    def checkSystemMap(self):
        if os.access("%s/System.map" % self.binaryPath, os.F_OK) == False:
            print "Symbol table file(System.map) doesn't exist"
            return False
        return True

    def generateGdbCommands_artik051(self):
        self.gdbCommandsPath = os.path.join(self.openocdPath, "gdbCommands")
        gdbCmdFile = open(self.gdbCommandsPath, 'w')
        gdbCmdFile.write("target remote:3333\n")
        gdbCmdFile.write("set confirm off\n")
        gdbCmdFile.write("file %s\n"
                % (os.path.join(self.binaryPath, "tinyara")))
        gdbCmdFile.write("break wait_ttrace_dump\n")
        gdbCmdFile.write("continue\n")
        gdbCmdFile.write("delete 1\n")
        gdbCmdFile.write("dump binary memory ramdump %s %s\n"
                % (self.addrBegin, self.addrEnd))
        gdbCmdFile.write("quit\n")
        gdbCmdFile.close()
        return True

    def generateGdbCommands_artik053(self):
        self.gdbCommandsPath = os.path.join(self.openocdPath, "gdbCommands")
        gdbCmdFile = open(self.gdbCommandsPath, 'w')
        gdbCmdFile.write("target remote:3333\n")
        gdbCmdFile.write("monitor reset halt\n")
        gdbCmdFile.write("monitor cortex_r4 maskisr on\n")
        gdbCmdFile.write("set confirm off\n")
        gdbCmdFile.write("file %s\n"
                % (os.path.join(self.binaryPath, "tinyara")))
        gdbCmdFile.write("break wait_ttrace_dump\n")
        gdbCmdFile.write("continue\n")
        gdbCmdFile.write("delete 1\n")
        gdbCmdFile.write("dump binary memory ramdump %s %s\n"
                % (self.addrBegin, self.addrEnd))
        gdbCmdFile.write("quit\n")
        gdbCmdFile.close()
        return True

    def generateGdbCommands(self):
        self.gdbCommandsPath = os.path.join(self.openocdPath, "gdbCommands")
        if (self.targetName == "artik051"):
            return self.generateGdbCommands_artik051()
        elif (self.targetName == "artik053"):
            return self.generateGdbCommands_artik053()
        else:
            print "Could not found target name: " + self.targetName
        return False

    def runGdbCommands(self):
        out = os.system("arm-none-eabi-gdb %s --command=%s\n" \
                % (os.path.join(self.openocdPath, "binary/tinyara"), \
                    self.gdbCommandsPath))
        print "-----------------------------------------\n"
        if self.checkRamdump() == False:
            return False
        os.system("mv ramdump %s" % (self.dumpFile))
        return out

    def setAddrBegin(self):
        self.addrBegin = self.findAddr(
                os.path.join(self.binaryPath, "System.map"), "packets")
        if self.addrBegin == False:
            return False
        self.addrBegin = "0x" + self.addrBegin
        print "addrBegin: " + self.addrBegin
        return True

    def setAddrEnd(self):
        self.addrEnd = self.findEndAddr(
                os.path.join(self.binaryPath, "System.map"), "packets")
        if self.addrEnd == False:
            return False
        self.addrEnd = "0x" + self.addrEnd
        print "addrEnd: " + self.addrEnd
        return True

    def setAddrDump(self):
        self.addrDump = self.findAddr(
                os.path.join(self.binaryPath, "System.map"),
                "wait_ttrace_dump")
        if self.addrDump == False:
            return False
        self.addrDump = "0x" + self.addrDump
        print "addrDump: " + self.addrDump
        return True

    def findAddr(self, filename, target):
        with open(filename, "r") as sysmap:
            for line in sysmap:
                if line.isspace():
                    continue
                if target in line:
                    return line.strip().split()[0]
        return False

    def findEndAddr(self, filename, target):
        flag = False
        with open(filename, "r") as sysmap:
            for line in sysmap:
                if line.isspace():
                    continue
                if flag == True:
                    return line.strip().split()[0]
                if target in line:
                    flag = True
        return False

    def extractDump(self, options):
        out = subprocess.check_output(["./scripts/parse_dump", self.dumpFile])
        if options.verbose == True:
            print out
        with open(self.traceFile, "w") as dump:
            dump.write(out)
        return True


def main():
    usage = "Usage: %prog -t [targetname] -o [filename]"
    desc = "Example: %prog -t artik053 -o dump.trace"
    parser = optparse.OptionParser(usage=usage, description=desc)
    parser.add_option('-t', '--target', dest='targetName',
            default='artik053',
            metavar='TARGETNAME',
            help="Specify target device name, "
            "[default:%default]")
    parser.add_option('-o', '--output', dest='dumpFile',
            default='dump',
            metavar='FILENAME',
            help="Specify dumpped file name, "
            "[default:%default]")
    parser.add_option('-b', '--binary', dest='binaryPath',
            default='../../build/output/bin',
            metavar='PATH',
            help="Specify tinyara binary path, "
            "[default:%default]")
    parser.add_option('-d', '--openocd', dest='openocdPath',
            default='../../build/configs/artik053/tools/openocd',
            metavar='PATH',
            help="Specify openocd path, "
            "[default:%default]")
    parser.add_option('-v', '--verbose', dest='verbose',
            action='store_true',
            default=False,
            help="Generate verbose output, "
            "[default:%default]")
    options, arg = parser.parse_args()
    options.curDir = os.path.dirname(os.path.abspath(sys.argv[0]))

    gdbman = GdbManager()
    gdbman.setTargetName(options.targetName)
    gdbman.setBinaryPath(options.binaryPath)
    gdbman.setopenocdPath(options.openocdPath)
    gdbman.setDumpFile(options.dumpFile)

    traceFile = os.path.splitext(options.dumpFile)[0] + '.trace'
    gdbman.setTraceFile(traceFile)

    gdbman.checkELF()
    gdbman.checkSystemMap()

    gdbman.setAddrBegin()
    gdbman.setAddrEnd()
    gdbman.setAddrDump()

    gdbman.generateGdbCommands()
    gdbman.connectTarget()
    time.sleep(3)
    gdbman.runGdbCommands()
    gdbman.extractDump(options)

if __name__ == '__main__':
    main()
