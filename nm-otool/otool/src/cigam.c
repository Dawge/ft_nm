/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cigam.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 19:11:10 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/10 19:21:11 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int				cigam_32(t_file_inf file, int off)
{
	file.fd = 0;
	off = 0;
	printf("Salut cigam32\n");
	file.cig = 1;
	handle_32(file, off);
	return (0);
}

int				cigam_64(t_file_inf file, int off)
{
	file.fd = 0;
	off = 0;
	printf("Salut cigam64\n");
	file.cig = 1;
	handle_64(file, off);
	return (0);
}
