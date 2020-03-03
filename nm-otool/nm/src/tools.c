/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:43:13 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/03 17:52:15 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include <errno.h>

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

int		ft_nm_put_error(char *name, char *error)
{
	ft_putstr("ft_nm: ");
	ft_putstr(name);
	ft_putstr(error);
	return (-1);
}

int		get_type(uint32_t arch)
{
	int					i;
	static uint32_t		arch_type[NB_CPU] = {CPU_TYPE_I386, CPU_TYPE_POWERPC};

	i = 0;
	while (i < NB_CPU)
	{
		if ((swap_u32(arch_type[i]) & CPU_ARCH_MASK) == arch)
			return (i);
		i++;
	}
	return (-1);
}

char	*put_arch(char *name, uint32_t arch)
{
	int				idx;
	char			*str;
	static char		*arch_name[NB_CPU] = {"i386", "ppc"};

	if ((idx = get_type(arch)) != -1)
	{
		str = ft_strjoin("\n", name);
		str = ft_strjoin(str, " (for architecture ");
		str = ft_strjoin(str, arch_name[idx]);
		str = ft_strjoin(str, "):\n");
		return (str);
	}
	return (NULL);
}
