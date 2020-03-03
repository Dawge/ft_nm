/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:07:56 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/03 18:10:32 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		sym_64(t_macho64 *inf, t_file_inf file, int offset, int *tab)
{
	int			i;
	int			off;

	i = 0;
	off = offset + inf->symtab.symoff;
	if (!(inf->symbol = (t_list_inf *)malloc(sizeof(t_list_inf) * \
					inf->symtab.nsyms)))
		return (ft_nm_put_error(file.name, NOT_VALID));
	while (i < (int)inf->symtab.nsyms)
	{
		read_lst_64(&inf->symbol[i].lst, file.content + off, sizeof(LST_64), \
				file);
		if ((off += sizeof(LST_64)) > file.inf.st_size)
			return (ft_nm_put_error(file.name, NOT_VALID));
		if (offset + inf->symtab.stroff + inf->symbol[i].lst.n_un.n_strx < \
				file.inf.st_size)
			inf->symbol[i].str = file.content + offset + inf->symtab.stroff + \
								inf->symbol[i].lst.n_un.n_strx;
		else
			inf->symbol[i].str = "bad string index";
		inf->symbol[i].type = put_type64(inf->symbol[i], tab);
		inf->symbol[i].printed = 0;
		i++;
	}
	return (0);
}

int				handle_64(t_file_inf file, int offset)
{
	int				off;
	int				i;
	int				i_sct;
	t_macho64		inf;
	int				sect_idx[3];

	i = 0;
	i_sct = 0;
	ft_bzero(&sect_idx, sizeof(int) * 3);
	ft_bzero(&inf, sizeof(t_macho64));
	read_header_64(&inf.hdr, file.content + offset, sizeof(HDR_64), file);
	off = offset;
	offset += sizeof(HDR_64);
	while (i < (int)inf.hdr.ncmds)
	{
		read_load_command(&inf.ld, file.content + offset, sizeof(LD), file);
		if (inf.ld.cmd == LC_SEGMENT_64)
		{
			if (pars_sct64(file, offset, (int *)&sect_idx, &i_sct) == -1)
				return (-1);
		}
		else if (inf.ld.cmd == LC_SYMTAB)
		{
			read_symtab(&inf.symtab, file.content + offset, sizeof(SYM), file);
			if (sym_64(&inf, file, off, (int *)&sect_idx) == -1)
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
	print_list64(inf.symbol, inf.symtab.nsyms, (int*)&sect_idx);
	return (0);
}
