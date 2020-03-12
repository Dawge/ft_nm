/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 18:17:03 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/12 18:50:46 by rostroh          ###   ########.fr       */
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

static int		check_dir(struct stat inf, char *name)
{
	if (S_ISDIR(inf.st_mode))
	{
		ft_printf("./ft_nm: %s: Is a directory\n", name);
		return (-1);
	}
	return (0);
}

static void		file_gestion(char *file_name, int nb_arg)
{
	t_file_inf	file;

	file.cig = 0;
	file.name = file_name;
	if ((file.fd = open(file_name, O_RDONLY)) != -1)
	{
		fstat(file.fd, &file.inf);
		if (check_dir(file.inf, file.name) != -1)
		{
			get_content(&file);
			ft_nm(file, nb_arg);
		}
	}
	else
		ft_open_error(file_name, errno);
}

int				main(int ac, char **av)
{
	int			i;

	i = 1;
	if (ac == 1)
		file_gestion("a.out", 1);
	while (i < ac)
	{
		if (ac > 2)
			ft_putchar('\n');
		file_gestion(av[i], ac);
		i++;
	}
	return (0);
}
