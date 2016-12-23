#ifndef _POWER_MONITOR_INTERFACE_H
#define _POWER_MONITOR_INTERFACE_H
enum power_state_e {
 POWER_STATE_HIGH = 0,
 POWER_STATE_MEDIUM,
 POWER_STATE_LOW
};

#define POWER_STATES_LEN 64
#define BATTERY_LIFETIME_LEN 32
#define BATTERY_LEVEL_LEN 32
void fetch_power_states(FAR char *powerstates);
void fetch_power_curr_state(char *currstate);
void fetch_power_time_spent(char *timespent);
void fetch_power_battery_state(char *batterystate);
void fetch_battery_lifetime(FAR char *expectedlifetime);
void fetch_battery_level(FAR char * batterylevel);
#endif