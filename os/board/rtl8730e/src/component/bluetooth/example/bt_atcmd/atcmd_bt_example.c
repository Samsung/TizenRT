/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdlib.h>
#include <string.h>
#include <log_service.h>
#include <bt_utils.h>
#include <atcmd_bt_impl.h>

_WEAK int bt_demo_main(void)
{
	AT_PRINTK("[ATBE] NO demo example!");
	return -1;
}

_WEAK int bt_audio_mp_test_main(void)
{
	AT_PRINTK("[ATBE] NO bt audio mp test example!");
	return -1;
}

_WEAK int ble_central_main(uint8_t enable)
{
	(void)enable;
	AT_PRINTK("[ATBE] NO central example!");
	return -1;
}

_WEAK int ble_peripheral_main(uint8_t enable)
{
	(void)enable;
	AT_PRINTK("[ATBE] NO peripheral example!");
	return -1;
}

_WEAK int ble_scatternet_main(uint8_t enable)
{
	(void)enable;
	AT_PRINTK("[ATBE] NO scatternet example!");
	return -1;
}

#if defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE
_WEAK int ble_mesh_device_main(uint8_t enable)
{
	(void)enable;
	AT_PRINTK("[ATBE] NO mesh device example!");
	return -1;
}
#endif

#if defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER
_WEAK int ble_mesh_provisioner_main(uint8_t enable)
{
	(void)enable;
	AT_PRINTK("[ATBE] NO mesh provisioner example!");
	return -1;
}
#endif

_WEAK int bt_a2dp_main(uint8_t role, uint8_t enable)
{
	(void)role;
	(void)enable;
	AT_PRINTK("[ATBE] NO a2dp example!");
	return -1;
}

_WEAK int bt_spp_main(uint8_t role, uint8_t enable)
{
	(void)role;
	(void)enable;
	AT_PRINTK("[ATBE] NO spp example!");
	return -1;
}

_WEAK int bt_hfp_main(uint8_t role, uint8_t enable)
{
	(void)role;
	(void)enable;
	AT_PRINTK("[ATBE] NO hfp example!");
	return -1;
}

_WEAK int ble_cis_acceptor_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble cis peripheral example!");
	return -1;
}
_WEAK int ble_cis_initiator_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble cis initiator example!");
	return -1;
}
_WEAK int ble_bis_broadcaster_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble bis broadcaster example!");
	return -1;
}
_WEAK int ble_bis_receiver_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble bis receiver example!");
	return -1;
}
_WEAK int ble_audio_broadcast_sink_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble audio broadcast sink example!");
	return -1;
}
_WEAK int ble_audio_broadcast_source_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble audio broadcast source example!");
	return -1;
}
_WEAK int ble_audio_unicast_server_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble audio unicast server example!");
	return -1;
}
_WEAK int ble_audio_unicast_client_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble audio unicast client example!");
	return -1;
}
_WEAK int ble_audio_acceptor_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble audio acceptor example!");
	return -1;
}
_WEAK int ble_audio_initiator_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble audio initiator example!");
	return -1;
}
_WEAK int ble_audio_commander_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBE] NO ble audio commander example!");
	return -1;
}
int atcmd_bt_demo(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	if (bt_demo_main()) {
		AT_PRINTK("[ATBE] Error: demo example failed!");
		return -1;
	}

	AT_PRINTK("[ATBE] demo example OK!");
	return 0;
}

int atcmd_bt_audio_mp_test(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	if (bt_audio_mp_test_main()) {
		AT_PRINTK("[ATBE] Error: bt audio mp test example failed!");
		return -1;
	}

	AT_PRINTK("[ATBE] bt audio mp test OK!");
	return 0;
}

int atcmd_bt_central(int argc, char *argv[])
{
	uint8_t op;
	char *action[] = {"disable", "enable"};

	/* It's neccessary to judge if the args num(argc) is correct, otherwise it may
	    be hardfault because the address of argv[x] may be NULL */
	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	if ((op = (uint8_t)str_to_int(argv[0])) > 1) {
		AT_PRINTK("[ATBE] Error: wrong value (%d) for central!", op);
		return -1;
	}

	if (ble_central_main(op)) {
		AT_PRINTK("[ATBE] Error: central example %s failed!", action[op]);
		return -1;
	}

	AT_PRINTK("[ATBE] central example %s OK!", action[op]);
	return 0;
}

int atcmd_bt_peripheral(int argc, char *argv[])
{
	uint8_t op;
	char *action[] = {"disable", "enable"};

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}


	if ((op = (uint8_t)str_to_int(argv[0])) > 1) {
		AT_PRINTK("[ATBE] Error: wrong value (%d) for peripheral!", op);
		return -1;
	}

	if (ble_peripheral_main(op)) {
		AT_PRINTK("[ATBE] Error: peripheral example %s failed!", action[op]);
		return -1;
	}

	AT_PRINTK("[ATBE] peripheral example %s OK!", action[op]);
	return 0;
}

int atcmd_bt_scatternet(int argc, char *argv[])
{
	uint8_t op;
	char *action[] = {"disable", "enable"};

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	if ((op = (uint8_t)str_to_int(argv[0])) > 1) {
		AT_PRINTK("[ATBE] Error: wrong value (%d) for scatternet!", op);
		return -1;
	}

	if (ble_scatternet_main(op)) {
		AT_PRINTK("[ATBE] Error: scatternet example %s failed!", action[op]);
		return -1;
	}

	AT_PRINTK("[ATBE] scatternet example %s OK!", action[op]);
	return 0;
}

#if defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE
extern int ble_mesh_device_main(uint8_t enable);
int atcmd_bt_mesh_device(int argc, char *argv[])
{
	uint8_t op;
	char *action[] = {"disable", "enable"};

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}
	op = (uint8_t)str_to_int(argv[0]);
	if ((op = (uint8_t)str_to_int(argv[0])) > 1) {
		AT_PRINTK("[ATBE] Error: wrong value (%d) for scatternet!", op);
		return -1;
	}

	if (ble_mesh_device_main(op)) {
		AT_PRINTK("[ATBE] Error: mesh device example %s failed!", action[op]);
		return -1;
	}

	AT_PRINTK("[ATBE] mesh device example %s OK!", action[op]);
	return 0;
}
#endif

#if defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER
extern int ble_mesh_provisioner_main(uint8_t enable);
int atcmd_bt_mesh_provisioner(int argc, char *argv[])
{
	uint8_t op;
	char *action[] = {"disable", "enable"};

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}
	op = (uint8_t)str_to_int(argv[0]);
	if ((op = (uint8_t)str_to_int(argv[0])) > 1) {
		AT_PRINTK("[ATBE] Error: wrong value (%d) for scatternet!", op);
		return -1;
	}

	if (ble_mesh_provisioner_main(op)) {
		AT_PRINTK("[ATBE] Error: mesh provisioner example %s failed!", action[op]);
		return -1;
	}

	AT_PRINTK("[ATBE] mesh provisioner example %s OK!", action[op]);
	return 0;
}
#endif

int atcmd_bt_a2dp(int argc, char *argv[])
{
	(void)argc;
	uint8_t role;
	uint8_t op;
	char *action[] = {"disable", "enable"};

	if (strcmp(argv[0], "snk") == 0) {
		role = 0;
		AT_PRINTK("[ATBE] Set a2dp role (snk)");
	} else if (strcmp(argv[0], "src") == 0) {
		role = 1;
		AT_PRINTK("[ATBE] Set a2dp role (src)");
	} else {
		AT_PRINTK("[ATBE] invalid role set");
		return -1;
	}
	if ((op = (uint8_t)str_to_int(argv[1])) > 2) {
		AT_PRINTK("[ATBE] Error: wrong value (%d) for a2dp demo !", op);
		return -1;
	}
	if (bt_a2dp_main(role, op)) {
		AT_PRINTK("[ATBE] Error: a2dp example %s failed!", action[op]);
		return -1;
	}

	AT_PRINTK("[ATBE] a2dp example %s OK!", action[op]);
	return 0;
}

int atcmd_bt_hfp(int argc, char *argv[])
{
	(void)argc;
	uint8_t role;
	uint8_t op;
	char *action[] = {"disable", "enable"};

	if (strcmp(argv[0], "hf") == 0) {
		role = 1;
		AT_PRINTK("[ATBE] Set hfp role (hand free)");
	} else if (strcmp(argv[0], "ag") == 0) {
		role = 0;
		AT_PRINTK("[ATBE] Set hfp role (audio gate)");
	} else {
		AT_PRINTK("[ATBE] invalid role set");
		return -1;
	}
	if ((op = (uint8_t)str_to_int(argv[1])) > 2) {
		AT_PRINTK("[ATBE] Error: wrong value (%d) for hfp demo !", op);
		return -1;
	}
	if (bt_hfp_main(role, op)) {
		AT_PRINTK("[ATBE] Error: hfp example %s failed!", action[op]);
		return -1;
	}

	AT_PRINTK("[ATBE] hfp example %s OK!", action[op]);
	return 0;
}

int atcmd_bt_spp(int argc, char *argv[])
{
	(void)argc;
	uint8_t role;
	uint8_t op;
	char *action[] = {"disable", "enable"};

	if (strcmp(argv[0], "client") == 0) {
		role = 0;
		AT_PRINTK("[ATBE] Set spp role (client)");
	} else if (strcmp(argv[0], "server") == 0) {
		role = 1;
		AT_PRINTK("[ATBE] Set spp role (server)");
	} else {
		AT_PRINTK("[ATBE] invalid role set");
		return -1;
	}
	if ((op = (uint8_t)str_to_int(argv[1])) > 2) {
		AT_PRINTK("[ATBE] Error: wrong value (%d) for spp!", op);
		return -1;
	}
	if (bt_spp_main(role, op)) {
		AT_PRINTK("[ATBE] Error: spp example %s failed!", action[op]);
		return -1;
	}

	AT_PRINTK("[ATBE] spp example %s OK!", action[op]);
	return 0;
}

int atcmd_ble_iso(int argc, char *argv[])
{
	int ret = 0;

	if (argc < 2) {
		AT_PRINTK("[ATBE] Error: wrong arg number (%d) for cis!", argc);
		return -1;
	}

	if (strcmp(argv[0], "acceptor") == 0) {
		ret =  ble_cis_acceptor_main(argc - 1, argv + 1); //{"disable", "enable"}
		if (ret != 0) {
			AT_PRINTK("[ATBE] Error: ble cis acceptor example %s failed! ret= %d", argv[1], ret);
		}

	} else if (strcmp(argv[0], "initiator") == 0) {
		ret =  ble_cis_initiator_main(argc - 1, argv + 1); //{"disable", "enable"}
		if (ret != 0) {
			AT_PRINTK("[ATBE] Error: ble cis initiator example %s failed! ret= %d", argv[1], ret);
		}

	} else if (strcmp(argv[0], "broadcaster") == 0) {
		ret =  ble_bis_broadcaster_main(argc - 1, argv + 1); //{"disable", "enable", "send"}
		if (ret != 0) {
			AT_PRINTK("[ATBE] Error: ble bis broadcaster example %s failed! ret= %d", argv[1], ret);
		}

	} else if (strcmp(argv[0], "receiver") == 0) {
		ret =  ble_bis_receiver_main(argc - 1, argv + 1); //{"disable", "enable"}
		if (ret != 0) {
			AT_PRINTK("[ATBE] Error: ble bis receiver example %s failed! ret= %d", argv[1], ret);
		}
	} else {
		AT_PRINTK("[ATBE] Error: wrong arg (%s) for ISO!", argv[0]);
		return -1;
	}

	return ret;
}

int atcmd_ble_audio(int argc, char *argv[])
{
	int ret = 0;

	if (argc < 2) {
		AT_PRINTK("[ATBE] Error: wrong arg number (%d) for le audio!", argc);
		return -1;
	}	

	if (strcmp(argv[0], "unicast") == 0) {
		if (argc < 3) {
			AT_PRINTK("[ATBE] Error: wrong arg number (%d) for le audio unicast!", argc);
			AT_PRINTK("[ATBE]usage: ATBE=leaudio,unicast,server or client,op(enable or disable or others) \r\n");
			return -1;
		}		
		if (strcmp(argv[1], "server") == 0) {
			ret =  ble_audio_unicast_server_main(argc - 2, argv + 2);
		} else if (strcmp(argv[1], "client") == 0) {
			ret =  ble_audio_unicast_client_main(argc - 2, argv + 2);
		} else {
			printf("[ATBE] Error: %s argv[1](%s) error\r\n", argv[0], argv[1]);
			return -1;
		}
	} else if (strcmp(argv[0], "broadcast") == 0) {
		if (argc < 3) {
			AT_PRINTK("[ATBE] Error: wrong arg number (%d) for le audio broadcast!", argc);
			AT_PRINTK("[ATBE]usage: ATBE=leaudio,broadcast,sink or source,op(enable or disable or others) \r\n");
			return -1;
		}	
		if (strcmp(argv[1], "sink") == 0) {
			ret =  ble_audio_broadcast_sink_main(argc - 2, argv + 2);
		} else if (strcmp(argv[1], "source") == 0) {
			ret =  ble_audio_broadcast_source_main(argc - 2, argv + 2);
		} else {
			printf("[ATBE] Error: %s argv[1](%s) error\r\n", argv[0], argv[1]);
			return -1;
		}
	} else if (strcmp(argv[0], "acceptor") == 0) {
		ret =  ble_audio_acceptor_main(argc - 1, argv + 1);
	} else if (strcmp(argv[0], "initiator") == 0) {
		ret =  ble_audio_initiator_main(argc - 1, argv + 1);
	} else {
		AT_PRINTK("[ATBE] Error: wrong argv[0] (%s) for le audio!", argv[0]);
		return -1;
	}

	if (ret != 0) {
		AT_PRINTK("[ATBE] Error: ble audio %s %s example failed! ret= %d", argv[0], argv[1], ret);
	}

	return ret;
}