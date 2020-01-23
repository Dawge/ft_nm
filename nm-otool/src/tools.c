/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:43:13 by rostroh           #+#    #+#             */
/*   Updated: 2020/01/21 17:06:42 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include <errno.h>
/*
int			ft_nm_read(t_file inf, void *buf, size_t nbytes)
{
	int			sz;

	if ((sz = read(inf.fd, buf, nbytes)) == -1)
	{
		printf("%d\n", errno);
		return (-1);
	}
	if ((uint64_t)sz < nbytes)
	{
		ft_nm_put_error(inf.name, NOT_VALID);
		return (-1);
	}
	return (0);
}*/

void		ft_nm_put_error(char *name, char *error)
{
	ft_putstr("ft_nm: ");
	ft_putstr(name);
	ft_putstr(error);
}
