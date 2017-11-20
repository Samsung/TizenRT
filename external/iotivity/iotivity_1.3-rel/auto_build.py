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
        Allowed values for <target_build>: all, linux_unsecured, linux_secured, linux_full, linux_unsecured_with_rd, linux_secured_with_rd,
        android, android_unsecured, android_secured, arduino, tizen, tizen_unsecured, tizen_secured, simulator, darwin, windows, msys
        Note: \"linux\" will build \"linux_unsecured\", \"linux_secured\", \"linux_secured_with_rd\", \"linux_unsecured_with_mq\", \"linux_secured_with_tcp\" & \"linux_unsecured_with_tcp\" & \"linux_unsecured_with_rd\".
        Any selection will build both debug and release versions of all available targets in the scope you've selected.
        To choose any specific command, please use the SCons commandline directly. Please refer to [IOTIVITY_REPO]/Readme.scons.txt.
    clean:
        python %s -c
    '''
    print (helpstr % (script, script))
    sys.exit(1)

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

    if not EXEC_MODE:
        print ("Would run : " + cmd_line)
    else:
        print ("Running : " + cmd_line)
        sys.stdout.flush()
        exit_code = subprocess.Popen(cmd_line, shell=True).wait()
        if exit_code != 0:
            sys.exit(exit_code)

def build_all(flag, extra_option_str):
    if platform.system() == "Linux":
        build_linux_unsecured(flag, extra_option_str)
        build_linux_secured(flag, extra_option_str)
        build_linux_unsecured_with_rm(flag, extra_option_str)
        build_linux_unsecured_with_rd(flag, extra_option_str)
        build_linux_secured_with_rd(flag, extra_option_str)
        build_linux_unsecured_with_mq(flag, extra_option_str)
        build_linux_unsecured_with_tcp(flag, extra_option_str)
        build_linux_secured_with_tcp(flag, extra_option_str)
        build_linux_unsecured_with_java(flag, extra_option_str)
        build_linux_secured_with_java(flag, extra_option_str)
        build_simulator(flag, extra_option_str)

        build_android_unsecured(flag, extra_option_str)
        build_android_secured(flag, extra_option_str)
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
                        'SECURED':0,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_secured_with_tcp(flag, extra_option_str):
    print ("*********** Build for linux with Secured TCP ************")
    build_options = {
                        'RELEASE':flag,
                        'WITH_TCP': 1,
                        'WITH_CLOUD':1,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_java(flag, extra_option_str):
    print ("*********** Build for linux with Java support ************")
    build_options = {
                        'RELEASE':flag,
                        'BUILD_JAVA': 1,
                        'TARGET_TRANSPORT': 'IP',
                    }
    call_scons(build_options, extra_option_str)

def build_linux_secured_with_java(flag, extra_option_str):
    print ("*********** Build for linux with Java support and secured ************")
    build_options = {
                        'RELEASE':flag,
                        'BUILD_JAVA': 1,
                        'TARGET_TRANSPORT': 'IP',
                        'SECURED': 1,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_tcp(flag, extra_option_str):
    print ("*********** Build for linux with TCP ************")
    build_options = {
                        'RELEASE':flag,
                        'WITH_TCP': 1,
                        'TARGET_TRANSPORT': 'IP',
                        'SECURED':0,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_rm(flag, extra_option_str):
    print ("*********** Build for linux with RoutingManager************")
    build_options = {
                        'ROUTING':'GW',
                        'RELEASE':flag,
                        'SECURED':0,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_secured(flag, extra_option_str):
    print ("*********** Build for linux with Security *************")
    build_options = {
                        'RELEASE':flag,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_full(flag, extra_option_str):
    print ("*********** Build for linux with full features *************")
    build_options = {
                        'MULTIPLE_OWNER':1,
                        'RELEASE':flag,
                        'SECURED':1,
                        'WITH_CLOUD':1,
                        'WITH_RA':1,
                        'WITH_RA_IBB':1,
                        'WITH_TCP':1,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_rd(flag, extra_option_str):
    print ("*********** Build for linux With Resource Directory *************")
    build_options = {
                        'RELEASE':flag,
                        'RD_MODE':'all',
                        'SECURED':0,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_secured_with_rd(flag, extra_option_str):
    print ("*********** Build for linux With Resource Directory & Security ************")
    build_options = {
                        'RELEASE':flag,
                        'RD_MODE':'all',
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_mq(flag, extra_option_str):
    print ("*********** Build for linux With Message Queue ************")
    build_options = {
                        'RELEASE':flag,
                        'WITH_MQ':'PUB,SUB,BROKER',
                        'SECURED':0,
                    }
    call_scons(build_options, extra_option_str)

def build_linux_unsecured_with_tcp(flag, extra_option_str):
    print ("*********** Build for linux With tcp ************")
    build_options = {
                        'RELEASE':flag,
                        'WITH_TCP':'1',
                        'SECURED':0,
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

def build_android_secured(flag, extra_option_str):
    build_android(flag, extra_option_str + " SECURED=1")

def build_android_unsecured(flag, extra_option_str):
    build_android(flag, extra_option_str + " SECURED=0")

def build_android_x86(flag, extra_option_str):
    """ Build Android x86 Suite """
    build_android_x86_with_ip(flag, extra_option_str)
    build_android_x86_with_bt(flag, extra_option_str)
    build_android_x86_with_ble(flag, extra_option_str)

def build_android_x86_with_ip(flag, extra_option_str):
    print ("*********** Build for android x86 *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'x86',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'IP',
                    }
    call_scons(build_options, extra_option_str)

def build_android_x86_with_bt(flag, extra_option_str):
    print ("*********** Build for android x86 with Bluetooth *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'x86',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'BT',
                    }
    call_scons(build_options, extra_option_str)

def build_android_x86_with_ble(flag, extra_option_str):
    print ("*********** Build for android x86 with Bluetooth Low Energy *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'x86',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'BLE',
                    }
    call_scons(build_options, extra_option_str)

def build_android_x86_with_rm(flag, extra_option_str):
    """ Build Android x86 Routing Manager Suite """
    build_android_x86_with_rm_and_ip(flag, extra_option_str)
    build_android_x86_with_rm_and_bt(flag, extra_option_str)
    build_android_x86_with_rm_and_ble(flag, extra_option_str)

def build_android_x86_with_rm_and_ip(flag, extra_option_str):
    print ("*********** Build for android x86 with Routing Manager *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'x86',
                        'ROUTING':'GW',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'IP',
                    }
    call_scons(build_options, extra_option_str)

def build_android_x86_with_rm_and_bt(flag, extra_option_str):
    print ("*********** Build for android x86 with Routing Manager and Bluetooth *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'x86',
                        'ROUTING':'GW',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'BT',
                    }
    call_scons(build_options, extra_option_str)

def build_android_x86_with_rm_and_ble(flag, extra_option_str):
    print ("*********** Build for android x86 with Routing Manager and Bluetooth Low Energy *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'x86',
                        'ROUTING':'GW',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'BLE',
                    }
    call_scons(build_options, extra_option_str)

def build_android_armeabi(flag, extra_option_str):
    """ Build Android Armeabi Suite """
    build_android_armeabi_with_ip(flag, extra_option_str)
    build_android_armeabi_with_bt(flag, extra_option_str)
    build_android_armeabi_with_ble(flag, extra_option_str)

def build_android_armeabi_with_ip(flag, extra_option_str):
    print ("*********** Build for android armeabi *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'armeabi',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'IP',
                    }
    call_scons(build_options, extra_option_str)

def build_android_armeabi_with_bt(flag, extra_option_str):
    print ("*********** Build for android armeabi with Bluetooth *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'armeabi',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'BT',
                    }
    call_scons(build_options, extra_option_str)

def build_android_armeabi_with_ble(flag, extra_option_str):
    print ("*********** Build for android armeabi with Bluetooth Low Energy *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'armeabi',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'BLE',
                    }
    call_scons(build_options, extra_option_str)

def build_android_armeabi_with_rm(flag, extra_option_str):
    """ Build Android Armeabi Routing Manager Suite """
    build_android_armeabi_with_rm_and_ip(flag, extra_option_str)
    build_android_armeabi_with_rm_and_bt(flag, extra_option_str)
    build_android_armeabi_with_rm_and_ble(flag, extra_option_str)

def build_android_armeabi_with_rm_and_ip(flag, extra_option_str):
    print ("*********** Build for android armeabi with Routing Manager *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'armeabi',
                        'ROUTING':'GW',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'IP',
                    }
    call_scons(build_options, extra_option_str)

def build_android_armeabi_with_rm_and_bt(flag, extra_option_str):
    print ("*********** Build for android armeabi with Routing Manager and Bluetooth *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'armeabi',
                        'ROUTING':'GW',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'BT',
                    }
    call_scons(build_options, extra_option_str)

def build_android_armeabi_with_rm_and_ble(flag, extra_option_str):
    print ("*********** Build for android armeabi with Routing Manager and Bluetooth Low Energy *************")
    build_options = {
                        'TARGET_OS':'android',
                        'TARGET_ARCH':'armeabi',
                        'ROUTING':'GW',
                        'RELEASE':flag,
                        'TARGET_TRANSPORT':'BLE',
                    }
    call_scons(build_options, extra_option_str)

def build_arduino(flag, extra_option_str):
    print ("*********** Build for arduino avr *************")
    extra_option_str = "resource " + extra_option_str
    build_options = {
                        'TARGET_OS':'arduino',
                        'UPLOAD':'false',
                        'BOARD':'mega',
                        'MULTIPLE_OWNER':0,
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
    print ("*********** Build for Tizen with options *************")
    cmd_line = os.getcwd() + "/gbsbuild.sh" + " " + extra_option_str
    if not EXEC_MODE:
        print ("Would run : " + cmd_line)
    else:
        print ("Running : " + cmd_line)
        exit_code = subprocess.Popen([cmd_line], shell=True).wait()
        if exit_code != 0:
            sys.exit(exit_code)

    print ("*********** Build for Tizen octbstack lib and sample *************")
    build_extra_options = "-f resource/csdk/stack/samples/tizen/build/SConscript " + extra_option_str
    build_options = {
                        'TARGET_OS':'tizen',
                        'TARGET_TRANSPORT':'IP',
                        'LOGGING':'true',
                        'RELEASE':flag,
                    }
    call_scons(build_options, build_extra_options)

    print ("*********** Build for Tizen octbstack lib and sample with Routing Manager*************")
    build_options['ROUTING'] = 'GW'
    call_scons(build_options, build_extra_options)

    print ("*********** Build for Tizen Easy-Setup  sample *************")
    build_options['ROUTING'] = 'EP'
    build_options['ES_TARGET_ENROLLEE'] = 'tizen'
    build_extra_options = "-f service/easy-setup/sampleapp/enrollee/tizen-sdb/EnrolleeSample/build/tizen/SConscript " + extra_option_str
    call_scons(build_options, build_extra_options)

def build_tizen_secured(flag, extra_option_str):
    build_tizen(flag, extra_option_str + " SECURED=1")

def build_tizen_unsecured(flag, extra_option_str):
    build_tizen(flag, extra_option_str + " SECURED=0")

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
                        'TARGET_TRANSPORT':'IP',
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
                        'TARGET_TRANSPORT':'IP',
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
                        'SECURED':0,
                        'TARGET_TRANSPORT':'IP',
                    }
    call_scons(build_options, extra_option_str)

def unit_tests():
    print ("*********** Unit test Start *************")
    build_options = {
                        'RELEASE':'false',
                    }
    extra_option_str = "-c ."
    call_scons(build_options, extra_option_str)

    build_options = {
                        'TEST':1,
                        'RELEASE':'false',
                        'SECURED':0,
                    }
    extra_option_str = ""
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
EXEC_MODE = os.getenv("EXEC_MODE", True)
if EXEC_MODE in ['false', 'False', '0']:
    EXEC_MODE = False

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

    elif str(sys.argv[1]) == "linux_full":
        build_linux_full("true", "")
        build_linux_full("false", "")

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

    elif str(sys.argv[1]) == "linux_unsecured_with_java":
        build_linux_unsecured_with_java("false", "")
        build_linux_unsecured_with_java("true", "")

    elif str(sys.argv[1]) == "linux_secured_with_java":
        build_linux_secured_with_java("false", "")
        build_linux_secured_with_java("true", "")

    elif str(sys.argv[1]) == "android":
        build_android("true", "")
        build_android("false", "")

    elif str(sys.argv[1]) == "android_unsecured":
        build_android_unsecured("true", "")
        build_android_unsecured("false", "")

    elif str(sys.argv[1]) == "android_secured":
        build_android_secured("true", "")
        build_android_secured("false", "")

    elif str(sys.argv[1]) == "android_x86":
        build_android_x86("true", "")
        build_android_x86("false", "")
        build_android_x86_with_rm("true", "")
        build_android_x86_with_rm("false", "")

    elif str(sys.argv[1]) == "android_x86_with_ip":
        build_android_x86_with_ip("true", "")
        build_android_x86_with_ip("false", "")

    elif str(sys.argv[1]) == "android_x86_with_bt":
        build_android_x86_with_bt("true", "")
        build_android_x86_with_bt("false", "")

    elif str(sys.argv[1]) == "android_x86_with_ble":
        build_android_x86_with_ble("true", "")
        build_android_x86_with_ble("false", "")

    elif str(sys.argv[1]) == "android_x86_with_rm_and_ip":
        build_android_x86_with_rm_and_ip("true", "")
        build_android_x86_with_rm_and_ip("false", "")

    elif str(sys.argv[1]) == "android_x86_with_rm_and_bt":
        build_android_x86_with_rm_and_bt("true", "")
        build_android_x86_with_rm_and_bt("false", "")

    elif str(sys.argv[1]) == "android_x86_with_rm_and_ble":
        build_android_x86_with_rm_and_ble("true", "")
        build_android_x86_with_rm_and_ble("false", "")

    elif str(sys.argv[1]) == "android_armeabi":
        build_android_armeabi("true", "")
        build_android_armeabi("false", "")
        build_android_armeabi_with_rm("true", "")
        build_android_armeabi_with_rm("false", "")

    elif str(sys.argv[1]) == "android_armeabi_with_ip":
        build_android_armeabi_with_ip("true", "")
        build_android_armeabi_with_ip("false", "")

    elif str(sys.argv[1]) == "android_armeabi_with_bt":
        build_android_armeabi_with_bt("true", "")
        build_android_armeabi_with_bt("false", "")

    elif str(sys.argv[1]) == "android_armeabi_with_ble":
        build_android_armeabi_with_ble("true", "")
        build_android_armeabi_with_ble("false", "")

    elif str(sys.argv[1]) == "android_armeabi_with_rm_and_ip":
        build_android_armeabi_with_rm_and_ip("true", "")
        build_android_armeabi_with_rm_and_ip("false", "")

    elif str(sys.argv[1]) == "android_armeabi_with_rm_and_bt":
        build_android_armeabi_with_rm_and_bt("true", "")
        build_android_armeabi_with_rm_and_bt("false", "")

    elif str(sys.argv[1]) == "android_armeabi_with_rm_and_ble":
        build_android_armeabi_with_rm_and_ble("true", "")
        build_android_armeabi_with_rm_and_ble("false", "")

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

    elif str(sys.argv[1]) == "tizen_unsecured":
        build_tizen_unsecured("true", "")
        build_tizen_unsecured("false", "")

    elif str(sys.argv[1]) == "tizen_secured":
        build_tizen_secured("true", "")
        build_tizen_secured("false", "")

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
