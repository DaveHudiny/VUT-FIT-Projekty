#makefile

CC = g++
CPPFLAGS = -std=c++17 -Wall
OBJS = client

all: $(OBJS)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $<
	

$(OBJS): main.o base64.o
	$(CC) $(CPPFLAGS) $^ -o $@
	rm *.o

#library.o: library.hpp library.cpp
main.o: main.hpp main.cpp

base64.o: base64.cpp base64.h

clean: 
	rm client

run: client
	./client
