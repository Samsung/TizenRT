ttrace parser
=============

  These are files for parsing T-trace logs to visualize T-trace result

Usage
======

1. The text file contains T-trace logs
  $ ./ttrace_tinyara.py [-i <input_filename>] [-o <output_foldername>]

  After running it, You can get HTML file which show T-trace results in <output_folder>.
  If you run it without '-i' options,
  ttrace_tinyara.py returned immediately.
  If you run it without '-o' options,
  HTML file will saved at same folder with input_filename.

2. Dump from artik053 device
  $ ./ttrace_tinyara.py [-d <model_name>]

  After running it, You can get HTML file which show T-trace results in current folder.
  ttrace_tinyara.py will connect target as GDB and set breakpoint at wait_ttrace_dump().
  You had traced on target($ ttrace -s <tags>), then run dump mode($ ttrace -d),
  GDB will catch it and dump ttrace buffer.

  Detail examples are below:
  1. HOST$ ./ttrace_tinyara.py -d artik053
  2. artik053$ ttrace -s apps libs ipc lock task
  3. artik053$ ttrace -f
  4. artik053$ ttrace -d

  Dump feature needs tinyara binary, System.map, artik053.cfg and openocd executable,
  They are located in tinyara/build/output/bin and
  tinyara/build/configs/artik053/tools/openocd.
  These paths are set by default.
  However, You had changed location of them,
  You can specified paths to ./scripts/ttrace_tinyaraDump.py with -b, -d options.

  for examples,
  $ HOST$ ./scripts/ttrace_tinyaraDump.py -t artik053 -b <binaryPath> -d <openocdPath>

Example
=======

  $ ./ttrace_tinyara.py -i sample/sample_log

  You can get results of parsing 'sample_log' in 'sample' folder.
