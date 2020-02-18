/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 17:07:26 by rostroh           #+#    #+#             */
/*   Updated: 2020/02/18 13:07:02 by rostroh          ###   ########.fr       */
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

void			cigam_64(t_file_inf file, int off)
{
	file.cig = 1;
	handle_64(file, off);
}

void			cigam_32(t_file_inf file, int off)
{
	file.cig = 1;
	handle_32(file, off);
}

static void		handle_arch(t_file_inf file)
{
	int			idx;
	int			offset;
	AR_HDR		hdr;
	HDR_64		mach_hdr;
	static void		(*func_dispenser[NB_MAGIC])(t_file_inf file, int off) = {&handle_32, &cigam_32, &handle_64, &cigam_64, NULL, NULL, NULL, NULL};

	offset = SARMAG;
	ft_memcpy(&hdr, file.content + offset, sizeof(AR_HDR));
//	printf("%x\n", ft_atoi(hdr.ar_size) + SARMAG + (int)sizeof(AR_HDR));
	offset += ft_atoi(hdr.ar_size) + (int)sizeof(AR_HDR);
	while (offset < file.inf.st_size)
	{
		printf("\n");
//		printf("offset debut : %x\n", offset);
		ft_memcpy(&hdr, file.content + offset, sizeof(AR_HDR));
//		printf("%d\n", ft_atoi(hdr.ar_name + 3));
//		printf("membre size = %x\n", ft_atoi(hdr.ar_size));
		if ((offset += (int)sizeof(AR_HDR)) > file.inf.st_size)
			return ;
//		printf("offset middle : %x\n", offset + ft_atoi(hdr.ar_size));
		printf("%s(%s):\n", file.name, file.content + offset);
		if ((offset += ft_atoi(hdr.ar_name + 3)) > file.inf.st_size)
			return ;
//		printf("offset fin : %x\n", offset + ft_atoi(hdr.ar_size));
		ft_memcpy(&mach_hdr, file.content + offset, sizeof(HDR_64));
		if ((idx = check_magic(mach_hdr.magic, file.name)) == -1)
			break ;
		func_dispenser[idx](file, offset);
		if ((offset += ft_atoi(hdr.ar_size) - ft_atoi(hdr.ar_name + 3)) > file.inf.st_size)
			break ;
//		printf("dernier offset : %x\n", offset);
	}
}

void			handle_fat32(t_file_inf file, int off)
{
	uint32_t	i;
	int			idx;
	uint32_t	magic;
	FAT_HDR		hdr;
	FAT_ARCH	arch;
	static void		(*func_dispenser[NB_MAGIC])(t_file_inf file, int off) = {&handle_32, &cigam_32, &handle_64, &cigam_64, NULL, NULL, NULL, NULL};

	i = 0;
	off = 0;
	read_header_fat(&hdr, file.content + off, sizeof(FAT_HDR), file.cig);
	off += sizeof(FAT_HDR);
	printf("Lol\n");
	while (i < hdr.nfat_arch)
	{
		read_arch(&arch, file.content + off, sizeof(FAT_ARCH), file.cig);
		put_arch(file.name, arch.cputype);
		file.cig = 0;
		ft_memcpy(&magic, file.content + arch.offset, sizeof(uint32_t));
		if ((idx = check_magic(magic, file.name)) < 0)
			return ;
		func_dispenser[idx](file, arch.offset);
		off += sizeof(FAT_ARCH);
		i++;
		file.cig = 1;
		if (i != hdr.nfat_arch)
			ft_putchar('\n');
	}
}

void			handle_fat64(t_file_inf file, int off)
{
	FAT_HDR		hdr;

	off = 0;
	file.cig = 0;
	printf("Lol\n");
	ft_memcpy(&hdr, file.content + off, sizeof(FAT_HDR));
}

void			cigam_fat32(t_file_inf file, int off)
{
	file.cig = 1;
	handle_fat32(file, off);
}

void			cigam_fat64(t_file_inf file, int off)
{
	file.cig = 1;
	off = 0;
	printf("Ceci est un fat_cigam64, bonne journee\n");
}

void			ft_nm(t_file_inf file, int print)
{
	int				idx;
	uint32_t		magic;
	static void		(*func_dispenser[NB_MAGIC])(t_file_inf file, int off) = {&handle_32, &cigam_32, &handle_64, &cigam_64, &handle_fat32, &cigam_fat32, &handle_fat64, &cigam_fat64};

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
	printf("%d\n", idx);
	func_dispenser[idx](file, 0);
}
