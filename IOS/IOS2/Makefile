
ALL: proj2

proj2: proj2.o
	gcc -std=gnu99 -lrt -pthread -Wall -Wextra -Werror -pedantic -o proj2 proj2.o

proj2.0: *.c
	gcc -std=gnu99 -lrt -pthread -Wall -Wextra -Werror -pedantic -c *.c

clean:
	rm -f proj2 *.o

run: proj2
	./proj2
