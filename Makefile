# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 22:45:46 by acosta-a          #+#    #+#              #
#    Updated: 2022/12/20 15:44:03 by acosta-a         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

SRCS = cub3D.c

FLAGS = -g -Werror -Wextra -Wall

OBJS = ${SRCS:.c=.o}

$(NAME) : $(OBJS)
	gcc $(FLAGS) $(SRCS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all clean

.PHONY: clean fclean re
