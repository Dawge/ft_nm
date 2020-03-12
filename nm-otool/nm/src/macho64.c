/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:07:56 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/12 14:34:54 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		sym_64(t_file_inf file, t_macho64 *inf)
{
	int			i;
	int			off;

	i = 0;
	off = file.off_arch + inf->symtab.symoff;
	if (!(inf->symbol = (t_list_inf *)malloc(sizeof(t_list_inf) * \
					inf->symtab.nsyms)))
		return (ft_nm_put_error(file.name, NOT_VALID));
	while (i < (int)inf->symtab.nsyms)
	{
		read_lst_64(&inf->symbol[i].lst, file.content + off, \
				sizeof(struct nlist_64), file);
		if ((off += sizeof(struct nlist_64)) > file.inf.st_size)
			return (ft_nm_put_error(file.name, NOT_VALID));
		if (file.off_arch + inf->symtab.stroff + \
				inf->symbol[i].lst.n_un.n_strx < file.inf.st_size)
			inf->symbol[i].str = file.content + file.off_arch + \
						inf->symtab.stroff + inf->symbol[i].lst.n_un.n_strx;
		else
			inf->symbol[i].str = "bad string index";
		inf->symbol[i].type = put_type64(inf->symbol[i], inf->sct_idx);
		inf->symbol[i].printed = 0;
		i++;
	}
	return (0);
}

static int		pars_seg(t_file_inf file, int offset, t_macho64 *inf)
{
	if (inf->ld.cmd == LC_SEGMENT_64)
	{
		if (pars_sct64(file, offset, inf) == -1)
		{
			printf("sct error\n");
			return (-1);
		}
	}
	else if (inf->ld.cmd == LC_SYMTAB)
	{
		read_symtab(&(inf->symtab), file.content + offset, \
				sizeof(struct symtab_command), file);
		if (sym_64(file, inf) == -1)
		{
			printf("symtab error\n");
			return (-1);
		}
	}
	return (0);
}

static int		pars_ld_cmd(t_file_inf file, int offset, t_macho64 inf)
{
	int				i;

	i = 0;
	while (i < (int)inf.hdr.ncmds)
	{
		read_load_command(&inf.ld, file.content + offset, \
				sizeof(struct load_command), file);
		if (pars_seg(file, offset, &inf) == -1)
		{
			return (-1);
		}
		if ((offset += inf.ld.cmdsize) > file.inf.st_size)
		{
			ft_nm_put_error(file.name, NOT_VALID);
			return (-1);
		}
		i++;
	}
	if (file.arch != NULL)
		printf("%s", file.arch);
	print_list64(inf.symbol, inf.symtab.nsyms, inf.sct_idx);
	return (0);
}

int				handle_64(t_file_inf file, int offset)
{
	t_macho64		inf;

	ft_bzero(&inf, sizeof(t_macho64));
	read_header_64(&inf.hdr, file.content + offset, \
			sizeof(struct mach_header_64), file);
	file.off_arch = offset;
	offset += sizeof(struct mach_header_64);
	pars_ld_cmd(file, offset, inf);
	return (0);
}
