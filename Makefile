CC		=	gcc

RM		=	rm -rf

SRC_DIR	=	src/

SRC		= 	$(SRC_DIR)main.c	\
			$(SRC_DIR)init.c	\
			$(SRC_DIR)map.c		\
			$(SRC_DIR)move.c	\
			$(SRC_DIR)player.c	\

OBJ		= 	$(SRC:.c=.o)

NAME	=	lemipc

CFLAGS	=	-I./inc -I./

CFLAGS	+=	-W -Wall -Werror -g

$(NAME)	:	$(OBJ)
		$(CC) -o $(NAME) $(OBJ)

clean	:
		$(RM) $(OBJ)

fclean	:	clean
		$(RM) $(NAME)

all		:	$(NAME)

re		: fclean all

.PHONY	: all clean fclean re
