/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 17:07:26 by rostroh           #+#    #+#             */
/*   Updated: 2020/02/26 21:31:25 by rostroh          ###   ########.fr       */
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

int				cigam_64(t_file_inf file, int off)
{
	file.cig = 1;
	return (handle_64(file, off));
}

int				cigam_32(t_file_inf file, int off)
{
	file.cig = 1;
	return (handle_32(file, off));
}

static void		handle_arch(t_file_inf file)
{
	int			idx;
	int			offset;
	AR_HDR		hdr;
	HDR_64		mach_hdr;
	static int		(*func_dispenser[NB_MAGIC])(t_file_inf file, int off) = {&handle_32, &cigam_32, &handle_64, &cigam_64, NULL, NULL, NULL, NULL};

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

static int		check_arch(t_file_inf file, uint32_t cputype, uint32_t fat_idx)
{
	uint32_t	i;
	int			off;
	FAT_HDR		hdr;
	FAT_ARCH	arch;

	i = 0;
	read_header_fat(&hdr, file.content, sizeof(FAT_HDR), file.cig);
	off = sizeof(FAT_HDR);
	//if ((cputype & swap_u32(CPU_ARCH_ABI64)) == swap_u32(CPU_ARCH_ABI64))
	//	return (1);
	while (i < hdr.nfat_arch)
	{
		read_arch(&arch, file.content + off, sizeof(FAT_ARCH), file.cig);
		if ((cputype & CPU_ARCH_MASK) == (arch.cputype & CPU_ARCH_MASK) && (cputype & swap_u32(CPU_ARCH_ABI64)) == 0x0 && (arch.cputype & swap_u32(CPU_ARCH_ABI64)) == swap_u32(CPU_ARCH_ABI64))
			return (0);
		if (i < fat_idx && cputype == (uint32_t)arch.cputype)
			return (0);
		off += sizeof(FAT_ARCH);
		i++;
	}
	return (1);
}

int				handle_fat32(t_file_inf file, int off)
{
	int			print;
	uint32_t	i;
	int			idx;
	uint32_t	magic;
	FAT_HDR		hdr;
	FAT_ARCH	arch;
	static int		(*func_dispenser[NB_MAGIC])(t_file_inf file, int off) = {&handle_32, &cigam_32, &handle_64, &cigam_64, NULL, NULL, NULL, NULL};

	i = 0;
	off = 0;
	print = 0;
	read_header_fat(&hdr, file.content + off, sizeof(FAT_HDR), file.cig);
	off += sizeof(FAT_HDR);
	while (i < hdr.nfat_arch)
	{
		read_arch(&arch, file.content + off, sizeof(FAT_ARCH), file.cig);
		if (check_arch(file, arch.cputype, i) == 1)
		{
			if (i + 1 < hdr.nfat_arch || print != 0)
				file.arch = put_arch(file.name, arch.cputype);
			file.cig = 0;
			ft_memcpy(&magic, file.content + arch.offset, sizeof(uint32_t));
			if ((idx = check_magic(magic, file.name)) < 0)
				return (-1);
			if (func_dispenser[idx](file, arch.offset) == -1)
				return (-1);
			file.cig = 1;
			print++;
		}
		off += sizeof(FAT_ARCH);
		i++;
	}
	return (0);
}

int			handle_fat64(t_file_inf file, int off)
{
	FAT_HDR		hdr;

	off = 0;
	file.cig = 0;
	ft_memcpy(&hdr, file.content + off, sizeof(FAT_HDR));
	return (0);
}

int			cigam_fat32(t_file_inf file, int off)
{
	file.cig = 1;
	return (handle_fat32(file, off));
}

int			cigam_fat64(t_file_inf file, int off)
{
	file.cig = 1;
	return (handle_fat64(file, off));
}

void			ft_nm(t_file_inf file, int print)
{
	int				idx;
	uint32_t		magic;
	static int		(*func_dispenser[NB_MAGIC])(t_file_inf file, int off) = {&handle_32, &cigam_32, &handle_64, &cigam_64, &handle_fat32, &cigam_fat32, &handle_fat64, &cigam_fat64};

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
