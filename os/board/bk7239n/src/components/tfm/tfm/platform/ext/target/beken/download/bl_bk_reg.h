
#ifndef _bk3000_CHIP_H_
#define _bk3000_CHIP_H_
#include "soc/bk7239n/reg_base.h"
#include "bl_config.h"
#define ENABLE   1
#define DISABLE  0

// AMBA Address Mapping
// AHB
#define BK3000_SYS_BASE_ADDR             (SOC_SYSTEM_REG_BASE)
#define BK3000_UART_BASE_ADDR            (SOC_UART0_REG_BASE)
#define BK3000_SPI_BASE_ADDR             (SOC_SPI_REG_BASE)
#define BK3000_WDT_BASE_ADDR             (SOC_WDT_REG_BASE)
#define REG_XVR_BASE_ADDR                (SOC_XVR_REG_BASE)
#define BK3000_PMU_BASE_ADDR             (SOC_AON_PMU_REG_BASE)
#define BK3000_GPIO_BASE_ADDR            (0x44000400)
#define BK3000_AWDT_BASE_ADDR            (0x44000600)
#define ANA_XVR_NUM                      (16)

//watchdog
#define BK3000_WDT_CONTROL               (*((volatile unsigned long *)(BK3000_WDT_BASE_ADDR+2*4)))
#define BK3000_WDT_CONFIG                (*((volatile unsigned long *)(BK3000_WDT_BASE_ADDR+4*4)))

// PMU define
#define BK3000_PMU_RESET_REASON_GET      (*((volatile unsigned int *)(BK3000_PMU_BASE_ADDR + 0x0*4)))

#define AON_PMU_REG01                    (*((volatile unsigned int *)(BK3000_PMU_BASE_ADDR + 0x1*4)))
#define AON_PMU_REG01_BIT_XTAL_MODE      (0x1 << 21)
#define AON_PMU_IS_XTAL_40M              ((AON_PMU_REG01 & AON_PMU_REG01_BIT_XTAL_MODE) == 1)
#define AON_PMU_REG41                    (*((volatile unsigned int *)(BK3000_PMU_BASE_ADDR + 0x41*4)))
#define AON_PMU_REG41_BIT_XTAL_SEL       (0x1 << 14)
#define AON_PMU_SET_XTAL_26M             AON_PMU_REG41 &= ~AON_PMU_REG41_BIT_XTAL_SEL
#define AON_PMU_SET_XTAL_40M             AON_PMU_REG41 |= AON_PMU_REG41_BIT_XTAL_SEL
#define AON_PMU_IS_XTAL_SEL_40M          ((AON_PMU_REG41 & AON_PMU_REG41_BIT_XTAL_SEL) == 1)

#define AON_PMU_REG03                    (*((volatile unsigned int *)(BK3000_PMU_BASE_ADDR + 0x3*4)))

#define AON_PMU_REG03_BIT_BOOT_FLAG      (0x1 << 0)
#define AON_PMU_REG03_SET_BF_PRIMARY     AON_PMU_REG03 |= AON_PMU_REG03_BIT_BOOT_FLAG
#define AON_PMU_REG03_CLR_BF_PRIMARY     AON_PMU_REG03 &= ~AON_PMU_REG03_BIT_BOOT_FLAG

#define DEEP_SLEEP_RESTART_BIT		     (0x1 << 1)
// 1: Boot from deep_sleep  0: Boot from POR
#define DEEP_SLEEP_RESTART               ((BK3000_PMU_RESET_REASON_GET & DEEP_SLEEP_RESTART_BIT ) != 0)

#define addSYSTEM_Reg0xa                 *((volatile unsigned long *) (BK3000_SYS_BASE_ADDR+0xa*4))
//uart enable _ok
#define addSYSTEM_Reg0xc                 *((volatile unsigned long *) (BK3000_SYS_BASE_ADDR+0xc*4))
#define UART0_ENABLE                     addSYSTEM_Reg0xc |= 0x4
#define UART0_DISABLE                    addSYSTEM_Reg0xc &= ~0x4
#define UART0_RESET                      REG_APB3_UART_RST = 1

//spi enable _ok
#define SPI0_ENABLE                      addSYSTEM_Reg0xc |= 0x2
#define SPI0_DISABLE                     addSYSTEM_Reg0xc &= ~0x2

#define PER_INT_ENABLE                   *((volatile unsigned long *) (BK3000_SYS_BASE_ADDR+0x20*4))
#define UART0_INT_INDEX_NUM              (4)
#define UART0_INT_INDEX                  (0x1U << 4 )
#define UART0_INT_ENABLE                 PER_INT_ENABLE |=  UART0_INT_INDEX
#define UART0_INT_DISABLE                PER_INT_ENABLE &=  ~UART0_INT_INDEX

#define addSYSTEM_Reg0x8                *((volatile unsigned long *) (BK3000_SYS_BASE_ADDR+0x8*4))

#define CKSEL_TIM0                      addSYSTEM_Reg0x8 |= 0x100000
#define TIM0_ENABLE                     addSYSTEM_Reg0xc |= 0x10

// flash_rw_sel & rebooot   ok
#define addSYSTEM_Reg0x2                 *((volatile unsigned long *) (BK3000_SYS_BASE_ADDR+0x2*4))
#define REBOOT                           addSYSTEM_Reg0x2 &= ~0x1
#define  SPI_FLASH_SEL                   (0x1U << 9 )
#define  SET_SPI_RW_FLASH                addSYSTEM_Reg0x2 |= SPI_FLASH_SEL
#define  SET_FLASHCTRL_RW_FLASH          addSYSTEM_Reg0x2 &= ~SPI_FLASH_SEL

// Pherial Register's Define  

//GPIO
#define BK3000_GPIO_0_CONFIG             (*((volatile unsigned long *)(BK3000_GPIO_BASE_ADDR+0*4)))
#define BK3000_GPIO_1_CONFIG             (*((volatile unsigned long *)(BK3000_GPIO_BASE_ADDR+1*4)))

#define BK3000_GPIO_6_CONFIG             (*((volatile unsigned long *)(BK3000_GPIO_BASE_ADDR+6*4)))
#define BK3000_GPIO_7_CONFIG             (*((volatile unsigned long *)(BK3000_GPIO_BASE_ADDR+7*4)))
#define BK3000_GPIO_8_CONFIG             (*((volatile unsigned long *)(BK3000_GPIO_BASE_ADDR+8*4)))
#define BK3000_GPIO_9_CONFIG             (*((volatile unsigned long *)(BK3000_GPIO_BASE_ADDR+9*4)))

#define BK3000_GPIO_10_CONFIG            (*((volatile unsigned long *)(BK3000_GPIO_BASE_ADDR+10*4)))
#define BK3000_GPIO_11_CONFIG            (*((volatile unsigned long *)(BK3000_GPIO_BASE_ADDR+11*4)))

#define sft_GPIO_INPUT                   (0x0)
#define sft_GPIO_OUTPUT                  (0x1)
#define sft_GPIO_INPUT_ENABLE            (0x2)
#define sft_GPIO_OUTPUT_ENABLE           (0x3)
#define sft_GPIO_PULL_MODE               (0x4)
#define sft_GPIO_PULL_ENABLE             (0x5)
#define sft_GPIO_FUNCTION_ENABLE         (0x6)
#define sft_GPIO_INPUT_MONITOR           (0x7)

 /* defination of registers
*/
#define REG_APB3_UART_RST                (*((volatile unsigned long *)(BK3000_UART_BASE_ADDR+0x02*4)))
#define REG_APB3_UART_CFG                (*((volatile unsigned long *)(BK3000_UART_BASE_ADDR+0x04*4)))
#define REG_APB3_UART_FIFO_THRESHOLD     (*((volatile unsigned long *)(BK3000_UART_BASE_ADDR+0x05*4)))
#define REG_APB3_UART_FIFO_STATUS        (*((volatile unsigned long *)(BK3000_UART_BASE_ADDR+0x06*4)))
#define REG_APB3_UART_DATA               (*((volatile unsigned long *)(BK3000_UART_BASE_ADDR+0x07*4)))
#define REG_APB3_UART_INT_ENABLE         (*((volatile unsigned long *)(BK3000_UART_BASE_ADDR+0x08*4)))
#define REG_APB3_UART_INT_FLAG           (*((volatile unsigned long *)(BK3000_UART_BASE_ADDR+0x09*4)))
#define REG_APB3_UART_FC_CFG             (*((volatile unsigned long *)(BK3000_UART_BASE_ADDR+0x0A*4)))
#define REG_APB3_UART_WAKEUP_ENABLE      (*((volatile unsigned long *)(BK3000_UART_BASE_ADDR+0x0B*4)))

#define UART_WRITE_BYTE(v)               (REG_APB3_UART_DATA=v)
#define UART_READ_BYTE()                 ((REG_APB3_UART_DATA>>8)&0xff)

#define bit_UART_TX_FIFO_FULL            (1<<16)

#define bit_UART_TX_FIFO_EMPTY           (1<<17)
#define bit_UART_RX_FIFO_FULL            (1<<18)
#define bit_UART_RX_FIFO_EMPTY           (1<<19)
#define bit_UART_TX_WRITE_READY          (1<<20)
#define bit_UART_RX_READ_READY           (1<<21)

#define sft_UART_CONF_TX_ENABLE          (0)
#define sft_UART_CONF_RX_ENABLE          (1)
#define sft_UART_CONF_IRDA               (2)
#define sft_UART_CONF_UART_LEN           (3)
#define sft_UART_CONF_PAR_EN             (5)
#define sft_UART_CONF_PAR_MODE           (6)
#define sft_UART_CONF_STOP_LEN           (7)
#define sft_UART_CONF_CLK_DIVID          (8)

#define sft_UART_FIFO_CONF_TX_FIFO       (0)
#define sft_UART_FIFO_CONF_RX_FIFO       (8)
#define sft_UART_FIFO_CONF_DETECT_TIME   (16)


#define bit_UART_INT_TX_NEED_WRITE       ( 1 << 0 )
#define bit_UART_INT_RX_NEED_READ        ( 1 << 1 )
#define bit_UART_INT_RX_OVER_FLOW        ( 1 << 2 )
#define bit_UART_INT_RX_PAR_ERR          ( 1 << 3 )
#define bit_UART_INT_RX_STOP_ERR         ( 1 << 4 )
#define bit_UART_INT_TX_STOP_END         ( 1 << 5 )
#define bit_UART_INT_RX_STOP_END         ( 1 << 6 )
#define bit_UART_INT_RXD_WAKEUP          ( 1 << 7 )

#define UART_TX_FIFO_SIZE                (128)
#define UART_RX_FIFO_SIZE                (128)
#define UART_TX_FIFO_COUNT               (REG_APB3_UART_FIFO_STATUS&0xff)
#define UART_RX_FIFO_COUNT               ((REG_APB3_UART_FIFO_STATUS>>8)&0xff)
#define UART_TX_FIFO_FULL                (REG_APB3_UART_FIFO_STATUS&0x00010000)
#define UART_TX_FIFO_EMPTY               (REG_APB3_UART_FIFO_STATUS&0x00020000)
#define UART_RX_FIFO_FULL                (REG_APB3_UART_FIFO_STATUS&0x00040000)
#define UART_RX_FIFO_EMPTY               (REG_APB3_UART_FIFO_STATUS&0x00080000)
#define UART_TX_WRITE_READY              (REG_APB3_UART_FIFO_STATUS&0x00100000)
#define UART_RX_READ_READY               (REG_APB3_UART_FIFO_STATUS&0x00200000)

#define UART_BAUDRATE_3250000            (3250000)
#define UART_BAUDRATE_3000000            (3000000)
#define UART_BAUDRATE_2000000            (2000000)
#define UART_BAUDRATE_921600             (921600)  //
#define UART_BAUDRATE_460800             (460800)
#define UART_BAUDRATE_256000             (256000)
#define UART_BAUDRATE_230400             (230400)  //
#define UART_BAUDRATE_115200             (115200)  //default
#define UART_BAUDRATE_9600               (9600)     //
#define UART_BAUDRATE_3000               (3000)     //
#define UART_BAUDRATE_3250               (3250)     //

#define UART_BAUDRATE_DEFAULE            (UART_BAUDRATE_115200) //UART_BAUDRATE_3000000
#define UART_CLOCK_FREQ_24M              (24000000)
#define UART_CLOCK_FREQ_26M              (26000000)
#define UART_CLOCK_FREQ_40M              (40000000)
#define UART_CLOCK_FREQ_48M              (48000000)
#define UART_CLOCK_FREQ_52M              (52000000)
#define UART_CLOCK_FREQ_80M              (80000000)
#define UART_CLOCK_FREQ                  (UART_CLOCK_FREQ_80M)
#define Beken_Write_Register(addr, data) (*((volatile u_int32 *)(addr))) = (u_int32)(data);
#define Beken_Read_Register(addr)        (*(volatile u_int32 *)(addr));

#ifdef FLASH_DEBUG
x
//////////////////////////////////////
//FLASH
#define BIT_ADDRESS_SW                 0
#define BIT_OP_TYPE_SW                 24
#define BIT_OP_SW                      29
#define BIT_WP_VALUE                   30
#define BIT_BUSY_SW                    31

#define SET_ADDRESS_SW                 (0xFFFFFF << BIT_ADDRESS_SW)
#define SET_OP_TYPE_SW                 (0x1F     << BIT_OP_TYPE_SW)
#define SET_OP_SW                      (0x1      << BIT_OP_SW)
#define SET_WP_VALUE                   (0x1      << BIT_WP_VALUE)
#define SET_BUSY_SW                    (0x1      << BIT_BUSY_SW)

#define BIT_FLASH_CLK_CONF             0
#define BIT_MODE_SEL                   4
#define BIT_FWREN_FLASH_CPU            9
#define BIT_WRSR_DATA                  10
#define BIT_CRC_EN                     26

#define SET_FLASH_CLK_CONF             (0xF      << BIT_FLASH_CLK_CONF)
#define SET_MODE_SEL                   (0x1F     << BIT_MODE_SEL)
#define SET_FWREN_FLASH_CPU            (0x1      << BIT_FWREN_FLASH_CPU)
#define SET_WRSR_DATA                  (0xFFFF   << BIT_WRSR_DATA)
#define SET_CRC_EN                     (0x1      << BIT_CRC_EN)

#define BIT_SR_DATA_FLASH              0
#define BIT_CRC_ERR_COUNTER            8
#define BIT_DATA_FLASH_SW_SEL          16
#define BIT_DATA_SW_FLASH_SEL          19

#define SET_SR_DATA_FLASH              (0xFF     << BIT_SR_DATA_FLASH)
#define SET_CRC_ERR_COUNTER            (0xFF     << BIT_CRC_ERR_COUNTER)
#define SET_DATA_FLASH_SW_SEL          (0x7      << BIT_DATA_FLASH_SW_SEL)
#define SET_DATA_SW_FLASH_SEL          (0x7      << BIT_DATA_SW_FLASH_SEL)

#define reg_FLASH_OPERATE_SW           (*((volatile unsigned long *)    (BK3000_FLASH_BASE_ADDR+0*4)))
#define reg_FLASH_DATA_SW_FLASH        (*((volatile unsigned long *)    (BK3000_FLASH_BASE_ADDR+1*4)))
#define reg_FLASH_DATA_FLASH_SW        (*((volatile unsigned long *)    (BK3000_FLASH_BASE_ADDR+2*4)))
#define reg_FLASH_RDID_DATA_FLASH      (*((volatile unsigned long *)    (BK3000_FLASH_BASE_ADDR+4*4)))
#define reg_FLASH_SR_DATA_CRC_CNT      (*((volatile unsigned long *)    (BK3000_FLASH_BASE_ADDR+5*4)))
#define reg_FLASH_CONF                 (*((volatile unsigned long *)    (BK3000_FLASH_BASE_ADDR+7*4)))

#endif //

#endif
// end of file


