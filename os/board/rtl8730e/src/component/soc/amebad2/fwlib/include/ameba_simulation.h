#ifndef _AMEBA_SIMULATION_H_
#define _AMEBA_SIMULATION_H_

#ifdef CONFIG_SIMULATION
#define POSTSIM_FLAG(x)   BKUP_Set(BKUP_REG1, BIT(x));
//#define SYSCFG_CHIPType_Get()   CHIP_TYPE_RTLSIM
#else
#define POSTSIM_FLAG(x)
#endif

/* FUNCTION */
VOID app_simulation_dump(u32 addr, u32 data);
VOID app_simulation_event_trigger(u8 event);
#endif /* _AMEBAD2_SIMULATION_H_ */
