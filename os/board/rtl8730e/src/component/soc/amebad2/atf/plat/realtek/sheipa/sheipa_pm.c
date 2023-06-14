/*
 * Copyright (c) 2015-2019, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <platform_def.h>

#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/psci/psci.h>
#include <plat/common/platform.h>
#include <lib/mmio.h>

#include "sheipa_private.h"

#define IPCAP_REG			0x41000580
#define IPC_CHAN_NUM		0x0
#define KM0_IPC_RAM			0x2301FD00
#define CNT_CONTROL_BASE	0xB0002000
#define SYSTIM_CNTCR		(0x0)
#define SYSTIM_CNT_EN_BIT	(0x1)

typedef struct _IPC_MSG_STRUCT_ {
	uint32_t msg_type;
	uint32_t msg;
	uint32_t msg_len;
	uint32_t rsvd;
} IPC_MSG_STRUCT, *PIPC_MSG_STRUCT;


typedef struct {
	unsigned int IDAU_BARx;		/*!< ,	Address offset: 0x00 */
	unsigned int IDAU_LARx;		/*!< ,	Address offset: 0x04 */
} MPC_EntryTypeDef;


typedef struct {
	MPC_EntryTypeDef ENTRY[8]; /*!< ,	Address offset: 0x00 ~ 0x3C*/
	unsigned int IDAU_CTRL;		/*!< ,	Address offset: 0x40 */
	unsigned int IDAU_LOCK;		/*!< ,	Address offset: 0x44 */
} MPC_TypeDef;


#define MPC1			((MPC_TypeDef			*) MPC1_BASE_S)

static IPC_MSG_STRUCT ipc_msg;
static MPC_TypeDef mpc_backup;

/*
 * The secure entry point to be used on warm reset.
 */
static unsigned long secure_entrypoint;

#ifndef AMEBAD2_TODO
extern uint32_t arm_gic_freq_get_div(void);
extern void arm_gic_freq_switch(uint32_t pre_div);
extern void arm_gic_freq_restore(uint32_t pre_div);
#endif

/* Make composite power state parameter till power level 0 */
#if PSCI_EXTENDED_STATE_ID

#define sheipa_make_pwrstate_lvl0(lvl0_state, pwr_lvl, type) \
		(((lvl0_state) << PSTATE_ID_SHIFT) | \
		 ((type) << PSTATE_TYPE_SHIFT))
#else
#define sheipa_make_pwrstate_lvl0(lvl0_state, pwr_lvl, type) \
		(((lvl0_state) << PSTATE_ID_SHIFT) | \
		 ((pwr_lvl) << PSTATE_PWR_LVL_SHIFT) | \
		 ((type) << PSTATE_TYPE_SHIFT))
#endif /* PSCI_EXTENDED_STATE_ID */


#define sheipa_make_pwrstate_lvl1(lvl1_state, lvl0_state, pwr_lvl, type) \
		(((lvl1_state) << PLAT_LOCAL_PSTATE_WIDTH) | \
		 sheipa_make_pwrstate_lvl0(lvl0_state, pwr_lvl, type))



/*
 *  The table storing the valid idle power states. Ensure that the
 *  array entries are populated in ascending order of state-id to
 *  enable us to use binary search during power state validation.
 *  The table must be terminated by a NULL entry.
 */
static const unsigned int sheipa_pm_idle_states[] = {
	/* State-id - 0x01 */
	sheipa_make_pwrstate_lvl1(PLAT_LOCAL_STATE_RUN, PLAT_LOCAL_STATE_RET,
							  MPIDR_AFFLVL0, PSTATE_TYPE_STANDBY),
	/* State-id - 0x02 */
	sheipa_make_pwrstate_lvl1(PLAT_LOCAL_STATE_RUN, PLAT_LOCAL_STATE_OFF,
							  MPIDR_AFFLVL0, PSTATE_TYPE_POWERDOWN),
	/* State-id - 0x22 */
	sheipa_make_pwrstate_lvl1(PLAT_LOCAL_STATE_OFF, PLAT_LOCAL_STATE_OFF,
							  MPIDR_AFFLVL1, PSTATE_TYPE_POWERDOWN),
	0,
};

void plat_sheipa_save_mpc(void)
{
	int i;

	for (i = 0; i < 8; i++) {
		mpc_backup.ENTRY[i].IDAU_BARx = MPC1->ENTRY[i].IDAU_BARx;
		mpc_backup.ENTRY[i].IDAU_LARx = MPC1->ENTRY[i].IDAU_LARx;
	}

	mpc_backup.IDAU_CTRL = MPC1->IDAU_CTRL;
}


void plat_sheipa_restore_mpc(void)
{
	int i;

	for (i = 0; i < 8; i++) {
		MPC1->ENTRY[i].IDAU_BARx = mpc_backup.ENTRY[i].IDAU_BARx;
		MPC1->ENTRY[i].IDAU_LARx = mpc_backup.ENTRY[i].IDAU_LARx;
	}

	MPC1->IDAU_CTRL = mpc_backup.IDAU_CTRL;
	MPC1->IDAU_LOCK = 1;
}


/*******************************************************************************
 * Platform handler called to check the validity of the power state
 * parameter. The power state parameter has to be a composite power state.
 ******************************************************************************/
static int sheipa_validate_power_state(unsigned int power_state,
									   psci_power_state_t *req_state)
{
	unsigned int state_id;
	int i;

	assert(req_state);

	/*
	 *  Currently we are using a linear search for finding the matching
	 *  entry in the idle power state array. This can be made a binary
	 *  search if the number of entries justify the additional complexity.
	 */
	for (i = 0; !!sheipa_pm_idle_states[i]; i++) {
		if (power_state == sheipa_pm_idle_states[i]) {
			break;
		}
	}

	/* Return error if entry not found in the idle state array */
	if (!sheipa_pm_idle_states[i]) {
		return PSCI_E_INVALID_PARAMS;
	}

	i = 0;
	state_id = psci_get_pstate_id(power_state);

	/* Parse the State ID and populate the state info parameter */
	while (state_id) {
		req_state->pwr_domain_state[i++] = state_id &
										   PLAT_LOCAL_PSTATE_MASK;
		state_id >>= PLAT_LOCAL_PSTATE_WIDTH;
	}
	return PSCI_E_SUCCESS;
}

/*******************************************************************************
 * Platform handler called to check the validity of the non secure
 * entrypoint.
 ******************************************************************************/
static int sheipa_validate_ns_entrypoint(uintptr_t entrypoint)
{
	/*
	 * Check if the non secure entrypoint lies within the non
	 * secure DRAM.
	 */
	if ((entrypoint >= NS_DRAM0_BASE) &&
		(entrypoint < (NS_DRAM0_BASE + NS_DRAM0_SIZE))) {
		return PSCI_E_SUCCESS;
	}

	return PSCI_E_INVALID_ADDRESS;
}

/*******************************************************************************
 * Platform handler called when a CPU is about to enter standby.
 ******************************************************************************/
static void sheipa_cpu_standby(plat_local_state_t cpu_state)
{
	assert(cpu_state == PLAT_LOCAL_STATE_RET);

	/*
	 * Enter standby state
	 * dsb is good practice before using wfi to enter low power states
	 */
	dsb();
	wfi();
}

/*******************************************************************************
 * Platform handler called when a power domain is about to be turned on. The
 * mpidr determines the CPU to be turned on.
 ******************************************************************************/
static int sheipa_pwr_domain_on(u_register_t mpidr)
{
	int rc = PSCI_E_SUCCESS;
	unsigned pos = plat_core_pos_by_mpidr(mpidr);
	uint64_t *hold_base = (uint64_t *)PLAT_SHEIPA_HOLD_BASE;

	hold_base[pos] = PLAT_SHEIPA_HOLD_STATE_GO;
	sev();

	return rc;
}

/*******************************************************************************
 * Platform handler called when a power domain is about to be turned off. The
 * target_state encodes the power state that each level should transition to.
 ******************************************************************************/
void sheipa_pwr_domain_off(const psci_power_state_t *target_state)
{
	//assert(0);
}


/*******************************************************************************
 * Platform handler called when a power domain is about to be suspended. The
 * target_state encodes the power state that each level should transition to.
 ******************************************************************************/
void sheipa_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	uint32_t msg_idx;
	uint32_t val;
	uint32_t *ipc_msg_addr;

	/* Set boot addr and flag*/
	extern void sp_min_warm_entrypoint(void);

	*(uint32_t *) PLAT_SHEIPA_WARM_BOOT_BASE = (uint32_t) sp_min_warm_entrypoint;
	val = *(uint32_t *)LSYS_BOOT_REASON_SW_REG;
	val |= LSYS_BIT_AP_WAKE_FROM_PG_FLAG;
	*(uint32_t *)LSYS_BOOT_REASON_SW_REG = val;

	/* Save mpc configuration */
	plat_sheipa_save_mpc();

	/* Send IPC msg to LP*/
	ipc_msg.msg = (uint32_t) NULL;
	ipc_msg.msg_len = 0;
	ipc_msg.msg_type = 0;

	/* IPC_Dir: 0x00000020, IPC_ChNum: 0 */
	msg_idx = 16 * ((0x00000020 >> 4) & 0xF) + 8 * (0x00000020 & 0xF) + IPC_CHAN_NUM;
	ipc_msg_addr = (uint32_t *)(KM0_IPC_RAM + msg_idx * 16);
	memcpy(ipc_msg_addr, &ipc_msg, sizeof(IPC_MSG_STRUCT));

	clean_dcache_range((uintptr_t)ipc_msg_addr, sizeof(IPC_MSG_STRUCT));

	/* wait all cmd done */
	__asm volatile("isb");

	val = mmio_read_32(IPCAP_REG);
	val |= (BIT(IPC_CHAN_NUM + 8));
	mmio_write_32(IPCAP_REG, val);
}

/*******************************************************************************
 * Platform handler called when a power domain has just been powered on after
 * being turned off earlier. The target_state encodes the low power state that
 * each level has woken up from.
 ******************************************************************************/
void sheipa_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	assert(target_state->pwr_domain_state[MPIDR_AFFLVL0] ==
		   PLAT_LOCAL_STATE_OFF);

	/* Only Core1 is running in here */
	uint32_t pre_div = arm_gic_freq_get_div();
	arm_gic_freq_switch(pre_div);

	/* TODO: This setup is needed only after a cold boot */
	plat_sheipa_gic_pcpu_init();
	/* Enable the gic cpu interface */
	plat_sheipa_gic_cpuif_enable();

	arm_gic_freq_restore(pre_div);
}

/*******************************************************************************
 * Platform handler called when a power domain has just been powered on after
 * having been suspended earlier. The target_state encodes the low power state
 * that each level has woken up from.
 ******************************************************************************/
void sheipa_pwr_domain_suspend_finish(const psci_power_state_t *target_state)
{
	uint32_t value;

	value = *(uint32_t *)LSYS_BOOT_REASON_SW_REG;
	value &= (~LSYS_BIT_AP_WAKE_FROM_PG_FLAG);
	*(uint32_t *)LSYS_BOOT_REASON_SW_REG = value;

	/* Enable systimer */
	value = mmio_read_32(CNT_CONTROL_BASE + SYSTIM_CNTCR);
	value |= SYSTIM_CNT_EN_BIT;
	mmio_write_32((CNT_CONTROL_BASE + SYSTIM_CNTCR), value);
}


void __dead2 sheipa_pwr_domain_pwr_down_wfi(const psci_power_state_t *target_state)
{
	/* wait to shutdown */
	while (1) {
		/* use wfe instead of wfi, will never come back here so sev isn't needed */
		wfe();
	}
}

/*******************************************************************************
 * Platform handlers to shutdown/reboot the system
 ******************************************************************************/
static void __dead2 sheipa_system_off(void)
{
	ERROR("SHEIPA System Off: operation not handled.\n");
	panic();
}

static void __dead2 sheipa_system_reset(void)
{
	ERROR("SHEIPA System Reset: operation not handled.\n");
	panic();
}

static const plat_psci_ops_t plat_sheipa_psci_pm_ops = {
	.cpu_standby = sheipa_cpu_standby,
	.pwr_domain_on = sheipa_pwr_domain_on,
	.pwr_domain_off = sheipa_pwr_domain_off,
	.pwr_domain_suspend = sheipa_pwr_domain_suspend,
	.pwr_domain_on_finish = sheipa_pwr_domain_on_finish,
	.pwr_domain_suspend_finish = sheipa_pwr_domain_suspend_finish,
	.pwr_domain_pwr_down_wfi = sheipa_pwr_domain_pwr_down_wfi,
	.system_off = sheipa_system_off,
	.system_reset = sheipa_system_reset,
	.validate_power_state = sheipa_validate_power_state,
	.validate_ns_entrypoint = sheipa_validate_ns_entrypoint
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
						const plat_psci_ops_t **psci_ops)
{
	uintptr_t *mailbox = (void *) PLAT_SHEIPA_TRUSTED_MAILBOX_BASE;

	*mailbox = sec_entrypoint;
	secure_entrypoint = (unsigned long) sec_entrypoint;
	*psci_ops = &plat_sheipa_psci_pm_ops;

	return 0;
}
