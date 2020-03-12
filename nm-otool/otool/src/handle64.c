/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:48:19 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/12 18:55:21 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int				sym_64(t_file_inf file, t_macho64 *inf)
{
	int					i;
	int					off;
	struct nlist_64		lst;

	i = 0;
	off = file.off_arch + inf->symtab.symoff;
	while (i < (int)inf->symtab.nsyms)
	{
		read_lst_64(&lst, file.content + off, sizeof(struct nlist_64), file);
		if ((off += sizeof(struct nlist_64)) > file.inf.st_size)
			return (ft_otool_put_error(file.name, NOT_VALID));
		if (file.off_arch + inf->symtab.stroff + lst.n_un.n_strx > \
				file.inf.st_size)
			return (ft_otool_put_error(file.name, NOT_VALID));
		i++;
	}
	return (0);
}

static int				pars_sct64(t_file_inf file, int offset, t_macho64 *inf)
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
		if (offset + sct.size > (uint64_t)file.inf.st_size && \
				ft_strcmp(sct.sectname, SECT_BSS) != 0)
			return (sect_err(file.name, i));
		if (ft_strcmp(sct.sectname, SECT_TEXT) == 0)
			inf->text = sct;
		i++;
		if ((offset += sizeof(struct section_64)) > file.inf.st_size)
			return (ft_otool_put_error(file.name, NOT_VALID));
	}
	return (0);
}

static int				pars_seg(t_file_inf file, int offset, t_macho64 *inf)
{
	if (inf->ld.cmd == LC_SEGMENT_64)
	{
		if (pars_sct64(file, offset, inf) == -1)
			return (-1);
	}
	else if (inf->ld.cmd == LC_SYMTAB)
	{
		read_symtab(&(inf->symtab), file.content + offset, \
				sizeof(struct symtab_command), file);
		if (sym_64(file, inf) == -1)
			return (-1);
	}
	return (0);
}

static int				pars_ld_cmd(t_file_inf file, int offset, t_macho64 *inf)
{
	int				i;

	i = 0;
	while (i < (int)inf->hdr.ncmds)
	{
		read_load_command(&inf->ld, file.content + offset, \
				sizeof(struct load_command), file);
		if (pars_seg(file, offset, inf) == -1)
			return (-1);
		if ((offset += inf->ld.cmdsize) > file.inf.st_size)
		{
			ft_otool_put_error(file.name, NOT_VALID);
			return (-1);
		}
		i++;
	}
	return (0);
}

int						handle_64(t_file_inf file, int offset)
{
	int				start_off;
	t_macho64		inf;

	start_off = offset;
	ft_bzero(&inf, sizeof(t_macho64));
	read_header_64(&inf.hdr, file.content + offset, \
			sizeof(struct mach_header_64), file);
	file.off_arch = offset;
	offset += sizeof(struct mach_header_64);
	if (pars_ld_cmd(file, offset, &inf) == -1)
		return (-1);
	if (file.arch == NULL && file.arch_idx != -1)
		ft_printf("%s:\n", file.name);
	hexa_out64(file, inf.text, start_off, file.cig);
	return (0);
}
