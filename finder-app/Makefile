CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -Werror -g
TARGET = writer
SRC = writer.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(TARGET) clean