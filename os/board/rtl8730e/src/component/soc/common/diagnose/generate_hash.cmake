#Note: Previously defined variables cannot be used directly in this file
#unless passed through -D
cmake_minimum_required(VERSION 3.20.0)
include(${c_CMAKE_FILES_DIR}/utility.cmake)
include(${c_CMAKE_FILES_DIR}/global_define.cmake)

string(REPLACE " " ";" fmt_head "${fmt_head}")
ameba_execute_process(
    COMMAND python ${c_DIAG_FMT_SCRIPT} h2j ${fmt_head} ${fmt_json}
)
execute_process(COMMAND_ERROR_IS_FATAL ANY
    COMMAND python ${c_DIAG_FMT_SCRIPT} hash ${fmt_json}
    OUTPUT_VARIABLE HASH_STRING
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
configure_file(${CMAKE_CURRENT_LIST_DIR}/ameba_diagnose_hash.c.in ${fmt_src} @ONLY)

#${fmt_full} MUST be checkout before regenerate
if(GIT_EXECUTABLE)
    set(attempt 1)
    set(MAX_ATTEMPTS 3)
    while(attempt LESS ${MAX_ATTEMPTS})
        execute_process(
            COMMAND ${GIT_EXECUTABLE} checkout ${fmt_full}
            WORKING_DIRECTORY ${c_CMPT_DIAGNOSE_DIR}
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output
            ERROR_VARIABLE error
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE
        )
        if(result EQUAL 0)
            break()
        endif()
        ameba_delay_second(1)

        math(EXPR attempt "${attempt} + 1")
    endwhile()
else()
    ameba_warning("No git to checkout ${fmt_full}")
endif()

ameba_execute_process(
    COMMAND python ${c_DIAG_FMT_SCRIPT} sum ${fmt_json} ${fmt_full}
)