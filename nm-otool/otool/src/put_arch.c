/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_arch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 16:50:17 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/11 16:51:01 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int		get_type(uint32_t arch)
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

char			*put_arch(char *name, uint32_t arch)
{
	int				idx;
	char			*str;
	static char		*arch_name[NB_CPU] = {"i386", "ppc"};

	if ((idx = get_type(arch)) != -1)
	{
		str = ft_strjoin(name, " (architecture ");
		str = ft_strjoin(str, arch_name[idx]);
		str = ft_strjoin(str, "):\n");
		return (str);
	}
	return (NULL);
}

