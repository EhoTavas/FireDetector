CC = gcc
CFLAGS = -Wall -pthread

TARGET = fire_detection

all: $(TARGET)

$(TARGET): main.o functions.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o functions.o

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f *.o $(TARGET)
