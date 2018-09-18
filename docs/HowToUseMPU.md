## MPU
* It supports 12 or 16 memory regions, depending on the configuration(ARMv7-R has 12 memory regions).
* Each region should be programmed with a base address and size.

### MPU Initialization and Memory Region Setup
Initialization of MPU is done by setting up memory regions. Memory regions are set up by using the functions in below table and each function requires 2 arguments which are _Region Base Address_ and _Region Size_ to setup a memory region.

The functions with name ```_priv_``` are used to configure the _kernel_(Privileged) MPU regions whereas the ```_user_``` functions are called to configure the _user_(Un-Privileged) MPU regions.

MPU region setup related macros and functions are defined in **[mpu.h](../os/arch/arm/src/armv7-r/mpu.h)** under *os/arch/arm/src/armv7-r/* directory.

```
-----------------------------------
       MPU Memory Region Info
-----------------------------------
RNUM:	int32 - Region Number
RBASE:	int32 - Region Base Address
RSIZE:	int32 - Region Size
RATTR:	int32 - Region Attribute
-----------------------------------
```

```
------------------------------------------------------------------
            MPU Init or Memory Region Setup Functions
------------------------------------------------------------------
mpu_control() : This function is used to Enable or Disable the MPU.
------------------------------------------------------------------
                       Memory Region Access Permission Attributes
                             Privileged      Un-Privileged
------------------------------------------------------------------
mpu_priv_stronglyordered() : Read Write        No Access
mpu_priv_noncache()        : Read Only         No Access
mpu_peripheral()           : Read Write        No Access
mpu_priv_flash()           : Read Only         No Access
mpu_user_flash()           : Read Only         Read Only
mpu_priv_intsram()         : Read Write        No Access
mpu_user_intsram()         : Read Write        Read Write
mpu_priv_extsram()         : Read Write        No Access
mpu_user_extsram()         : Read Write        Read Write
mpu_priv_intsram_wb()      : Read Write        No Access
mpu_user_intsram_wb()      : Read Write        Read Write
------------------------------------------------------------------
```

### Memory Region Setup Functions

```mpu_priv_stronglyordered()``` - This function is used to configure a memory region as strongly ordered memory with _not cacheable_, _not bufferable_ and _shareable_ attributes.

```mpu_priv_noncache()``` - This function is used to configure a memory region as internal memory with _not cacheable_, _not bufferable_, _shareable_ and _instruction access disable_ attributes.

```mpu_peripheral()``` - This function is used to configure a memory region as peripheral address space with _shareable_, _bufferable_ and _instruction access disable_ attributes.

```mpu_priv_flash()``` - This function is used to configure a memory region as privileged program flash with _cacheable_ attributes.

```mpu_user_flash()``` - This function is used to configure a memory region as user program flash with _cacheable_ attributes.

```mpu_priv_intsram()``` - This function is used to configure a memory region as privileged internal SRAM region with _shareable_, _cacheable_ attributes.

```mpu_user_intsram()``` - This function is used to configure a memory region as user internal SRAM with _shareable_, _cacheable_ attributes.

```mpu_priv_extsram()``` -  This function is used to configure a memory region as privileged external SRAM region with _shareable_, _cacheable_ and _bufferable_ attributes.

```mpu_user_extsram()``` - This function is used to configure a memory region as user external SRAM region with _shareable_, _cacheable_ and _bufferable_ attributes.

```mpu_priv_intsram_wb()``` - This function is similar to mpu_priv_intsram() but with WB/WA cache policy. 

```mpu_user_intsram_wb()``` - This function is similar to mpu_user_intsram() but with WB/WA cache policy. 

### Example:
```
#ifdef CONFIG_ARMV7M_MPU
int s5j_mpu_initialize(void)
{
#ifdef CONFIG_ARCH_CHIP_S5JT200
	/*
	 * Vector Table		0x02020000	0x02020FFF	4
	 * Reserved		0x02021000	0x020217FF	2
	 * BL1			0x02021800	0x020237FF	8
	 * TizenRT		0x02023800	0x0210FFFF	946(WBWA)
	 * WIFI			0x02110000	0x0215FFFF	320(NCNB)
	 */

	/* Region 0, Set read only for memory area */
	mpu_priv_flash(0x0, 0x80000000);

	/* Region 1, for ISRAM(0x02020000++1280KB, RW-WBWA */
	mpu_user_intsram_wb(S5J_IRAM_PADDR, S5J_IRAM_SIZE);

	/* Region 2, wifi driver needs non-$(0x02110000++320KB, RW-NCNB */
	mpu_priv_noncache(S5J_IRAM_PADDR + ((4 + 2 + 8 + 946) * 1024), (320 * 1024));

	/* Region 3, for FLASH area, default to set WBWA */
	mpu_user_intsram_wb(S5J_FLASH_PADDR, S5J_FLASH_SIZE);

	/* region 4, for Sflash Mirror area to be read only */
	mpu_priv_flash(S5J_FLASH_MIRROR_PADDR, S5J_FLASH_MIRROR_SIZE);

	/* Region 5, for SFR area read/write, strongly-ordered */
	mpu_priv_stronglyordered(S5J_PERIPHERAL_PADDR, S5J_PERIPHERAL_SIZE);

	/*
	 * Region 6, for vector table,
	 * set the entire high vector region as read-only.
	 */
	mpu_priv_flash(S5J_IRAM_MIRROR_PADDR, S5J_IRAM_MIRROR_SIZE);

	mpu_control(true);
#endif
	return 0;
}
#endif
```
