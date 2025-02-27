BINARY=creep.out
SRC= creep.c cJSON.c
OPTS=
CFLAGS= -Wall -Werror -pedantic -Wextra

build:
	gcc -o $(BINARY) $(CFLAGS) $(SRC)
