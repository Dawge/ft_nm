/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 17:07:26 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/10 11:33:36 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		check_magic(uint32_t magic, char *name)
{
	int					i;
	static uint32_t		magic_tab[NB_MAGIC] = {MH_MAGIC, MH_CIGAM, MH_MAGIC_64,\
		MH_CIGAM_64, FAT_MAGIC, FAT_CIGAM, FAT_MAGIC_64, FAT_CIGAM_64};

	i = 0;
	while (i < NB_MAGIC)
	{
		if (magic == magic_tab[i])
			return (i);
		i++;
	}
	ft_nm_put_error(name, NOT_VALID);
	return (-1);
}

int				dispens(t_file_inf file, uint32_t magic, int offset)
{
	int				idx;
	static int		(*func_dispenser[NB_MAGIC])(t_file_inf file, int off) = \
	{&handle_32, &cigam_32, &handle_64, &cigam_64, \
		&cigam_32, &cigam_64, &cigam_fat32, &cigam_fat64};

	if ((idx = check_magic(magic, file.name)) < 0)
		return (-1);
	return (func_dispenser[idx](file, offset));
}

static void		handle_arch(t_file_inf file)
{
	int							offset;
	struct ar_hdr				hdr;
	struct mach_header_64		mach_hdr;

	offset = SARMAG;
	ft_memcpy(&hdr, file.content + offset, sizeof(struct ar_hdr));
	offset += ft_atoi(hdr.ar_size) + (int)sizeof(struct ar_hdr);
	while (offset < file.inf.st_size)
	{
		printf("\n");
		ft_memcpy(&hdr, file.content + offset, sizeof(struct ar_hdr));
		if ((offset += (int)sizeof(struct ar_hdr)) > file.inf.st_size)
			return ;
		printf("%s(%s):\n", file.name, file.content + offset);
		if ((offset += ft_atoi(hdr.ar_name + 3)) > file.inf.st_size)
			return ;
		ft_memcpy(&mach_hdr, file.content + offset, \
				sizeof(struct mach_header_64));
		dispens(file, mach_hdr.magic, offset);
		if ((offset += ft_atoi(hdr.ar_size) - ft_atoi(hdr.ar_name + 3)) > \
				file.inf.st_size)
			break ;
	}
}

void			ft_nm(t_file_inf file, int print)
{
	int				idx;
	uint32_t		magic;
	static int		(*func_dispenser[NB_MAGIC])(t_file_inf file, int off) = \
	{&handle_32, &cigam_32, &handle_64, &cigam_64, \
		&handle_fat32, &cigam_fat32, &handle_fat64, &cigam_fat64};

	if (ft_strncmp(ARMAG, file.content, SARMAG) == 0)
	{
		handle_arch(file);
		return ;
	}
	ft_memcpy(&magic, file.content, sizeof(magic));
	if ((idx = check_magic(magic, file.name)) == -1)
		return ;
	if (idx <= 3 && print > 2)
		printf("%s:\n", file.name);
	file.arch = NULL;
	func_dispenser[idx](file, 0);
}
