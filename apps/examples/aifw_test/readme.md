## **Application compilation steps**
1. Run _./os/dbuild.sh menu_
2. Select "5. Clean Build and Re-Configure" to clean tizenrt. This step is not needed while building our code for the first time after a fresh clone.
3. Select "11. rtl8721csm" to select board.
4. Select "9. ai_tfmicro" to select configuration of rtl8721csm.
5. Select "3. Modify Current Configuration" to modify current configuration of tizenrt.
6. In menuconfig, go to "AI Framework". Turn on "AI Framework", then go to "AIFW Debug Logs" and turn on necessary logs.
7. Go back to main menuconfig screen, and go to "Application Configuration", then go to "Examples" and turn on "AIFW test application" and exit menuconfig.
8. Select "1. Build with Current Configuration" or run command _./os/dbuild.sh_to build tizenrt.

## **Steps to build Smart FS**
1. Copy necessary files in folder tools/fs/contents-smartfs/rtl8721csm/base-files/AI
2. Run _./os/dbuild.sh menu_
3. Select "6. Build SmartFS Image" to build smart fs.