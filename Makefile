# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/30 12:05:40 by marvin            #+#    #+#              #
#    Updated: 2024/09/30 12:05:40 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	philo
CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror
RM				=	rm -f
AR				=	ar rcs

BLUE			= \033[0;34m
PURPLE			= \033[0;35m
WHITE			= \033[0;37m
BLACK			= \033[0;30m
CYAN			= \033[0;36m
YELLOW			= \033[0;33m
RED				= \033[0;31m
GREEN			= \033[0;32m
NC				= \033[0m

INC				= 	inc/
SRC_DIR			= 	src/
OBJ_DIR			= 	obj/
	   
SRC 			= $(SRC_DIR)philo.c \
				  $(SRC_DIR)init_philos.c \
				  $(SRC_DIR)routine.c \
				  $(SRC_DIR)parse_input.c \
				  $(SRC_DIR)utils.c \
				  $(SRC_DIR)mutex.c \
				  $(SRC_DIR)time.c
		
OBJ = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC))

all : $(NAME)

$(NAME) : $(OBJ)
	clear; \
	echo "$(RED)███████╗███████╗███████╗███████╗███████╗███████╗"; \
	echo "╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝"; \
	echo "██████╗ ██╗  ██╗██╗██╗      ██████╗  "; \
	echo "██╔══██╗██║  ██║██║██║     ██╔═══██╗ "; \
	echo "██████╔╝███████║██║██║     ██║   ██║ "; \
	echo "██╔═══╝ ██╔══██║██║██║     ██║   ██║ "; \
	echo "██║     ██║  ██║██║███████╗╚██████╔╝ "; \
	echo "╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝    -sbruma"; \
	echo "███████╗███████╗███████╗███████╗███████╗███████╗"; \
	echo "╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝"; \
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -I $(INC)
	@pid=$$!; \
	sleep 1; \
	clear; \
	echo "$(YELLOW)███████╗███████╗███████╗███████╗███████╗███████╗"; \
	echo "╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝"; \
	echo "██████╗ ██╗  ██╗██╗██╗      ██████╗  "; \
	echo "██╔══██╗██║  ██║██║██║     ██╔═══██╗ "; \
	echo "██████╔╝███████║██║██║     ██║   ██║ "; \
	echo "██╔═══╝ ██╔══██║██║██║     ██║   ██║ "; \
	echo "██║     ██║  ██║██║███████╗╚██████╔╝ "; \
	echo "╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝    -sbruma"; \
	echo "███████╗███████╗███████╗███████╗███████╗███████╗"; \
	echo "╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝"; \
	sleep 1; \
	clear; \
	echo "$(GREEN)███████╗███████╗███████╗███████╗███████╗███████╗"; \
	echo "╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝"; \
	echo "██████╗ ██╗  ██╗██╗██╗      ██████╗  "; \
	echo "██╔══██╗██║  ██║██║██║     ██╔═══██╗ "; \
	echo "██████╔╝███████║██║██║     ██║   ██║ "; \
	echo "██╔═══╝ ██╔══██║██║██║     ██║   ██║ "; \
	echo "██║     ██║  ██║██║███████╗╚██████╔╝ "; \
	echo "╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝    -sbruma"; \
	echo "███████╗███████╗███████╗███████╗███████╗███████╗"; \
	echo "╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝"; \
	wait $$pid

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

clean :
	@$(RM) -f $(OBJ)
	@$(RM) -rf $(OBJ_DIR)
	clear; \
	echo "$(CYAN)███████╗███████╗███████╗███████╗███████╗███████╗"; \
	echo "╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝"; \
	echo " ██████╗██╗     ███████╗ █████╗ ███╗   ██╗"; \
	echo "██╔════╝██║     ██╔════╝██╔══██╗████╗  ██║"; \
	echo "██║     ██║     █████╗  ███████║██╔██╗ ██║"; \
	echo "██║     ██║     ██╔══╝  ██╔══██║██║╚██╗██║"; \
	echo "╚██████╗███████╗███████╗██║  ██║██║ ╚████║"; \
	echo "╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝"; \
	echo "███████╗███████╗███████╗███████╗███████╗███████╗"; \
	echo "╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝╚══════╝ $(NC)"; 

fclean : clean
	@$(RM) $(NAME)

re : clean fclean all