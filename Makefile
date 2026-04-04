CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -pedantic

SRCS := src/main.c src/parser.c src/queries.c src/sentences.c src/state.c

OBJS := $(SRCS:.c=.o)


.PHONY: default grade clean

default: spireslayer

spireslayer: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

grade:
	python3 test/grader.py ./spireslayer test-cases

clean:
	rm -f spireslayer
