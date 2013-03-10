CFLAGS=-Wall -g

clean:
	rm -f pforth

all:
	cc -Wall -g    main.c   -o pforth
