# Makefile to build network from network_sim.c

CC = gcc
TARGET = DistanceVector
SRC = network_sim.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm $(TARGET)
