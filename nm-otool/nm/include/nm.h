/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:29:43 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/12 14:38:06 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/mman.h>
# include <mach-o/swap.h>
# include <mach-o/stab.h>
# include <mach-o/loader.h>
# include <mach-o/ranlib.h>
# include <mach-o/fat.h>
# include <mach/machine.h>
# include <ar.h>

# define OPEN_ERROR 2
# define READ_ERROR 1

# define NB_MAGIC 8

# define NOT_VALID " The file was not recognized as a valid object file\n"
# define TRUNC_ERR " truncated or malformed object "

typedef struct					s_error
{
	int							err;
	char						*message;
}								t_error;

typedef struct					s_file_inf
{
	int							fd;
	int							sz;
	int							cig;
	int							off_arch;
	int							arch_idx;
	char						*name;
	char						*arch;
	char						*content;
	struct stat					inf;
}								t_file_inf;

# define TEXT_IDX 0
# define DATA_IDX 1
# define BSS_IDX 2
# define NB_SCT_INF 3

# define NB_CPU 2

typedef struct					s_list_inf_32
{
	int							printed;
	char						*str;
	struct nlist				lst;
	char						type;
}								t_list_inf_32;

typedef struct					s_macho32
{
	int							sct_count;
	int							sct_idx[3];
	struct mach_header			hdr;
	struct load_command			ld;
	struct segment_command		*sgm;
	struct section				**sct;
	struct symtab_command		symtab;
	t_list_inf_32				*symbol;
}								t_macho32;

typedef struct					s_list_inf
{
	int							printed;
	char						*str;
	struct nlist_64				lst;
	char						type;
}								t_list_inf;

typedef struct					s_macho64
{
	int							sct_count;
	int							sct_idx[3];
	struct mach_header_64		hdr;
	struct load_command			ld;
	struct segment_command_64	*sgm;
	struct section_64			**sct;
	struct symtab_command		symtab;
	t_list_inf					*symbol;
}								t_macho64;

/*
**	check_arch.h
*/
int								check_arch32(t_file_inf file, uint32_t cputype,\
		uint32_t fat_idx);
int								check_arch64(t_file_inf file, uint32_t cputype,\
		uint32_t fat_idx);

/*
**	error.c
*/
void							ft_open_error(char *name, int errno);
void							ft_read_error(char *name, int errno);

/*
**	ft_nm.c
*/
void							ft_nm(t_file_inf file, int print);
int								dispens(t_file_inf file, uint32_t magic, \
		int offset);
/*
**	handle_cigam.c
*/
int								cigam_64(t_file_inf file, int off);
int								cigam_32(t_file_inf file, int off);
int								cigam_fat32(t_file_inf file, int off);
int								cigam_fat64(t_file_inf file, int off);

/*
** handle_fat.c
*/
int								handle_fat32(t_file_inf file, int off);
int								handle_fat64(t_file_inf file, int off);

/*
**	macho32.c
*/
int								handle_32(t_file_inf file, int offset);

/*
**	macho64.c
*/
int								handle_64(t_file_inf file, int offset);

/*
**	pars_sct.c
*/
int								pars_sct32(t_file_inf file, int offset, \
		t_macho32 *inf);
int								pars_sct64(t_file_inf file, int offset, \
		t_macho64 *inf);

/*
**	print_list32.c
*/
void							print_list32(t_list_inf_32 *sym, int sz, \
		int *tab);

/*
**	print_list64.c
*/
void							print_list64(t_list_inf *sym, int sz, int *tab);

/*
**	read_struct.c
*/
void							read_load_command(struct load_command *dst, \
		void *src, size_t size, t_file_inf file);
void							read_symtab(struct symtab_command *dst, \
		void *src, size_t size, t_file_inf file);

/*
**	read_struct_32.c
*/
void							read_header_32(struct mach_header *dst, \
		void *src, size_t size, t_file_inf file);
void							read_seg_32(struct segment_command *dst, \
		void *src, size_t size, t_file_inf file);
void							read_lst_32(struct nlist *dst, void *src, \
		size_t size, t_file_inf file);
void							read_sct_32(struct section *dst, void *src, \
		size_t size, t_file_inf file);

/*
**	read_struct_64.c
*/
void							read_header_64(struct mach_header_64 *dst, \
		void *src, size_t size, t_file_inf file);
void							read_seg_64(struct segment_command_64 *dst, \
		void *src, size_t size, t_file_inf file);
void							read_lst_64(struct nlist_64 *dst, void *src, \
		size_t size, t_file_inf file);
void							read_sct_64(struct section_64 *dst, void *src, \
		size_t size, t_file_inf file);

/*
**	read_struct_fat.c
*/
void							read_arch32(struct fat_arch *dst, void *src, \
		size_t size, int cig);
void							read_arch64(struct fat_arch_64 *dst, void *src,\
		size_t size, int cig);
void							read_header_fat(struct fat_header *dst, \
		void *src, size_t size, int cig);

/*
**	swap.c
*/
uint16_t						swap_u16(uint16_t nb);
uint32_t						swap_u32(uint32_t nb);
uint64_t						swap_u64(uint64_t nb);

/*
**	tools.c
*/
int								sect_err(char *name, int sect);
int								ft_nm_put_error(char *name, char *error);
char							*put_arch(char *name, uint32_t arch);

/*
**	type.c
*/
char							put_type32(t_list_inf_32 sym, int *tab);
char							put_type64(t_list_inf sym, int *tab);
#endif
