/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 15:36:41 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/11 15:25:04 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"
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
		ft_otool_put_error(file->name, NOT_VALID);
}

static int		check_dir(struct stat inf, char *name)
{
	if (S_ISDIR(inf.st_mode))
	{
		printf("./ft_nm: %s: Is a directory\n", name);
		return (-1);
	}
	return (0);
}

static void		file_gestion(char *file_name)
{
	t_file_inf	file;

	file.cig = 0;
	file.name = file_name;
	file.arch = NULL;
	if ((file.fd = open(file_name, O_RDONLY)) != -1)
	{
		fstat(file.fd, &file.inf);
		if (check_dir(file.inf, file.name) != -1)
		{
			get_content(&file);
			ft_otool(file);
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
		printf("Error ./ft_otool: At least one file must be specified\n");
	while (i < ac)
	{
		file_gestion(av[i]);
		i++;
	}
	return (0);
}
