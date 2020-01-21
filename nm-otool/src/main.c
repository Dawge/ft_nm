/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 18:17:03 by rostroh           #+#    #+#             */
/*   Updated: 2020/01/20 16:34:44 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include <errno.h>

static int		check_magic(uint32_t magic, char *name)
{
	int					i;
	static uint32_t		magic_tab[NB_MAGIC] = {MH_MAGIC, MH_CIGAM, MH_MAGIC_64,\
		MH_CIGAM_64, FAT_MAGIC, FAT_CIGAM, FAT_MAGIC_64, FAT_CIGAM_64};

	i = 0;
	while (i < NB_MAGIC)
	{
		if (magic == magic_tab[i])
			return (0);
		i++;
	}
	ft_nm_put_error(name, NOT_VALID);
	return (-1);
}

static void		manage(t_file inf)
{
	int			i;
	uint32_t	padding[4];
	char		*pad;
	int			tmp;
	t_macho64	file;

	ft_bzero(&padding, sizeof(uint32_t) * 4);
	if (ft_nm_read(inf, &file.hdr, sizeof(MACH_HDR)) == -1)
		return ;
	if (check_magic(file.hdr.magic, inf.name) == -1)
		return ;
	i = 0;
	printf("size of all ldcmd = %d\n", file.hdr.sizeofcmds);
	printf("nb cmd = %d\n", file.hdr.ncmds);
	pad = (char *)malloc(sizeof(char) * 4);
	if (ft_nm_read(inf, pad, sizeof(uint32_t)) == -1)
		return ;
	if (!(file.sgm = (SGM_CMD*)malloc(file.hdr.ncmds * sizeof(SGM_CMD))))
		return ;
	ft_bzero(file.sgm, file.hdr.ncmds * sizeof(SGM_CMD));
	printf("size = 0x%lx\n", sizeof(SGM_CMD));
	while (i < (int)file.hdr.ncmds)
	{
		if (ft_nm_read(inf, &(file.sgm[i]), sizeof(SGM_CMD)) == -1)
			return ;

		tmp = file.sgm[i].cmdsize - sizeof(SGM_CMD);
		pad = (char *)malloc(sizeof(char) * tmp);

		printf("0x%x\n", file.sgm[i].cmdsize - (int)sizeof(SGM_CMD));
		printf("pour i = %d segment : %s\ncmd_size = 0x%x\nnb sect : 0x%x\n", i, file.sgm[i].segname, file.sgm[i].cmdsize, file.sgm[i].nsects);
		printf("offset : 0x%x\n", file.sgm[i].fileoff);
		if (file.sgm[i].cmd != LC_SEGMENT_64 && file.sgm[i].cmd != LC_SEGMENT)
		{
			ft_nm_put_error(inf.name, NOT_VALID);
			return ;
		}
		if (ft_nm_read(inf, pad, tmp) == -1)
			return ;
		ft_putchar('\n');
		i++;
	}
}

int				main(int ac, char **av)
{
	int			i;
	t_file		inf;

	i = 1;
	while (i < ac)
	{
		inf.name = av[i];
		if ((inf.fd = open(av[i], O_RDONLY)) != -1)
			manage(inf);
		else
			ft_open_error(av[i], errno);
		i++;
	}
	return (0);
}
