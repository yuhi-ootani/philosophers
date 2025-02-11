# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    MAKEFILE                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: otaniyuhi <otaniyuhi@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 11:52:00 by otaniyuhi         #+#    #+#              #
#    Updated: 2025/02/11 12:07:24 by otaniyuhi        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = philo

CC = cc 
CFLAGS =  -Wall -Wextra -Werror

OBJ_DIR = objs

SRCS = a_main.c b_parsing.c c_init.c d_dinner.c d_monitor.c e_clean.c \
       pthread_handle.c set_get_mutex.c time_utils.c write_status.c
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all : $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# The -p option in the command mkdir -p $(OBJ_DIR) 
# tells mkdir to create the entire directory tree (all parent directories) 
# if they don't already exist
$(OBJ_DIR)/%.o : %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)
 
re : fclean all

.PHONY: all clean fclean re