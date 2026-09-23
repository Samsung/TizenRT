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
# NOTE: This regeneration block is only used by the standalone armino/CMake build
# of bk_wifi (e.g. when producing the prebuilt libwifi.a). The bk7239n firmware in
# TizenRT is compiled by the Makefile build system, which never parses this file;
# there the equivalent "edit db.txt -> single-build regen" logic lives in
# src/Make.defs. Keep the two in sync when the db/scripts flow changes.
#
# Regenerate regdb_bin.c from db.txt *before* compiling, so that editing db.txt
# and doing a single build picks up the change in one shot (no need to build twice).
# The human-readable channel/power table (Excel) is refreshed in scripts/ as well.
set(REGDB_SCRIPTS ${CMAKE_CURRENT_LIST_DIR}/scripts)
set(REGDB_DB      ${REGDB_SCRIPTS}/db.txt)
set(REGDB_BIN_C   ${CMAKE_CURRENT_LIST_DIR}/src/regdb_bin.c)
set(REGDB_TMP_BIN ${CMAKE_CURRENT_BINARY_DIR}/regulatory.db)
set(REGDB_XLSX    ${REGDB_SCRIPTS}/regdb_channel_map.xlsx)

add_custom_command(
	OUTPUT ${REGDB_BIN_C}
	COMMAND ${REGDB_SCRIPTS}/db2fw.py ${REGDB_TMP_BIN} ${REGDB_DB}
	COMMAND ${REGDB_SCRIPTS}/bin2c.py ${REGDB_TMP_BIN} ${REGDB_BIN_C}
	# refresh the human-readable channel/power table (non-fatal if openpyxl is missing)
	COMMAND sh -c "python3 ${REGDB_SCRIPTS}/gen_regdb_xlsx.py ${REGDB_DB} ${REGDB_XLSX} || true"
	DEPENDS ${REGDB_DB}
	        ${REGDB_SCRIPTS}/db2fw.py
	        ${REGDB_SCRIPTS}/bin2c.py
	        ${REGDB_SCRIPTS}/dbparse.py
	        ${REGDB_SCRIPTS}/gen_regdb_xlsx.py
	COMMENT "Generating regdb_bin.c and regdb_channel_map.xlsx from db.txt"
	VERBATIM)

# Force the generation to run before the component is compiled/linked.
add_custom_target(bk_wifi_regdb_gen DEPENDS ${REGDB_BIN_C})
add_dependencies(${target} bk_wifi_regdb_gen)

# Keep the original generated-headers consistency check (runs after build).
add_custom_command(
	TARGET ${target} POST_BUILD
	COMMAND sh ${REGDB_SCRIPTS}/check_generated_headers.sh ${CMAKE_CURRENT_LIST_DIR}
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

