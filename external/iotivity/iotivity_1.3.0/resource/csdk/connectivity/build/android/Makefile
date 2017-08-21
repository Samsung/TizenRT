#Assumes ndk directory is at ~/
#set ndk directory PATH in .bashrc and use the ndk-build directly
#change NDK_PATH to ${your_ndk_path}
NDK_PATH = ~/ndk
NDK_BUILD = $(NDK_PATH)/ndk-build

BUILD_DIR = $(CURDIR)
APPLICATION_BUILD = $(BUILD_DIR)
ROOT_DIR = $(BUILD_DIR)

#Clean files
LIBS_DIR = $(ROOT_DIR)/libs
OBJ_DIR = $(ROOT_DIR)/obj

all:
	$(NDK_BUILD) NDK_PROJECT_PATH=$(APPLICATION_BUILD)

clean :
	@$(RM) -rf $(LIBS_DIR) $(OBJ_DIR)
