Compiling Interface APIs FOR Android:
===================================
•Preconditons :
•Supported version : 5.0 ( Lollipop )
•Required NDK version : android-ndk-r10d ( https://developer.android.com/tools/sdk/ndk/index.html )

Building CA Library for android :

Run the SCons as mentioned in connectivity/build/How_To_Build.txt

Building CA Samples for Android :

Run the SCons as mentioned in connectivity/build/How_To_Build.txt
Edit connectivity/SConscript to build the APK

uncomment the below line to build the sample :
env.SConscript('./samples/' + target_os + '/casample/SConscript')

Note : If this is enabled before building the SO files, Build will fail.
Ensure to disable the script whenever not required

Installing the APK :

Run the following command :

$(ADB_HOME)/adb install resource/csdk/connectivity/samples/android/casample/sampleService/build/outputs/apk/sampleService-debug.apk
