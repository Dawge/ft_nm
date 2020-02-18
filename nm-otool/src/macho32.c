/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:07:56 by rostroh           #+#    #+#             */
/*   Updated: 2020/02/18 13:07:30 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void		sort_nlist(t_list_inf_32 *symbol, int sz)
{
	int				i;
	t_list_inf_32		tmp;

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

static char		put_type(t_list_inf_32 *sym, int idx, int *tab)
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

static int		find_name(t_list_inf_32 *sym, int idx, int sz)
{
	int			i;

	i = 0;
	while (i < sz)
	{
		if (i != idx && ft_strcmp(sym[i].str, sym[idx].str) == 0)
		{
			sym[idx].lst.n_value = sym[i].lst.n_value;
//			printf("sym[%d] = %s et sym[%d] = %s\n", i, sym[i].str, idx, sym[idx].str);
			if ((sym[i].lst.n_type & N_PEXT) != N_PEXT)
				return (0);
		}
		i++;
	}
	return (1);
}

static void		print_not_dupe(t_list_inf_32 *sym, int idx, int sz, int *tab)
{
	int				i;
	char			type;
	static char		tab_type[NB_SCT_INF] = {'t', 'd', 'b'};

	i = 0;
	type = 0x0;
	if (find_name(sym, idx, sz) == 1)
	{
		while (i < NB_SCT_INF)
		{
			if (tab[i] == sym[idx].lst.n_sect)
			{
				type = tab_type[i];
				break ;
			}
			i++;
		}
		if (type != 0x0 && sym[idx].str[0] != '\0')
		{
			if (sym[idx].lst.n_type == N_GSYM)
				type = 's';
			else if (sym[idx].lst.n_type == N_FUN)
				type = 't';
			else
				return ;
			printf("%08x %c %s\n", sym[idx].lst.n_value, type, sym[idx].str);
			//printf("%d : name = %s\nchar1 = %d, ntype = 0x%x, nsect = 0x%x et desc = 0x%x\n", idx, sym[idx].str, sym[idx].str[0], sym[idx].lst.n_type, sym[idx].lst.n_sect, sym[idx].lst.n_desc);
		}
	}
}

static void		print_list(t_list_inf_32 *sym, int sz, int *tab)
{
	int			i;
	char		type;

	i = 0;
	type = tab[0];
	while (i < sz)
	{
		//printf("type = 0x%x, ntype = 0x%x, desc = 0x%x, value = 0x%x et sect = 0x%x\nstr = %s\n", type, sym[i].lst.n_type, sym[i].lst.n_desc, sym[i].lst.n_value, sym[i].lst.n_sect, sym[i].str);
		if ((sym[i].lst.n_type & N_PEXT) != N_PEXT)
		{
			type = put_type(sym, i, tab);
	//		if (sym[i].lst.n_type & N_STAB)
	//			;//printf("-");
			if (type != 0x0 && sym[i].str[0] != '\0' && sym[i].lst.n_type != 0x20)
			{
				if (sym[i].lst.n_value == 0x0 && type != 'T')
					printf("%10c %s\n", type, sym[i].str);
				else
					printf("%08x %c %s\n", sym[i].lst.n_value, type, sym[i].str);
			}
			else
				print_not_dupe(sym, i, sz, tab);
		}
		i++;
	}
}

static int		sym_32(t_macho32 *inf, t_file_inf file, int offset)
{
	int			i;
	int			off;

	i = 0;
	off = offset + inf->symtab.symoff;
	if (!(inf->symbol = (t_list_inf_32 *)malloc(sizeof(t_list_inf_32) * inf->symtab.nsyms)))
	{
		return (ft_nm_put_error(file.name, NOT_VALID));
	}
	while (i < (int)inf->symtab.nsyms)
	{
		read_lst_32(&inf->symbol[i].lst, file.content + off, sizeof(LST), file);
		if ((off += sizeof(LST)) > file.inf.st_size)
		{
			return (ft_nm_put_error(file.name, NOT_VALID));
		}
		inf->symbol[i].str = file.content + offset + inf->symtab.stroff + inf->symbol[i].lst.n_un.n_strx;
		i++;
	}
	sort_nlist(inf->symbol, inf->symtab.nsyms);
	return (0);
}

static int		pars_section(t_file_inf file, int offset, int *tab, int *i_sct)
{
	uint32_t	i;
	SGM			sgm;
	SCT			sct;

	i = 0;
	read_seg_32(&sgm, file.content + offset, sizeof(SGM), file);
	//printf("cig = %d, off = 0x%x, cmdsize = %d\n", file.cig, offset, sgm.cmdsize);
	if (offset + sgm.cmdsize > file.inf.st_size)
	{
	//	ft_putstr("2\n");
		return (ft_nm_put_error(file.name, NOT_VALID));
	}
	if ((offset += sizeof(SGM)) > file.inf.st_size)
	{
	//	ft_putstr("3\n");
		return (ft_nm_put_error(file.name, NOT_VALID));
	}
	while (i < sgm.nsects)
	{
		read_sct_32(&sct, file.content + offset, sizeof(SCT), file);
		//ft_memcpy(&sct, file.content + offset, sizeof(SCT));
		if (offset + sct.size > file.inf.st_size)
			return (sect_err(file.name, i));
		if (ft_strcmp(sct.sectname, SECT_TEXT) == 0)
			tab[TEXT_IDX] = *i_sct + 1;
		if (ft_strcmp(sct.sectname, SECT_DATA) == 0)
			tab[DATA_IDX] = *i_sct + 1;
		if (ft_strcmp(sct.sectname, SECT_BSS) == 0)
			tab[BSS_IDX] = *i_sct + 1;
		i++;
		(*i_sct)++;
		if ((offset += sizeof(SCT)) > file.inf.st_size)
		{
	//		ft_putstr("1\n");
			return (ft_nm_put_error(file.name, NOT_VALID));
		}
	}
	return (0);
}

void			handle_32(t_file_inf file, int offset)
{
	int				off;
	int				i;
	int				i_sct;
	t_macho32		inf;
	int				sect_idx[3];

	ft_putstr("MDR\n");
	i = 0;
	i_sct = 0;
	ft_bzero(&sect_idx, sizeof(int) * 3);
	read_header_32(&inf.hdr, file.content + offset, sizeof(HDR), file);
	off = offset;
	offset += sizeof(HDR);
	while (i < (int)inf.hdr.ncmds)
	{
		read_load_command(&inf.ld, file.content + offset, sizeof(LD), file);
		if (inf.ld.cmd == LC_SEGMENT)
		{
			if (pars_section(file, offset, (int *)&sect_idx, &i_sct) == -1)
				return ;
		}
		else if (inf.ld.cmd == LC_SYMTAB)
		{
			read_symtab(&inf.symtab, file.content + offset, sizeof(SYM), file);
			if (sym_32(&inf, file, off) == -1)
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
