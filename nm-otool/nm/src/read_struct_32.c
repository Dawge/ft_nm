/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_struct_64.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 11:41:48 by rostroh           #+#    #+#             */
/*   Updated: 2020/02/12 21:25:48 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void		read_header_32(HDR *dst, void *src, size_t size, t_file_inf file)
{
	ft_memcpy(dst, src, size);
	if (file.cig == 1)
	{
		dst->filetype = swap_u32(dst->filetype);
		dst->ncmds = swap_u32(dst->ncmds);
		dst->sizeofcmds = swap_u32(dst->sizeofcmds);
		dst->flags = swap_u32(dst->flags);
	}
}

void		read_seg_32(SGM *dst, void *src, size_t size, t_file_inf file)
{
	ft_memcpy(dst, src, size);
	if (file.cig == 1)
	{
		dst->cmd = swap_u32(dst->cmd);
		dst->cmdsize = swap_u32(dst->cmdsize);
		dst->nsects = swap_u32(dst->nsects);
	}
}

void		read_lst_32(LST *dst, void *src, size_t size, t_file_inf file)
{
	ft_memcpy(dst, src, size);
	if (file.cig == 1)
	{
		dst->n_un.n_strx = swap_u32(dst->n_un.n_strx);
		dst->n_value = swap_u32(dst->n_value);
	}
}

void		read_sct_32(SCT *dst, void *src, size_t size, t_file_inf file)
{
	ft_memcpy(dst, src, size);
	if (file.cig == 1)
	{
		dst->size = swap_u32(dst->size);
	}
}
