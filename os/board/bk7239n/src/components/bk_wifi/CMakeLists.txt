set(incs include include/bk_private)
set(srcs)
if(CONFIG_WIFI_ENABLE)
list(APPEND srcs
	src/bk_wifi_adapter.c
	src/bk_workqueue.c
	src/hostapd_intf.c
	src/lsig_monitor.c
	src/net_param.c
	src/pbuf.c
	src/phy.c
	src/regdb.c
	src/regdb_bin.c
	src/reg.c
	src/rw_ieee80211.c
	src/rw_msdu.c
	src/rw_msg_rx.c
	src/rw_msg_tx.c
	src/rw_task.c
	src/rw_tx_buffering.c
	src/rwm_proto.c
	src/rwnx_misc.c
	src/rwnx_params.c
	src/rwnx_rx.c
	src/rwnx_td.c
	src/rwnx_tx.c
	src/rwnx_txq.c
	src/rwnx_utils.c
	src/sa_ap.c
	src/sa_station.c
	src/skbuff.c
	src/soft_encrypt.c
	src/wifi_api.c
	src/wifi_config.c
	src/wifi_frame.c
	src/wifi_init.c
	src/wifi_netif.c
	src/wifi_v2.c
	#src/wifi_wpa_cmd.c
)
if(CONFIG_AT)
list(APPEND srcs
	src/wifi_at.c
)
set(components ../../components)
set(IDK_INC ../../include)
list(APPEND incs
	${components}/at_server
	${components}/at_server/_at_server
	${components}/at_server/_at_server_port
	${components}/bk_net/include
	${components}/bk_event
	${IDK_INC}/modules
	)
endif()
endif()
if(CONFIG_NET_PARAM)
list(APPEND srcs
	src/net_param.c
)
endif()

armino_component_register(SRCS "${srcs}"
    INCLUDE_DIRS "${incs}"
    PRIV_REQUIRES bk_common wpa_supplicant-2.10 psa_mbedtls lwip_intf_v2_1 uart_debug bk_phy
    REQUIRES bk_netif)


get_filename_component(config_component_name ${CMAKE_CURRENT_LIST_DIR} NAME)
set(config_component_name ${config_component_name})
armino_component_get_target(target "${config_component_name}")
armino_build_get_property(armino_path ARMINO_PATH)

if(CONFIG_WIFI_ENABLE)
add_custom_command(
	TARGET ${target} POST_BUILD
	COMMAND sh ${CMAKE_CURRENT_LIST_DIR}/scripts/check_generated_headers.sh ${CMAKE_CURRENT_LIST_DIR}
	COMMAND ${CMAKE_CURRENT_LIST_DIR}/scripts/db2fw.py regulatory.db ${CMAKE_CURRENT_LIST_DIR}/scripts/db.txt
	COMMAND ${CMAKE_CURRENT_LIST_DIR}/scripts/bin2c.py regulatory.db ${CMAKE_CURRENT_LIST_DIR}/src/regdb_bin.c
	COMMENT "Check bk_wifi/include/generated/ directory have changes not submitted ..."
	VERBATIM)
endif()

if(CONFIG_WIFI_ENABLE)

	if (CONFIG_TZ AND NOT CONFIG_SPE)
		if(CONFIG_WIFI_CSI_EN)
			set (wifi_elem libwifi_csi_nspe.a)
		else()
			set (wifi_elem libwifi_nspe.a)
		endif()

	else()
		if(CONFIG_WIFI_CSI_EN)
			set (wifi_elem libwifi_csi.a)
		else()
			set (wifi_elem libwifi.a)
		endif()

	endif()

	foreach(lib_name ${wifi_elem})
		#message("WARINIG lib_name ${ARMINO_PATH}/components/bk_libs/${ARMINO_SOC}_$ENV{PROJECT_LIBS}/libs/${lib_name}")
		add_prebuilt_library(${lib_name} "${ARMINO_PATH}/components/bk_libs/${ARMINO_SOC}/libs/${lib_name}")
		target_link_libraries(${COMPONENT_LIB} INTERFACE ${lib_name})
	endforeach()

endif()

armino_component_register_call_subdirs()

