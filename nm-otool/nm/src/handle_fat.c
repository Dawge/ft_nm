/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 14:36:03 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/03 14:48:24 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		check_arch(t_file_inf file, uint32_t cputype, uint32_t fat_idx)
{
	uint32_t	i;
	int			off;
	FAT_HDR		hdr;
	FAT_ARCH	arch;

	i = 0;
	read_header_fat(&hdr, file.content, sizeof(FAT_HDR), file.cig);
	off = sizeof(FAT_HDR);
	while (i < hdr.nfat_arch)
	{
		read_arch(&arch, file.content + off, sizeof(FAT_ARCH), file.cig);
		if ((cputype & CPU_ARCH_MASK) == (arch.cputype & CPU_ARCH_MASK) && \
				(cputype & swap_u32(CPU_ARCH_ABI64)) == 0x0 && \
		(arch.cputype & swap_u32(CPU_ARCH_ABI64)) == swap_u32(CPU_ARCH_ABI64))
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
	uint32_t	magic;
	FAT_HDR		hdr;
	FAT_ARCH	arch;

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
			if (dispens(file, magic, arch.offset) == -1)
				return (-1);
			file.cig = 1;
			print++;
		}
		off += sizeof(FAT_ARCH);
		i++;
	}
	return (0);
}

int				handle_fat64(t_file_inf file, int off)
{
	FAT_HDR		hdr;

	off = 0;
	file.cig = 0;
	ft_memcpy(&hdr, file.content + off, sizeof(FAT_HDR));
	return (0);
}
