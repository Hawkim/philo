# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nal-haki <nal-haki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/19 13:14:09 by nal-haki          #+#    #+#              #
#    Updated: 2025/01/10 12:43:38 by nal-haki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= philo

CC 			= cc
RM 			= rm -rf
INCLUDE 	= -Iinclude

CFLAGS = -Wall -Wextra -Werror -pthread $(INCLUDE)

SRC_DIR 	= src/
OBJ_DIR 	= obj/

SRC 		= 	philo.c \
				thread_utils.c \
				parse_args.c \
				init.c \
				utils.c

OBJ 		= $(addprefix $(OBJ_DIR), $(SRC:.c=.o))


all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c 
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@echo ""${BLUE}$(NAME)""${NC}Compiling..."\c"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo ""${GREEN}Complete""$(NC)""

clean:
	@echo ""${BLUE}$(NAME)""${NC}Cleaning..."\c"
	@$(RM) $(OBJ_DIR)
	@echo ""${GREEN}Complete""$(NC)""

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

# Colours
WHITE	=	'\033[0;37m'
YELLOW	=	'\033[0;33m'
BLUE	=	'\033[0;34m'
GREEN	=	'\033[0;32m'
RED		=	'\033[0;31m'
NC		=	'\033[0m' # no colour
#> /dev/null

val		= 	valgrind --leak-check=full --track-fds=all --track-origins=yes --show-reachable=yes -s
run		=	./$(NAME)

norm:
	norminette ./src ./include

# * Should finish

test1: # should complete two meals each 
	./philo 2 400 100 100 2

# * No philos should die
test2: # no philosopher should die
	./philo 5 800 200 200

test3: # no philosopher should die
	./philo 4 410 200 200

test4:
	./philo 3 620 200 200

# * Philo should die

test5: # one death
	./philo 4 310 200 100

test6: # should take left fork then die 
	./philo 1 400 100 100 2

# * Should die

testy: # should any die? depends if you think philos should die at 600 or at 601
	./philo 3 600 200 60

# * Should not run

testz: # should not run
	./philo 0 400 100 100 2

bench:
	./philo 200 410 200 200

bench2:
	./philo 199 610 200 200

val1:
	valgrind --tool=helgrind ./philo 5 800 200 200

val2:
	valgrind --tool=drd ./philo 5 800 200 200