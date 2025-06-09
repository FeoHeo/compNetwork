# Makefile to build network from network_sim.c

CC = gcc
TARGET = network
SRC = network_sim.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm $(TARGET)
