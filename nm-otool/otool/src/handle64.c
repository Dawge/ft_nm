/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:48:19 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/10 20:23:13 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int				hexaout(t_file_inf file, struct section_64 sct, int off)
{
	uint32_t		i;

	i = 0;
	printf("off = %x\n", off + sct.offset);
	printf("Contents of (%s,%s) section", sct.segname, sct.sectname);
	while (i < sct.size)
	{
		if (i % 16 == 0)
			printf("\n%016llx\t ", sct.addr + i);
		printf("%02x ", (unsigned char)*(file.content + sct.offset + off + i));
		i++;
	}
	printf("\n");
	return (0);
}

static int				pars_sct64(t_file_inf file, int offset)
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
		printf("text off = %x\n", sct.offset);
		if (ft_strcmp(sct.sectname, SECT_TEXT) == 0)
			return (hexaout(file, sct, offset));
		if (offset + sct.size > (uint64_t)file.inf.st_size)
			return (sect_err(file.name, i));
		i++;
		if ((offset += sizeof(struct section_64)) > file.inf.st_size)
			return (ft_otool_put_error(file.name, NOT_VALID));
	}
	return (0);
}

static int				pars_seg(t_file_inf file, int offset, t_macho64 *inf)
{
	struct segment_command_64	sgm;

	if (inf->ld.cmd == LC_SEGMENT_64)
	{
		read_seg_64(&sgm, file.content + offset, sizeof(sgm), file);
		if (pars_sct64(file, offset) == -1)
			return (-1);
	}
	return (0);
}

static int				pars_ld_cmd(t_file_inf file, int offset, t_macho64 inf)
{
	int				i;

	i = 0;
	while (i < (int)inf.hdr.ncmds)
	{
		read_load_command(&inf.ld, file.content + offset, \
				sizeof(struct load_command), file);
		if (pars_seg(file, offset, &inf) == -1)
			return (-1);
		if ((offset += inf.ld.cmdsize) > file.inf.st_size)
		{
			ft_otool_put_error(file.name, NOT_VALID);
			return (-1);
		}
		i++;
	}
	if (file.arch != NULL)
		printf("%s", file.arch);
	return (0);
}

int				handle_64(t_file_inf file, int offset)
{
	t_macho64		inf;

	printf("Offset - %x\n", offset);
	ft_bzero(&inf, sizeof(t_macho64));
	read_header_64(&inf.hdr, file.content + offset, \
			sizeof(struct mach_header_64), file);
	file.off_arch = offset;
	offset += sizeof(struct mach_header_64);
	if (pars_ld_cmd(file, offset, inf) == -1)
		return (-1);
	return (0);
}
