CC = g++

CFLAGS = -Wall -g
#CFLAGS = -Wall -O2

TARGET = CA_SIMULATOR

all:  $(TARGET)

CA_SIMULATOR: main.o human.o CA.o globals.o auxilary.o
	$(CC) $(CFLAGS) -o CA_SIMULATOR main.o human.o CA.o globals.o auxilary.o
	

main.o: main.cpp human.h CA.h globals.h auxilary.h
	$(CC) $(CFLAGS) -c main.cpp

CA.o: CA.h human.h globals.h auxilary.h

human.o: human.h globals.h auxilary.h

globals.o: globals.h

auxilary.o: auxilary.h

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET)
	rm *.o

