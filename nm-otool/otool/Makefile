# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rostroh <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/17 11:17:25 by rostroh           #+#    #+#              #
#    Updated: 2020/03/12 18:46:04 by rostroh          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ../ft_otool

SRC = main.c tools.c ft_otool.c read_struct.c read_struct_32.c read_struct_64.c\
	  read_struct_fat.c swap.c handle32.c handle64.c cigam.c fat32.c put_arch.c\
	  hexa_out.c fat64.c

OBJ = $(SRC:.c=.o)

SRC_DIR = ./src
OBJ_DIR = ./obj
INC_DIR = ../include

OBJS = $(OBJ:%=$(OBJ_DIR)/%)

INC = ft_otool.h

HEAD = $(INC_DIR)/$(INC)

LIBFT = libft.a
LIB_DIR = ../libft
LFT = $(LIB_DIR)/$(LIBFT)
LIB = -L $(LIB_DIR) -l$(LIBFT:lib%.a=%)

OBJ_PATH = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

FLG = -Wall -Werror -Wextra -fsanitize=address -g3

CC = gcc

all:
	@make -C $(LIB_DIR)
	@make $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(FLG) -I $(INC_DIR) -o $@ -c -fPIC $<

$(OBJS): $(HEAD)

$(NAME): $(OBJS)
	$(CC) $(FLG) $(OBJS) -o $@ $(LIB)

clean:
	@rm -rf $(OBJ_DIR)
	@make $@ -C $(LIB_DIR)

fclean: clean
	@rm -rf $(NAME)
	@make $@ -C $(LIB_DIR)

re: fclean all

print-%:
	@echo $*=$($*)
