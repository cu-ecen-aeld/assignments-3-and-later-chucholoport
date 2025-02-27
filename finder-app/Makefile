CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
TARGET = writer
SRC = writer.c

$(TARGET): $(SRC)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f *.o $(TARGET)

.PHONY: $(TARGET) clean