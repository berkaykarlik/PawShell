CC = g++
CFLAGS = -Wall -Wextra -std=c++11
TARGET = barksh
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
