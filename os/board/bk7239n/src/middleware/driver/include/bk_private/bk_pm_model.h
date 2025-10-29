
typedef enum _pm_dev_state_ {
    PM_UNUSABLE = 0,
    PM_USABLE,
} PM_DEV_STATE;

enum DEV_PM_ID {
    PM_ID_FLASH = 0,
    PM_ID_UART,
    PM_ID_TIMER,
    PM_ID_USB,
    PM_ID_CALENDAR,
    PM_ID_AUDIO,
    PM_ID_PWM,
    PM_ID_GPIO,
    PM_ID_RF,
    PM_ID_BLE,
    PM_ID_MAC,
    PM_ID_MAX = 15,
};

enum DEV_SLEEP_LEVEL {
    IDLE_PS = 0,
    NORMAL_PS,
    LOWVOL_PS,
    DEEP_PS,
};



typedef union pm_status {
    uint32_t value;
    struct {
        uint32_t normal_ps_remain_26m;
        uint32_t resv: 15;
        uint32_t unconditional_ps_support: 1;
        uint32_t unconditional_ps_suspend_allow: 1;
        uint32_t unconditional_ps_sleeped: 1;
        uint32_t unconditional_ps_resume_allow: 1;
        uint32_t normal_ps_support: 1;
        uint32_t normal_ps_suspend_allow: 1;
        uint32_t normal_ps_sleeped: 1;
        uint32_t normal_ps_resume_allow: 1;
        uint32_t lowvol_ps_support: 1;
        uint32_t lowvol_ps_suspend_allow: 1;
        uint32_t lowvol_ps_sleeped: 1;
        uint32_t lowvol_ps_resume_allow: 1;
        uint32_t deep_ps_support: 1;
        uint32_t deep_ps_suspend_allow: 1;
        uint32_t deep_ps_sleeped: 1;
        uint32_t deep_ps_resume_allow: 1;
    } bits;
} PM_STATUS;

typedef struct _pm_ops_ {
    UINT32 ( *pm_init ) ( UINT32 op_flag );
    UINT32 ( *pm_deinit ) ( UINT32 op_flag );
    UINT32 ( *suspend ) ( UINT32 pm_level );
    UINT32 ( *resume ) ( UINT32 pm_level );
    PM_STATUS ( *status ) ( UINT32 op_flag );
    UINT32 ( *get_sleep_time ) ( void );
} DEV_PM_OPS_S, *DEV_PM_OPS_PTR;


typedef struct _pm_ {
    char *name;
    UINT32 id;
    UINT32 state;
    DEV_PM_OPS_S *ops;
} DRV_PM_S, *DRV_PM_PTR;

extern UINT32 dev_pm_init ( void );
extern int dev_pm_register ( UINT32 id, char *name, DEV_PM_OPS_PTR pmptr );
extern UINT32 dev_pm_unregister ( UINT32 id );


