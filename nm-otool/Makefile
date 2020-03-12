# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rostroh <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/03/12 18:34:51 by rostroh           #+#    #+#              #
#    Updated: 2020/03/12 18:48:49 by rostroh          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NM_DIR = ./nm/
OTOOL_DIR = ./otool/

all: ft_otool ft_nm

ft_nm:
	make -C $(NM_DIR)

ft_otool:
	make -C $(OTOOL_DIR)

clean:
	make $@ -C $(NM_DIR)
	make $@ -C $(OTOOL_DIR)

fclean:
	make $@ -C $(NM_DIR)
	make $@ -C $(OTOOL_DIR)

re: fclean all
