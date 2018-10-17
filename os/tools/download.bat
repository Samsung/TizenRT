@echo off

rem
rem Copyright 2018 Samsung Electronics All Rights Reserved.
rem
rem Licensed under the Apache License, Version 2.0 (the "License");
rem you may not use this file except in compliance with the License.
rem You may obtain a copy of the License at
rem
rem http://www.apache.org/licenses/LICENSE-2.0
rem
rem Unless required by applicable law or agreed to in writing,
rem software distributed under the License is distributed on an
rem "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
rem either express or implied. See the License for the specific
rem language governing permissions and limitations under the License.
rem

set CURPATH=%CD%
set BATPATH=%~dp0
set BUILDPATH=%BATPATH%..\..\build
set BINPATH=%BUILDPATH%\output\bin
reg Query "HKLM\Hardware\Description\System\CentralProc>essor\0" | find /i "x86" > NUL && set OSBIT=win32 || set OSBIT=win64

call %BATPATH%board.bat

if "%BOARD%"=="artik053" goto :DownloadARTIK_NONSECURE
if "%BOARD%"=="artik053s" goto :DownloadARTIK_SECURE
if "%BOARD%"=="artik055s" goto :DownloadARTIK_SECURE
if "%BOARD%"=="cy4390x" goto :DownloadCypress
goto :NoBoard

:DownloadARTIK_SECURE
set RTBIN=tinyara_head.bin-signed
goto DownloadARTIK

:DownloadARTIK_NONSECURE
set RTBIN=tinyara_head.bin
goto DownloadARTIK

:DownloadARTIK
set OPENOCDPATH=%BUILDPATH%\tools\openocd\%OSBIT%
set SCRIPTPATH=%BUILDPATH%\configs\artik05x\scripts
set PREBUILTBINPATH=%BUILDPATH%\configs\%BOARD%\bin
set PREBUILTCOMMANDS="flash_protect off; flash_write bl1 bl1.bin; flash_protect on; flash_write bl2 bl2.bin; flash_write sssfw sssfw.bin; flash_write wlanfw wlanfw.bin; "
if exist %BINPATH%\romfs.img set ROMFS=flash_write rom romfs.img;
if exist %BINPATH%\ota.bin set OTA=flash_write ota ota.bin;
set OSCOMMANDS="flash_write os %RTBIN%; %ROMFS% %OTA% "

if not exist %OPENOCDPATH%\openocd.exe (
	echo "The openocd tool is not in %OPENOCDPATH%"
	exit 1
)

cd %PREBUILTBINPATH%
%OPENOCDPATH%\openocd.exe -f artik05x.cfg -s %SCRIPTPATH% -c %PREBUILTCOMMANDS% -c "init; reset; exit; "

cd %BINPATH%
%OPENOCDPATH%\openocd.exe -f artik05x.cfg -s %SCRIPTPATH% -c %OSCOMMANDS% -c "init; reset; exit; "
goto End

:DownloadCypress
set CYTOOLSPATH=%BUILDPATH%\configs\%BOARD%\tools
set PREBUILTBINPATH=%CYTOOLSPATH%\binary
set OPENOCDPATH=%CYTOOLSPATH%\OpenOCD\%OSBIT%

set CFGFILE1=%CYTOOLSPATH%\OpenOCD\BCM9WCD1EVAL1.cfg
set CFGFILE2=%CYTOOLSPATH%\OpenOCD\BCM4390x.cfg
set CFGFILE3=%CYTOOLSPATH%\OpenOCD\BCM4390x_gdb_jtag.cfg
set STRIPFILE=tinyara_master_strip
set ROMFSFILE=romfs.img
set BLFILE=waf.bootloader.43909.trx.bin

set BLCOMMAND="sflash_write_file %BLFILE% 0x00000000 BCM943909WCD1_3-P320-SoC.43909 1 43909; "

if exist %BINPATH%\romfs.img set ROMCOMMAND="sflash_write_file %ROMFSFILE% 0x00408000 BCM943909WCD1_3-P320-SoC.43909 0 43909; "
set OSCOMMAND="sflash_write_file %STRIPFILE%  0x0008D000 BCM943909WCD1_3-P320-SoC.43909 0 43909; %ROMCOMMAND% "

if not exist %OPENOCDPATH%\openocd-all-brcm-libftdi.exe (
	echo "The openocd tool is not in %OPENOCDPATH%"
	exit 1
)

cd %PREBUILTBINPATH%
%OPENOCDPATH%\openocd-all-brcm-libftdi.exe -f %CFGFILE1% -f %CFGFILE2% -f %CFGFILE3% -c %BLCOMMAND% -c shutdown

cd %BINPATH%
%OPENOCDPATH%\openocd-all-brcm-libftdi.exe -f %CFGFILE1% -f %CFGFILE2% -f %CFGFILE3% -c %OSCOMMAND% -c shutdown

goto End

:NoBoard
echo Wrong Board Name

:ShowUsage
echo Usage: %~nx0 -b BOARD
echo Download TizenRT binary into BOARD
echo 	-b BOARD      Set board name like artik053, cy4390x
echo 	--help        Show this text and exit

:End
cd %CURPATH%
echo %BOARD% Download Done
