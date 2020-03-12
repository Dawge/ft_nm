/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cigam.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 19:11:10 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/12 16:35:31 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int			cigam_32(t_file_inf file, int off)
{
	file.cig = 1;
	return (handle_32(file, off));
}

int			cigam_64(t_file_inf file, int off)
{
	file.cig = 1;
	return (handle_64(file, off));
}

int			cigam_fat32(t_file_inf file, int off)
{
	file.cig = 1;
	return (handle_fat32(file, off));
}

int			cigam_fat64(t_file_inf file, int off)
{
	file.cig = 1;
	return (handle_fat64(file, off));
}
