# ARTIK053

![ARTIK053](../../../external/docs/media/a053.jpg)

The ARTIK053 is a SOC for Wi-Fi™ IoT solutions. The ARTIK053 has a Wi-Fi subsystem, security subsystem, and application subsystem.

## Information

The Samsung ARTIK™ 053 Module is a highly integrated module for secure Internet of Things (IoT) devices that require Wi-Fi®. It is based on an ARM® Cortex® R4 core with on-chip memories, a complete 2.4GHz Wi-Fi® Phy, MAC layer processing, a large complement of standard digital buses, a PUF-based security system and power management. The module is packaged with additional external Flash memory, a hardware Secure Element and a single integrated 2.4GHz structural antenna.
Aimed especially at power-sensitive devices needing Wi-Fi®, the ARTIK053 Module provides excellent performance in a variety of environments, with a feature set tailored specifically for IoT end nodes.

## How to Build

This guide is a summary. For more information, please see [here](../../../README.md#how-to-build).
```bash
$ git clone https://github.com/SamsungARTIK/TizenRT.git
$ cd TizenRT
```
```bash
$ ./os/config.sh artik053/nettest
$ make -C os
```
```bash
$ make -C os download ALL
```

### ARTIK053 Starter Kit PinOut
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
                     3.3V |_|  | ARTIK053  |  |_| 10 XSPI1_CSN
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
                  XGPIO23(gpio52) 19 |_|_| 20 XSPI0_MISO
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

| PinNo |   Pin Name   | Module Internal PULL | Module External PULL | RESET | PUD |                    FEATURE                    |
|:-----:|:------------:|:--------------------:|:--------------------:|:-----:|:---:|:---------------------------------------------:|
|   51  |   XSPI0_CLK  |           -          |           -          |   I   |  PD |                                               |
|   49  |   XSPI0_CSN  |           -          |           -          |   I   |  PD |                                               |
|   48  |  XSPI0_MISO  |           -          |           -          |   I   |  PD |                                               |
|   50  |  XSPI0_MOSI  |           -          |           -          |   I   |  PD |                                               |
|   72  |   XSPI1_CLK  |           -          |          PU          |   I   |  PD |                     LED700                    |
|   74  |   XSPI1_CSN  |           -          |           -          |   I   |  PD |                                               |
|   71  |  XSPI1_MISO  |           -          |           -          |   I   |  PD |                                               |
|   73  |  XSPI1_MOSI  |           -          |           -          |   I   |  PD |                                               |
|   53  |  XUART1_RXD  |           -          |           -          |   I   |  PD |                                               |
|   52  |  XUART1_TXD  |           -          |           -          |   I   |  PD |                                               |
|   54  |  XUART2_RXD  |           -          |           -          |   I   |  PD |                                               |
|   55  |  XUART2_TXD  |           -          |           -          |   I   |  PD |                                               |
|   56  |  XUART3_RXD  |           -          |           -          |   I   |  PD |                                               |
|   57  |  XUART3_TXD  |           -          |           -          |   I   |  PD |                                               |
|   42  |  XPWMTOUT_0  |           -          |           -          |   I   |  PD |                                               |
|   41  |  XPWMTOUT_1  |           -          |           -          |   I   |  PD |                                               |
|   44  |  XPWMTOUT_2  |           -          |           -          |   I   |  PD |                                               |
|   45  |  XPWMTOUT_3  |           -          |           -          |   I   |  PD |                                               |
|   40  |  XPWMTOUT_4  |           -          |           -          |   I   |  PD |                                               |
|   43  |  XPWMTOUT_5  |           -          |           -          |   I   |  PD |                                               |
|   70  |    XGPIO0    |           -          |           -          |   I   |  PD |                                               |
|   68  |    XGPIO1    |           -          |           -          |   I   |  PD |                                               |
|   65  |    XGPIO2    |           -          |           -          |   I   |  PD |                                               |
|   69  |    XGPIO3    |           -          |           -          |   I   |  PD |                                               |
|   66  |    XGPIO4    |           -          |           -          |   I   |  PD |                                               |
|   67  |    XGPIO5    |           -          |           -          |   I   |  PD |                                               |
|   62  |    XGPIO6    |           -          |           -          |   I   |  PD |                                               |
|   64  |    XGPIO7    |           -          |           -          |   I   |  PD |                                               |
|   63  |    XGPIO8    |           -          |           -          |   I   |  PD |                                               |
|   60  |    XGPIO9    |           -          |           -          |   I   |  PD |                                               |
|   59  |    XGPIO10   |           -          |           -          |   I   |  PD |                                               |
|   61  |    XGPIO11   |           -          |           -          |   I   |  PD |                                               |
|   58  |    XGPIO12   |           -          |           -          |   I   |  PD |                                               |
|   10  |    XGPIO13   |           -          |          PU          |   I   |  PD |             SW702 (for XLED TEST)             |
|   9   |    XGPIO14   |           -          |          PU          |   I   |  PD |                     SW701                     |
|   12  |    XGPIO15   |           -          |          PU          |   I   |  PD |             SW703 (for XLED TEST)             |
|   11  |    XGPIO16   |           -          |          PU          |   I   |  PD |              LED (for XLED TEST)              |
|   8   |    XGPIO17   |           -          |           -          |   I   |  PD |                                               |
|   7   |    XGPIO18   |           -          |           -          |   I   |  PD |                                               |
|   6   |    XGPIO19   |           -          |           -          |   I   |  PD |                                               |
|   13  |    XGPIO20   |           -          |          PU          |   I   |  PD |              LED (for XLED TEST)              |
|   5   |    XGPIO21   |           -          |           -          |   I   |  PD |                                               |
|   19  |    XGPIO22   |           -          |           -          |   I   |  PD |                                               |
|   18  |    XGPIO23   |           -          |           -          |   I   |  PD |                                               |
|   4   |    XGPIO24   |           -          |           -          |   I   |  PD |                                               |
|   3   |    XGPIO25   |           -          |           -          |   I   |  PD |                                               |
|   2   |    XGPIO26   |           -          |           -          |   I   |  PD |                                               |
|   75  |    XGPIO27   |           -          |           -          |   I   |  PD |                                               |
|   39  |    XGPIO28   |           -          |           -          |   I   |  PD |                                               |
|   28  |    XEINT0    |           -          |           -          |   I   |  PD |                                               |
|   30  |    XEINT1    |           -          |           -          |   I   |  PD |                                               |
|   29  |    XEINT2    |           -          |           -          |   I   |  PD |                                               |
|   33  |   XI2C0_SCL  |                      |          PU          |   I   |  PD |                                               |
|   34  |   XI2C0_SDA  |           -          |          PU          |   I   |  PD |                                               |
|   35  |   XI2C1_SCL  |           -          |          PU          |   I   |  PD |                                               |
|   36  |   XI2C1_SDA  |           -          |          PU          |   I   |  PD |                                               |
|   17  |  XADC0AIN_3  |           -          |          PD          |       |     |                                               |
|   16  |  XADC0AIN_2  |           -          |          PD          |       |     |                                               |
|   14  |  XADC0AIN_1  |           -          |          PD          |       |     |                                               |
|   15  |  XADC0AIN_0  |           -          |          PD          |       |     |                                               |
|   47  |     RXD0     |           -          |          PU          |   I   |  PD | connected LEVEL Shifter (Need to External PU) |
|   46  |     TXD0     |           -          |          PU          |   I   |  PD | connected LEVEL Shifter (Need to External PU) |
|   38  |  XDEBUG_RXD  |           -          |           -          |   I   |  PD |              for DEBUG (FT2232HL)             |
|   37  |  XDEBUG_TXD  |           -          |           -          |   I   |  PD |              for DEBUG (FT2232HL)             |
|   26  | XJTAG_TRST_N |           -          |           -          |   I   |  PD |              for DEBUG (FT2232HL)             |
|   22  |   XJTAG_TMS  |           -          |          PU          |   I   |  PU |              for DEBUG (FT2232HL)             |
|   24  |   XJTAG_TCK  |           -          |           -          |   I   |  PD |              for DEBUG (FT2232HL)             |
|   23  |   XJTAG_TDI  |           -          |          PU          |   I   |  PU |              for DEBUG (FT2232HL)             |
|   25  |   XJTAG_TDO  |           -          |           -          |   I   |  PD |              for DEBUG (FT2232HL)             |
|   21  |   XRESET_N   |          PU          |           -          |   I   |  PD |                                               |
|   31  |    PWR_RST   |           -          |          NA          |   NA  |  NA |      Not Connected IC, DCDC IC Enable Pin     |
