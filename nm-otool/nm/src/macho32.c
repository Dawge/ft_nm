/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:07:56 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/05 15:42:01 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		sym_32(t_file_inf file, t_macho32 *inf)
{
	int			i;
	int			off;

	i = 0;
	off = file.off_arch + inf->symtab.symoff;
	if (!(inf->symbol = (t_list_inf_32 *)malloc(sizeof(t_list_inf_32) * \
					inf->symtab.nsyms)))
		return (ft_nm_put_error(file.name, NOT_VALID));
	while (i < (int)inf->symtab.nsyms)
	{
		read_lst_32(&inf->symbol[i].lst, file.content + off, sizeof(LST), file);
		if ((off += sizeof(LST)) > file.inf.st_size)
			return (ft_nm_put_error(file.name, NOT_VALID));
		if (file.off_arch + inf->symtab.stroff + \
				inf->symbol[i].lst.n_un.n_strx < file.inf.st_size)
			inf->symbol[i].str = file.content + file.off_arch + \
						inf->symtab.stroff + inf->symbol[i].lst.n_un.n_strx;
		else
			inf->symbol[i].str = "bad string index";
		inf->symbol[i].type = put_type32(inf->symbol[i], inf->sct_idx);
		inf->symbol[i].printed = 0;
		i++;
	}
	return (0);
}

static int		pars_seg(t_file_inf file, int offset, t_macho32 *inf)
{
	if (inf->ld.cmd == LC_SEGMENT)
	{
		if (pars_sct32(file, offset, inf) == -1)
			return (-1);
	}
	else if (inf->ld.cmd == LC_SYMTAB)
	{
		read_symtab(&(inf->symtab), file.content + offset, sizeof(SYM), file);
		if (sym_32(file, inf) == -1)
			return (-1);
	}
	return (0);
}

static int		pars_ld_cmd(t_file_inf file, int offset, t_macho32 inf)
{
	int				i;

	i = 0;
	while (i < (int)inf.hdr.ncmds)
	{
		read_load_command(&inf.ld, file.content + offset, sizeof(LD), file);
		if (pars_seg(file, offset, &inf) == -1)
			return (-1);
		if ((offset += inf.ld.cmdsize) > file.inf.st_size)
		{
			ft_nm_put_error(file.name, NOT_VALID);
			return (-1);
		}
		i++;
	}
	if (file.arch != NULL)
		printf("%s", file.arch);
	print_list32(inf.symbol, inf.symtab.nsyms, inf.sct_idx);
	return (0);
}

int				handle_32(t_file_inf file, int offset)
{
	t_macho32		inf;

	ft_bzero(&inf, sizeof(t_macho32));
	read_header_32(&inf.hdr, file.content + offset, sizeof(HDR), file);
	file.off_arch = offset;
	offset += sizeof(HDR);
	if (pars_ld_cmd(file, offset, inf) == -1)
		return (-1);
	return (0);
}
