/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 12:58:24 by niragne           #+#    #+#             */
/*   Updated: 2019/07/01 16:43:15 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM
# define FT_NM

# include "libft.h"
# include <stdio.h>
# include <sys/mman.h>
# include <stdint.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach/machine.h>
# include <mach/vm_prot.h>
# include <mach-o/nlist.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include "symbols.h"
# include "sections.h"

# define DEFAULT_NM_FILE	"a.out"

# define CORRUPTED			3
# define DYLIB_MAGIC		0x72613c21

typedef enum e_bin_type
{
	E_UNKNOWN,
	E_32,
	E_64,
	E_FAT32,
	E_FAT64,
	E_ARCHIVE
}			t_bin_type;

typedef union				u_header_union
{
	struct mach_header		*header32;
	struct mach_header_64	*header64;
	struct fat_header		*fat_header;
}							t_header_union;

typedef struct				s_header_parser
{
	char					*filename;
	uint32_t				magic;
	void					*ptr;
	uint64_t				offset;
	t_bin_type				type;
	t_header_union			header_union;
	int						should_swap : 1;
	t_section_arr			section_arr;
	t_tree					*symbols;
	cpu_type_t				cputype;
}							t_header_parser;

struct						s_nm_browser
{
	void					*ptr;
	struct stat				st;
	int						ret;
	long					(*sort_func)(void *, void *);
	char					sort_mult;
	char					*filename;
//	int						has_64 : 1;
	int						has_bad_index : 1;
	int						nb_args;
	char					*progname;
	t_list					*parsers;
};

typedef struct s_nm_browser	t_nm_browser;

/*
** debug.c
*/
void						nm_print_header(t_header_parser *parser);
void						debug_mach_header(struct mach_header *ptr);
void						print_arch32(struct fat_arch *fat_arch);
/*
** options_tools.c
*/
void						nm_option_error(char *str, t_nm_browser *browser);
int							describe_option(char *str);
void						print_help(void);

/*
** options_func.c
*/
void						nm_opt_n(t_nm_browser *browser, int *options);
void						nm_opt_p(t_nm_browser *browser, int *options);
void						nm_opt_r(t_nm_browser *browser, int *options);
/*
** options.c
*/
int							parse_options(int *i, int ac,
								char **av, t_nm_browser *browser);
/*
** init.c
*/
void						init_browser_general(t_nm_browser *browser,
								char *progname);
int							init_browser(t_nm_browser *browser, char *filename);
void						init_parser(t_header_parser *parser,
								void *ptr, uint64_t offset, char *filename);


/*
** fill_tools.c
*/
int							add_symbol_to_browser(t_header_parser *parser,
								t_nm_browser *browser, t_symbol *new_symbol);
int							nm_perror(char *error_message,
								t_nm_browser *browser);
t_symbol					*nm_new_symbol32(struct nlist *nlist,
								char *symbol_name, t_nm_browser *browser);
t_symbol					*nm_new_symbol64(struct nlist_64 *nlist,
								char *symbol_name, t_nm_browser *browser);
int							should_add_symbol(uint8_t n_type, uint16_t n_stab,
								char *name, t_nm_browser *browser);
uint32_t					max_uint32(uint32_t a, uint32_t b);
/*
** fill.c
*/
int							fill_browser(t_header_parser *parser,
								t_nm_browser *browser);

/*
** fill32.c
*/
int							fill_browser32(t_header_parser *parser,
									t_nm_browser *browser);
/*
** fill64.c
*/
int							fill_browser64(t_header_parser *parser,
									t_nm_browser *browser);
/*
** fill_fat32.c
*/
int							fill_browser_fat32(
								t_header_parser *parser,
									t_nm_browser *browser);
char						*get_cpu_name(cpu_type_t cpu);
/*
** print.c
*/
void						nm_print(t_nm_browser *browser);

/*
** fill_debug.c
*/
int							fill_debug64(t_symbol *symbol,
								t_section_arr section_arr,
									t_nm_browser *browser);
int							fill_debug32(t_symbol *symbol,
								t_section_arr section_arr,
									t_nm_browser *browser);

/*
** free.c
*/
void						free_parser(t_header_parser *parser);
void						free_parser_lst(void *content, size_t dummy);

/*
** swap.c
*/
void	swap_mach_header64(struct mach_header_64 *header64);
void	swap_mach_header(struct mach_header *header32, int should_swap);
void	swap_fat_header(struct fat_header *fat_header);
void	swap_fat_arch(struct fat_arch *fat_arch, int should_swap);
void	swap_load_command(struct load_command *lc, int should_swap);
void	swap_segment_command(struct segment_command *seg, int should_swap);
void	swap_segment_command_64(struct segment_command_64 *seg,
			int should_swap);
void	swap_symtab_command(struct symtab_command *sym, int should_swap);
void	swap_nlist(struct nlist *nlist, int should_swap);

/*
** corrupted.c
*/
int		is_corrupted_string(char *str, t_nm_browser *browser);
int		is_corrupted_data(void *address, size_t size, t_nm_browser *browser);
int		is_corrupted_segment_command_64(struct segment_command_64 *seg,
			uint64_t offset, t_nm_browser *browser);
int		is_corrupted_offset(uint64_t offset, uint64_t size,
			t_nm_browser *browser);

/*
** fill_archive.c
*/
int							fill_browser_archive(t_header_parser *parser,
								t_nm_browser *browser);
#endif
