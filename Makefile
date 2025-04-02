BINARY=creep.out
SRC= creep.c string_array.c cJSON.c
OPTS=
CFLAGS= -Wall -pedantic -Wextra

build:
	gcc -o $(BINARY) $(CFLAGS) $(SRC)
