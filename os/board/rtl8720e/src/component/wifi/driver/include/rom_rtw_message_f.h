// Debug message
// DBG_DM_ANT_DIV
ROM_F_RTW_MSGPOOL(DM_ANT_DIV_1, "\n*** Main_Cnt= (( %u )), Main_Cnt_CCK= (( %u )), Main_RSSI= (( %u )) \n")
ROM_F_RTW_MSGPOOL(DM_ANT_DIV_2, "*** Aux_Cnt = (( %u )), Aux_Cnt_CCK = (( %u )), Aux_RSSI = (( %u )) \n")
ROM_F_RTW_MSGPOOL(DM_ANT_DIV_3, "*** MAC ID:[ %u ] , TargetAnt = (( %s )), decision_type = (( %u ))\n")
ROM_F_RTW_MSGPOOL(DM_ANT_DIV_4, "*** Phy_AntSel_A=[ %d, %d, %d] \n")
ROM_F_RTW_MSGPOOL(DM_ANT_DIV_5, "[ Update Rx-Idle-Ant ] RxIdleAnt =%s\n")

// freertos_ioctl.c
// 0x76 read sys
ROM_F_RTW_MSGPOOL(READ_SYS_1,	"read sys_reg(0x%x)=0x%x\n")
// 0x77 write sys
ROM_F_RTW_MSGPOOL(WRITE_SYS_1,	"write sys_reg(0x%x)=0x%x\n")
// 0xdd dump info
ROM_F_RTW_MSGPOOL(DUMP_INFO_9,	"ID: 0x%02x\n")

// wlan driver DBG_871X_LEVEL
#define ROM_F_RTW_MSGPOOL_871X(name,str) ROM_F_RTW_MSGPOOL(name,DRIVER_PREFIX str)

#undef ROM_F_RTW_MSGPOOL
#undef ROM_F_RTW_MSGPOOL_871X
