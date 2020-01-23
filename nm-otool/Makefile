# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rostroh <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/17 11:17:25 by rostroh           #+#    #+#              #
#    Updated: 2020/01/21 17:15:18 by rostroh          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_nm

SRC = main.c error.c tools.c ft_nm.c

OBJ = $(SRC:.c=.o)

SRC_DIR = ./src
OBJ_DIR = ./obj
INC_DIR = ./include

OBJS = $(OBJ:%=$(OBJ_DIR)/%)

INC = nm.h

HEAD = $(INC_DIR)/$(INC)

LIBFT = libft.a
LIB_DIR = ./libft
LFT = $(LIB_DIR)/$(LIBFT)
LIB = -L $(LIB_DIR) -l$(LIBFT:lib%.a=%)

OBJ_PATH = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

FLG = -Wall -Werror -Wextra

CC = gcc

all:
	@make -C $(LIB_DIR)
	@make $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo mdr
	gcc $(FLG) -I $(INC_DIR) -o $@ -c -fPIC $<

$(OBJS): $(HEAD)

$(NAME): $(OBJS)
	$(CC) $(FLG) $(OBJS) -o $@ $(LIB)
	#$(CC) $(FLG) $(OBJS) $(LFT) -shared -o $@ $(LIB)
	#ln -fs $(NAME) libft_malloc.so

clean:
	@rm -rf $(OBJ_DIR)
	@make $@ -C $(LIB_DIR)

fclean: clean
	@rm -rf $(NAME)
	@make $@ -C $(LIB_DIR)

re: fclean all

print-%:
	@echo $*=$($*)
