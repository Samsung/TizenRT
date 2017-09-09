import subprocess
import sys
import os

from config import EXCLUDE_PATH_LIST

from common import Result


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def print_bold(str):
    print(bcolors.BOLD + str + bcolors.ENDC)


def print_green(str):
    print(bcolors.OKGREEN + str + bcolors.ENDC)


def print_fail(str):
    print(bcolors.FAIL + str + bcolors.ENDC)


def print_warning(str):
    print(bcolors.WARNING + str + bcolors.ENDC)


def execute(cmd):
    print('$ ' + cmd)
    subprocess.check_call(cmd, shell=True)


def execute_return_output(cmd, is_print_cmd=True):
    if is_print_cmd:
        print('$ ' + cmd)
    process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT)
    output = ''
    for line in iter(process.stdout.readline, ''):
        output += line
        sys.stdout.write(line)
        sys.stdout.flush()
    process.poll()
    return Result(exitcode=process.returncode, message='', data=output)


def write_result(filename, message):
    subprocess.call('rm -rf {}'.format(filename), shell=True)
    f = open(filename, 'w')
    f.write(message + '\n')
    f.close()


def raise_exception_if_exitcode_is_error(result):
    if result.exitcode != 0:
        raise subprocess.CalledProcessError(
            cmd=result.message, returncode=result.exitcode)


def find_tizenrt_source_files(targets):
    files_list = []
    for target in targets:
        if os.path.isfile(target):
            if __has_exclude_path(os.path.abspath(target)):
                continue
            if not __is_source_file(target):
                continue
            files_list.append(target)
    return files_list


def __has_exclude_path(path):
    for exclude in EXCLUDE_PATH_LIST:
        if exclude in path:
            return True
    return False


def __is_source_file(filename):
    ext = os.path.splitext(filename)[-1]
    if ext == '.c':
        return True
    if ext == '.h':
        return True
    return False

def get_test_options(verbose, name, repeat, group = ''):
    options = []
    if verbose:
        options.append('-v')
    if name is not None and name != '':
        options.append('-n {}'.format(name))
    if repeat > 1:
        options.append('-r {}'.format(repeat))
    if group is not None and group != '':
        options.append('-g {}'.format(group))
    return ' '.join(options)