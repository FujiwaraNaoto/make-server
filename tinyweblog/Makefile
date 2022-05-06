CC = gcc
TARGET = main
OBJ = DieWithError.o hacking.o hacking-network.o main.o logger.o

.PHONY: all
all: $(TARGET)


$(TARGET):$(OBJ)
	$(CC) $^ -Wall -o $@

%.o:%.c
	$(CC) -c $<


.PNONY: clean
.clean:
	rm -rf *.o


main.o : main.c hacking.h hacking-network.h logger.h DieWithError.c
DieWithError.o : DieWithError.c DieWithError.h
hacking-network.o : hacking-network.c hacking-network.h
logger.o : logger.c logger.h
hacking.o : hacking.c hacking.h

