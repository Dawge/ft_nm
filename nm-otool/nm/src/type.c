/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 16:12:47 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/03 16:38:36 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static char		get_type(uint8_t n_type, uint8_t n_sect, int *tab)
{
	int				i;
	static char		tab_type[NB_SCT_INF] = {'t', 'd', 'b'};

	i = 0;
	if ((n_type & N_TYPE) == N_SECT)
	{
		while (i < NB_SCT_INF)
		{
			if (n_sect == tab[i])
				return (tab_type[i]);
			i++;
		}
		return ('s');
	}
	return (0x0);
}

char			put_type32(t_list_inf_32 sym, int *tab)
{
	char			type;

	type = get_type(sym.lst.n_type, sym.lst.n_sect, tab);
	if ((sym.lst.n_type & N_TYPE) == N_UNDF)
	{
		if (sym.lst.n_type == 0x00)
			type = '?';
		else if (sym.lst.n_value == 0)
			type = 'U';
		else
			type = 'c';
	}
	if (sym.lst.n_desc & N_WEAK_REF && type != 'U')
		type = 'w';
	else if ((sym.lst.n_type & N_TYPE) == N_ABS)
		type = 'A';
	else if ((sym.lst.n_type & N_TYPE) == N_INDR)
		type = 'I';
	else if (sym.lst.n_type & N_EXT && type != 'U' && type != 'A' && \
			type != 'I')
		type -= 32;
	return (type);
}

char			put_type64(t_list_inf sym, int *tab)
{
	char			type;

	type = get_type(sym.lst.n_type, sym.lst.n_sect, tab);
	if ((sym.lst.n_type & N_TYPE) == N_UNDF)
	{
		if (sym.lst.n_type == 0x00)
			type = '?';
		else if (sym.lst.n_value == 0)
			type = 'U';
		else
			type = 'c';
	}
	if (sym.lst.n_desc & N_WEAK_REF && type != 'U')
		type = 'w';
	else if ((sym.lst.n_type & N_TYPE) == N_ABS)
		type = 'A';
	else if ((sym.lst.n_type & N_TYPE) == N_INDR)
		type = 'I';
	else if (sym.lst.n_type & N_EXT && type != 'U' && type != 'A' && \
			type != 'I')
		type -= 32;
	return (type);
}
