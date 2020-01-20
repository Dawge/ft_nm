/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:43:13 by rostroh           #+#    #+#             */
/*   Updated: 2020/01/20 14:25:42 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int			ft_nm_read(t_file inf, void *buf, size_t nbytes)
{
	int			sz;

	if ((sz = read(inf.fd, buf, nbytes)) == -1)
		return (-1);//printf("%d\n", errno);
	if ((uint64_t)sz < nbytes)
	{
		ft_nm_put_error(inf.name, NOT_VALID);
		return (-1);
	}
	return (0);
}

void		ft_nm_put_error(char *name, char *error)
{
	ft_putstr("ft_nm: ");
	ft_putstr(name);
	ft_putstr(error);
}
