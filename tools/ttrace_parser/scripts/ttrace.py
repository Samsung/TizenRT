#!/usr/bin/env python
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

# Copyright (c) 2011 The Chromium Authors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#    * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""Android system-wide tracing utility.

This is a tool for capturing a trace that includes data from both userland and
the kernel.  It creates an HTML file for visualizing the trace.
"""
import os
import sys
import time
import zlib
import errno
import string
import select
import optparse
import pid_parser
import subprocess

flattened_css_file = 'style.css'
flattened_js_file = 'tscript.js'

g_device_serial = None


class OptionParserIgnoreErrors(optparse.OptionParser):
    def error(self, msg):
        pass

    def exit(self):
        pass

    def print_usage(self):
        pass

    def print_help(self):
        pass

    def print_version(self):
        pass


def compose_html_win(script_dir, options, css, js, templates):
    data = []

    ret_fd = os.open(options.from_file_win, os.O_RDONLY | os.O_BINARY)

    out = os.read(ret_fd, 4096)
    parts = out.split('TRACE:', 1)
    data.append(parts[1])

    while True:
        out = os.read(ret_fd, 4096)
        keepReading = False
        if len(out) > 0:
            keepReading = True
            data.append(out)
        if not keepReading:
            break

    data = ''.join(data)

    if data.startswith('\r\n'):
        data = data.replace('\r\n', '\n')

    data = data[1:]

    html_filename = options.output_file
    html_prefix = read_asset(script_dir, 'prefix.html')
    html_suffix = read_asset(script_dir, 'suffix.html')

    html_file = open(html_filename, 'w')
    html_file.write(html_prefix % (css, js, templates))

    size = 4096
    dec = zlib.decompressobj()

    for chunk in (data[i:i + size] for i in xrange(0, len(data), size)):
        decoded_chunk = dec.decompress(chunk)
        html_chunk = decoded_chunk.replace('\n', '\\n\\\n')
        html_file.write(html_chunk)

    html_out = dec.flush().replace('\n', '\\n\\\n')

    # write body
    html_file.write(html_out)

    # write suffix
    html_file.write(html_suffix)
    html_file.close()
    print "\n    wrote file://%s\n" % os.path.abspath(options.output_file)

    return


def compose_html(script_dir, options, css, js, templates):
    html_filename = options.output_file
    html_prefix = read_asset(script_dir, 'prefix.html')
    html_suffix = read_asset(script_dir, 'suffix.html')

    html_file = open(html_filename, 'w')
    html_file.write(html_prefix % (css, js, templates))

    cur_dir = os.getcwd()

    # remove useless 2 lines
    with open(os.path.join(cur_dir, options.from_text_file), "r") as input:
        with open(os.path.join(cur_dir, options.from_text_file + 'composing'), "wb") as output:
            for line in input:
                if "capturing trace" in line:
                    continue
                elif "TRACE:" in line:
                    continue
                elif " done" in line:
                    continue
                elif '\n' == line:
                    continue
                else:
                    output.write(line)

    # case not compressed, boot case
    html_out = read_asset(script_dir, os.path.join(cur_dir, options.from_text_file + 'composing'))
    html_out = html_out.replace('\n', '\\n\\\n')
    os.remove(os.path.join(cur_dir, options.from_text_file + 'composing'))

    # write body
    html_file.write(html_out)

    # Write suffix
    html_file.write(html_suffix)
    html_file.close()
    print "\n    wrote file://%s\n" % os.path.abspath(options.output_file)

    return


def get_os_cmd(cmdARGS):
    fd_popen = subprocess.Popen(cmdARGS.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    ready = select.select([fd_popen.stdout, fd_popen.stderr], [], [fd_popen.stdout, fd_popen.stderr])
    if fd_popen.stdout in ready[0]:
        out = os.read(fd_popen.stdout.fileno(), 4096)
        return out
    else:
        return 0


def sdb_shell(str_param):
    global g_options
    cmd_str = ['sdb']
    if g_device_serial:
        cmd_str.extend(['-s', str(g_device_serial)])
    cmd_str.extend([str(str_param)])
    os.system(string.join(cmd_str))
    os.system('sleep 2')


def is_sdb_available():
    no = 0
    max_no = 10
    sdb_shell('kill-server')
    while(no < max_no):
        str_cmd = get_os_cmd('sdb start-server')
        str_cmd = get_os_cmd('sdb devices')
        os.system('sleep 2')
        l_devices = str_cmd.split('\n')
        if len(l_devices) > 3:
            if g_device_serial is None:
                    print 'Please specify serial with -e option'
                    sys.exit(1)
        dev_type = str_cmd.split("List of devices attached")[-1].split()
        if 'device' in dev_type:
            print 'Ready to connect'
            return dev_type[0]
        else:
            no = no + 1
            print 'retry...' + str(no)
            sdb_shell('kill-server')
    if no == max_no:
        print 'Could not connect to SDB devices'
        sys.exit(1)


def set_sdb_root():
    dev_type = is_sdb_available()
    if dev_type == 0:
        return 0

    sdb_shell('root on')
    if not ('emulator' in dev_type):
        sdb_shell('shell change-booting-mode.sh --update')

    print 'SDB was rooted!!!'
    return 1


def trace_bootup(cmd):
    if set_sdb_root() == 0:
        return

    print cmd + ' > /etc/ttrace.conf\''
    str_cmd = cmd + ' > /etc/ttrace.conf\''
    os.system(str_cmd)
    os.system('sleep 2')

    sdb_shell('shell sync')
    sdb_shell('shell reboot')
    sdb_shell('kill-server')


def add_sdb_serial(command, serial):
    if serial is not None:
        command.insert(1, serial)
        command.insert(1, '-s')


def main():
    global g_device_serial
    usage = "Usage: %prog [options] [category1 [category2 ...]]"
    desc = "Example: %prog -b 32768 -t 15 gfx input view sched freq"
    parser = optparse.OptionParser(usage=usage, description=desc)
    parser.add_option('-o', dest='output_file', help='write HTML to FILE',
            default='trace.html', metavar='FILE')
    parser.add_option('-t', '--time', dest='trace_time', type='int',
            help='trace for N seconds', metavar='N')
    parser.add_option('-b', '--buf-size', dest='trace_buf_size', type='int',
            help='use a trace buffer size of N KB', metavar='N')
    parser.add_option('-l', '--list-categories', dest='list_categories', default=False,
            action='store_true', help='list the available categories and exit')
    parser.add_option('-u', '--bootup', dest='trace_bootup', default=False,
            action='store_true', help='trace boot up')
    parser.add_option('--link-assets', dest='link_assets', default=False,
            action='store_true', help='link to original CSS or JS resources '
            'instead of embedding them')
    parser.add_option('--from-file', dest='from_file', action='store',
            help='read the trace from a file (compressed) rather than running a live trace')
    parser.add_option('--from-file-win', dest='from_file_win', action='store',
            help='read the trace from a file (compressed) rather than running a live trace on windows')
    parser.add_option('--from-text-file', dest='from_text_file', action='store',
            help='read the trace from a file (not compressed) rather than running a live trace')
    parser.add_option('--asset-dir', dest='asset_dir', default='trace-viewer',
            type='string', help='')
    parser.add_option('-e', '--serial', dest='device_serial', type='string',
            help='sdb device serial number')
    parser.add_option('--async_start', dest='async_start', default=False, action='store_true',
            help='start circular trace and return immediately')
    parser.add_option('--async_dump', dest='async_dump', default=False, action='store_true',
            help='dump the current contents of circular trace buffer')
    parser.add_option('--async_stop', dest='async_stop', default=False, action='store_true',
            help='stop tracing and dump the current contents of circular trace buffer')
    parser.add_option('--append', dest='append', default=False, action='store_true',
            help='append traces to the existing traces. do not clear the trace buffer')
    parser.add_option('--backup', dest='backup', default=False, action='store_true',
            help='back up the existing traces to /tmp/trace.backup and then clear the trace buffer')

    options, args = parser.parse_args()

    if options.list_categories:
        atrace_args = ['sdb', 'shell', 'atrace', '--list_categories']
        expect_trace = False
    elif options.from_file is not None:
        atrace_args = ['cat', options.from_file]
        expect_trace = True
    elif options.from_file_win is not None:
        atrace_args = ['type', options.from_file_win]
        expect_trace = True
    elif options.from_text_file is not None:
        atrace_args = ['cat', options.from_text_file]
        expect_trace = True
    else:
        if options.trace_bootup:
            atrace_args = ['sdb', 'shell', '\'echo', 'atrace']
            expect_trace = True
        else:
            atrace_args = ['sdb', 'shell', 'atrace', '-z']
            expect_trace = True

        if options.trace_time is not None:
            if options.trace_time > 0:
                atrace_args.extend(['-t', str(options.trace_time)])
            else:
                parser.error('the trace time must be a positive number')

        if options.trace_buf_size is not None:
            if options.trace_buf_size > 0:
                atrace_args.extend(['-b', str(options.trace_buf_size)])
            else:
                parser.error('the trace buffer size must be a positive number')

        atrace_args.extend(args)

    if atrace_args[0] == 'sdb':
        add_sdb_serial(atrace_args, options.device_serial)
        if options.device_serial:
            g_device_serial = str(options.device_serial).strip()
        else:
            g_device_serial = None

    script_dir = os.path.dirname(os.path.abspath(sys.argv[0]))

    if options.link_assets:
        src_dir = os.path.join(script_dir, options.asset_dir, 'src')
        build_dir = os.path.join(script_dir, options.asset_dir, 'build')

        js_files, js_flattenizer, css_files, templates = get_assets(src_dir, build_dir)

        css = '\n'.join(linked_css_tag % (os.path.join(src_dir, f)) for f in css_files)
        js = '<script language="javascript">\n%s</script>\n' % js_flattenizer
        js += '\n'.join(linked_js_tag % (os.path.join(src_dir, f)) for f in js_files)

    else:
        css_filename = os.path.join(script_dir, flattened_css_file)
        js_filename = os.path.join(script_dir, flattened_js_file)
        css = compiled_css_tag % (open(css_filename).read())
        js = compiled_js_tag % (open(js_filename).read())
        templates = ''

    html_filename = options.output_file

    if options.trace_bootup:
        print "Trace for bootup"
        atrace_args.extend(['--async_start'])
        trace_bootup(string.join(atrace_args))

        print "Please pull out the usb cable on target"
        os.system('sleep ' + '40')
        print "Please plug the usb cable to target"
        os.system('sleep ' + '20')

        atrace_args.remove('--async_start')
        atrace_args.remove('\'echo')
        atrace_args.extend(['-z', '--async_stop'])
        expect_trace = True
    if options.from_text_file:
        compose_html(script_dir, options, css, js, templates)
        return
    elif options.from_file_win:
        compose_html_win(script_dir, options, css, js, templates)
        return
    elif options.from_file:
        print "From file"

    if options.async_start:
        atrace_args.extend(['--async_start'])
    if options.async_dump:
        atrace_args.extend(['--async_dump'])
    if options.async_stop:
        atrace_args.extend(['--async_stop'])
    if options.append:
        atrace_args.extend(['--append'])
    if options.backup:
        atrace_args.extend(['--backup'])
        backup_trace = True

    sdb = subprocess.Popen(atrace_args, stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)

    if options.async_start:
        return

    result = None
    data = []

    # Read the text portion of the output and watch for the 'TRACE:' marker that
    # indicates the start of the trace data.
    while result is None:
        ready = select.select([sdb.stdout, sdb.stderr], [], [sdb.stdout, sdb.stderr])
        if sdb.stderr in ready[0]:
            err = os.read(sdb.stderr.fileno(), 4096)
            sys.stderr.write(err)
            sys.stderr.flush()
        if sdb.stdout in ready[0]:
            out = os.read(sdb.stdout.fileno(), 4096)
            parts = out.split('\nTRACE:', 1)

            txt = parts[0].replace('\r', '')
            if len(parts) == 2:
                # The '\nTRACE:' match stole the last newline from the text, so add it
                # back here.
                txt += '\n'
            sys.stdout.write(txt)
            sys.stdout.flush()

            if len(parts) == 2:
                data.append(parts[1])
                sys.stdout.write("downloading trace...")
                sys.stdout.flush()
                break

        result = sdb.poll()

    # Read and buffer the data portion of the output.
    while True:
        ready = select.select([sdb.stdout, sdb.stderr], [], [sdb.stdout, sdb.stderr])
        keepReading = False
        if sdb.stderr in ready[0]:
            err = os.read(sdb.stderr.fileno(), 4096)
            if len(err) > 0:
                keepReading = True
                sys.stderr.write(err)
                sys.stderr.flush()
        if sdb.stdout in ready[0]:
            out = os.read(sdb.stdout.fileno(), 4096)
            if len(out) > 0:
                keepReading = True
                data.append(out)

        if result is not None and not keepReading:
            break

        result = sdb.poll()

    if result == 0:
        if expect_trace:
            if not data:
                print >> sys.stderr, ('No data was captured.    Output file was not ' +
                    'written.')
                sys.exit(1)
            else:
                # Indicate to the user that the data download is complete.
                print " done\n"

            data = ''.join(data)

            # Collapse CRLFs that are added by sdb shell.
            if data.startswith('\r\n'):
                data = data.replace('\r\n', '\n')

            # Skip the initial newline.
            data = data[1:]

            html_prefix = read_asset(script_dir, 'prefix.html')
            html_suffix = read_asset(script_dir, 'suffix.html')

            html_file = open(html_filename, 'w')
            trace_filename = html_filename + '.trace.raw'
            trace_file = open(trace_filename, 'w')
            html_file.write(html_prefix % (css, js, templates))

            size = 4096
            dec = zlib.decompressobj()
            for chunk in (data[i:i + size] for i in xrange(0, len(data), size)):
                decoded_chunk = dec.decompress(chunk)
                html_chunk = decoded_chunk.replace('\n', '\\n\\\n')
                html_file.write(html_chunk)
                trace_file.write(html_chunk)

            html_out = dec.flush().replace('\n', '\\n\\\n')
            html_file.write(html_out)

            # Write suffix
            html_file.write(html_suffix)
            html_file.close()
            trace_file.close()
            pid_parser.parse(trace_filename)
            os.remove(trace_filename)
            print "\nwrote file://%s\n" % os.path.abspath(options.output_file)

    else:  # i.e. result != 0
        print >> sys.stderr, 'sdb returned error code %d' % result
        sys.exit(1)


def read_asset(src_dir, filename):
    return open(os.path.join(src_dir, filename)).read()


def get_assets(src_dir, build_dir):
    sys.path.append(build_dir)
    gen = __import__('generate_standalone_timeline_view', {}, {})
    parse_deps = __import__('parse_deps', {}, {})
    gen_templates = __import__('generate_template_contents', {}, {})
    filenames = gen._get_input_filenames()
    load_sequence = parse_deps.calc_load_sequence(filenames, src_dir)

    js_files = []
    js_flattenizer = "window.FLATTENED = {};\n"
    js_flattenizer += "window.FLATTENED_RAW_SCRIPTS = {};\n"
    css_files = []

    for module in load_sequence:
        js_files.append(os.path.relpath(module.filename, src_dir))
        js_flattenizer += "window.FLATTENED['%s'] = true;\n" % module.name
        for dependent_raw_script_name in module.dependent_raw_script_names:
            js_flattenizer += (
                    "window.FLATTENED_RAW_SCRIPTS['%s'] = true;\n"
                    % dependent_raw_script_name)
        for style_sheet in module.style_sheets:
            css_files.append(os.path.relpath(style_sheet.filename, src_dir))

    templates = gen_templates.generate_templates()

    sys.path.pop()

    return (js_files, js_flattenizer, css_files, templates)


compiled_css_tag = """<style type="text/css">%s</style>"""
compiled_js_tag = """<script language="javascript">%s</script>"""

linked_css_tag = """<link rel="stylesheet" href="%s"></link>"""
linked_js_tag = """<script language="javascript" src="%s"></script>"""


if __name__ == '__main__':
    main()
