/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 13:48:09 by rostroh           #+#    #+#             */
/*   Updated: 2020/02/12 20:46:59 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void		read_load_command(LD *dst, void *src, size_t size, t_file_inf file)
{
	ft_memcpy(dst, src, size);
	if (file.cig == 1)
	{
		dst->cmd = swap_u32(dst->cmd);
		dst->cmdsize = swap_u32(dst->cmdsize);
	}
}

void		read_symtab(SYM *dst, void *src, size_t size, t_file_inf file)
{
	ft_memcpy(dst, src, size);
	if (file.cig == 1)
	{
		dst->symoff = swap_u32(dst->symoff);
		dst->nsyms = swap_u32(dst->nsyms);
		dst->stroff = swap_u32(dst->stroff);
	}
}
