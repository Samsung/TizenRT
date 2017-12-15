# Media Test Example

## What is Media Test Example?
This is an example for testing TizenRT's Media Framework.
Media Framework has functions which related to the playing and recording sound.

## How to run this example?
1. Build server example in your Linux environment.
<pre>$ cd ($YOUR_APP_DIR)/examples/media_test/server
$ g++ -pthread -o server server_example_linux.c
$ ./server_example_linux</pre>

2. Fix IP address in test example application.
3. In media_tash_main.c 163 Line, you can see the ip address.
4. Fix the ip address correct to your environment.
5. Build TizenRT binary
<pre>os/tools $ ./configure.sh artik053/audio
os/tools $ cd ..
os $ make menuconfig</pre>
6. Turn On 'Media Supporting'
7. Turn On 'DM' in 'Device Management'
8. Turn On 'Media test example' in 'Application Configuration/Examples'
9. Modify Entry point 'media_test_cb'