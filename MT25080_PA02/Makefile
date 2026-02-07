# MT25080 - Makefile for GRS PA02
CC = gcc
CFLAGS = -Wall -pthread -O2
# O2 optimization usually good for performance comparison

all: MT25080_Part_A1_Server MT25080_Part_A2_Server MT25080_Part_A3_Server MT25080_Part_A_Client

MT25080_Part_A1_Server: MT25080_Part_A1_Server.c MT25080_Part_A_Common.h
	$(CC) $(CFLAGS) -o MT25080_Part_A1_Server MT25080_Part_A1_Server.c

MT25080_Part_A2_Server: MT25080_Part_A2_Server.c MT25080_Part_A_Common.h
	$(CC) $(CFLAGS) -o MT25080_Part_A2_Server MT25080_Part_A2_Server.c

MT25080_Part_A3_Server: MT25080_Part_A3_Server.c MT25080_Part_A_Common.h
	$(CC) $(CFLAGS) -o MT25080_Part_A3_Server MT25080_Part_A3_Server.c

MT25080_Part_A_Client: MT25080_Part_A_Client.c MT25080_Part_A_Common.h
	$(CC) $(CFLAGS) -o MT25080_Part_A_Client MT25080_Part_A_Client.c

clean:
	rm -f MT25080_Part_A1_Server MT25080_Part_A2_Server MT25080_Part_A3_Server MT25080_Part_A_Client
