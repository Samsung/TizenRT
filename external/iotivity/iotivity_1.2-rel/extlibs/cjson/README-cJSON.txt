This dependency comes from: https://github.com/DaveGamble/cJSON.git. The
cJSON.c and cJSON.h files are copied from that repo.

Commit ID from when files were last copied: 11fd27ade73c5049295a58eb0587e283241d0e70
Release tag when files were last copied (if applicable): v1.0.1

With the following changes:
* git revert 679004914fceb1c418545fb33c707fb454cbc4bb (Define a boolean type)
  * Causes a build break on Arduino because it declares 'bool' to be 'int'

