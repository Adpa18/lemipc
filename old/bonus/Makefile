CC		=	gcc

RM		=	rm -rf

SRC_DIR	=	src/

SRC		= 	$(SRC_DIR)main.c

OBJ		= 	$(SRC:.c=.o)

NAME	=	lemipcGraphic

CFLAGS	=	-I./inc

CFLAGS	+=	-W -Wall -Werror -D_REENTRANT

LIBS	=	-lSDL2 -lpthread

$(NAME)	:	$(OBJ)
		$(CC) -o $(NAME) $(OBJ) $(LIBS)

clean	:
		$(RM) $(OBJ)

fclean	:	clean
		$(RM) $(NAME)

all		:	$(NAME)

re		: fclean all

.PHONY	: all clean fclean re
