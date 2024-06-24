# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrechuli <mrechuli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/24 18:04:12 by mrechuli          #+#    #+#              #
#    Updated: 2024/06/24 18:27:18 by mrechuli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC = main.c utils.c init.c threads.c monitor.c routine.c

OBJ = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror -O3 -pthread

HEADER = philo.h

SANITIZER = -fsanitize=thread

.PHONY: all clean fclean re debug

all: $(NAME)

$(NAME): $(OBJ)
	@cc $(FLAGS) -o $(NAME) $(OBJ)

%.o: %.c $(HEADER)
	@cc $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

debug: FLAGS += $(SANITIZER)
debug: re