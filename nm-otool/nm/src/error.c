/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:33:23 by rostroh           #+#    #+#             */
/*   Updated: 2020/01/18 19:00:26 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void			ft_open_error(char *name, int errno)
{
	int					i;
	static t_error		open_err[OPEN_ERROR] = {
		{2, ": No such file or directory.\n"}, \
		{13, ": Permission denied.\n"} \
	};

	i = 0;
	while (i < OPEN_ERROR)
	{
		if (open_err[i].err == errno)
		{
			ft_nm_put_error(name, open_err[i].message);
			break ;
		}
		i++;
	}
}

