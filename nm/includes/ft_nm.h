/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 12:58:24 by niragne           #+#    #+#             */
/*   Updated: 2019/06/19 13:52:39 by niragne          ###   ########.fr       */
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
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>

typedef enum e_bin_type
{
	E_32,
	E_64,
	E_FAT32,
	E_FAT64
}			t_bin_type;

typedef union u_browser_union
{
	struct mach_header *header32;
	struct mach_header_64 *header64;
	struct fat_header *header_fat;
	struct fat_header64 *header_fat64;
}			t_browser_union;

struct s_nm_browser
{
	uint32_t		magic;
	t_bin_type		type;
	t_browser_union type_union;
};

typedef struct s_nm_browser t_nm_browser;

#endif