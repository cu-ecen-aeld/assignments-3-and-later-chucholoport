CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -Werror -g
TARGET = writer
SRC = writer.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean