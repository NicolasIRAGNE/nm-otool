/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 12:58:24 by niragne           #+#    #+#             */
/*   Updated: 2019/06/20 19:46:28 by ldedier          ###   ########.fr       */
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

struct						s_nm_browser
{
	uint32_t				magic;
	void					*ptr;
	t_bin_type				type;
	t_header_union			header_union;
	struct stat				st;
	t_tree					*symbols;
	t_section_arr			section_arr;
	int						ret;
	long					(*sort_func)(void *, void *);
	char					sort_mult;
	char					*filename;
};

typedef struct s_nm_browser	t_nm_browser;

void						nm_print_header(t_nm_browser *browser);


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
void						init_browser_general(t_nm_browser *browser);
int							init_browser(t_nm_browser *browser, char *filename);
/*
** fill_tools.c
*/
int							add_symbol_to_browser(t_nm_browser *browser,
								t_symbol *new_symbol);
int							nm_perror(char *error_message,
								t_nm_browser *browser);
t_symbol					*nm_new_symbol32(struct nlist *nlist,
								char *symbol_name);
t_symbol					*nm_new_symbol64(struct nlist_64 *nlist,
								char *symbol_name);

/*
** fill.c
*/
int							fill_browser(t_nm_browser *browser);

/*
** fill32.c
*/
int							fill_browser32(t_nm_browser *browser);

/*
** fill64.c
*/
int							fill_browser64(t_nm_browser *browser);

/*
** print.c
*/
void						nm_print(t_nm_browser *browser);

#endif
