.PHONY: all
all: test

%.o: %.c
	gcc -g -Wall -c $< -o $@
test: shaco.o
	gcc -g -o $@ $<
