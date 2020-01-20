/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:29:43 by rostroh           #+#    #+#             */
/*   Updated: 2020/01/18 19:13:12 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

#include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <mach-o/swap.h>
# include <mach-o/stab.h>
# include <mach-o/loader.h>
# include <mach-o/ranlib.h>
# include <mach-o/fat.h>
# include <ar.h>

# define OPEN_ERROR 2
# define READ_ERROR 1

# define NB_MAGIC 8

# define NOT_VALID " The file was not recognized as a valid object file\n"

typedef struct		s_error
{
	int				err;
	char			*message;
}					t_error;

typedef struct		s_file
{
	int				fd;
	char			*name;
}					t_file;

# define MACH_HDR struct mach_header
# define SGM_CMD struct segment_command
# define SECTION struct section
# define SYM_CMD struct symtab_command
# define LST struct nlist
# define LD_CMD struct load_command

typedef struct		s_macho64
{
	MACH_HDR		hdr;
	LD_CMD			*ld;
	SGM_CMD			*sgm;
	SECTION			**sct;
}					t_macho64;

/*
**	error.c
*/
void				ft_open_error(char *name, int errno);
void				ft_read_error(char *name, int errno);

/*
**	tools.c
*/
int					ft_nm_read(t_file inf, void *buf, size_t nbytes);
void				ft_nm_put_error(char *name, char *error);
#endif
