/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 17:07:26 by rostroh           #+#    #+#             */
/*   Updated: 2020/01/28 19:25:18 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		check_magic(uint32_t magic, char *name)
{
	int					i;
	static uint32_t		magic_tab[NB_MAGIC] = {MH_MAGIC, MH_CIGAM, MH_MAGIC_64,\
		MH_CIGAM_64, FAT_MAGIC, FAT_CIGAM, FAT_MAGIC_64, FAT_CIGAM_64};

	i = 0;
	while (i < NB_MAGIC)
	{
		if (magic == magic_tab[i])
			return (i);
		i++;
	}
	ft_nm_put_error(name, NOT_VALID);
	return (-1);
}

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
				printf("-");
			if (type != 0x0)
			{
				if (sym[i].lst.n_value == 0x0)
					printf("%18c %s\n", type, sym[i].str);
				else
					printf("%016llx %c %s\n", sym[i].lst.n_value, type, sym[i].str);
			}
		}//ft_putendl(sym[i].str);
		i++;
	}
}

static void		sym_64(t_macho64 *inf, t_file_inf file)
{
	int			i;
	int			offset;

	i = 0;
	offset = inf->symtab.symoff;
	if (!(inf->symbol = (t_list_inf *)malloc(sizeof(t_list_inf) * inf->symtab.nsyms)))
		return ;
	while (i < (int)inf->symtab.nsyms)
	{
		ft_memcpy(&inf->symbol[i].lst, file.content + offset, sizeof(LST_64));
		offset += sizeof(LST_64);
		inf->symbol[i].str = file.content + inf->symtab.stroff + inf->symbol[i].lst.n_un.n_strx;
		i++;
	}
	sort_nlist(inf->symbol, inf->symtab.nsyms);
}

static int		pars_section(t_file_inf file, int offset, int *tab, int *i_sct)
{
	uint32_t	i;
	SGM_64		sgm;
	SCT_64		sct;

	i = 0;
//	printf("%s %s %s\n", SECT_TEXT, SECT_DATA, SECT_BSS);
//	printf("?? %d pour %lld et %d\n", offset, file.inf.st_size, sgm.nsects);
	ft_memcpy(&sgm, file.content + offset, sizeof(SGM_64));
	if ((offset += sizeof(SGM_64)) > file.inf.st_size)
	{
		return (-1);
	}
	while (i < sgm.nsects)
	{
		ft_memcpy(&sct, file.content + offset, sizeof(SCT_64));
		if (ft_strcmp(sct.sectname, SECT_TEXT) == 0)
			tab[TEXT_IDX] = *i_sct + 1;
		if (ft_strcmp(sct.sectname, SECT_DATA) == 0)
			tab[DATA_IDX] = *i_sct + 1;
		if (ft_strcmp(sct.sectname, SECT_BSS) == 0)
			tab[BSS_IDX] = *i_sct + 1;
		i++;
		(*i_sct)++;
		if ((offset += sizeof(SCT_64)) > file.inf.st_size)
			return (-1);
	}
	return (0);
}

static void		handle_64(t_file_inf file)
{
	int				i;
	int				i_sct;
	t_macho64		inf;
	size_t			offset;
	int				sect_idx[3];

	i = 0;
	i_sct = 0;
	ft_bzero(&sect_idx, sizeof(int) * 3);
	ft_memcpy(&inf.hdr, file.content, sizeof(HDR_64));
	offset = sizeof(HDR_64);
	while (i < (int)inf.hdr.ncmds)
	{
		ft_memcpy(&inf.ld, file.content + offset, sizeof(LD));
		if (inf.ld.cmd == LC_SEGMENT_64)
		{
			SGM		truc;
			ft_memcpy(&truc, file.content + offset, sizeof(SGM));
			if (pars_section(file, offset, (int *)&sect_idx, &i_sct) == -1)
			{
				ft_nm_put_error(file.name, NOT_VALID);
				return ;
			}
			//offset += sizeof(SGM_64);
		}
		else if (inf.ld.cmd == LC_SYMTAB)
		{
			ft_memcpy(&inf.symtab, file.content + offset, sizeof(SYM));
			sym_64(&inf, file);
		}
		offset += inf.ld.cmdsize;
		i++;
	}
	print_list(inf.symbol, inf.symtab.nsyms, (int*)&sect_idx);
}

void			ft_nm(t_file_inf file)
{
	int				idx;
	uint32_t		magic;
	static void		(*func_dispenser[NB_MAGIC])(t_file_inf file) = {NULL, NULL, &handle_64, NULL, NULL, NULL, NULL, NULL};

	ft_memcpy(&magic, file.content, sizeof(magic));
	if ((idx = check_magic(magic, file.name)) == -1)
		return ;
	func_dispenser[idx](file);
}
