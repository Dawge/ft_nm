/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:48:19 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/11 17:04:27 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int				hexaout(t_file_inf file, struct section_64 sct, \
		int off, int cig)
{
	uint32_t		i;

	i = 0;
	if (file.arch != NULL)
		printf("%s", file.arch);
	printf("Contents of (%s,%s) section", sct.segname, sct.sectname);
	while (i < sct.size)
	{
		if (cig == 0)
			print_data64(file.content, i, sct.addr, sct.offset + off);
		else
			print_data64(file.content, i, swap_u32(sct.addr), \
					swap_u32(sct.offset) + off);
		i++;
		if (cig == 0)
			printf(" ");
		else
		{
			if (i % 4 == 0)
				printf(" ");
		}
	}
	printf("\n");
	return (0);
}

static int				pars_sct64(t_file_inf file, int offset, int start_off)
{
	uint32_t						i;
	struct segment_command_64		sgm;
	struct section_64				sct;

	i = 0;
	read_seg_64(&sgm, file.content + offset, sizeof(struct segment_command_64),\
			file);
	if (offset + sgm.cmdsize > file.inf.st_size)
		return (ft_otool_put_error(file.name, NOT_VALID));
	if ((offset += sizeof(struct segment_command_64)) > file.inf.st_size)
		return (ft_otool_put_error(file.name, NOT_VALID));
	while (i < sgm.nsects)
	{
		read_sct_64(&sct, file.content + offset, sizeof(sct), file);
		if (offset + sct.size > (uint64_t)file.inf.st_size)
			return (sect_err(file.name, i));
		if (ft_strcmp(sct.sectname, SECT_TEXT) == 0)
			return (hexaout(file, sct, start_off, file.cig));
		i++;
		if ((offset += sizeof(struct section_64)) > file.inf.st_size)
			return (ft_otool_put_error(file.name, NOT_VALID));
	}
	return (0);
}

static int				pars_seg(t_file_inf file, int offset, t_macho64 *inf, \
		int start_off)
{
	if (inf->ld.cmd == LC_SEGMENT_64)
	{
		if (pars_sct64(file, offset, start_off) == -1)
			return (-1);
	}
	return (0);
}

static int				pars_ld_cmd(t_file_inf file, int offset, t_macho64 inf,\
		int start_off)
{
	int				i;

	i = 0;
	while (i < (int)inf.hdr.ncmds)
	{
		read_load_command(&inf.ld, file.content + offset, \
				sizeof(struct load_command), file);
		if (pars_seg(file, offset, &inf, start_off) == -1)
			return (-1);
		if ((offset += inf.ld.cmdsize) > file.inf.st_size)
		{
			ft_otool_put_error(file.name, NOT_VALID);
			return (-1);
		}
		i++;
	}
	return (0);
}

int				handle_64(t_file_inf file, int offset)
{
	int				start_off;
	t_macho64		inf;

	start_off = offset;
	if (file.arch == NULL && file.arch_idx != -1)
		printf("%s:\n", file.name);
	ft_bzero(&inf, sizeof(t_macho64));
	read_header_64(&inf.hdr, file.content + offset, \
			sizeof(struct mach_header_64), file);
	file.off_arch = offset;
	offset += sizeof(struct mach_header_64);
	if (pars_ld_cmd(file, offset, inf, start_off) == -1)
		return (-1);
	return (0);
}
