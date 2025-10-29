set(incs .)

if (CONFIG_BKREG)
set(srcs 
	cmd_help.c
	cmd_reg.c
	command_ate.c
	command_ate_adc.c
	command_ate_audio.c
	command_ate_memcheck.c
	command_ate_vdddig.c
	command_line.c
	command_table.c
	udebug.c)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_wifi bk_adapter bk_phy bk_bluetooth
	)
