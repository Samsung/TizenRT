include araui.mk

TARGET = sim

# Application
CSRCS += src/sim_main.c

# Driver Abstraction Layer (DAL)
CSRCS += src/dal/dal_sdl.c

all: $(TARGET)

$(TARGET): $(CSRCS)
	@echo "CC:  " $@
	$(CC) $(CFLAGS) -o $@ $(CSRCS) $(LDFLAGS)

clean:
	@find . -name '*.o' -type f -delete
	@find ../../../../framework -name '*.o' -type f -delete
	@rm -rf ./*.dSYM
	@rm -rf $(TARGET)
