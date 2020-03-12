/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 16:47:24 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/12 16:35:46 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int			check_arch32(t_file_inf file, uint32_t cputype, \
		uint32_t fat_idx)
{
	uint32_t				i;
	int						off;
	struct fat_header		hdr;
	struct fat_arch			arch;

	i = 0;
	read_header_fat(&hdr, file.content, sizeof(struct fat_header), file.cig);
	off = sizeof(struct fat_header);
	while (i < hdr.nfat_arch)
	{
		read_arch32(&arch, file.content + off, sizeof(struct fat_arch), \
				file.cig);
		if ((cputype & CPU_ARCH_MASK) == (arch.cputype & CPU_ARCH_MASK) && \
				(cputype & swap_u32(CPU_ARCH_ABI64)) == 0x0 && \
		(arch.cputype & swap_u32(CPU_ARCH_ABI64)) == swap_u32(CPU_ARCH_ABI64))
			return (0);
		if (i < fat_idx && cputype == (uint32_t)arch.cputype)
			return (0);
		off += sizeof(struct fat_arch);
		i++;
	}
	return (1);
}

static int			setup32(t_file_inf file, struct fat_header hdr, \
		struct fat_arch arch, int print)
{
	uint32_t	magic;

	if (check_arch32(file, arch.cputype, file.arch_idx) == 1)
	{
		if ((uint32_t)(file.arch_idx + 1) < hdr.nfat_arch || print != 0)
			file.arch = put_arch(file.name, arch.cputype);
		file.cig = 0;
		ft_memcpy(&magic, file.content + arch.offset, sizeof(uint32_t));
		if (dispens(file, magic, arch.offset) == -1)
			return (-1);
		file.cig = 1;
		print++;
	}
	return (print);
}

int					handle_fat32(t_file_inf file, int off)
{
	int						print;
	struct fat_header		hdr;
	struct fat_arch			arch;

	off = 0;
	print = 0;
	file.arch_idx = 0;
	read_header_fat(&hdr, file.content + off, sizeof(struct fat_header), \
			file.cig);
	off += sizeof(struct fat_header);
	while ((uint32_t)file.arch_idx < hdr.nfat_arch)
	{
		read_arch32(&arch, file.content + off, sizeof(struct fat_arch), \
				file.cig);
		if ((print = setup32(file, hdr, arch, print)) == -1)
			return (-1);
		off += sizeof(struct fat_arch);
		file.arch_idx++;
	}
	return (0);
}
