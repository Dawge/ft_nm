/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:48:19 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/10 19:11:06 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int				hexaout(t_file_inf file, struct section sct)
{
	uint32_t		i;

	i = 0;
	printf("Contents of (%s,%s) section", sct.segname, sct.sectname);
	while (i < sct.size)
	{
		if (i % 16 == 0)
			printf("\n%08x\t ", sct.addr+ i);
		printf("%02x ", (unsigned char)*(file.content + sct.offset + i));
		i++;
	}
	printf("\n");
	return (0);
}

static int				pars_sct32(t_file_inf file, int offset)
{
	uint32_t						i;
	struct segment_command			sgm;
	struct section					sct;

	i = 0;
	read_seg_32(&sgm, file.content + offset, sizeof(struct segment_command), \
			file);
	if (offset + sgm.cmdsize > file.inf.st_size)
		return (ft_otool_put_error(file.name, NOT_VALID));
	if ((offset += sizeof(struct segment_command)) > file.inf.st_size)
		return (ft_otool_put_error(file.name, NOT_VALID));
	while (i < sgm.nsects)
	{
		read_sct_32(&sct, file.content + offset, sizeof(struct section), file);
		if (ft_strcmp(sct.sectname, SECT_TEXT) == 0)
			return (hexaout(file, sct));
		if (offset + sct.size > file.inf.st_size)
			return (sect_err(file.name, i));
		i++;
		if ((offset += sizeof(struct section)) > file.inf.st_size)
			return (ft_otool_put_error(file.name, NOT_VALID));
	}
	return (0);
}

static int				pars_seg(t_file_inf file, int offset, t_macho32 *inf)
{

	if (inf->ld.cmd == LC_SEGMENT)
	{
		if (pars_sct32(file, offset) == -1)
			return (-1);
	}
	return (0);
}

static int				pars_ld_cmd(t_file_inf file, int offset, t_macho32 inf)
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

int				handle_32(t_file_inf file, int offset)
{
	t_macho32		inf;

	ft_bzero(&inf, sizeof(t_macho32));
	read_header_32(&inf.hdr, file.content + offset, \
			sizeof(struct mach_header), file);
	file.off_arch = offset;
	offset += sizeof(struct mach_header);
	if (pars_ld_cmd(file, offset, inf) == -1)
		return (-1);
	return (0);
}