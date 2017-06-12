#!/usr/bin/python

import os
import sys
import platform
import subprocess
import multiprocessing

# help message
def helpmsg(script):
    helpstr = '''
Usage:
    build:
        python %s <targetbuild>
        Allowed values for <target_build>: all, linux_unsecured, linux_secured, linux_unsecured_with_ra, linux_secured_with_ra, linux_unsecured_with_rd, linux_secured_with_rd, android, arduino, tizen, simulator, darwin, windows, msys
        Note: \"linux\" will build \"linux_unsecured\", \"linux_secured\", \"linux_unsecured_with_ra\", \"linux_secured_with_ra\", \"linux_secured_with_rd\", \"linux_unsecured_with_mq\", \"linux_secured_with_tcp\" & \"linux_unsecured_with_tcp\" & \"linux_unsecured_with_rd\".
        Any selection will build both debug and release versions of all available targets in the scope you've selected.
        To choose any specific command, please use the SCons commandline directly. Please refer to [IOTIVITY_REPO]/Readme.scons.txt.
    clean:
        python %s -c
    '''
    print (helpstr % (script, script))
    sys.exit()

def call_scons(build_options, extra_option_str):
    """
    This function formats and runs a scons command
    Arguments:
    build_options    -- {Dictionary} build flags (keys) associated with values;
    extra_option_str -- {String} extra options to append to scons command
    """
    cmd_line = "scons VERBOSE=" + VERBOSE
    for key in build_options:
        cmd_line += " " + key + "=" + str(build_options[key])

    cmd_line += " " + str(extra_option_str)

    print ("Running : " + cmd_line)
    sys.stdout.flush()
    exit_code = subprocess.Popen([cmd_line], shell=True).wait()
    if exit_code != 0:
        exit(exit_code)

def build_all(flag, extra_option_str):
    if platform.system() == "Linux":
        build_linux_unsecured(flag, extra_option_str)
        build_linux_secured(flag, extra_option_str)
        build_linux_unsecured_with_ra(flag, extra_option_str)
        build_linux_secured_with_ra(flag, extra_option_str)
        build_linux_unsecured_with_rm(flag, extra_option_str)
        build_linux_unsecured_with_rd(flag, extra_option_str)
        build_linux_secured_with_rd(flag, extra_option_str)
        build_linux_unsecured_with_mq(flag, extra_option_str)
        build_linux_unsecured_with_tcp(flag, extra_option_str)
        build_linux_secured_with_tcp(flag, extra_option_str)
        build_simulator(flag, extra_option_str)

    build_android(flag, extra_option_str)
    build_arduino(flag, extra_option_str)
    build_tizen(flag, extra_option_str)

    if platform.system() == "Windows":
        build_windows(flag, extra_option_str)

    if platform.system() == "Darwin":
        build_darwin(flag, extra_option_str)

def build_linux(flag, extra_option_str):
    build_linux_unsecured(flag, extra_option_str)
    build_linux_secured(flag, extra_option_str)

def build_linux_unsecured(flag, extra_option_str):
    print ("*********** Build for linux ************")
    build_options = {
                        'RELEASE':flag,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_secured_with_tcp(flag, extra_option_str):
    print ("*********** Build for linux with Secured TCP ************")
    build_options = {
                        'RELEASE':flag,
                        'WITH_TCP': 1,
                        'WITH_CLOUD':1,
                        'SECURED':1,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_tcp(flag, extra_option_str):
    print ("*********** Build for linux with TCP ************")
    build_options = {
                        'RELEASE':flag,
                        'WITH_TCP': 1,
                        'TARGET_TRANSPORT': 'IP',
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_rm(flag, extra_option_str):
    print ("*********** Build for linux with RoutingManager************")
    build_options = {
                        'ROUTING':'GW',
                        'RELEASE':flag,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_secured(flag, extra_option_str):
    print ("*********** Build for linux with Security *************")
    build_options = {
                        'RELEASE':flag,
                        'SECURED':1,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_ra(flag, extra_option_str):
    print ("*********** Build for linux With Remote Access *************")
    build_options = {
                        'RELEASE':flag,
                        'WITH_RA':1,
                        'WITH_RA_IBB':1,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_secured_with_ra(flag, extra_option_str):
    print ("*********** Build for linux With Remote Access & Security ************")
    build_options = {
                        'RELEASE':flag,
                        'WITH_RA':1,
                        'WITH_RA_IBB':1,
                        'SECURED':1,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_rd(flag, extra_option_str):
    print ("*********** Build for linux With Resource Directory *************")
    build_options = {
                        'RELEASE':flag,
                        'RD_MODE':'all',
                    }
    call_scons(build_options, extra_option_str)

def build_linux_secured_with_rd(flag, extra_option_str):
    print ("*********** Build for linux With Resource Directory & Security ************")
    build_options = {
                        'RELEASE':flag,
                        'RD_MODE':'all',
                        'SECURED':1,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_mq(flag, extra_option_str):
    print ("*********** Build for linux With Message Queue ************")
    build_options = {
                        'RELEASE':flag,
                        'WITH_MQ':'PUB,SUB,BROKER',
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_tcp(flag, extra_option_str):
    print ("*********** Build for linux With tcp ************")
    build_options = {
                        'RELEASE':flag,
                        'WITH_TCP':'1',
                    }
    call_scons(build_options, extra_option_str)

def build_android(flag, extra_option_str):
    # Note: for android, as oic-resource uses C++11 feature stoi and to_string,
    # it requires gcc-4.9, currently only android-ndk-r10(for linux)
    # and windows android-ndk-r10(64bit target version) support these features.
    print ("*********** Build for android armeabi *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'armeabi',
                        'RELEASE':flag,
                    }
    call_scons(build_options, extra_option_str)

def build_android_x86(flag, extra_option_str):
    print ("*********** Build for android x86 *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'x86',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'IP',
                    }
    call_scons(build_options, extra_option_str)

    build_options['TARGET_TRANSPORT'] = 'BT'
    call_scons(build_options, extra_option_str)

    build_options['TARGET_TRANSPORT'] = 'BLE'
    call_scons(build_options, extra_option_str)

def build_android_x86_with_rm(flag, extra_option_str):
    print ("*********** Build for android x86 with Routing Manager *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'x86',
                        'ROUTING':'GW',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'IP',
                    }
    call_scons(build_options, extra_option_str)

    build_options['TARGET_TRANSPORT'] = 'BT'
    call_scons(build_options, extra_option_str)

    build_options['TARGET_TRANSPORT'] = 'BLE'
    call_scons(build_options, extra_option_str)

def build_android_armeabi(flag, extra_option_str):
    print ("*********** Build for android armeabi *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'armeabi',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'IP',
                    }
    call_scons(build_options, extra_option_str)

    build_options['TARGET_TRANSPORT'] = 'BT'
    call_scons(build_options, extra_option_str)

    build_options['TARGET_TRANSPORT'] = 'BLE'
    call_scons(build_options, extra_option_str)

def build_android_armeabi_with_rm(flag, extra_option_str):
    print ("*********** Build for android armeabi with Routing Manager*************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'armeabi',
                        'ROUTING':'GW',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'IP',
                    }
    call_scons(build_options, extra_option_str)

    build_options['TARGET_TRANSPORT'] = 'BT'
    call_scons(build_options, extra_option_str)

    build_options['TARGET_TRANSPORT'] = 'BLE'
    call_scons(build_options, extra_option_str)

def build_arduino(flag, extra_option_str):
    print ("*********** Build for arduino avr *************")
    extra_option_str = "resource " + extra_option_str
    build_options = {
                        'TARGET_OS':'arduino',
                        'UPLOAD':'false',
                        'BOARD':'mega',
                        'TARGET_ARCH':'avr',
                        'TARGET_TRANSPORT':'IP',
                        'SHIELD':'ETH',
                        'RELEASE':flag,
                    }
    call_scons(build_options, extra_option_str)

    build_options['SHIELD'] = 'WIFI'
    call_scons(build_options, extra_option_str)

    build_options['TARGET_TRANSPORT'] = 'BLE'
    build_options['SHIELD']           = 'RBL_NRF8001'
    call_scons(build_options, extra_option_str)

    print ("*********** Build for arduino arm *************")
    build_options['BOARD']            = 'arduino_due_x'
    build_options['TARGET_ARCH']      = 'arm'
    build_options['TARGET_TRANSPORT'] = 'IP'
    build_options['SHIELD']           = 'ETH'
    call_scons(build_options, extra_option_str)

    build_options['SHIELD'] = 'WIFI'
    call_scons(build_options, extra_option_str)

    # BLE support for the Arduino Due is currently unavailable.

def build_tizen(flag, extra_option_str):
    print ("*********** Build for Tizen *************")
    cmd_line = "/bin/sh " + os.getcwd() + "/gbsbuild.sh"
    print ("Running : " + cmd_line)
    subprocess.Popen([cmd_line], shell=True).wait()

    print ("*********** Build for Tizen octbstack lib and sample *************")
    extra_option_str = "-f resource/csdk/stack/samples/tizen/build/SConscript " + extra_option_str
    build_options = {
                        'TARGET_OS':'tizen',
                        'TARGET_TRANSPORT':'IP',
                        'LOGGING':'true',
                        'RELEASE':flag,
                    }
    call_scons(build_options, extra_option_str)

    print ("*********** Build for Tizen octbstack lib and sample with Security*************")
    build_options['SECURED'] = 1
    call_scons(build_options, extra_option_str)

    print ("*********** Build for Tizen octbstack lib and sample with Routing Manager*************")
    del build_options['SECURED']
    build_options['ROUTING'] = 'GW'
    call_scons(build_options, extra_option_str)

# Mac OS and iOS
def build_darwin(flag, extra_option_str):
    print ("*********** Build for OSX *************")
    build_options = {
                        'TARGET_OS':'darwin',
                        'SYS_VERSION':'10.9',
                        'RELEASE':flag,
                    }
    call_scons(build_options, extra_option_str)

    print ("*********** Build for IOS i386 *************")
    build_options = {
                        'TARGET_OS':'ios',
                        'TARGET_ARCH':'i386',
                        'SYS_VERSION':'7.0',
                        'RELEASE':flag,
                    }
    call_scons(build_options, extra_option_str)

    print ("*********** Build for IOS x86_64 *************")
    build_options['TARGET_ARCH'] = 'x86_64'
    call_scons(build_options, extra_option_str)

    print ("*********** Build for IOS armv7 *************")
    build_options['TARGET_ARCH'] = 'armv7'
    call_scons(build_options, extra_option_str)

    print ("*********** Build for IOS armv7s *************")
    build_options['TARGET_ARCH'] = 'armv7s'
    call_scons(build_options, extra_option_str)

    print ("*********** Build for IOS arm64 *************")
    build_options['TARGET_ARCH'] = 'arm64'
    call_scons(build_options, extra_option_str)

# Windows
def build_windows(flag, extra_option_str):
    print ("*********** Build for Windows *************")
    os.environ["SCONSFLAGS"] = ""
    build_options = {
                        'TARGET_OS':'windows',
                        'TARGET_ARCH':'amd64',
                        'RELEASE':flag,
                        'WITH_RA':0,
                        'TARGET_TRANSPORT':'IP',
                        'SECURED':1,
                        'WITH_TCP':0,
                        'BUILD_SAMPLE':'ON',
                        'LOGGING':'off',
                        'TEST':1,
                        'RD_MODE':'all',
                    }
    call_scons(build_options, extra_option_str)

# Windows msys
def build_msys(flag, extra_option_str):
    print ("*********** Build for msys_nt *************")
    os.environ["SCONSFLAGS"] = ""
    build_options = {
                        'TARGET_OS':'msys_nt',
                        'TARGET_ARCH':'x86_64',
                        'RELEASE':flag,
                        'WITH_RA':0,
                        'TARGET_TRANSPORT':'IP',
                        'SECURED':1,
                        'WITH_TCP':0,
                        'BUILD_SAMPLE':'ON',
                        'LOGGING':'off',
                        'TEST':1,
                        'RD_MODE':'all',
                    }
    call_scons(build_options, extra_option_str)

def build_simulator(flag, extra_option_str):
    print ("*********** Build for simulator plugin *************")
    build_options = {
                        'SIMULATOR':1,
                        'RELEASE':flag,
                    }
    call_scons(build_options, extra_option_str)

def unit_tests():
    print ("*********** Unit test Start *************")
    build_options = {
                        'RELEASE':'false',
                    }
    extra_option_str = "resource -c"
    call_scons(build_options, extra_option_str)

    build_options = {
                        'LOGGING':'false',
                        'RELEASE':'false',
                    }
    extra_option_str = "resource"
    call_scons(build_options, extra_option_str)

    build_options = {
                        'TEST':1,
                        'RELEASE':'false',
                    }
    call_scons(build_options, extra_option_str)
    build_options = {
                        'TEST':1,
                        'SECURED':1,
                        'RELEASE':'false',
                    }
    call_scons(build_options, extra_option_str)

    print ("*********** Unit test Stop *************")

# Main module starts here
if os.getenv("SCONSFLAGS", "") == "":
    os.environ["SCONSFLAGS"] = "-Q -j " + str(multiprocessing.cpu_count())

arg_num     = len(sys.argv)
script_name = sys.argv[0]

# May be overridden in user's shell
VERBOSE = os.getenv("VERBOSE", "1")

if arg_num == 1:
    build_all("true", "")
    build_all("false", "")
    unit_tests()

elif arg_num == 2:
    if str(sys.argv[1]) == '-c':
        build_all("true", "-c")
        build_all("false", "-c")

    elif str(sys.argv[1]) == "all":
        build_all("true", "")
        build_all("false", "")
        unit_tests()

    elif str(sys.argv[1]) == "linux":
        build_linux("true", "")
        build_linux("false", "")

    elif str(sys.argv[1]) == "linux_unsecured":
        build_linux_unsecured("true", "")
        build_linux_unsecured("false", "")
        build_linux_unsecured_with_rm("true", "")
        build_linux_unsecured_with_rm("false", "")

    elif str(sys.argv[1]) == "linux_secured":
        build_linux_secured("true", "")
        build_linux_secured("false", "")

    elif str(sys.argv[1]) == "linux_unsecured_with_ra":
        build_linux_unsecured_with_ra("true", "")
        build_linux_unsecured_with_ra("false", "")

    elif str(sys.argv[1]) == "linux_secured_with_ra":
        build_linux_secured_with_ra("true", "")
        build_linux_secured_with_ra("false", "")

    elif str(sys.argv[1]) == "linux_unsecured_with_rd":
        build_linux_unsecured_with_rd("true", "")
        build_linux_unsecured_with_rd("false", "")

    elif str(sys.argv[1]) == "linux_secured_with_rd":
        build_linux_secured_with_rd("true", "")
        build_linux_secured_with_rd("false", "")

    elif str(sys.argv[1]) == "linux_unsecured_with_mq":
        build_linux_unsecured_with_mq("true", "")
        build_linux_unsecured_with_mq("false", "")

    elif str(sys.argv[1]) == "linux_unsecured_with_tcp":
        build_linux_unsecured_with_tcp("true", "")
        build_linux_unsecured_with_tcp("false", "")

    elif str(sys.argv[1]) == "linux_secured_with_tcp":
        build_linux_secured_with_tcp("false", "")
        build_linux_secured_with_tcp("true", "")

    elif str(sys.argv[1]) == "android":
        build_android("true", "")
        build_android("false", "")

    elif str(sys.argv[1]) == "android_x86":
        build_android_x86("true", "")
        build_android_x86("false", "")
        build_android_x86_with_rm("true", "")
        build_android_x86_with_rm("false", "")

    elif str(sys.argv[1]) == "android_armeabi":
        build_android_armeabi("true", "")
        build_android_armeabi("false", "")
        build_android_armeabi_with_rm("true", "")
        build_android_armeabi_with_rm("false", "")

    elif str(sys.argv[1]) == "arduino":
        build_arduino("true", "")
        build_arduino("false", "")

    elif str(sys.argv[1]) == "windows":
        build_windows("true", "")
        build_windows("false", "")

    elif str(sys.argv[1]) == "msys":
        build_msys("true", "")
        build_msys("false", "")

    elif str(sys.argv[1]) == "tizen":
        build_tizen("true", "")
        build_tizen("false", "")

    elif str(sys.argv[1]) == "simulator":
        build_simulator("true", "")
        build_simulator("false", "")

    elif str(sys.argv[1]) == "darwin":
        build_darwin("true", "")
        build_darwin("false", "")

    elif str(sys.argv[1]) == "unit_tests":
        unit_tests()

    else:
        helpmsg(script_name)
else:
        helpmsg(script_name)

print ("===================== done =====================")
