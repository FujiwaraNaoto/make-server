CC = gcc
TARGET = pcap1
OBJ = DieWithError.o pcap1.o
CFLAGS = -Wall -lpcap


$(TARGET):$(OBJ)
	$(CC) $^ $(CFLAGS) -o $@


%.o:%.c
	$(CC) -c $<



DieWithError.o: DieWithError.c DieWithError.h
pcap1.o: pcap1.c