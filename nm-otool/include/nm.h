/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:29:43 by rostroh           #+#    #+#             */
/*   Updated: 2020/01/18 17:47:52 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

#include "libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <mach-o/swap.h>
# include <mach-o/stab.h>
# include <mach-o/loader.h>
# include <mach-o/ranlib.h>
# include <mach-o/fat.h>
# include <ar.h>

# define OPEN_ERROR 2

typedef struct		s_error
{
	int				err;
	char			*message;
}					t_error;

# define MACH_HDR struct mach_header
# define SGM_CMD struct segment_command
# define SECTION struct section
# define SYM_CMD struct symtab_command
# define LST struct nlist

typedef struct		s_macho64
{
	MACH_HDR		hdr;
	SGM_CMD			*sgm;
	SECTION			**sct;
}					t_macho64;

/*
**	error.c
*/
void				ft_open_error(char *name, int errno);

/*
**	tools.c
*/
void				ft_put_three_string(char *s1, char *s2, char *s3);
#endif
