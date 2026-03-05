###########################################################################
#
# Copyright 2026 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################

-include $(TOPDIR)/Make.defs

APP_OBJ = up_userspace$(OBJEXT)
COMMON_OBJ = up_userspace_common$(OBJEXT)

all: app_obj
.PHONY: all app_obj common_obj clean distclean

app_obj: $(APP_OBJ)

common_obj: $(COMMON_OBJ)

$(APP_OBJ): up_userspace.c
	$(Q) $(CC) $(APPDEFINE) -c $(CELFFLAGS) $< -o $@

$(COMMON_OBJ): up_userspace.c
	$(Q) $(CC) -c $(CFLAGS) -D__COMMON_BINARY__ $< -o $@

clean:
	$(call DELFILE, $(APP_OBJ))
	$(call DELFILE, $(COMMON_OBJ))
	$(call CLEAN)

distclean: clean
