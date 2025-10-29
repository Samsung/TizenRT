#-------------------------------------------------------------------------------
# Copyright (c) 2017-2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------


#Print a message and exit with failure
#
#Examples:
#   failure("Something is wrong!")

function(failure)
	message(FATAL_ERROR "${ARGN}")
endfunction()

#Convert \ directory separators to / on windows systems
#
#Convert the directory separators to forward slash on windows. Avoid
#conversion if path contains any forward slashes to avoid mixing up cygwin or
#mingw paths where an extra caharacter is escaped (i.e. "/c/Program\ Files/")
#
#Examples:
#   set(MY_PATH "C:\foo\bar")
#   win_fix_dir_sep(PATH MY_PATH)
#
#INPUTS:
#   PATH  - (mandatory) - name of the string variable to operate on
#
#OUTPUTS
#   PATH is modified as needed.
#
function(win_fix_dir_sep)
	#Parse our arguments
	set( _OPTIONS_ARGS )            #No option (on/off) arguments (e.g. IGNORE_CASE)
	set( _ONE_VALUE_ARGS  PATH )    #Single option arguments (e.g. PATH "./foo/bar")
	set( _MULTI_VALUE_ARGS )        #List arguments (e.g. LANGUAGES C ASM CXX)
	cmake_parse_arguments(_MY_PARAMS "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN} )

	#Check mandatory parameters
	if(NOT _MY_PARAMS_PATH)
		failure("win_fix_dir_sep(): Missing mandatory parameter PATH!")
	endif()
	set(_PATH ${_MY_PARAMS_PATH})

	#To avoid trouble on windows change directory separator.
	if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
		#Do not convert directory separator if there are forward slashes
		#present. This is to avoid mixing up escaped characters in cygwin
		#or mingw paths (i.e. c:/Program\ Files/something)
		string(FIND "${${_PATH}}" "/" _is_found)
		if (_is_found LESS 0)
			string(REPLACE "\\" "/" ${_PATH} "${${_PATH}}")
			set(${_PATH} "${${_PATH}}" PARENT_SCOPE)
		endif()
	endif()
endfunction()
