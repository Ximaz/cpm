##
## Makefile
## Builds the libcpm.a
##
CC=gcc
LIBFLAGS=-I./include -L./ -lcpm
CFLAGS=-Wall -Wextra -Werror -pedantic -O3# -fsanitize=leak

SRC=src/config.c \
	src/parser.c \
	src/util.c \
	src/set.c
OBJ=*.o

NAME=cpm
TEST=test.c
LIBNAME=libcpm.a

$(NAME): $(LIBNAME)
	$(CC) $(CFLAGS) -o $(NAME) $(TEST) $(LIBFLAGS)

$(LIBNAME): $(OBJ)
	ar rc $(LIBNAME) obj/$(OBJ)

$(OBJ): $(SRC) obj/
	$(CC) $(CFLAGS) -c $(SRC)
	mv $(OBJ) obj/

obj/:
	mkdir -p obj

all: $(NAME)

clean:
	rm -rf obj/$(OBJ)

fclean:
	rm -rf $(LIBNAME) $(NAME)

re: fclean clean

.PHONY: all clean re
