CFLAGS=-Wall -g

clean:
	rm -f psiforth

all:
	cc -Wall -g    main.c   -o psiforth
