# TizenRT Application Generator

### Requirements
- Latest bash
- Generally, Ubuntu linux has the latest bash

### How to use?

In any directory, run the appgen shell script and just enter the application name that you would make.

Your application would be generated at the apps/examples/

```sh
TizenRT/os$ tools/appgen.sh
TizenRT Application Generator
======================= v 1.0
Enter application name: test application
[Summary]
Application Name: test application
Configuration Key: CONFIG_APP_TEST_APPLICATION
Entry Function: test_application_main
Continue? (y/N): y
Generating...
Your application is located at /home/gcjjyy/workspace/github.com/gcjjyy/TizenRT/apps/examples/test_application
Done..
```

### After generated
```sh
TizenRT/os$ make menuconfig
```

- Enter "Application Configuration/Examples"
- Turn on your application
- Set the entry point to your application
