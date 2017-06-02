STEPS to setup Arduino on linux machine for building CA code:

1) Download the Arduino package as per your system specification from the given link :
	http://arduino.cc/en/main/software

2) Unzip the downloaded package to some location say ARDUINO_DIR(This is the arduino installation directory).
	ARDUINO_DIR = <PATH_TO_ARDUINO_INSTALL_DIR>/arduino-1.5.x

3) Change permissions for executable in your "ARDUINO_DIR"
		chmod -R +x <ARDUINO_DIR>

4.1) ONE TIME ONLY - ADDITIONAL LIBRARIES (Time)
	Arduino builds are dependent on latest Time library. Download it from here:
		I. http://playground.arduino.cc/uploads/Code/Time.zip
	Place this library in the "ARDUINO_DIR/libraries/Time" folder where other libraries are present.
		[P.S - The Time library files path should be as follows "ARDUINO_DIR/libraries/Time/Time/{***files***}"]
	Some files in this library has windows style line endings, for patch to run properly, please execute following command:
		cd <ARDUINO_DIR>
		find ./libraries/Time/ -type f -exec dos2unix {} \;

4.2) ONE TIME ONLY - ADDITIONAL LIBRARIES (BLE)
	Note: Prior to building for BLE Transport, following additional steps are required:
	(1) Download RedBearLab Arduino RBL_nRF8001 Library from link(https://github.com/RedBearLab/nRF8001/archive/25643e7b1b7da3740406325a471e3faa4b948747.zip)
	(2) Extract zip file and apply RBL patch(connectivity/lib/arduino/RBL_nRF8001.patch) to "RBL_nRF8001" folder by following below steps,
		i)  Go to downloaded RBL_Library Folder ("nRF8001-master/Arduino/libraries/RBL_nRF8001")
		ii) Apply dos2unix for RBL_Library Folder and RBL Patch File (RBL_nRF8001.patch), if we have downloaded Library in Non UNIX Platform.
		iii) Apply RBL Patch[connectivity\lib\arduino\RBL_nRF8001.patch] by using the below command
			patch -p1 < RBL_Patch_File_Location(RBL_nRF8001.patch)
	(3) Copy the patched RBL_nRF8001 Library and place it in "arduino-<x.x.x>/libraries" folder
	(4) Download Nordic Arduino BLE library [Tag: "0.9.5.Beta" Version] from the link (https://github.com/NordicSemiconductor/ble-sdk-arduino/archive/0.9.5.beta.zip).
		Extract the zip file and copy the "BLE" folder available under "libraries" folder and place it in "arduino-<x.x.x>/libraries" folder
		i)  Apply dos2unix for the "BLE" folder.
		ii) Go to the BLE folder and apply the patch[connectivity\lib\arduino\arduino_due_ble.patch] using the below command.
			patch -p1 < path/to/arduino_due_ble.patch

5) ONE TIME ONLY - PATCH
	Complete patch has been provided at : "connectivity\lib\arduino\arduino_libraries.patch"
		Copy this patch file to "ARDUINO_DIR/"

6) To apply patch give the command :
		patch -p1 < arduino_libraries.patch
	To undo the patch give the command :
		patch -R -p1 < arduino_libraries.patch

7) ONE TIME ONLY - FIRMWARE UPGRADE (FOR WIFI SHIELD)
	To support UDP over WiFi, WiFi shield needs to run latest firmware. Please refer:
	(http://arduino.cc/en/Hacking/WiFiShieldFirmwareUpgrading)
	for instructions on how to upgrade your firmware.

8) Launch Arduino IDE (required for logs and port and board selection.)
	Goto to "ARDUINO_DIR" directory.
	Launch command := sudo ./arduino

9) Select your board and port under the tools option in Arduino IDE.
