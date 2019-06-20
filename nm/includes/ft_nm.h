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
/*
typedef struct				s_symbol
{
	t_symbol_union			symbol_union;
	t_symbol_union			symbol_union;
}
-9
typedef struct				s_symbol64
{
	struct nlist_64*		nlist;
	char					*name;
}							t_symbol64;

struct						s_nm_browser
{
	uint32_t				magic;
	void					*ptr;
	t_bin_type				type;
	t_header_union			header_union;
	struct stat				st;
	t_tree					*symbols;
	int						ret;
	int						(*sort_func)(void *, void *);
	char					sort_mult;
	char					*filename;
};

typedef struct s_nm_browser	t_nm_browser;

void						nm_print_header(t_nm_browser *browser);

/*
** symbols.c
*/
int							cmp_symbol_alpha(void *s1, void *s2);

#endif
