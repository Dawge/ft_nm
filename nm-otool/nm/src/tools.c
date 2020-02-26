/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:43:13 by rostroh           #+#    #+#             */
/*   Updated: 2020/02/26 21:34:28 by rostroh          ###   ########.fr       */
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
	//	printf("\n%s (for architecture %s):\n", name, arch_name[idx]);
}
/*
void	put_arch(char *name, uint32_t arch)
{
	printf("leaf = 0x%x\n", swap_u32(CPU_TYPE_I386));
	if (arch == PPC)
		printf("ppc):\n");
	else if (arch == i386)
		printf("i386):\n");
	else
		printf("0x%x):\n", arch);
}*/
