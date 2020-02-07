/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:07:56 by rostroh           #+#    #+#             */
/*   Updated: 2020/02/07 14:19:25 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void		sort_nlist(t_list_inf *symbol, int sz)
{
	int				i;
	t_list_inf		tmp;

	i = 0;
	while (i < sz - 1)
	{
		if (ft_strcmp(symbol[i].str, symbol[i + 1].str) > 0)
		{
			tmp = symbol[i];
			symbol[i] = symbol[i + 1];
			symbol[i + 1] = tmp;
			i = 0;
		}
		else
			i++;
	}
}

static char		put_type(t_list_inf *sym, int idx, int *tab)
{
	int				i;
	char			type;
	static char		tab_type[NB_SCT_INF] = {'t', 'd', 'b'};

	i = 0;
	type = 0x0;
	if ((sym[idx].lst.n_type & N_TYPE) == N_SECT)
	{
		type = 's';
		while (i < NB_SCT_INF)
		{
			if (sym[idx].lst.n_sect == tab[i])
				type = tab_type[i];
			i++;
		}
	}
	if ((sym[idx].lst.n_type & N_TYPE) == N_UNDF)
	{
		if (sym[idx].lst.n_value == 0)
			type = 'U';
		else
			type = 'c';
	}
	if (sym[idx].lst.n_desc & N_WEAK_REF)
		type = 'w';
	else if ((sym[idx].lst.n_type & N_TYPE) == N_ABS)
		type = 'a';
	else if ((sym[idx].lst.n_type & N_TYPE) == N_INDR)
		type = 'i';
	else if (sym[idx].lst.n_type & N_EXT && type != 'U')
		type -= 32;
	return (type);
}

static void		print_list(t_list_inf *sym, int sz, int *tab)
{
	int			i;
	char		type;

	i = 0;
	while (i < sz)
	{
		if ((sym[i].lst.n_type & N_PEXT) != N_PEXT)
		{
			type = put_type(sym, i, tab);
			if (sym[i].lst.n_type & N_STAB)
				;//printf("-");
			if (type != 0x0)
			{
				if (sym[i].lst.n_value == 0x0 && type != 'T')
					printf("%18c %s\n", type, sym[i].str);
				else
					printf("%016llx %c %s\n", sym[i].lst.n_value, type, sym[i].str);
			}
		}//ft_putendl(sym[i].str);
		i++;
	}
}

static int		sym_64(t_macho64 *inf, t_file_inf file, int offset)
{
	int			i;
	int			off;

	i = 0;
	off = offset + inf->symtab.symoff;
	if (!(inf->symbol = (t_list_inf *)malloc(sizeof(t_list_inf) * inf->symtab.nsyms)))
		return (ft_nm_put_error(file.name, NOT_VALID));
	while (i < (int)inf->symtab.nsyms)
	{
		read_lst_64(&inf->symbol[i].lst, file.content + off, sizeof(LST_64), file);
		if ((off += sizeof(LST_64)) > file.inf.st_size)
			return (ft_nm_put_error(file.name, NOT_VALID));
		inf->symbol[i].str = file.content + offset + inf->symtab.stroff + inf->symbol[i].lst.n_un.n_strx;
		i++;
	}
	sort_nlist(inf->symbol, inf->symtab.nsyms);
	return (0);
}

static int		pars_section(t_file_inf file, int offset, int *tab, int *i_sct)
{
	uint32_t	i;
	SGM_64		sgm;
	SCT_64		sct;

	i = 0;
	read_seg_64(&sgm, file.content + offset, sizeof(SGM_64), file);
	if (offset + sgm.cmdsize > file.inf.st_size)
		return (ft_nm_put_error(file.name, NOT_VALID));
	if ((offset += sizeof(SGM_64)) > file.inf.st_size)
		return (ft_nm_put_error(file.name, NOT_VALID));
	while (i < sgm.nsects)
	{
		ft_memcpy(&sct, file.content + offset, sizeof(SCT_64));
		if ((long)(offset + sct.size) > file.inf.st_size)
			return (sect_err(file.name, i));
		if (ft_strcmp(sct.sectname, SECT_TEXT) == 0)
			tab[TEXT_IDX] = *i_sct + 1;
		if (ft_strcmp(sct.sectname, SECT_DATA) == 0)
			tab[DATA_IDX] = *i_sct + 1;
		if (ft_strcmp(sct.sectname, SECT_BSS) == 0)
			tab[BSS_IDX] = *i_sct + 1;
		i++;
		(*i_sct)++;
		if ((offset += sizeof(SCT_64)) > file.inf.st_size)
			return (ft_nm_put_error(file.name, NOT_VALID));
	}
	return (0);
}

void			handle_64(t_file_inf file, int offset)
{
	int				off;
	int				i;
	int				i_sct;
	t_macho64		inf;
	int				sect_idx[3];

	i = 0;
	i_sct = 0;
	ft_bzero(&sect_idx, sizeof(int) * 3);
	read_header_64(&inf.hdr, file.content + offset, sizeof(HDR_64), file);
	off = offset;
	offset += sizeof(HDR_64);
	while (i < (int)inf.hdr.ncmds)
	{
		read_load_command(&inf.ld, file.content + offset, sizeof(LD), file);
		if (inf.ld.cmd == LC_SEGMENT_64)
		{
			if (pars_section(file, offset, (int *)&sect_idx, &i_sct) == -1)
				return ;
		}
		else if (inf.ld.cmd == LC_SYMTAB)
		{
			read_symtab(&inf.symtab, file.content + offset, sizeof(SYM), file);
			if (sym_64(&inf, file, off) == -1)
				return ;
		}
		if ((offset += inf.ld.cmdsize) > file.inf.st_size)
		{
			ft_nm_put_error(file.name, NOT_VALID);
			return ;
		}
		i++;
	}
	print_list(inf.symbol, inf.symtab.nsyms, (int*)&sect_idx);
}
