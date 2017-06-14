BUILDING for ARDUINOMEGA:
========================================
PREREQ: Arduino MUST be installed in the machine where we compile. Refer to Arduino_Setup_README file for help.

1) Go to "connectivity\build\arduino" directory, Open Makefile and change ARDUINO_PORT according to the PORT selected in ARDUINO IDE.
2) BUILD : Go to "connectivity\build" directory. To build for a specific TRANSPORT [ETHERNET,BLE,WIFI], use the following command :
	sudo make PLATFORM=arduinomega TRANSPORT=<ETHERNET/BLE/WIFI> ARDUINO_DIR = <PATH_TO_ARDUINO_INSTALL_DIR>/arduino-1.5.x
	P.S : For PATH_TO_ARDUINO_INSTALL_DIR value refer Arduino_Setup_README.txt file.

Flashing Interface APIs for ARDUINOMEGA:
========================================
Precondition: Connect Arduino Mega Board to Linux PC. Open Arduino IDE and select corresponding connected PORT
1) To Install, connect the ArduinoMega Board with corresponding Shields to Linux PC.
2) Perform sudo make install PLATFORM=arduinomega TRANSPORT=BLE
   It will flash BLE Transport Adaptor in Arduino Board. Similarly, TRANSPORT can be changed to ETHERNET, BLE, WIFI etc..

To Run Adapter code in ARDUINOMEGA Board:
========================================
PreCondition: Flash the executable in ARDUINO MEGA BOARD. Follow above Step(Flashing Interface APIs for ARDUINOMEGA)
1) Open Serial Monitor in Arduino IDE. It will execute the flashed binary and logs will be available in serial Monitor under BaudRate(115200)
