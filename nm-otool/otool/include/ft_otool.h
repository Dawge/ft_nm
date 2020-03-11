/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 15:45:05 by rostroh           #+#    #+#             */
/*   Updated: 2020/03/11 17:03:10 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/mman.h>
# include <mach-o/fat.h>
# include <mach-o/swap.h>
# include <mach-o/stab.h>
# include <mach-o/loader.h>
# include <mach-o/ranlib.h>
# include <mach/machine.h>
# include <ar.h>

# define NB_CPU 2
# define NB_MAGIC 8

# define OPEN_ERR 2

# define NOT_VALID " The file was not recognized as a valid object file\n"
# define TRUNC_ERR " truncated or malformated object "

typedef struct					s_error
{
	int							err;
	char						*message;
}								t_error;

typedef struct					s_file_inf
{
	int							fd;
	int							cig;
	int							off_arch;
	int							arch_idx;
	char						*name;
	char						*arch;
	char						*content;
	struct stat					inf;
}								t_file_inf;

typedef struct					s_macho32
{
	struct mach_header			hdr;
	struct load_command			ld;
	struct segment_command		*sgm;
	struct section				**sct;
}								t_macho32;

typedef struct					s_macho64
{
	struct mach_header_64		hdr;
	struct load_command			ld;
	struct segment_command_64	*sgm;
	struct section_64			**sct;
}								t_macho64;

/*
**	cigam.c
*/
int								cigam_32(t_file_inf file, int off);
int								cigam_64(t_file_inf file, int off);
int								cigam_fat32(t_file_inf file, int off);
int								cigam_fat64(t_file_inf file, int off);

/*
**	fat32.c
*/
int								handle_fat32(t_file_inf file, int off);

/*
**	ft_otool.c
*/
void							ft_otool(t_file_inf file);
int								dispens(t_file_inf file, uint32_t magic, \
		int offset);

/*
**	handle32.c
*/
int								handle_32(t_file_inf file, int offset);

/*
**	handle64.c
*/
int								handle_64(t_file_inf file, int offset);

/*
**	put_arch.c
*/
char							*put_arch(char *name, uint32_t arch);

/*
**	read_struct.c
*/
void							read_load_command(struct load_command *dst, \
		void *src, size_t size, t_file_inf file);

/*
**	read_struct_32.c
*/
void							read_header_32(struct mach_header *dst, \
		void *src, size_t size, t_file_inf file);
void							read_seg_32(struct segment_command *dst, \
		void *src, size_t size, t_file_inf file);
void							read_sct_32(struct section *dst, void *src, \
		size_t size, t_file_inf file);

/*
**	read_struct_64.c
*/
void							read_header_64(struct mach_header_64 *dst, \
		void *src, size_t size, t_file_inf file);
void							read_seg_64(struct segment_command_64 *dst, \
		void *src, size_t size, t_file_inf file);
void							read_sct_64(struct section_64 *dst, void *src, \
		size_t size, t_file_inf file);

/*
**	read_struct_fat.c
*/
void							read_arch32(struct fat_arch *dst, void *src, \
		size_t sz, int cig);
void							read_arch64(struct fat_arch_64 *dst, void *src,\
		size_t sz, int cig);
void							read_header_fat(struct fat_header *dst, \
		void *src, size_t sz, int cig);

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
int								ft_otool_put_error(char *name, char *error);
void							ft_open_error(char *name, int errno);
void							print_data32(char *content, int idx, int addr, \
		int off);
void							print_data64(char *content, int idx, int addr, \
		int off);
#endif
