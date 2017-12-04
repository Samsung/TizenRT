# ARTIK053s

![ARTIK053s](../../../docs/media/a053.jpg)

The ARTIK053s is a SOC for Wi-Fi™ IoT solutions. The ARTIK053s has a Wi-Fi subsystem, security subsystem, and application subsystem.

## Information

The Samsung ARTIK™ 053s Module is a highly integrated module for secure Internet of Things (IoT) devices that require Wi-Fi®. It is based on an ARM® Cortex® R4 core with on-chip memories, a complete 2.4GHz Wi-Fi® Phy, MAC layer processing, a large complement of standard digital buses, a PUF-based security system and power management. The module is packaged with additional external Flash memory, a hardware Secure Element and a single integrated 2.4GHz structural antenna.
Aimed especially at power-sensitive devices needing Wi-Fi®, the ARTIK053s Module provides excellent performance in a variety of environments, with a feature set tailored specifically for IoT end nodes.

### ARTIK053s Starter Kit PinOut
#### CON710 CON711 CON708 CON709
```
                 XGPIO13(gpio42) <- SW702 | SW703 -> XGPIO15(gpio44)
                XGPIO16(gpio45) <- LED703 | LED702 -> XGPIO20(gpio49)
                                             [CON708]
                           |           |     * -> XI2C0_SCL
                           |           |     * -> XI2C0_SDA
                           |           |     * -> AREF
              [CON710]     |           |     * -> GND
               NC <- *     |           |    13 -> XSPI1_CLK
               NC <- *     |           |    12 -> XSPI1_MISO
            RESET <- *     |           |    11 -> XSPI1_MOSI
             3.3V <- *     | ARTIK053s |    10 -> XSPI1_CSN
               NC <- *     |           |     9 -> XPWMTOUT_4
              GND <- *     |           |     8 -> XGPIO21(gpio50)
              GND <- *     |           |     [CON709]
              Vin <- *     |           |     7 -> XGPIO19(gpio48)
              [CON711]     |           |     6 -> XPWMTOUT_2
            XADC0 <- A0    +-----------+     5 -> XPWMTOUT_1
            XADC1 <- A1                      4 -> XGPIO18(gpio47)
            XADC2 <- A2                      3 -> XPWMTOUT_0
            XADC3 <- A3                      2 -> XGPIO17(gpio46)
            XADC4 <- A4                      1 -> XUART0_TX
            XADC5 <- A5                      0 -> XUART0_RX

                   [CON703]
                   2 4 6 8 10 12 14 16 18 20 22 24
                   1 3 5 7  9 11 13 15 17 19 21 23
                   [CON704]
                   2 4 6 8 10 12 14 16 18 20 22 24
                   1 3 5 7  9 11 13 15 17 19 21 23
```
#### CON703
```
                 XPWMTOUT_1 <-  1 |  2 -> VCC_EXT3P3
                 XPWMTOUT_2 <-  3 |  4 -> XADC6
                 XPWMTOUT_3 <-  5 |  6 -> XADC7
                 XPWMTOUT_0 <-  7 |  8 -> XI2C1_SCL
                 XUART1_RXD <-  9 | 10 -> XI2C1_SDA
                 XUART1_TXD <- 11 | 12 -> GND
            XGPIO26(gpio55) <- 13 | 14 -> VCC_EXT3P3
            XGPIO25(gpio54) <- 15 | 16 -> XSPI0_CLK
            XGPIO24(gpio53) <- 17 | 18 -> XSPI0_CSN
            XGPIO23(gpio52) <- 19 | 20 -> XSPI0_MISO
            XGPIO22(gpio51) <- 21 | 22 -> XSPI0_MOSI
             XEINT0(gpio57) <- 23 | 24 -> GND
```
#### CON704
```
                 XPWMTOUT_4 <-  1 |  2 -> VCC_EXT3P3
                 XPWMTOUT_5 <-  3 |  4 -> XUART2_RXD
             XEINT2(gpio59) <-  5 |  6 -> XUART2_TXD
             XEINT1(gpio58) <-  7 |  8 -> XUART3_RXD
            XGPIO12(gpio41) <-  9 | 10 -> XUART3_TXD
            XGPIO10(gpio39) <- 11 | 12 -> GND
             XGPIO9(gpio38) <- 13 | 14 -> VCC_EXT3P3
            XGPIO11(gpio40) <- 15 | 16 -> XGPIO4(gpio33)/XSPI2_CLK
             XGPIO8(gpio37) <- 17 | 18 -> XGPIO5(gpio34)/XSPI2_CSN
             XGPIO2(gpio31) <- 19 | 20 -> XGPIO6(gpio35)/XSPI2_MISO
             XGPIO1(gpio30) <- 21 | 22 -> XGPIO7(gpio36)/XSPI2_MOSI
             XGPIO3(gpio32) <- 23 | 24 -> GND
```

## How to Build

This guide is a summary. For more information, please see [here](../artik05x/README.md#how-to-build).
```bash
$ git clone https://github.com/SamsungARTIK/TizenRT.git
$ cd TizenRT
$ cd os/tools
$ ./configure.sh artik053s/nettest
$ cd ..
$ make
```
```bash
$ make download ALL
```
