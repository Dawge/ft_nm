/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 18:17:03 by rostroh           #+#    #+#             */
/*   Updated: 2020/01/18 17:48:02 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int			main(int ac, char **av)
{
	int			i;
	int			fd;
	int			errno;

	i = 1;
	while (i < ac)
	{
		if ((fd = open(av[i], O_RDONLY)) != -1)
			;//manage(fd);
		else
			ft_open_error(av[i], errno);
			//printf("errno = %d\n", errno);
		i++;
	}
	return (0);
}
