/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/06/20 19:45:42 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	get_header(t_nm_browser *browser)
{
	browser->magic = *(uint32_t*)browser->ptr;
	if (browser->magic == MH_MAGIC_64 || browser->magic == MH_CIGAM_64)
	{
//		ft_printf("Executable 64 bits\n");
		browser->type = E_64;
		browser->header_union.header64 = (struct mach_header_64*)browser->ptr;
	}
	else if (browser->magic == MH_MAGIC || browser->magic == MH_CIGAM)
	{
//		ft_printf("Executable 32 bits\n");
		browser->type = E_32;
		browser->header_union.header32 = (struct mach_header*)browser->ptr;
	}
	else if (browser->magic == FAT_MAGIC || browser->magic == FAT_CIGAM)
	{
		ft_printf("Executable fat 32 bits\n");
		browser->type = E_FAT32;
		browser->header_union.fat_header = (struct fat_header*)browser->ptr;
	}
	else if (browser->magic == FAT_MAGIC_64 || browser->magic == FAT_CIGAM_64)
	{
		ft_printf("Executable fat 64 bits\n");
		browser->type = E_FAT64;
		browser->header_union.fat_header = (struct fat_header*)browser->ptr;
	}
	else
		browser->type = E_UNKNOWN;
}

int	fill_browser(t_nm_browser *browser)
{
	get_header(browser);
	if (browser->type == E_UNKNOWN)
		return (nm_perror("unknown magic file", browser));
	else if (browser->type == E_64)
		return (fill_browser64(browser));
	else if (browser->type == E_32)
		return (fill_browser32(browser));
	return (0);
}
