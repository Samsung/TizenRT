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
import os
import sys
import time
import glob
import optparse

cycleIdDict = dict()
workIdDict = dict()
beginDict = dict()
endDict = dict()
cycleGap = 0
temporalId = 1000
parserDirPath = "scripts"

ftraceLogs = []


class TraceItem:
    def __init__(self):
        self.taskname = "null"
        self.pid = "0"
        self.tgid = "0"
        self.core = "[000]"
        self.flags = "...1"
        self.timestamp = "0"
        self.function_type = "null"
        self.pair_type = "B"
        self.message = "0"

    def extractTime(self, line):
        self.timestamp = line.strip('[]').replace(':', '.')
        return self.timestamp

    def extractPid(self, line):
        self.pid = line.strip(':')
        return self.pid

    def extractPairType(self, line):
        self.pair_type = line.split('|')[0].upper()
        return self.pair_type

    def extractMsg(self, line):
        self.message = (line.split('|'))[1]
        return self.message

    def composeSchedLine(self):
        self.function_type = "sched_switch"
        line = "%s-%s %s %s %s: %s: %s" % (
                self.taskname, self.pid, self.core, self.flags, self.timestamp,
                self.function_type, self.message)
        return line

    def composeNormalLine(self):
        self.function_type = "tracing_mark_write"
        line = "%s-%s %s %s %s: %s: %s|%s|%s" % (self.taskname, self.pid,
                self.core, self.flags, self.timestamp,
                self.function_type, self.pair_type, self.pid, self.message)
        return line

    def composeLine(self):
        if self.pair_type == 'S':
            line = self.composeSchedLine()
        else:
            line = self.composeNormalLine()
        return line

    def addLineToFtraceLogs(self, line):
        ftraceLogs.append(line)
        return


def writeFtraceLogs(options):
    with open(options.outputFile, "wb") as output:
        for line in ftraceLogs:
            if (options.verbose == True):
                print line
            output.write(line + "\n")
    return True


def translateTinyaraLogs(options):
    item = TraceItem()
    filename = os.path.join(options.inputFile)
    with open(filename, "r") as rawLogs:
        for line in rawLogs:
            if (line.isspace()):
                continue
            lineList = line.strip().split(None, 2)
            time = item.extractTime(lineList[0])
            pid = item.extractPid(lineList[1])
            pair_type = item.extractPairType(lineList[2])
            msg = item.extractMsg(lineList[2])
            translatedLine = item.composeLine()
            if (options.verbose == True):
                print translatedLine
            item.addLineToFtraceLogs(translatedLine)
    return True


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


def makeHtml(options):
    htmlfile = options.outputFile.replace(options.outputExt, '.html')
    if os.name == 'nt':
        os.system("%s/ttrace.py --from-text-file=%s -o %s\n"
                % (parserDirPath, options.outputFile, htmlfile))
    else:
        os.system("./%s/ttrace.py --from-text-file=%s -o %s\n"
                % (parserDirPath, options.outputFile, htmlfile))
    return True


def findAddr(filename, target):
    with open(filename, "r") as sysmap:
        for line in sysmap:
            if line.isspace():
                continue
            if target in line:
                return line.strip().split()[0]
    return False


def main():
        usage = "Usage: %prog [options]"
        desc = "Example: %prog -i logs.txt -o ttrace_dump"
        parser = optparse.OptionParser(usage=usage, description=desc)
        parser.add_option('-i', '--input', dest='inputFile',
                default=None,
                metavar='FILENAME',
                help="Parsed text file only, Not support dumpped file, "
                "[default:%default]")
        parser.add_option('-d', '--dump', dest='dump',
                default=None,
                metavar='MODELNAME',
                help="Dump trace buffer and generate html report, "
                "[default:%default]")
        parser.add_option('-o', '--output', dest='outputFile',
                default=None,
                metavar='FILENAME',
                help="Output file that html report saved, "
                "[default:%default]")
        parser.add_option('-v', '--verbose', dest='verbose',
                action="store_true",
                default=False,
                help="Generate verbose output, "
                "[default:%default]")

        options, arg = parser.parse_args()
        options.curDir = os.path.dirname(os.path.abspath(sys.argv[0]))

        if (options.inputFile == None and options.dump == None):
            print "Please specify reading from file or dump"
            exit()

        if (options.inputFile != None and options.dump != None):
            print "Please choose just one option for reading logs"
            exit()

        if (options.dump != None):
            if (options.dump != "artik051" and options.dump != "artik053"):
                print "%s is not supported" % (options.dump)
                print "T-trace dump supports artik051, artik053"
                exit()
            os.system("./%s/ttrace_tinyaraDump.py -t %s -o %s\n" \
                    % (parserDirPath, options.dump, "dump.bin"))
            options.inputFile = "%s/dump.trace" % (options.curDir)

        if (options.inputFile != None):
            # Check inputFile existance,
            if not os.access(options.inputFile, os.F_OK | os.R_OK):
                print "ERROR: " + "Can not read " + options.inputFile
                return
            print "Input file: " + options.inputFile

            options.inputFolder = os.path.split(options.inputFile)[0]
            options.inputFilenameExt = os.path.split(options.inputFile)[1]
            options.inputFileName = \
            os.path.splitext(options.inputFilenameExt)[0]

        if (options.outputFile != None):
            options.outputFolder = os.path.split(options.outputFile)[0]
            options.outputFileName = os.path.split(options.outputFile)[1]
            if not os.access(options.outputFolder, os.F_OK | os.W_OK):
                os.mkdir(options.outputFolder)
        else:
            if (options.inputFile != None):
                options.outputFolder = options.inputFolder
                options.outputFileName = options.inputFileName
            else:
                options.outputFolder = options.curDir
                options.outputFileName = "report"
        options.outputExt = ".ftrace"
        options.outputFilenameExt = options.outputFileName + options.outputExt
        options.outputFile = \
        os.path.join(options.outputFolder, options.outputFilenameExt)

        print "output file will be saved at %s" % (options.outputFile)

        translateTinyaraLogs(options)
        writeFtraceLogs(options)
        makeHtml(options)


if __name__ == '__main__':
        main()
