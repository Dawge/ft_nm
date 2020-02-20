/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 18:17:03 by rostroh           #+#    #+#             */
/*   Updated: 2020/02/20 14:00:44 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include <errno.h>

#define BUF_SIZE 256

static void		get_content(t_file_inf *file)
{
	int			size;
	char		buf[BUF_SIZE];

	if (!(file->content = (char*)malloc(sizeof(char) * (file->inf.st_size))))
		return ;
	size = read(file->fd, file->content, file->inf.st_size);
	if (size < file->inf.st_size || read(file->fd, &buf, BUF_SIZE) > 0)
		ft_nm_put_error(file->name, NOT_VALID);
}

int				main(int ac, char **av)
{
	int			i;
	t_file_inf	file;

	i = 1;
	while (i < ac)
	{
		if (ac > 2)
			ft_putchar('\n');
		file.cig = 0;
		file.name = av[i];
		if ((file.fd = open(av[i], O_RDONLY)) != -1)
		{
			fstat(file.fd, &file.inf);
			get_content(&file);
			ft_nm(file, ac);
		}
		else
			ft_open_error(av[i], errno);
		i++;
	}
	return (0);
}
