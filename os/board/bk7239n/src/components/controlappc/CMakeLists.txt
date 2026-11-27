set(incs ./)
set(srcs)
if (CONFIG_QUICK_TRACK)
	set(srcs 
	"controlappc_main.c"
	"controlapp_eloop.c"
	"indigo_api.c"
	"indigo_packet.c"
	"controlapp_utils.c"
	"indigo_api_callback_dut.c"
	"vendor_specific_dut.c"
	"quick_uart.c"
	)
endif()
armino_component_register(SRCS "${srcs}" 
						  INCLUDE_DIRS "${incs}" 
						  PRIV_REQUIRES wpa_supplicant-2.10 )
