# Webserver Test Application

### Features

- HTTP/HTTPS
- GET/PUT/POST/DELETE method
- Supports Keep-alive
- Chunked Transfer
- Trailer header

### Menuconfig Setup
**For build configuration *rtl8721csm/security_hal_test_tz* refer to**
(build/configs/rtl8721csm/security_hal_test_tz/defconfig)
- *In SE Selection turn-off HW rsa verification and HW rsa encryption to test in ecdsa*

**For build configuration *qemu/tc_16m***
(Turn ON "Disable time check" because system time will not match to certificate)
- Networking Support > Protocols > Disable Time Check

**Enable web server**
> Networking support > Protocols > Webserver 

> Application Configuration > Examples > Webserver example

> Application Configuration > Examples > Webserver test

## webserver_test usage :

**webserver_test iterations method uri [options...]**

- iterations range [1,10000]
- method and uri parameters should be given,  [options] parameters are optional.
- method is one of "GET", "POST", "PUT", "DELETE".  
- url should start with "http://" or "https://".
   
[options] parameter support several different options.
-    chunk-count=Z        : Enables chunked encoding and send Z chunks (default is disabled)
-    test_entity=SIZE : Input test entity dummy data (default is 0)
-    keep-alive=X     : Turns on keep-alive and sends X requests (default Keep-Alive is OFF)
-    trailer=1        : Turns on Trailer-header

Example usage:

Start webserver : `webserver start none`

Run test application : `webserver_test 3 POST https://127.0.0.1/ chunk-count=4 test_entity=5 trailer=1 keep-alive=2`

Above command runs 3 test iterations inside the application

Keep-Alive is ON

In each keep-alive session 2 POST requests are made

In each POST request 4 chunks of the data of size 5 bytes is send along with trailer header
