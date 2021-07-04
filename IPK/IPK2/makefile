#makefile

CC = g++
CPPFLAGS = -std=c++17 -Wall
OBJS = ipk-sniffer

all: $(OBJS) clean

run: ipk-sniffer
	./ipk-sniffer

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -lpcap

ipk-sniffer: main.o
	$(CC) $(CFLAGS) $^ -o $@ -lpcap

main.o: main.h main.cpp
clean: 
	rm *.o 