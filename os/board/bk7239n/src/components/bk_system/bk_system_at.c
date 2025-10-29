#include "atsvr_unite.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "components/system.h"
#include "bk_system_at.h"

void at_system_reboot(void)
{
	bk_reboot();
}

static int at_misc_reset_system(int sync,int argc, char **argv)
{
    if(argc != 0){
		atsvr_cmd_rsp_error();
        return -1;
    }
	atsvr_cmd_rsp_ok();
	at_system_reboot();
    return 0 ;
}

const struct _atsvr_command misc_cmds_table[] = {
	ATSVR_CMD_HADLER("AT+RST","AT+RST",NULL,at_misc_reset_system,false,0,0,NULL,false),
};

void at_misc_cmd_init(void)
{
	atsvr_register_commands(misc_cmds_table, sizeof(misc_cmds_table) / sizeof(struct _atsvr_command),"misc",NULL);

}

