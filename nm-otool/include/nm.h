/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:29:43 by rostroh           #+#    #+#             */
/*   Updated: 2020/02/06 15:34:32 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include "libft.h"
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

typedef struct		s_file_inf
{
	int				fd;
	int				sz;
	int				cig;
	char			*name;
	char			*content;
	struct stat		inf;
}					t_file_inf;

# define HDR struct mach_header
# define SGM struct segment_command
# define SCT struct section
# define SYM struct symtab_command
# define LST struct nlist
# define LD struct load_command

# define HDR_64 struct mach_header_64
# define SGM_64 struct segment_command_64
# define SCT_64 struct section_64
# define LST_64 struct nlist_64

# define AR_HDR struct ar_hdr

# define TEXT_IDX 0
# define DATA_IDX 1
# define BSS_IDX 2
# define NB_SCT_INF 3

typedef struct		s_list_inf_32
{
	char			*str;
	LST				lst;
	char			type;
}					t_list_inf_32;

typedef struct		s_macho32
{
	HDR				hdr;
	LD				ld;
	SGM				*sgm;
	SCT				**sct;
	SYM				symtab;
	t_list_inf_32	*symbol;
}					t_macho32;

typedef struct		s_list_inf
{
	char			*str;
	LST_64			lst;
	char			type;
}					t_list_inf;

typedef struct		s_macho64
{
	HDR_64			hdr;
	LD				ld;
	SGM_64			*sgm;
	SCT_64			**sct;
	SYM				symtab;
	t_list_inf		*symbol;
}					t_macho64;

/*
**	error.c
*/
void				ft_open_error(char *name, int errno);
void				ft_read_error(char *name, int errno);

/*
**	ft_nm.c
*/
void				ft_nm(t_file_inf file);

/*
**	macho32.c
*/
void				handle_32(t_file_inf file, int offset);

/*
**	macho64.c
*/
void				handle_64(t_file_inf file, int offset);

/*
**	read_struct.c
*/
void				read_load_command(LD *dst, void *src, size_t size, t_file_inf file);
void				read_symtab(SYM *dst, void *src, size_t size, t_file_inf file);

/*
**	read_struct_32.c
*/
void				read_header_32(HDR *dst, void *src, size_t size, t_file_inf file);
void				read_seg_32(SGM *dst, void *src, size_t size, t_file_inf file);
void				read_lst_32(LST *dst, void *src, size_t size, t_file_inf file);

/*
**	read_struct_64.c
*/
void				read_header_64(HDR_64 *dst, void *src, size_t size, t_file_inf file);
void				read_seg_64(SGM_64 *dst, void *src, size_t size, t_file_inf file);
void				read_lst_64(LST_64 *dst, void *src, size_t size, t_file_inf file);

/*
**	swap.c
*/
uint16_t			swap_u16(uint16_t nb);
uint32_t			swap_u32(uint32_t nb);
uint64_t			swap_u64(uint64_t nb);

/*
**	tools.c
*/
void				ft_nm_put_error(char *name, char *error);
#endif
