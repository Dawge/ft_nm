/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_sct.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 17:44:16 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/03 18:55:05 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		sect_idx(char *sect_name)
{
	if (ft_strcmp(sect_name, SECT_TEXT) == 0)
		return (TEXT_IDX);
	if (ft_strcmp(sect_name, SECT_DATA) == 0)
		return (DATA_IDX);
	if (ft_strcmp(sect_name, SECT_BSS) == 0)
		return (BSS_IDX);
	return (-1);
}

int				pars_sct32(t_file_inf file, int offset, int *tab, int *i_sct)
{
	int			idx;
	uint32_t	i;
	SGM			sgm;
	SCT			sct;

	i = 0;
	read_seg_32(&sgm, file.content + offset, sizeof(SGM), file);
	if (offset + sgm.cmdsize > file.inf.st_size)
		return (ft_nm_put_error(file.name, NOT_VALID));
	if ((offset += sizeof(SGM)) > file.inf.st_size)
		return (ft_nm_put_error(file.name, NOT_VALID));
	while (i < sgm.nsects)
	{
		read_sct_32(&sct, file.content + offset, sizeof(SCT), file);
		if (offset + sct.size > file.inf.st_size)
			return (sect_err(file.name, i));
		if ((idx = sect_idx(sct.sectname)) != -1)
			tab[idx] = *i_sct + 1;
		i++;
		(*i_sct)++;
		if ((offset += sizeof(SCT)) > file.inf.st_size)
			return (ft_nm_put_error(file.name, NOT_VALID));
	}
	return (0);
}

int				pars_sct64(t_file_inf file, int offset, t_macho64 *inf)
{
	int			idx;
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
		read_sct_64(&sct, file.content + offset, sizeof(SCT_64), file);
		if ((long)(offset + sct.size) > file.inf.st_size && \
				ft_strcmp(sct.sectname, SECT_BSS) != 0)
			return (sect_err(file.name, i));
		if ((idx = sect_idx(sct.sectname)) != -1)
			inf->sct_idx[idx] = inf->sct_count + 1;
		i++;
		inf->sct_count++;
		if ((offset += sizeof(SCT_64)) > file.inf.st_size)
			return (ft_nm_put_error(file.name, NOT_VALID));
	}
	return (0);
}
