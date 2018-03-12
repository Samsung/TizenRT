# ARTIK055s

![ARTIK055s](../../../external/docs/media/a055.jpg)

The ARTIK055s is a SOC for Wi-Fi™ IoT solutions. The ARTIK055s has a Wi-Fi subsystem, security subsystem, and application subsystem.

## Information

The Samsung ARTIK™ 055s Module is a highly integrated module for secure Internet of Things (IoT) devices that require Wi-Fi®. It is based on an ARM® Cortex® R4 core with on-chip memories, a complete 2.4GHz Wi-Fi® Phy, MAC layer processing, a large complement of standard digital buses, a PUF-based security system and power management. The module is packaged with additional external Flash memory, a hardware Secure Element and a single integrated 2.4GHz structural antenna.
Aimed especially at power-sensitive devices needing Wi-Fi®, the ARTIK055s Module provides excellent performance in a variety of environments, with a feature set tailored specifically for IoT end nodes.

## How to Build

This guide is a summary. For more information, please see [here](../../../README.md#how-to-build).
```bash
$ git clone https://github.com/SamsungARTIK/TizenRT.git
$ cd TizenRT
```
```bash
$ ./os/config.sh artik055s/nettest
$ make -C os
```
```bash
$ make -C os download ALL
```

### ARTIK055s Starter Kit PinOut
#### CON710 CON711 CON708 CON709
```
                    XGPIO13(gpio42) SW702 |_|_| SW703 XGPIO15(gpio44)
                   XGPIO16(gpio45) LED703 |_|_| LED702 XGPIO20(gpio49)
                                ___________       [CON708]
                               |           |  |_| SCL XI2C0_SCL
                               |           |  |_| SDA XI2C0_SDA
                               |           |  |_| AREF
                 [CON710]      |           |  |_| GND
                       NC |_|  |           |  |_| 13 XSPI1_CLK
                       NC |_|  |           |  |_| 12 XSPI1_MISO
                    RESET |_|  |           |  |_| 11 XSPI1_MOSI
                     3.3V |_|  | ARTIK055s |  |_| 10 XSPI1_CSN
                       NC |_|  |           |  |_|  9 XPWMTOUT_4
                      GND |_|  |           |  |_|  8 XGPIO21(gpio50)
                      GND |_|  |           |      [CON709]
                      Vin |_|  |           |  |_|  7 XGPIO19(gpio48)
                 [CON711]      |           |  |_|  6 XPWMTOUT_2
             XADC0AIN0 A0 |_|  |___________|  |_|  5 XPWMTOUT_1 / XUART3_RTS
             XADC0AIN1 A1 |_|                 |_|  4 XGPIO18(gpio47)
             XADC0AIN2 A2 |_|                 |_|  3 XPWMTOUT_0 / XUART3_CTS
             XADC0AIN3 A3 |_|                 |_|  2 XGPIO17(gpio46)
                    NC A4 |_|                 |_|  1 XUART0_TX
                    NC A5 |_|                 |_|  0 XUART0_RX

                       [CON703]
                       2  4  6  8 10 12 14 16 18 20 22 24
                       1  3  5  7  9 11 13 15 17 19 21 23
                       [CON704]
                       2  4  6  8 10 12 14 16 18 20 22 24
                       1  3  5  7  9 11 13 15 17 19 21 23
```
#### CON703
```
          XPWMTOUT_1 / XUART3_RTS  1 |_|_|  2 VCC_EXT3P3
                       XPWMTOUT_2  3 |_|_|  4 NC
                       XPWMTOUT_3  5 |_|_|  6 NC
          XPWMTOUT_0 / XUART3_CTS  7 |_|_|  8 XI2C1_SCL
          XUART1_RXD / XUART2_CTS  9 |_|_| 10 XI2C1_SDA
          XUART1_TXD / XUART2_RTS 11 |_|_| 12 GND
      XGPIO26(gpio55) / XI2S0_SDO 13 |_|_| 14 VCC_EXT3P3
     XGPIO25(gpio54) / XI2S0_LRCK 15 |_|_| 16 XSPI0_CLK
     XGPIO24(gpio53) / XI2S0_BCLK 17 |_|_| 18 XSPI0_CSN
      XGPIO27(gpio56) / XI2S0_SDI 19 |_|_| 20 XSPI0_MISO
                  XGPIO22(gpio51) 21 |_|_| 22 XSPI0_MOSI
                   XEINT0(gpio57) 23 |_|_| 24 GND
```
#### CON704
```
                       XPWMTOUT_4  1 |_|_|  2 VCC_EXT3P3
                       XPWMTOUT_5  3 |_|_|  4 XUART2_RXD
                   XEINT2(gpio59)  5 |_|_|  6 XUART2_TXD
                   XEINT1(gpio58)  7 |_|_|  8 XUART3_RXD
                  XGPIO12(gpio41)  9 |_|_| 10 XUART3_TXD
     XGPIO10(gpio39) / XSPI3_MISO 11 |_|_| 12 GND
       XGPIO9(gpio38) / XSPI3_CSN 13 |_|_| 14 VCC_EXT3P3
     XGPIO11(gpio40) / XSPI3_MOSI 15 |_|_| 16 XGPIO4(gpio33) / XSPI2_CLK
       XGPIO8(gpio37) / XSPI3_CLK 17 |_|_| 18 XGPIO5(gpio34) / XSPI2_CSN
       XGPIO2(gpio31) / XI2C3_SCL 19 |_|_| 20 XGPIO6(gpio35) / XSPI2_MISO
       XGPIO1(gpio30) / XI2C2_SDA 21 |_|_| 22 XGPIO7(gpio36) / XSPI2_MOSI
       XGPIO3(gpio32) / XI2C3_SDA 23 |_|_| 24 GND
```

## Pin Assign

The hardware pull status is as follows. The default values set in the TizenRT software can be found [s5jt200_pinconfig.h](../../../os/arch/arm/src/s5j/chip/s5jt200_pinconfig.h).

| PinNo |   Pin Name   | Module Internal PULL | Module External PULL | RESET | PUD |                FEATURE               |
|:-----:|:------------:|:--------------------:|:--------------------:|:-----:|:---:|:------------------------------------:|
|   35  |   XSPI0_CLK  |           -          |           -          |   I   |  PD |                                      |
|   37  |   XSPI0_CSN  |           -          |           -          |   I   |  PD |                                      |
|   38  |  XSPI0_MISO  |           -          |           -          |   I   |  PD |                                      |
|   36  |  XSPI0_MOSI  |           -          |           -          |   I   |  PD |                                      |
|   56  |   XSPI1_CLK  |           -          |           -          |   I   |  PD |                LED700                |
|   58  |   XSPI1_CSN  |           -          |           -          |   I   |  PD |                                      |
|   55  |  XSPI1_MISO  |           -          |           -          |   I   |  PD |                                      |
|   57  |  XSPI1_MOSI  |           -          |           -          |   I   |  PD |                                      |
|   39  |  XUART1_RXD  |           -          |           -          |   I   |  PD |                                      |
|   40  |  XUART1_TXD  |           -          |           -          |   I   |  PD |                                      |
|   41  |  XUART2_RXD  |           -          |           -          |   I   |  PD |                                      |
|   42  |  XUART2_TXD  |           -          |           -          |   I   |  PD |                                      |
|   44  |  XUART3_RXD  |           -          |           -          |   I   |  PD |                                      |
|   45  |  XUART3_TXD  |           -          |           -          |   I   |  PD |                                      |
|   33  |  XPWMTOUT_0  |           -          |           -          |   I   |  PD |                                      |
|   31  |  XPWMTOUT_1  |           -          |           -          |   I   |  PD |                                      |
|   30  |  XPWMTOUT_2  |           -          |           -          |   I   |  PD |                                      |
|   29  |  XPWMTOUT_3  |           -          |           -          |   I   |  PD |                                      |
|   26  |  XPWMTOUT_4  |           -          |           -          |   I   |  PD |                                      |
|   25  |  XPWMTOUT_5  |           -          |           -          |   I   |  PD |                                      |
|   -   |    XGPIO0    |           -          |           -          |   I   |  PD |                                      |
|   59  |    XGPIO1    |           -          |           -          |   I   |  PD |                                      |
|   60  |    XGPIO2    |           -          |           -          |   I   |  PD |                                      |
|   61  |    XGPIO3    |           -          |           -          |   I   |  PD |                                      |
|   54  |    XGPIO4    |           -          |           -          |   I   |  PD |                                      |
|   53  |    XGPIO5    |           -          |           -          |   I   |  PD |                                      |
|   52  |    XGPIO6    |           -          |           -          |   I   |  PD |                                      |
|   51  |    XGPIO7    |           -          |           -          |   I   |  PD |                                      |
|   49  |    XGPIO8    |           -          |           -          |   I   |  PD |                                      |
|   47  |    XGPIO9    |           -          |           -          |   I   |  PD |                                      |
|   50  |    XGPIO10   |           -          |           -          |   I   |  PD |                                      |
|   48  |    XGPIO11   |           -          |           -          |   I   |  PD |                                      |
|   46  |    XGPIO12   |           -          |           -          |   I   |  PD |                                      |
|   72  |    XGPIO13   |           -          |          PU          |   I   |  PD |         SW702 (for XLED TEST)        |
|   73  |    XGPIO14   |           -          |          PU          |   I   |  PD |                 SW701                |
|   69  |    XGPIO15   |           -          |          PU          |   I   |  PD |         SW703 (for XLED TEST)        |
|   70  |    XGPIO16   |           -          |           -          |   I   |  PD |          LED (for XLED TEST)         |
|   71  |    XGPIO17   |           -          |           -          |   I   |  PD |                                      |
|   65  |    XGPIO18   |           -          |           -          |   I   |  PD |                                      |
|   66  |    XGPIO19   |           -          |           -          |   I   |  PD |                                      |
|   67  |    XGPIO20   |           -          |           -          |   I   |  PD |          LED (for XLED TEST)         |
|   68  |    XGPIO21   |           -          |           -          |   I   |  PD |                                      |
|   6   |    XGPIO22   |           -          |           -          |   I   |  PD |                                      |
|   7   |    XGPIO23   |           -          |           -          |   I   |  PD |                                      |
|   62  |    XGPIO24   |           -          |           -          |   I   |  PD |                                      |
|   63  |    XGPIO25   |           -          |           -          |   I   |  PD |                                      |
|   64  |    XGPIO26   |           -          |           -          |   I   |  PD |                                      |
|   34  |    XGPIO27   |           -          |           -          |   I   |  PD |                                      |
|   -   |    XGPIO28   |           -          |           -          |   I   |  PD |                                      |
|   14  |    XEINT0    |           -          |           -          |   I   |  PD |                                      |
|   15  |    XEINT1    |           -          |           -          |   I   |  PD |                                      |
|   16  |    XEINT2    |           -          |           -          |   I   |  PD |                                      |
|   17  |   XI2C0_SCL  |                      |          PU          |   I   |  PD |                                      |
|   18  |   XI2C0_SDA  |           -          |          PU          |   I   |  PD |                                      |
|   19  |   XI2C1_SCL  |           -          |          PU          |   I   |  PD |                                      |
|   20  |   XI2C1_SDA  |           -          |          PU          |   I   |  PD |                                      |
|   5   |  XADC0AIN_3  |           -          |          PD          |       |     |                                      |
|   4   |  XADC0AIN_2  |           -          |          PD          |       |     |                                      |
|   3   |  XADC0AIN_1  |           -          |          PD          |       |     |                                      |
|   2   |  XADC0AIN_0  |           -          |          PD          |       |     |                                      |
|   21  |     RXD0     |           -          |          PU          |   I   |  PD |                                      |
|   22  |     TXD0     |           -          |          PU          |   I   |  PD |                                      |
|   23  |  XDEBUG_RXD  |           -          |           -          |   I   |  PD |         for DEBUG (FT2232HL)         |
|   24  |  XDEBUG_TXD  |           -          |           -          |   I   |  PD |         for DEBUG (FT2232HL)         |
|   9   | XJTAG_TRST_N |           -          |           -          |   I   |  PD |         for DEBUG (FT2232HL)         |
|   10  |   XJTAG_TMS  |           -          |          PU          |   I   |  PU |         for DEBUG (FT2232HL)         |
|   11  |   XJTAG_TCK  |           -          |           -          |   I   |  PD |         for DEBUG (FT2232HL)         |
|   13  |   XJTAG_TDI  |           -          |          PU          |   I   |  PU |         for DEBUG (FT2232HL)         |
|   12  |   XJTAG_TDO  |           -          |           -          |   I   |  PD |         for DEBUG (FT2232HL)         |
|   8   |   XRESET_N   |          PU          |           -          |   I   |  PD |                                      |
|   -   |    PWR_RST   |           -          |          NA          |   NA  |  NA | Not Connected IC, DCDC IC Enable Pin |
