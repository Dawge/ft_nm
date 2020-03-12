/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 14:36:03 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/10 11:40:36 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		setup32(t_file_inf file, struct fat_header hdr, \
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

int				handle_fat32(t_file_inf file, int off)
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

static int		setup64(t_file_inf file, struct fat_header hdr, \
		struct fat_arch_64 arch, int print)
{
	uint32_t	magic;

	if (check_arch64(file, arch.cputype, file.arch_idx) == 1)
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

int				handle_fat64(t_file_inf file, int off)
{
	int						print;
	struct fat_header		hdr;
	struct fat_arch_64		arch;

	off = 0;
	print = 0;
	file.arch_idx = 0;
	read_header_fat(&hdr, file.content + off, sizeof(struct fat_header), \
			file.cig);
	off += sizeof(struct fat_header);
	while ((uint32_t)file.arch_idx < hdr.nfat_arch)
	{
		read_arch64(&arch, file.content + off, sizeof(struct fat_arch), \
				file.cig);
		if ((print = setup64(file, hdr, arch, print)) == -1)
			return (-1);
		off += sizeof(struct fat_arch);
		file.arch_idx++;
	}
	return (0);
}
