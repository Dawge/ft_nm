/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 17:07:26 by rostroh           #+#    #+#             */
/*   Updated: 2020/01/23 11:37:35 by rostroh          ###   ########.fr       */
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

static void		put_type(t_file_inf file, t_list_inf *sym)
{
	int				i;
	t_macho64		inf;
	size_t			offset;

	i = 0;
	ft_memcpy(&inf.hdr, file.content, sizeof(HDR_64));
	offset = sizeof(HDR_64);
	while (i < inf.hdr.ncmds)
	{
		else
			offset += inf.ld.cmdsize;
		i++;
	}
}

static void		print_list(t_file_inf file, t_list_inf *sym, int sz)
{
	int			i;

	i = 0;
	while (i < sz)
	{
		if ((sym[i].lst.n_type & N_PEXT) != N_PEXT)
		{
			if (sym[i].lst.n_value == 0x0)
				ft_putstr("                 ");
			else
				printf("%016llx ", sym[i].lst.n_value);
			if (sym[i].lst.n_sect == NO_SECT && (sym[i].lst.n_type & N_EXT) == N_EXT)
				ft_putstr("U ");
			else
				put_type(file, sym);
			printf("0x%x %s %d\n", sym[i].lst.n_type, sym[i].str, sym[i].lst.n_sect);
		}
		//ft_putendl(sym[i].str);
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

static int		pars_section(t_file_inf file, int offset, int *tab)
{
	int			i;
	SGM_64		sgm;
	SCT_64		sct;

	i = 0;
	ft_memcpy(&sgm, file.content + offset, sizeof(SGM_64));
	if ((offset += sizeof(SGM_64)) > file.sz)
		return (-1);
	while (i < sgm.nsects)
	{
		ft_memcpy(&sct, file.content + offset, sizeof(SCT_64));
		if (ft_strcpy(sct.sectname, SECT_TEXT) == 0)
			tab[TEXT_IDX] = i + 1;
		i++;
	}
}

static void		handle_64(t_file_inf file)
{
	int				i;
	t_macho64		inf;
	size_t			offset;
	int				sect_idx[3];

	i = 0;
	ft_memcpy(&inf.hdr, file.content, sizeof(HDR_64));
	offset = sizeof(HDR_64);
	while (i < (int)inf.hdr.ncmds)
	{
		ft_memcpy(&inf.ld, file.content + offset, sizeof(LD));
		if (inf.ld.cmd == LC_SEGMENT_64)
		{
			offset += sizeof(SGM_64);
			pars_section(file, offset, (int *)&sect_idx);
		}
		else if (inf.ld.cmd == LC_SYMTAB)
		{
			ft_memcpy(&inf.symtab, file.content + offset, sizeof(SYM));
			sym_64(&inf, file);
			printf("0x%X et 0x%X\n", inf.symtab.symoff, inf.symtab.stroff);
		}
		else
			offset += inf.ld.cmdsize;
		i++;
	}
	print_list(file, inf.symbol, inf.symtab.nsyms);
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
