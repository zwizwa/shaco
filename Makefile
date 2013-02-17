.PHONY: all clean
all: test

clean:
	rm -f *~ *.o test

%.o: %.c
	gcc -g -Wall -c $< -o $@
test: test.o
	gcc -g -o $@ $<
