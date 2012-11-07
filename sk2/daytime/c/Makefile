all: $(basename $(wildcard *.c))

%: %.c
	gcc -O0 -Wall -g $< -o $@ -pthread

