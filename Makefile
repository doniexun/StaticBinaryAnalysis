##
## Makefile for  in /home/bartholomew/programmation/c/prototype
##
## Made by Bartholomew
## Bartholomew de la Villardiere <exabot@gmail.eu>
##
## Started on  mer. mars 12 04:39:48 2014
## Last update mer. mars 12 04:39:48 2014
##

CC		?=	gcc
CFLAGS		+=	-W -Wall -pedantic -ansi -Wextra -O3
CFLAGS		+=	-I./include
LDFLAGS		+=	-lcapstone -ldl
VPATH		=	./src

SRC		=	main.c		\
			get_nb_params.c
OBJ		=	$(SRC:.c=.o)
NAME		=	get_params
DEBUG		=	-g

all: $(NAME)

$(NAME):$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: CFLAGS += $(DEBUG)
debug: re
