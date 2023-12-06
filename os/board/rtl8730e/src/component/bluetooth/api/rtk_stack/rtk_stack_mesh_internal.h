#ifndef __RTK_BT_MESH_STACK_INTERNAL_H__
#define __RTK_BT_MESH_STACK_INTERNAL_H__

#include <app_msg.h>
#include <rtk_bt_common.h>

uint16_t bt_stack_mesh_init(void *data);

void bt_stack_mesh_deinit(void);

void ble_mesh_handle_io_msg(T_IO_MSG *io_msg);

void rtk_bt_mesh_set_device_uuid(void);

bool generic_on_off_client_model_init(void);

bool generic_on_off_server_model_init(void);

bool light_lightness_client_model_init(void);

bool light_lightness_server_model_init(void);

bool light_lightness_setup_server_model_init(void);

bool light_ctl_client_model_init(void);

bool light_ctl_server_model_init(void);

bool light_ctl_setup_server_model_init(void);

bool light_ctl_temperature_server_model_init(void);

bool light_hsl_client_model_init(void);

bool light_hsl_server_model_init(void);

bool light_hsl_hue_server_model_init(void);

bool light_hsl_saturation_server_model_init(void);

bool light_hsl_setup_server_model_init(void);

bool light_xyl_client_model_init(void);

bool light_xyl_server_model_init(void);

bool light_xyl_setup_server_model_init(void);

bool light_lc_client_model_init(void);

bool light_lc_server_model_init(void);

bool light_lc_setup_server_model_init(void);

bool time_client_model_init(void);

bool time_server_model_init(void);

bool time_setup_server_model_init(void);

bool scheduler_client_model_init(void);

bool scheduler_server_model_init(void);

bool scheduler_setup_server_model_init(void);

bool scene_client_model_init(void);

bool scene_server_model_init(void);

bool scene_setup_server_model_init(void);

bool generic_default_transition_time_client_model_init(void);

bool generic_default_transition_time_server_model_init(void);

bool generic_level_client_model_init(void);

bool generic_level_server_model_init(void);

bool generic_power_on_off_client_model_init(void);

bool generic_power_on_off_server_model_init(void);

bool generic_power_on_off_setup_server_model_init(void);

bool generic_power_level_client_model_init(void);

bool generic_power_level_server_model_init(void);

bool generic_power_level_setup_server_model_init(void);

bool generic_battery_client_model_init(void);

bool generic_battery_server_model_init(void);

bool generic_location_client_model_init(void);

bool generic_location_server_model_init(void);

bool generic_location_setup_server_model_init(void);

bool generic_property_client_model_init(void);

bool generic_user_property_server_model_init(void);

bool generic_admin_property_server_model_init(void);

bool generic_manu_property_server_model_init(void);

bool generic_client_property_server_model_init(void);

bool sensor_client_model_init(void);

bool sensor_server_model_init(void);

bool sensor_setup_server_model_init(void);

bool health_client_model_init(void);

bool health_server_model_init(void);

uint16_t bt_mesh_stack_act_handle(rtk_bt_cmd_t *p_cmd);

uint8_t rtk_bt_mesh_config_client_model_indicate_event(uint32_t event_code, void *pdata, uint32_t len);

uint16_t bt_mesh_config_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_onoff_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_datatrans_model_act_handle(rtk_bt_cmd_t *p_cmd);

bool datatrans_model_init(void);

uint16_t bt_mesh_remote_prov_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

void remote_prov_client_init(void);

uint16_t bt_mesh_light_lightness_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_light_ctl_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_light_hsl_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_light_xyl_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_light_lc_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_time_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_scheduler_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_scene_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_default_transition_time_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_level_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_power_on_off_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_power_level_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_battery_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_location_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_property_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_sensor_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_health_client_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_user_property_server_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_admin_property_server_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_generic_manu_property_server_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_scene_server_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_scene_setup_server_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_sensor_server_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_sensor_setup_server_model_act_handle(rtk_bt_cmd_t *p_cmd);

uint16_t bt_mesh_health_server_model_act_handle(rtk_bt_cmd_t *p_cmd);
#endif
