/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#include <stdlib.h>
#include "errno.h"
#include "debug_printf.h"
#include "psci.h"

/**
 * struct arm_smccc_res - Result from SMC/HVC call
 * @a0-a3 result values from registers 0 to 3
 */
struct arm_smccc_res {
	unsigned long a0;
	unsigned long a1;
	unsigned long a2;
	unsigned long a3;
};

/**
 * struct arm_smccc_quirk - Contains quirk information
 * @id: quirk identification
 * @state: quirk specific information
 * @a6: Qualcomm quirk entry for returning post-smc call contents of a6
 */
struct arm_smccc_quirk {
	int	id;
	union {
		unsigned long a6;
	} state;
};

/**
 * __arm_smccc_smc() - make SMC calls
 * @a0-a7: arguments passed in registers 0 to 7
 * @res: result values from registers 0 to 3
 * @quirk: points to an arm_smccc_quirk, or NULL when no quirks are required.
 *
 * This function is used to make SMC calls following SMC Calling Convention.
 * The content of the supplied param are copied to registers 0 to 7 prior
 * to the SMC instruction. The return values are updated with the content
 * from register 0 to 3 on return from the SMC instruction.  An optional
 * quirk structure provides vendor specific behavior.
 */

extern void __arm_smccc_smc(unsigned long a0, unsigned long a1,
							unsigned long a2, unsigned long a3, unsigned long a4,
							unsigned long a5, unsigned long a6, unsigned long a7,
							struct arm_smccc_res *res, struct arm_smccc_quirk *quirk);

#define arm_smccc_smc(...) __arm_smccc_smc(__VA_ARGS__, NULL)

#define PSCI_FN_NATIVE(version, name)	PSCI_##version##_FN_##name

static unsigned long invoke_psci_fn(unsigned long function_id,
									unsigned long arg0, unsigned long arg1,
									unsigned long arg2)
{
	struct arm_smccc_res res;

	arm_smccc_smc(function_id, arg0, arg1, arg2, 0, 0, 0, 0, &res);
	return res.a0;
}

static int psci_to_linux_errno(int err)
{
	switch (err) {
	case PSCI_RET_SUCCESS:
		return 0;
	case PSCI_RET_NOT_SUPPORTED:
		return -EOPNOTSUPP;
	case PSCI_RET_INVALID_PARAMS:
	case PSCI_RET_INVALID_ADDRESS:
		return -EINVAL;
	case PSCI_RET_DENIED:
		return -EPERM;
	};

	return -EINVAL;
}

uint32_t psci_get_version(void)
{
	return invoke_psci_fn(PSCI_0_2_FN_PSCI_VERSION, 0, 0, 0);
}

int psci_cpu_suspend(uint32_t state, unsigned long entry_point)
{
	return invoke_psci_fn(PSCI_FN_NATIVE(0_2, CPU_SUSPEND), state, entry_point, 0);
}

int psci_cpu_off(uint32_t state)
{
	int err;

	err = invoke_psci_fn(PSCI_0_2_FN_CPU_OFF, state, 0, 0);
	return psci_to_linux_errno(err);
}

int psci_cpu_on(unsigned long cpuid, unsigned long entry_point)
{
	int err;

	err = invoke_psci_fn(PSCI_FN_NATIVE(0_2, CPU_ON), cpuid, entry_point, 0);
	return psci_to_linux_errno(err);
}

int psci_migrate(unsigned long cpuid)
{
	int err;
	err = invoke_psci_fn(PSCI_FN_NATIVE(0_2, MIGRATE), cpuid, 0, 0);
	return psci_to_linux_errno(err);
}

int psci_affinity_info(unsigned long target_affinity,
					   unsigned long lowest_affinity_level)
{
	return invoke_psci_fn(PSCI_FN_NATIVE(0_2, AFFINITY_INFO),
						  target_affinity, lowest_affinity_level, 0);
}

int psci_migrate_info_type(void)
{
	return invoke_psci_fn(PSCI_0_2_FN_MIGRATE_INFO_TYPE, 0, 0, 0);
}

unsigned long psci_migrate_info_up_cpu(void)
{
	return invoke_psci_fn(PSCI_FN_NATIVE(0_2, MIGRATE_INFO_UP_CPU),
						  0, 0, 0);
}

void psci_sys_reset(void)
{
	invoke_psci_fn(PSCI_0_2_FN_SYSTEM_RESET, 0, 0, 0);
}

void psci_sys_poweroff(void)
{
	invoke_psci_fn(PSCI_0_2_FN_SYSTEM_OFF, 0, 0, 0);
}

/*
 * Detect the presence of a resident Trusted OS which may cause CPU_OFF to
 * return DENIED (which would be fatal).
 */
static void psci_init_migrate(void)
{
	unsigned long cpuid;
	int type;

	type = psci_migrate_info_type();

	if (type == PSCI_0_2_TOS_MP) {
		debug_printf("Trusted OS migration not required\n");
		return;
	}

	if (type == PSCI_RET_NOT_SUPPORTED) {
		debug_printf("MIGRATE_INFO_TYPE not supported.\n");
		return;
	}

	if (type != PSCI_0_2_TOS_UP_MIGRATE &&
		type != PSCI_0_2_TOS_UP_NO_MIGRATE) {
		debug_printf("MIGRATE_INFO_TYPE returned unknown type (%d)\n", type);
		return;
	}

	cpuid = psci_migrate_info_up_cpu();
	if (cpuid & ~0xFFFFFF) {
		debug_printf("MIGRATE_INFO_UP_CPU reported invalid physical ID (0x%lx)\n",
					 cpuid);
		return;
	}

	debug_printf("Trusted OS resident on physical CPU 0x%lx\n", cpuid);
}

int psci_init(void)
{
	uint32_t ver = psci_get_version();

	if (PSCI_VERSION_MAJOR(ver) == 0 && PSCI_VERSION_MINOR(ver) < 2) {
		debug_printf("Conflicting PSCI version detected.\n");
		return -EINVAL;
	}

	psci_init_migrate();

	return 0;
}
