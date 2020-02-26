/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_struct_fat.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 16:04:57 by rostroh           #+#    #+#             */
/*   Updated: 2020/02/07 16:27:20 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void		read_arch(FAT_ARCH *dst, void *src, size_t sz, int cig)
{
	ft_memcpy(dst, src, sz);
	if (cig == 1)
	{
		dst->offset = swap_u32(dst->offset);
		dst->size = swap_u32(dst->size);
	}
}

void		read_header_fat(FAT_HDR *dst, void *src, size_t sz, int cig)
{
	ft_memcpy(dst, src, sz);
	if (cig == 1)
		dst->nfat_arch = swap_u32(dst->nfat_arch);
}
