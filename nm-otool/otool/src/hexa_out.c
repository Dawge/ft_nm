/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexa_out.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/12 16:29:16 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/12 18:55:40 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int			hexa_out32(t_file_inf file, struct section sct, int off, int cig)
{
	uint32_t		i;

	i = 0;
	if (file.arch != NULL)
		ft_printf("%s", file.arch);
	ft_printf("Contents of (%s,%s) section", sct.segname, sct.sectname);
	while (i < sct.size)
	{
		if (cig == 0)
			print_data32(file.content, i, sct.addr, sct.offset + off);
		else
			print_data32(file.content, i, swap_u32(sct.addr), \
					swap_u32(sct.offset) + off);
		i++;
		if (cig == 0)
			ft_printf(" ");
		else
		{
			if (i % 4 == 0)
				ft_printf(" ");
		}
	}
	ft_printf("\n");
	return (0);
}

int			hexa_out64(t_file_inf file, struct section_64 sct, int off, int cig)
{
	uint32_t		i;

	i = 0;
	if (file.arch != NULL)
		ft_printf("%s", file.arch);
	ft_printf("Contents of (%s,%s) section", sct.segname, sct.sectname);
	while (i < sct.size)
	{
		if (cig == 0)
			print_data64(file.content, i, sct.addr, sct.offset + off);
		else
			print_data64(file.content, i, swap_u32(sct.addr), \
					swap_u32(sct.offset) + off);
		i++;
		if (cig == 0)
			ft_printf(" ");
		else
		{
			if (i % 4 == 0)
				ft_printf(" ");
		}
	}
	ft_printf("\n");
	return (1);
}
