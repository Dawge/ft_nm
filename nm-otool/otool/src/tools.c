/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 15:57:09 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/10 18:18:29 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int		ft_otool_put_error(char *name, char *error)
{
	ft_putstr("ft_nm: ");
	ft_putstr(name);
	ft_putstr(error);
	return (-1);
}

void			ft_open_error(char *name, int errno)
{
	int					i;
	static t_error		open_err[OPEN_ERR] = {
		{2, ": No such file or directory.\n"}, \
		{13, ": Permission denied.\n"} \
	};

	i = 0;
	while (i < OPEN_ERR)
	{
		if (open_err[i].err == errno)
		{
			ft_otool_put_error(name, open_err[i].message);
			break ;
		}
		i++;
	}
}

int		sect_err(char *name, int sect)
{
	ft_putstr("ft_nm: ");
	ft_putstr(name);
	ft_putstr(TRUNC_ERR);
	ft_putstr("(offset field plus size field of section ");
	ft_putnbr(sect);
	ft_putstr(" in LC_SEGMENT command 1 extends past the end of the file)\n");
	return (-1);
}
