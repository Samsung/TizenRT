CC = gcc

TARGET = png2c

LDFLAGS = -lpng

CSRCS = png2c.c

$(TARGET) : $(CSRCS)
	$(CC) -o $(TARGET) $(CSRCS) $(LDFLAGS)

clean:
	rm $(TARGET)

