macro(initialize_pal)
    set(PAL_INITIALIZED TRUE)
endmacro()

macro(add_pal_to_target TGT)
    target_include_directories(${TGT} PUBLIC ${OM_PAL_DIR})
endmacro()
