/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list64.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 17:58:26 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/03 18:09:46 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		find_addr(t_list_inf *sym, int sz)
{
	int			i;
	int			idx;

	i = 0;
	idx = 0;
	while (i < sz)
	{
		if (sym[i].lst.n_value < sym[idx].lst.n_value && sym[i].printed == 0)
			idx = i;
		if (sym[idx].printed == 1)
			idx = i;
		i++;
	}
	return (idx);
}

static int		find_alph(t_list_inf *sym, int sz)
{
	int			i;
	int			idx;

	i = 0;
	idx = 0;
	while (i < sz)
	{
		if (ft_strcmp(sym[i].str, "bad string index") == 0)
			return (find_addr(sym, sz));
		if (ft_strcmp(sym[i].str, sym[idx].str) == 0 && sym[i].printed == 0)
		{
			if (sym[idx].type == 'I' || sym[idx].printed == 1)
				idx = i;
		}
		else
		{
			if (ft_strcmp(sym[i].str, sym[idx].str) < 0 && sym[i].printed == 0)
				idx = i;
			else if (sym[idx].printed == 1)
				idx = i;
		}
		i++;
	}
	return (idx);
}

static void		print_sym(t_list_inf *sym, int idx, char type)
{
	if (type == 'I')
		printf("%18c %s (indirect for %s)\n", type, sym[idx].str, sym[idx].str);
	else if (sym[idx].lst.n_value == 0x0 && type != 'T' && type != 't' \
			&& type != 'A')
		printf("%18c %s\n", type, sym[idx].str);
	else
		printf("%016llx %c %s\n", sym[idx].lst.n_value, type, sym[idx].str);
}

void			print_list64(t_list_inf *sym, int sz, int *tab)
{
	int			i;
	int			idx;
	char		type;

	i = 0;
	while (i < sz)
	{
		idx = find_alph(sym, sz);
		sym[idx].printed = 1;
		type = put_type64(sym[idx], tab);
		if (type != 0x0 && sym[idx].str[0] != '\0' \
				&& sym[idx].lst.n_type != 0x20)
			print_sym(sym, idx, type);
		i++;
	}
}
