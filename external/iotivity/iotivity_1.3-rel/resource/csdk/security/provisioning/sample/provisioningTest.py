#!/usr/bin/python -W

import subprocess
import os
import time
import sys
import shutil
import platform
import argparse
import textwrap

# Note: to see the return value of a process
#       - in Windows type: echo %errorlevel%
#       - in Linux type: echo $?

# Resets the state of sampleserver_justworks and autoprovisioningclient so that
# state from one test is not present during subsequent tests
def cleanup(iotivity_base_path, exe_path):

    # Copy fresh oic_svr_db_server_justworks.dat
    dat_file_src = os.path.join(iotivity_base_path, 'resource', 'csdk', 'security', 'provisioning', 'sample', 'oic_svr_db_server_justworks.dat' )
    dat_file_dest = exe_path
    shutil.copy(dat_file_src, dat_file_dest)

    # Copy fresh oic_svr_db_client.dat
    dat_file_src = os.path.join(iotivity_base_path, 'resource', 'examples', 'oic_svr_db_client.dat' )
    dat_file_dest = exe_path
    shutil.copy(dat_file_src, dat_file_dest)

    #Delete provisioning DB file
    db_file = os.path.join(exe_path, 'oic_autoprvn_mng.db')
    if os.path.isfile(db_file):
        os.unlink(db_file)

# Print the environment variables (useful for debugging)
def print_environment():
    for key in os.environ.keys():
        print("%30s %s \n" % (key,os.environ[key]))
    print('current PATH is %s' % sys.path)


### main ###

# Number of unit tests in autoprovisioningclient
NUM_TESTS = 7

usage = '''
 Run end-to-end certificate tests between autoprovisioningclient and sampleserver_justworks
 Usage Notes
   - script assumes it's being run from the root of iotivity, e.g.:
         t:\\iotivity\\resource\\csdk\\security\\provisioning\\sample\\provisioningTest.py
   - I have added
        t:\\iotivity\\out\\windows\\win32\\amd64\\debug\\resource\\csdk
        t:\\iotivity\\out\\windows\\win32\\amd64\\debug
     to my PATH
   - The discovery timeout in autoprovisioning client may be a bit agressive, 3 seconds
'''

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description=usage
)

parser.add_argument('--arch', nargs='?', choices = ['amd64', 'x86', 'arm'], help= 'Architecture, one of x86, amd64 or arm. Defaults to amd64.', default='amd64')
parser.add_argument('--build', nargs='?', choices = ['debug', 'release'], help= 'Build type, one of debug or release. Defaults to debug.', default='debug')
parser.add_argument('--onetest', nargs='?', help= 'Run a single test, specified by number (1, ..., ' + str(NUM_TESTS) + '). By default all tests are run.')

args = parser.parse_args()

iotivity_base_path = os.getcwd()
os_name = platform.system()
if os_name == 'Windows':
    os_directory = 'windows\win32'
elif os_name == 'Linux':
    os_directory = 'linux'

exe_path = os.path.join(iotivity_base_path, 'out', os_directory, args.arch, args.build, 'resource', 'csdk', 'security', 'provisioning', 'sample')

# Set PATH so octbstack.dll is found
cwd = os.getcwd()
sys.path.append(os.path.join(cwd, exe_path))
sys.path.append(os.path.join(cwd, 'out', os_directory, args.arch, args.build))

# Work in the output dir with the test binaries
os.chdir(exe_path)

output_text = ""
num_failures = 0

test_range = range(1, NUM_TESTS + 1)    #default to running all tests
if args.onetest:
    try:
        if int(args.onetest) > NUM_TESTS or int(args.onetest) < 1:
            print('Argument to --onetest out of range')
            sys.exit(-1)
        test_range = range(int(args.onetest), int(args.onetest) + 1)
    except ValueError:
        print('invalid argument to --onetest')
        sys.exit(-1)

for i in test_range:
    print('\nRunning test %d...\n' % i)

    # Clear state from previous test
    cleanup(iotivity_base_path, exe_path)

    # Start the device/server with a non-blocking call
    #note: Popen can take file descriptors to redirect the processes stdin, stdout, stderr
    try:
        server_process = subprocess.Popen('sampleserver_justworks')
    except:
        print('Failed to start sampleserver_justworks: %s', sys.exc_info()[0])
        sys.exit(-1)

    # Run the auto provisioning client with the test number as argument, block until it returns
    return_code = subprocess.call(["autoprovisioningclient", str(i)])

    if return_code != 0:
        num_failures += 1
        print("Test %d failed" % i)
        output_text  += "Test %d failed\n" % i
    else:
        print("Test %d passed" % i)


    server_process.kill()


print("\n------------------------------------")
print(" Test Results: %d of %d tests passed" % (len(test_range) - num_failures, len(test_range)))
print("------------------------------------")
print(output_text)
print('\n')
