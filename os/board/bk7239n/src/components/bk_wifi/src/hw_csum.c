#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <arch/types.h>
#include "lwip/debug.h"
#include "lwip/arch.h"
#include <stdint.h>
#include <stdbool.h>
#include "wifi_v2.h"
#include <os/os.h>
#ifndef REG_WRITE
#define REG_WRITE(addr, val) (*(volatile uint32_t *)(addr) = (uint32_t)(val))
#endif
#ifndef REG_READ
#define REG_READ(addr) (*(volatile uint32_t *)(addr))
#endif
#if CONFIG_SPE
#define IPCHKSUM_BASE                       (0x4980DE00)
#else
#define IPCHKSUM_BASE                       (0x5980DE00)
#endif
/* ip checksum base address */
#define IPCHKSUM_CHECKSUM_BASE_ADDR         (IPCHKSUM_BASE + 0*4)
/* ip checksum length */
#define IPCHKSUM_REG0X1_ADDR                (IPCHKSUM_BASE + 1*4)
#define IPCHKSUM_CHECKSUM_LENGTH_POSI       (0)
#define IPCHKSUM_CHECKSUM_LENGTH_MASK       (0xFFFF)
/* ip checksum start/enable */
#define IPCHKSUM_REG0X2_ADDR                (IPCHKSUM_BASE + 2*4)
#define IPCHKSUM_CHECKSUM_START             (1 << 0)
/* ip checksum data valid */
#define IPCHKSUM_REG0X3_ADDR                (IPCHKSUM_BASE + 3*4)
#define IPCHKSUM_IP_CHECKSUM_VALID          (1 << 0)
/* ip checksum data */
#define IPCHKSUM_REG0X4_ADDR                (IPCHKSUM_BASE + 4*4)
#define IPCHKSUM_IP_CHECKSUM_POSI           (0)
#define IPCHKSUM_IP_CHECKSUM_MASK           (0xFFFF)
/* ip checksum interrupt status/write 1 clear */
#define IPCHKSUM_REG0X5_ADDR                (IPCHKSUM_BASE + 5*4)
#define IPCHKSUM_CHECKSUM_INT_STATUS        (1 << 0)
static void ipchksum_active(bool enable)
{
    REG_WRITE(IPCHKSUM_REG0X2_ADDR, enable);
}
static void ipchksum_set_base_addr(uint32_t val)
{
    REG_WRITE(IPCHKSUM_CHECKSUM_BASE_ADDR, val);
}
static void ipchksum_set_length(uint16_t val)
{
    REG_WRITE(IPCHKSUM_REG0X1_ADDR, val);
}
static bool ipchksum_get_valid_flag(void)
{
    return (REG_READ(IPCHKSUM_REG0X3_ADDR) & 1);
}
static uint16_t ipchksum_get_data(void)
{
    return (REG_READ(IPCHKSUM_REG0X4_ADDR) & IPCHKSUM_IP_CHECKSUM_MASK);
}
static void ipchksum_clr_int(bool enable)
{
    if (enable)
        REG_WRITE(IPCHKSUM_REG0X5_ADDR, 1);
}
static void ipchksum_start(uint32_t addr, uint16_t len)
{
    ipchksum_set_base_addr(addr);
    ipchksum_set_length(len);
    ipchksum_active(1);
}
/*
 * calculate ip checksum.
 * @addr: buf address, aligned address allowed
 * @len:  buf lenght
 *
 * return ip checksum. Mutex may be used to avoid concurrent access.
 */
uint16_t ipchksum_get_result(uint32_t addr, uint16_t len)
{
    ipchksum_start(addr, len);
    while (!ipchksum_get_valid_flag());
    ipchksum_clr_int(1);
    return ipchksum_get_data();
}
static volatile bool hw_ipcsum_holed = false;
extern uint16_t lwip_standard_chksum(const void *dataptr, int len);
/*
 * hw ip checksum verison of lwip_standard_chksum.
 * Uses rtos critical section to protect hardware checksum access.
 * Falls back to software checksum if hardware is busy.
 * 
 * @param dataptr: data pointer
 * @param len: data length
 * @return: checksum value
 */
uint16_t hw_ipcksum_standard_chksum(const void *dataptr, int len)
{
    uint16_t csum;
    uint32_t irq_flags;
    
    irq_flags = rtos_enter_critical();
    if (hw_ipcsum_holed) {
        rtos_exit_critical(irq_flags);
        goto sw;
    }
    
    hw_ipcsum_holed = true;
    rtos_exit_critical(irq_flags);
    
    csum = ipchksum_get_result((uintptr_t)dataptr, (uint16_t)len);
    
    irq_flags = rtos_enter_critical();
    hw_ipcsum_holed = false;
    rtos_exit_critical(irq_flags);
    
    return csum;

sw:
    WIFI_LOGI("hw_ipcksum_standard_chksum: use SW len=%d\n", len);
    csum = lwip_standard_chksum(dataptr, len);
    return csum;
}
